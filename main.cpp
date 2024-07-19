
#include <SFML/Graphics.hpp>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
#include <chrono>

#define WIDTH 800
#define HEIGHT 800
#define ROWS 20
#define COLS 20
#define CELL_SIZE (WIDTH / COLS)

enum Algorithm { DIJKSTRA, BFS, DFS, ASTAR };

struct Node {
    int x, y;
    bool isWall;
    bool visited;
    float distance;
    float heuristic;
    Node* parent;

    Node(int x = 0, int y = 0) : x(x), y(y), isWall(false), visited(false), distance(INT_MAX), heuristic(0), parent(nullptr) {}

    float getCost() const {
        return distance + heuristic;
    }
};

std::vector<std::vector<Node>> createGrid(int rows, int cols) {
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = Node(i, j);
        }
    }
    return grid;
}

std::vector<Node*> getNeighbors(Node& node, std::vector<std::vector<Node>>& grid) {
    std::vector<Node*> neighbors;
    int x = node.x;
    int y = node.y;
    if (x > 0) neighbors.push_back(&grid[x-1][y]);
    if (x < ROWS-1) neighbors.push_back(&grid[x+1][y]);
    if (y > 0) neighbors.push_back(&grid[x][y-1]);
    if (y < COLS-1) neighbors.push_back(&grid[x][y+1]);
    return neighbors;
}

void resetGrid(std::vector<std::vector<Node>>& grid, Node* start, Node* end) {
    for (auto& row : grid) {
        for (auto& node : row) {
            node.visited = false;
            node.parent = nullptr;
            node.distance = INT_MAX;
            node.heuristic = 0;
        }
    }
    start->distance = 0;
}

void drawGrid(sf::RenderWindow& window, std::vector<std::vector<Node>>& grid, Node* start, Node* end) {
    window.clear();

    for (auto& row : grid) {
        for (auto& node : row) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(node.y * CELL_SIZE, node.x * CELL_SIZE);
            if (node.isWall) {
                cell.setFillColor(sf::Color::Black);
            } else if (&node == start) {
                cell.setFillColor(sf::Color::Green);
            } else if (&node == end) {
                cell.setFillColor(sf::Color::Red);
            } else if (node.visited) {
                cell.setFillColor(sf::Color::Cyan);
            } else {
                cell.setFillColor(sf::Color::White);
            }
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(1);
            window.draw(cell);
        }
    }

    window.display();
}

void drawPath(sf::RenderWindow& window, Node* end) {
    Node* current = end;
    while (current) {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition(current->y * CELL_SIZE, current->x * CELL_SIZE);
        cell.setFillColor(sf::Color::Yellow);
        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(1);
        window.draw(cell);
        current = current->parent;
    }

    window.display();
}

void dijkstra(std::vector<std::vector<Node>>& grid, Node* start, Node* end, sf::RenderWindow& window) {
    auto comp = [](Node* a, Node* b) { return a->distance > b->distance; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> pq(comp);
    start->distance = 0;
    pq.push(start);

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (current == end) return;

        if (current->visited) continue;
        current->visited = true;

        for (Node* neighbor : getNeighbors(*current, grid)) {
            if (neighbor->visited || neighbor->isWall) continue;

            float newDist = current->distance + 1; // All edges have the same weight
            if (newDist < neighbor->distance) {
                neighbor->distance = newDist;
                neighbor->parent = current;
                pq.push(neighbor);

                drawGrid(window, grid, start, end); // Update grid visualization
                sf::sleep(sf::milliseconds(100)); // Delay for visualization (adjust as needed)
            }
        }
    }
}

void bfs(std::vector<std::vector<Node>>& grid, Node* start, Node* end, sf::RenderWindow& window) {
    std::queue<Node*> q;
    start->visited = true;
    q.push(start);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current == end) return;

        for (Node* neighbor : getNeighbors(*current, grid)) {
            if (neighbor->visited || neighbor->isWall) continue;

            neighbor->visited = true;
            neighbor->parent = current;
            q.push(neighbor);

            drawGrid(window, grid, start, end); // Update grid visualization
            sf::sleep(sf::milliseconds(100)); // Delay for visualization (adjust as needed)
        }
    }
}

