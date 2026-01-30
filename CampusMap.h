#ifndef CAMPUSMAP_H
#define CAMPUSMAP_H

#include <iostream>
#include <string>
#include <climits>
using namespace std;

// CLASSES IMPLEMENTATION FOR QUEUE AND STACK TO USE IN DFS AND IN BFS
class Queue // this is Queue implementation to handle complaints
{
    struct Node
    {
        int data;
        Node *next;
    };
    Node *front;
    Node *rear;

public:
    Queue() : front(NULL), rear(NULL) {}
    // add the complaint in the queue
    void enqueue(int val)
    {
        Node *newNode = new Node;
        newNode->data = val;
        newNode->next = NULL;
        if (rear == NULL)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
    // remove complaint after processing the complaint
    int dequeue()
    {
        if (front == NULL)
        {
            return -1;
        }
        Node *temp = front;
        int val = temp->data;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        delete temp;
        return val;
    }
    // Check is complaint list is empty or not
    bool isEmpty()
    {
        return front == NULL;
    }
    // Destructor to remove all the complaints
    ~Queue()
    {
        while (front)
        {
            Node *temp = front;
            front = front->next;
            delete temp;
        }
    }
};

// stack implmentation for messaging system
class Stack
{
    // node for message
    struct Node
    {
        int data;
        Node *next;
    };
    Node *top;

public:
    // constructor
    Stack() : top(NULL) {}
    // Add the message that send to some person in the stack
    void push(int val)
    {
        Node *newNode = new Node;
        newNode->data = val;
        newNode->next = top;
        top = newNode;
    }
    // Delete the recent message that is sent to a other person
    int pop()
    {
        if (top == NULL)
        {
            return -1;
        }
        Node *temp = top;
        int val = temp->data;
        top = top->next;
        delete temp;
        return val;
    }
    // Check that messaging list is empty or not
    bool isEmpty()
    {
        return top == NULL;
    }
    // Destrctor to delete all the chat
    ~Stack()
    {
        while (top)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }
};

// this is an weighted undirected graph
class CampusMap
{
private:
    // Struct of building that contain name of building and the number of rooms in that building
    struct BuildingNode
    {
        string buildingName;
        void *roomsAVLRoot; // pointer for module C integration
    };

    struct Edge
    {
        string destBuildingName;
        string relation; // road / walkway
        string status;   // open/closed
        int distance;    // weight (distance between buildings)
        Edge *next;
    };

