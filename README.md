🎓 University Management System (Custom Data Structures)
A comprehensive, console-based enterprise simulation built in C++. This project focuses on the ground-up implementation of complex data structures, avoiding the C++ Standard Template Library (STL) to demonstrate deep algorithmic knowledge and manual memory management.

🚀 Core Modules & Data Architectures
🔐 User Authentication (Hash Table with Chaining)
Structure: Custom-built Hash Table using Chaining for collision handling and Dynamic Resizing for performance.

Features: Secure user registration/login, role-based access (Student/Teacher/Admin), and profile management.

Complexity: O(1) average time complexity for user lookups.

🗺️ Campus Map (Weighted Undirected Graph)
Structure: Adjacency List using custom Linked Lists.

Pathfinding: Implementation of Dijkstra’s Algorithm for shortest path calculation between buildings.

Traversals: Support for Breadth-First Search (BFS) and Depth-First Search (DFS) to visualize campus connectivity.

🏫 Room Scheduling (AVL Trees per Building)
Structure: Self-balancing AVL Trees to ensure O(logn) search, insertion, and deletion.

Features: Room reservation system with double-booking prevention and filtering by room type (Lab/Classroom/Office).

Analytics: Functions for tree height, leaf counting, and Inorder/Preorder/Postorder traversals.

🎫 Maintenance Ticketing (Linked-List Queue)
Structure: FIFO (First-In-First-Out) Queue implemented via a Linked List with Head and Tail pointers.

Logic: Manages maintenance complaints across campus buildings, ensuring fair, chronological processing.

💬 Messaging & Notifications (LIFO Stacks)
Structure: Stack-based conversation history using Singly Linked Lists.

Features: Real-time user-to-user messaging and notification stacks to view the most recent updates first.

🛠️ Key Technical Challenges Solved
Manual Memory Management: Built every structure (Stacks, Queues, Lists) from scratch without std::vector or std::list.

Cross-Module Integration: * Linked User profiles (Hash Table) to Room Reservations (AVL).

Implemented "Cascade Deletion": Removing a building automatically cleans up its AVL tree nodes and associated complaint tickets.

Algorithmic Rigor: Implemented Dijkstra’s and AVL Rotations (LL, RR, LR, RL) manually to maintain system integrity.

🖥️ How to Run
Clone the repository.

Compile using G++:

Bash

g++ main.cpp -o UniversitySystem
Run the executable:

Bash

./UniversitySystem