void dfs(std::vector<std::vector<Node>>& grid, Node* start, Node* end, sf::RenderWindow& window) {
    std::stack<Node*> s;
    start->visited = true;
    s.push(start);

    while (!s.empty()) {
        Node* current = s.top();
        s.pop();

        if (current == end) return;

        for (Node* neighbor : getNeighbors(*current, grid)) {
            if (neighbor->visited || neighbor->isWall) continue;

            neighbor->visited = true;
            neighbor->parent = current;
            s.push(neighbor);

            drawGrid(window, grid, start, end); // Update grid visualization
            sf::sleep(sf::milliseconds(100)); // Delay for visualization (adjust as needed)
        }
    }
}

float heuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

void astar(std::vector<std::vector<Node>>& grid, Node* start, Node* end, sf::RenderWindow& window) {
    auto comp = [](Node* a, Node* b) { return a->getCost() > b->getCost(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> pq(comp);
    start->distance = 0;
    start->heuristic = heuristic(start, end);
    pq.push(start);

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (current == end) return;

        if (current->visited) continue;
        current->visited = true;

        for (Node* neighbor : getNeighbors(*current, grid)) {
            if (neighbor->visited || neighbor->isWall) continue;

            float newDist = current->distance + 1;
            if (newDist < neighbor->distance) {
                neighbor->distance = newDist;
                neighbor->heuristic = heuristic(neighbor, end);
                neighbor->parent = current;
                pq.push(neighbor);

                drawGrid(window, grid, start, end); // Update grid visualization
                sf::sleep(sf::milliseconds(100)); // Delay for visualization (adjust as needed)
            }
        }
    }
}

void readInputFromFile(std::vector<std::vector<Node>>& grid) {
    std::ifstream inputFile("input3.txt");
    if (!inputFile) {
        std::cerr << "Failed to open input.txt\n";
        exit(1);
    }

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int value;
            inputFile >> value;
            grid[i][j].isWall = (value == 0);
        }
    }

    inputFile.close();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Maze Solver");
    window.setFramerateLimit(10);

    std::vector<std::vector<Node>> grid = createGrid(ROWS, COLS);
    Node* start = &grid[0][0];
    Node* end = &grid[ROWS-1][COLS-1];

    readInputFromFile(grid);

    bool isRunning = false;
    Algorithm algorithm;

    std::cout << "Choose algorithm: 1. Dijkstra   2. BFS   3. DFS   4. A*\n";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            algorithm = DIJKSTRA;
            std::cout << "Using algorithm: Dijkstra\n";
            break;
        case 2:
            algorithm = BFS;
            std::cout << "Using algorithm: BFS\n";
            break;
        case 3:
            algorithm = DFS;
            std::cout << "Using algorithm: DFS\n";
            break;
        case 4:
            algorithm = ASTAR;
            std::cout << "Using algorithm: A*\n";
            break;
        default:
            std::cout << "Invalid choice. Exiting...\n";
            return 1;
    }

    resetGrid(grid, start, end);

    sf::Clock clock;
    float elapsed = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!isRunning) {
            switch (algorithm) {
                case DIJKSTRA:
                    dijkstra(grid, start, end, window);
                    break;
                case BFS:
                    bfs(grid, start, end, window);
                    break;
                case DFS:
                    dfs(grid, start, end, window);
                    break;
                case ASTAR:
                    astar(grid, start, end, window);
                    break;
                default:
                    std::cerr << "Invalid algorithm choice.\n";
                    return 1;
            }
            isRunning = true;
        }

        // Check if algorithm is completed
        if (isRunning) {
            // Highlight shortest path
            drawPath(window, end);

            // End the visualization loop
            isRunning = false;
        }

        elapsed += clock.restart().asSeconds();
    }

    return 0;
}