    BuildingNode *buildings;
    Edge **adjList;
    int buildingCount;
    int Size;
    // Resize when the building is full then new buildings are made
    void resize()
    {
        int oldSize = Size;
        Size = Size * 2;
        // building resize
        BuildingNode *newBuildings = new BuildingNode[Size];
        for (int i = 0; i < oldSize; i++)
        {
            newBuildings[i] = buildings[i];
        }
        delete[] buildings;
        buildings = newBuildings;
        // adjacency list resize
        Edge **newList = new Edge *[Size];
        for (int i = 0; i < oldSize; i++)
        {
            newList[i] = adjList[i];
        }
        for (int i = oldSize; i < Size; i++)
        {
            newList[i] = NULL;
        }
        delete[] adjList;
        adjList = newList;
    }
    // To find the index of building with name
    int findBuildingIndex(string name)
    {
        for (int i = 0; i < buildingCount; i++)
        {
            if (buildings[i].buildingName == name)
            {
                return i; // if found return index
            }
        }
        return -1;
    }
    // Recurrsive function to print the path from building to building
    void printPath(int *parent, int j)
    {
        if (parent[j] == -1)
        {
            cout << buildings[j].buildingName;
            return;
        }
        printPath(parent, parent[j]);
        cout << " -> " << buildings[j].buildingName;
    }

public:
    // Constructor to make a new building
    CampusMap(int s = 10)
    {
        Size = s;
        buildingCount = 0;
        buildings = new BuildingNode[s];
        adjList = new Edge *[s];
        for (int i = 0; i < s; i++)
        {
            adjList[i] = NULL;
            buildings[i].roomsAVLRoot = NULL;
        }
    }
    // Desturctor to Destroy all the buildings
    ~CampusMap()
    {
        for (int i = 0; i < buildingCount; i++)
        {
            Edge *current = adjList[i];
            while (current)
            {
                Edge *temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] adjList;
        delete[] buildings;
    }
    // new building addition
    void addBuilding(string name)
    {
        if (findBuildingIndex(name) != -1)
        {
            cout << "Building '" << name << "' already exists!" << endl;
            return;
        }
        // check if the all the building is full then make a new buildings
        if (buildingCount == Size)
        {
            resize();
        }

        buildings[buildingCount].buildingName = name;
        buildings[buildingCount].roomsAVLRoot = NULL; // module C - AVL tree for rooms in building
        adjList[buildingCount] = NULL;
        buildingCount++;
        cout << "Building '" << name << "' added successfully!" << endl;
    }
    // remove building and roads(edges)
    void removeBuilding(string name)
    {
        int index = findBuildingIndex(name);
        if (index == -1)
        {
            cout << "Building '" << name << "' not found!" << endl;
            return;
        }
        // edge removing between buldings by bulding name
        for (int i = 0; i < buildingCount; i++)
        {
            if (i == index)
                continue;

            Edge *current = adjList[i];
            Edge *prev = NULL;

            while (current)
            {
                if (current->destBuildingName == name)
                {
                    Edge *toDelete = current;
                    if (prev == NULL)
                    {
                        adjList[i] = current->next;
                        current = current->next;
                    }
                    else
                    {
                        prev->next = current->next;
                        current = current->next;
                    }
                    delete toDelete;
                }
                else
                {
                    prev = current;
                    current = current->next;
                }
            }
        }
        // Remove all the edges of building and then remove the building

        Edge *current = adjList[index];
        while (current)
        {
            Edge *temp = current;
            current = current->next;
            delete temp;
        }
        // Arrange the buildings after remove one building and leftshift the remaining buildings
        for (int i = index; i < buildingCount - 1; i++)
        {
            buildings[i] = buildings[i + 1];
            adjList[i] = adjList[i + 1];
        }

        buildingCount--;
        adjList[buildingCount] = NULL;

        cout << "Building '" << name << "' removed successfully!" << endl;
    }

    // adding undirected path between two buildings
    // default enteries have been set in case parameters not provided
    bool addPath(string src, string dest, int distance = 100, string relation = "walkway", string status = "open")
    {
        int srcIndex = findBuildingIndex(src);
        int destIndex = findBuildingIndex(dest);
        if (srcIndex == -1)
        {
            cout << "Source building '" << src << "' not found!" << endl;
            return false;
        }
        if (destIndex == -1)
        {
            cout << "Destination building '" << dest << "' not found!" << endl;
            return false;
        }

        // path verification if already exist
        Edge *temp = adjList[srcIndex];
        while (temp)
        {
            if (temp->destBuildingName == dest)
            {
                cout << "Path already exists between '" << src << "' and '" << dest << "'!" << endl;
                return false;
            }
            temp = temp->next;
        }

        // desitination is in source edge because it is undirected
        Edge *newEdge = new Edge;
        newEdge->destBuildingName = dest;
        newEdge->relation = relation;
        newEdge->status = status;
        newEdge->distance = distance;
        newEdge->next = adjList[srcIndex];
        adjList[srcIndex] = newEdge;

        // source is in destination edge because it is undirected
        newEdge = new Edge;
        newEdge->destBuildingName = src;
        newEdge->relation = relation;
        newEdge->status = status;
        newEdge->distance = distance;
        newEdge->next = adjList[destIndex];
        adjList[destIndex] = newEdge;

        cout << "Path added between '" << src << "' and '" << dest << "' (Distance: " << distance << "m, " << relation << ", " << status << ")" << endl;
        return true;
    }

    // delete edge between buildings
    void removePath(string src, string dest)
    {
        int srcIndex = findBuildingIndex(src);
        int destIndex = findBuildingIndex(dest);

        if (srcIndex == -1)
        {
            cout << "Source building '" << src << "' not found!" << endl;
            return;
        }
        if (destIndex == -1)
        {
            cout << "Destination building '" << dest << "' not found!" << endl;
            return;
        }

        bool pathFound = false;

        // edge remove from source to destination because it is undirected graph
        Edge *current = adjList[srcIndex];
        Edge *prev = NULL;

        while (current)
        {
            if (current->destBuildingName == dest)
            {
                pathFound = true;
                Edge *toDelete = current;
                if (prev == NULL)
                {
                    adjList[srcIndex] = current->next;
                    current = current->next;
                }
                else
                {
                    prev->next = current->next;
                    current = current->next;
                }
                delete toDelete;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }

        // edge remove from destination to source because it is undirected graph
        current = adjList[destIndex];
        prev = NULL;

        while (current)
        {
            if (current->destBuildingName == src)
            {
                Edge *toDelete = current;
                if (prev == NULL)
                {
                    adjList[destIndex] = current->next;
                    current = current->next;
                }
                else
                {
                    prev->next = current->next;
                    current = current->next;
                }
                delete toDelete;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }

        if (pathFound)
        {
            cout << "Path removed between '" << src << "' and '" << dest << "'" << endl;
        }
        else
        {
            cout << "No path exists between '" << src << "' and '" << dest << "'!" << endl;
        }
    }

    // check if edge already exists
    bool isConnected(string src, string dest)
    {
        int srcIndex = findBuildingIndex(src);
        int destIndex = findBuildingIndex(dest);

        if (srcIndex == -1 || destIndex == -1)
        {
            return false;
        }

        Edge *current = adjList[srcIndex];
        while (current)
        {
            if (current->destBuildingName == dest)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    //  BFS USING QUEUE
    void BFS(string startBuilding)
    {
        int start = findBuildingIndex(startBuilding);
        if (start == -1)
        {
            cout << "Building not in graph. Invalid" << endl;
            return;
        }

        if (buildingCount == 0)
        {
            cout << "No buildings in campus map!" << endl;
            return;
        }

        // initializing all visited to false at start
        bool *visited = new bool[buildingCount];
        for (int i = 0; i < buildingCount; i++)
        {
            visited[i] = false;
        }

        cout << "\n\nStarting BFS from: " << buildings[start].buildingName << endl;
        cout << "Order: ";

        // Main BFS algorithm
        Queue q;          // making queue to store vertices
        q.enqueue(start); // enqueuing starting vertix
        visited[start] = true;
        // will go on till queue is empty
        while (!q.isEmpty())
        {
            int current = q.dequeue();
            cout << buildings[current].buildingName << " -> ";

            // visit all neighbours of the current vertex and
            // enqueue all children which are not visited already
            Edge *temp = adjList[current];
            while (temp)
            {
                int neighborIndex = findBuildingIndex(temp->destBuildingName);
                if (neighborIndex != -1 && !visited[neighborIndex])
                {
                    visited[neighborIndex] = true;
                    q.enqueue(neighborIndex);
                }
                temp = temp->next;
            }
        }

        // This handle the vertices that are unreachable from start
        for (int i = 0; i < buildingCount; i++)
        {
            if (!visited[i])
            {
                q.enqueue(i);
                visited[i] = true;
                // same logic as above just handling leftover vertices
                while (!q.isEmpty())
                {
                    int current = q.dequeue();
                    cout << buildings[current].buildingName << " -> ";

                    Edge *temp = adjList[current];
                    while (temp)
                    {
                        int neighborIndex = findBuildingIndex(temp->destBuildingName);
                        if (neighborIndex != -1 && !visited[neighborIndex])
                        {
                            visited[neighborIndex] = true;
                            q.enqueue(neighborIndex);
                        }
                        temp = temp->next;
                    }
                }
            }
        }

        cout << "FINISHED\n\n";
        delete[] visited;
    }

    //  DFS algorithm implementation using stack
    void DFS(string startBuilding)
    {
        int start = findBuildingIndex(startBuilding);
        if (start == -1)
        {
            cout << "Building not exist. Invalid building" << endl;
            return;
        }

        if (buildingCount == 0)
        {
            cout << "No buildings in campus map!" << endl;
            return;
        }

        bool *visited = new bool[buildingCount];
        for (int i = 0; i < buildingCount; i++)
        {
            visited[i] = false;
        }

        cout << "\n\nStarting DFS from: " << buildings[start].buildingName << endl;
        cout << "DFS Order: ";

        Stack s; // making stack and pushing starting building
        s.push(start);
        // main algorithm for DFS
        while (!s.isEmpty())
        {
            int current = s.pop();
            // pop the top and push all its children
            if (!visited[current])
            {
                visited[current] = true;
                cout << buildings[current].buildingName << " -> ";

                // unvisited children /buildings push to stack
                Edge *temp = adjList[current];
                while (temp)
                {
                    int neighborIndex = findBuildingIndex(temp->destBuildingName);
                    if (neighborIndex != -1 && !visited[neighborIndex])
                    {
                        s.push(neighborIndex);
                    }
                    temp = temp->next;
                }
            }
        }

        // Handle the unreachable vertices that are unreachable from start
        // same algo as above
        for (int i = 0; i < buildingCount; i++)
        {
            if (!visited[i])
            {
                s.push(i);

                while (!s.isEmpty())
                {
                    int current = s.pop();

                    if (!visited[current])
                    {
                        visited[current] = true;
                        cout << buildings[current].buildingName << " -> ";

                        Edge *temp = adjList[current];
                        while (temp)
                        {
                            int neighborIndex = findBuildingIndex(temp->destBuildingName);
                            if (neighborIndex != -1 && !visited[neighborIndex])
                            {
                                s.push(neighborIndex);
                            }
                            temp = temp->next;
                        }
                    }
                }
            }
        }

        cout << "FINISHED\n\n";
        delete[] visited;
    }

    //  DJKISTARA Algorithm Finding shortest path between two buildings
    void shortestPath(string sourceBuilding, string destBuilding)
    {
        int source = findBuildingIndex(sourceBuilding);
        int destination = findBuildingIndex(destBuilding);

        if (source == -1)
        {
            cout << "Source building '" << sourceBuilding << "' not found!" << endl;
            return;
        }
        if (destination == -1)
        {
            cout << "Destination building '" << destBuilding << "' not found!" << endl;
            return;
        }

        int *dist = new int[buildingCount];
        int *parent = new int[buildingCount];
        bool *visited = new bool[buildingCount];

        // initializing not visited and infinite distance at all vertices
        for (int i = 0; i < buildingCount; i++)
        {
            dist[i] = INT_MAX;
            parent[i] = -1;
            visited[i] = false;
        }
        dist[source] = 0;

        // algo to find shortest path
        for (int count = 0; count < buildingCount; count++)
        {
            // finding minimum distance vertex
            int minDist = INT_MAX;
            int u = -1;
            for (int i = 0; i < buildingCount; i++)
            {
                if (!visited[i] && dist[i] < minDist)
                {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1)
                break; // end of available reachable vertices
            visited[u] = true;

            // update neighbour distances( main algo logic)
            Edge *temp = adjList[u];
            while (temp)
            {
                int v = findBuildingIndex(temp->destBuildingName);
                if (v != -1)
                {
                    int weight = temp->distance;
                    // Only consider paths that are "open"
                    if (!visited[v] && temp->status == "open" && dist[u] != INT_MAX && dist[u] + weight < dist[v])
                    {
                        dist[v] = dist[u] + weight;
                        parent[v] = u;
                    }
                }
                temp = temp->next;
            }
        }
        // print path
        cout << "\n\n\nDJKISTRA Source: " << buildings[source].buildingName << endl;
        cout << "Destination: " << buildings[destination].buildingName << endl;

        if (dist[destination] == INT_MAX)
        {
            cout << "Distance: INF (No path exists)" << endl;
        }
        else
        {
            cout << "Distance: " << dist[destination] << " meters" << endl;
            cout << "Path: ";
            printPath(parent, destination);
            cout << endl;
        }
        cout << "\n\n\n\n";
        delete[] dist;
        delete[] parent;
        delete[] visited;
    }

    // showing the entire campus map (adjacency list representation)
    void displayAdjacencyList()
    {
        // Check if bulding count is zero or not
        if (buildingCount == 0)
        {
            cout << "\nNo buildings in campus map!" << endl;
            return;
        }
        // Print adjency list
        cout << "\n\n CAMPUS MAP Adjacency List: " << endl;
        for (int i = 0; i < buildingCount; i++)
        {
            cout << buildings[i].buildingName << " -> ";
            Edge *temp = adjList[i];
            if (!temp)
            {
                cout << "No connections";
            }
            while (temp)
            {
                cout << temp->destBuildingName << " [" << temp->distance << "m, "
                     << temp->relation << ", " << temp->status << "]";
                if (temp->next)
                    cout << " -> ";
                temp = temp->next;
            }
            cout << endl;
        }
        cout << "\n\n\n"
             << endl;
    }

    // Display all buildings
    void displayAllBuildings()
    {
        if (buildingCount == 0)
        {
            cout << "\nNo buildings in campus map!" << endl;
            return;
        }

        cout << "\n\nBUILDINGS: " << endl;
        for (int i = 0; i < buildingCount; i++)
        {
            cout << (i + 1) << ". " << buildings[i].buildingName << endl;
        }
        cout << "Total Buildings: " << buildingCount << endl;
        cout << "\n\n";
    }

    // HELPER Functions
    bool buildingExists(string name)
    {
        return findBuildingIndex(name) != -1;
    }

    int getBuildingCount()
    {
        return buildingCount;
    }

    // Get AVL root pointer for a building (for MODULE C integration)
    void *&getBuildingRoomsAVL(const string buildingName)
    {
        int index = findBuildingIndex(buildingName);
        if (index == -1)
        {
            // Return a static NULL pointer for invalid building
            static void *nullPtr = NULL;
            return nullPtr;
        }
        return buildings[index].roomsAVLRoot;
    }
};

#endif