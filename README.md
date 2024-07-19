# Maze-Solver-Visualization

## Maze Solver Visualization

This project visualizes four different pathfinding algorithms (Dijkstra, BFS, DFS, and A*) on a grid-based maze. The maze is represented as a 20x20 grid, where each cell can either be a wall or an open path. The user can choose which algorithm to use, and the program will display the steps taken by the chosen algorithm to find the shortest path from the start node to the end node.

### Features

- Visualizes the following pathfinding algorithms:
  - Dijkstra
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - A*
- Allows the user to load a maze from an input file.
- Displays the process of each algorithm step by step.
- Highlights the final path from the start to the end node.

### Pathfinding Algorithms

#### Dijkstra's Algorithm

Dijkstra's algorithm is a famous algorithm used for finding the shortest paths between nodes in a graph, which may represent, for example, road networks. It is considered a greedy algorithm and guarantees the shortest path in terms of cost. In this maze visualization, the cost is the number of steps from the start to the end node.

#### Breadth-First Search (BFS)

Breadth-First Search is a simple graph traversal algorithm that explores all nodes at the present depth level before moving on to nodes at the next depth level. BFS is optimal for finding the shortest path in an unweighted graph, which makes it ideal for our grid-based maze where each move from one cell to another has the same cost.

#### Depth-First Search (DFS)

Depth-First Search is an algorithm for traversing or searching tree or graph data structures. The algorithm starts at the root node (selecting some arbitrary node as the root in the case of a graph) and explores as far as possible along each branch before backtracking. Unlike BFS, DFS is not guaranteed to find the shortest path.

#### A* Search Algorithm

A* Search is a graph traversal and path search algorithm that is often used in many fields of computer science due to its completeness, optimality, and optimal efficiency. A* uses heuristics to guide its search. In this maze visualization, it uses the Manhattan distance as the heuristic to estimate the cost from the current node to the end node.

### Prerequisites

- SFML (Simple and Fast Multimedia Library)



