#include <iostream>
#include <string>
#include <limits>
#include "PeopleDirectory.h"
#include "CampusMap.h"
#include "Room&Scheduling(AVL).h"
#include "Complaints.h"
#include "Message.h"

using namespace std;

// Global current logged in user
string currentUser = "";
string currentUserRole = "";

// Forward declarations
void mainMenu(PeopleDirectory &users, CampusMap &campus, ComplaintManager &complaints);
void userMenu(PeopleDirectory &users, CampusMap &campus, ComplaintManager &complaints);
void campusMapMenu(CampusMap &campus);
void roomsSchedulingMenu(CampusMap &campus, PeopleDirectory &users);
void complaintsMenu(ComplaintManager &complaints, PeopleDirectory &users, CampusMap &campus);
void messagingMenu(PeopleDirectory &users);

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
// ---------------------------
// User & Authentication Menu new
// ---------------------------
void userMenu(PeopleDirectory &users, CampusMap &campus, ComplaintManager &complaints)
{
    int choice;
    string userName, password, role, department, email;
    string newPassword, newDepartment, newEmail;

    while (true)
    {
        cout << "\n========== User & Authentication Menu ==========" << endl;
        cout << "1. Register New User" << endl;
        cout << "2. Login" << endl;
        cout << "3. Update Profile" << endl;
        cout << "4. Delete User" << endl;
        cout << "5. Search User" << endl;
        cout << "6. Display All Users (buckets)" << endl;
        cout << "7. Back" << endl;
        cout << "===============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1: // Register
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can register new users." << endl;
                break;
            }
            cout << "Enter username: ";
            getline(cin, userName);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter role (student/teacher/admin): ";
            getline(cin, role);
            cout << "Enter department: ";
            getline(cin, department);
            cout << "Enter email: ";
            getline(cin, email);
            users.registerUser(userName, password, role, department, email);
            break;

        case 2: // Login
            cout << "Enter username: ";
            getline(cin, userName);
            cout << "Enter password: ";
            getline(cin, password);
            if (users.login(userName, password))
            {
                currentUser = userName;
                currentUserRole = users.getUserRole(userName);
            }
            break;

        case 3: // Update Profile
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can update profile." << endl;
                break;
            }
            cout << "Enter username: ";
            getline(cin, userName);
            cout << "Enter new password (leave empty to keep current): ";
            getline(cin, newPassword);
            cout << "Enter new department (leave empty to keep current): ";
            getline(cin, newDepartment);
            cout << "Enter new email (leave empty to keep current): ";
            getline(cin, newEmail);
            users.updateProfile(userName, newPassword, newDepartment, newEmail);
            break;

        case 4: // Delete User
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can delete users." << endl;
                break;
            }
            cout << "Enter username to delete: ";
            getline(cin, userName);
            complaints.removeComplaintsByUser(userName);
            users.deleteUser(userName);
            if (currentUser == userName)
            {
                currentUser = "";
                currentUserRole = "";
                cout << "You have been logged out." << endl;
            }
            break;

        case 5: // Search User
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can search users." << endl;
                break;
            }
            cout << "Enter username to search: ";
            getline(cin, userName);
            users.displayUserDetails(userName);
            break;

        case 6: // Display All Users
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can view all users." << endl;
                break;
            }
            users.displayAllUsers();
            break;

        case 7:
            return;

        default:
            cout << "Invalid choice! Try again." << endl;
        }
    }
}

// ---------------------------
// Campus Map Menu
// ---------------------------
void campusMapMenu(CampusMap &campus)
{
    int choice;
    string src, dest, relation, status;
    int distance;

    while (true)
    {
        cout << "\n========== Campus Map Menu ==========" << endl;
        cout << "1. Add Building" << endl;
        cout << "2. Remove Building" << endl;
        cout << "3. Add Path between buildings" << endl;
        cout << "4. Remove Path" << endl;
        cout << "5. Find Shortest Path (Dijkstra)" << endl;
        cout << "6. BFS from Building" << endl;
        cout << "7. DFS from Building" << endl;
        cout << "8. Display Adjacency List" << endl;
        cout << "9. Display All Buildings" << endl;
        cout << "10. Back" << endl;
        cout << "=====================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1: // Add Building
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can modify campus map." << endl;
                break;
            }
            cout << "Enter building name: ";
            getline(cin, src);
            campus.addBuilding(src);
            break;
        case 2: // Remove Building
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can modify campus map." << endl;
                break;
            }
            cout << "Enter building name to remove: ";
            getline(cin, src);
            campus.removeBuilding(src);
            break;
        case 3: // Add Path
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can modify campus map." << endl;
                break;
            }
        start:
            cout << "Enter source building: ";
            getline(cin, src);
            cout << "Enter destination building: ";
            getline(cin, dest);
            cout << "Enter distance (meters): ";
            cin >> distance;
            clearInputBuffer();
            cout << "Enter relation (road/walkway): ";
            getline(cin, relation);
            cout << "Enter status (open/closed): ";
            getline(cin, status);
            if (!campus.addPath(src, dest, distance, relation, status))
            {
                goto start;
            }
            break;
        case 4: // Remove Path
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can modify campus map." << endl;
                break;
            }
            cout << "Enter source building: ";
            getline(cin, src);
            cout << "Enter destination building: ";
            getline(cin, dest);
            campus.removePath(src, dest);
            break;

        case 5: // Dijkstra
            cout << "Enter source building: ";
            getline(cin, src);
            cout << "Enter destination building: ";
            getline(cin, dest);
            campus.shortestPath(src, dest);
            break;
        case 6: // BFS
            cout << "Enter starting building (for BFS): ";
            getline(cin, src);
            campus.BFS(src);
            break;
        case 7: // DFS
            cout << "Enter starting building (for DFS): ";
            getline(cin, src);
            campus.DFS(src);
            break;
        case 8: // Adjency list
            campus.displayAdjacencyList();
            break;
        case 9:
            campus.displayAllBuildings();
            break;
        case 10:
            return;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    }
}

// ---------------------------
// Rooms & Scheduling Menu new
// ---------------------------
void roomsSchedulingMenu(CampusMap &campus, PeopleDirectory &users)
{
    int choice;
    string buildingName, roomID, roomType, userName;
    int floorNumber;
    RoomsAVL *currentBuildingAVL = NULL;
    string selectedBuilding = "";

    while (true)
    {
        cout << "\n========== Rooms & Scheduling Menu ==========" << endl;
        if (selectedBuilding != "")
            cout << "Currently Selected Building: " << selectedBuilding << endl;
        cout << "1. Select Building\n2. Insert Room\n3. Delete Room\n4. Search Room\n5. Search Rooms by Type\n6. Reserve Room\n7. Cancel Reservation\n8. Print Inorder Traversal\n9. Print Preorder Traversal\n10. Print Postorder Traversal\n11. Display Tree Statistics\n12. Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
        { // Select building
            campus.displayAllBuildings();
            cout << "Enter building name to select: ";
            getline(cin, buildingName);
            if (!campus.buildingExists(buildingName))
            {
                cout << "Building does not exist!\n";
                break;
            }
            void *&avlPtr = campus.getBuildingRoomsAVL(buildingName);
            if (avlPtr == NULL)
                avlPtr = new RoomsAVL(buildingName);
            currentBuildingAVL = (RoomsAVL *)avlPtr;
            selectedBuilding = buildingName;
            cout << "Building '" << buildingName << "' selected!\n";
            break;
        }
        case 2:
        { // Insert Room
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can insert new rooms." << endl;
                break;
            }
            cout << "Enter room ID: ";
            getline(cin, roomID);
            cout << "Enter floor number: ";
            cin >> floorNumber;
            clearInputBuffer();
            cout << "Enter room type (classroom/lab/office): ";
            getline(cin, roomType);
            currentBuildingAVL->insertRoom(roomID, floorNumber, roomType);
            break;
        }
        case 3:
        { // Delete Room
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can modify rooms." << endl;
                break;
            }
            cout << "Enter room ID to delete: ";
            getline(cin, roomID);
            currentBuildingAVL->deleteRoom(roomID);
            break;
        }
        case 4:
        {
        case 5:
        { // Search Rooms
            if (choice == 4)
            {
                cout << "Enter room ID: ";
                getline(cin, roomID);
                currentBuildingAVL->displayRoomDetails(roomID);
            }
            else
            {
                cout << "Enter room type: ";
                getline(cin, roomType);
                currentBuildingAVL->searchRoomsByType(roomType);
            }
            break;
        }
        }
        case 6:
        { // Reserve Room
            cout << "Enter room ID to reserve: ";
            getline(cin, roomID);
            cout << "Enter username: ";
            getline(cin, userName);
            if (!users.userExists(userName))
            {
                cout << "User does not exist!\n";
                break;
            }
            currentBuildingAVL->reserveRoom(roomID, userName);
            users.updateLastBookedRoom(userName, selectedBuilding + " - " + roomID);
            break;
        }
        case 7:
        { // Cancel Reservation
            cout << "Enter room ID to cancel reservation: ";
            getline(cin, roomID);
            currentBuildingAVL->cancelReservation(roomID);
            break;
        }
        case 8:
        {
            currentBuildingAVL->printInorder();
            break;
        }
        case 9:
        {
            currentBuildingAVL->printPreorder();
            break;
        }
        case 10:
        {
            currentBuildingAVL->printPostorder();
            break;
        }
        case 11:
        {
            cout << "Height: " << currentBuildingAVL->getHeight() << ", Leaves: " << currentBuildingAVL->countLeaves()
                 << ", Internal: " << currentBuildingAVL->countInternalNodes() << endl;
            break;
        }
        case 12:
        {
            return;
        }
        default:
        {
            cout << "Invalid choice!\n";
            break;
        }
        }
    }
}

void complaintsMenu(ComplaintManager &complaints, PeopleDirectory &users, CampusMap &campus)
{
    int choice;
    string raisedBy, building, roomID, description;

    while (true)
    {
        cout << "\n========== Complaints Menu ==========" << endl;
        cout << "1. Submit New Complaint" << endl;
        cout << "2. Process Next Complaint" << endl;
        cout << "3. View Next Complaint" << endl;
        cout << "4. Display All Pending Complaints" << endl;
        cout << "5. Complaint Statistics" << endl;
        cout << "6. Back" << endl;
        cout << "=====================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1: // Submit
            if (currentUser == "")
            {
                cout << "Please login first!\n";
                break;
            }
            raisedBy = currentUser; // automatically the logged-in user
            cout << "Enter building name: ";
            getline(cin, building);
            if (!campus.buildingExists(building))
            {
                cout << "Building does not exist!\n";
                break;
            }
            cout << "Enter room ID: ";
            getline(cin, roomID);
            cout << "Enter complaint description: ";
            getline(cin, description);
            complaints.enqueue(raisedBy, building, roomID, description);
            break;

        case 2: // Process complaint
            if (currentUserRole != "admin")
            {
                cout << "Access Denied! Only admins can process complaints.\n";
                break;
            }
            {
                Complaint *processed = complaints.dequeue();
                if (processed != NULL)
                {
                    delete processed;
                }
            }
            break;

        case 3: // View next
            complaints.peek();
            break;

        case 4: // Display all
            complaints.displayQueue();
            break;

        case 5: // Statistics
            complaints.displayStatistics();
            break;

        case 6:
            return;

        default:
            cout << "Invalid choice!\n";
            break;
        }
    }
}

void messagingMenu(PeopleDirectory &users)
{
    int choice;
    string toUser, messageText;

    static MessageManager *userMessages = nullptr;

    while (true)
    {
        cout << "\n========== Messaging Menu ==========" << endl;
        if (currentUser != "")
            cout << "Logged in as: " << currentUser << endl;
        else
        {
            cout << "Not logged in!\n";
            break;
        }

        cout << "1. Send Message to User" << endl;
        cout << "2. View Latest Message in Conversation" << endl;
        cout << "3. Pop Latest Message" << endl;
        cout << "4. Display Conversation" << endl;
        cout << "5. Display All Conversations" << endl;
        cout << "6. Back" << endl;
        cout << "====================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        if (userMessages == nullptr)
            userMessages = new MessageManager(currentUser);

        switch (choice)
        {
        case 1: // Send message
            cout << "Enter recipient username: ";
            getline(cin, toUser);
            if (!users.userExists(toUser))
            {
                cout << "User does not exist!\n";
                break;
            }
            cout << "Enter message: ";
            getline(cin, messageText);
            userMessages->sendMessage(currentUser, toUser, messageText);
            break;

        case 2: // View latest
            cout << "Enter friend's username: ";
            getline(cin, toUser);
            userMessages->viewLatestMessage(toUser);
            break;

        case 3: // Pop latest
            cout << "Enter friend's username: ";
            getline(cin, toUser);
            {
                Message *popped = userMessages->popLatestMessage(toUser);
                if (popped != nullptr)
                {
                    delete popped;
                }
            }
            break;

        case 4: // Display conversation
            cout << "Enter friend's username: ";
            getline(cin, toUser);
            userMessages->displayConversation(toUser);
            break;

        case 5: // Display all conversations
            userMessages->displayAllConversations();
            break;

        case 6:
            return;

        default:
            cout << "Invalid choice!\n";
            break;
        }
    }
}

void mainMenu(PeopleDirectory &users, CampusMap &campus, ComplaintManager &complaints)
{
    int choice;

    while (true)
    {
        cout << "\n================ MAIN MENU =================" << endl;
        if (currentUser != "")
        {
            cout << "Logged in: " << currentUser << " (" << currentUserRole << ")" << endl;
        }
        else
        {
            cout << "\nGo to the User & Authentication and login\n";
        }
        cout << "1. User & Authentication (Hash Table)" << endl;
        cout << "2. Campus Map (Graph)" << endl;
        cout << "3. Rooms & Scheduling (AVL)" << endl;
        cout << "4. Complaints (Queue)" << endl;
        cout << "5. Messaging (Stacks)" << endl;
        cout << "6. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            userMenu(users, campus, complaints);
            break;
        case 2:
            campusMapMenu(campus);
            break;
        case 3:
            roomsSchedulingMenu(campus, users);
            break;
        case 4:
            complaintsMenu(complaints, users, campus);
            break;
        case 5:
            messagingMenu(users);
            break;
        case 6:
            cout << "\nThank you for using University Management System! Goodbye!\n";
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    }
}

int main()
{
    cout << "===============================================================================================" << endl;
    cout << "                                 UNIVERSITY MANAGEMENT SYSTEM                                  " << endl;
    cout << "===============================================================================================" << endl;
    cout << "                      National University of Computer & Emerging Sciences                      " << endl;
    cout << "                                 Faisalabad - Chiniot Campus                                   " << endl;
    cout << "===============================================================================================" << endl;

    // Initialize system managers
    PeopleDirectory users(11);   // Hash Table
    CampusMap campus(10);        // Graph
    ComplaintManager complaints; // Queue

    int n;
    cout << "\n========================= Initialize Current Strcture of University =======================\n";
    // -----------------------------------------------------------
    // 1. Register Users from User Input
    // -----------------------------------------------------------
    cout << "How many users do you want to register? ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline

    for (int i = 0; i < n; i++)
    {
        cout << "\n--- Enter User " << i + 1 << " Details ---\n";

        string username, password, role, department, email;

        cout << "Username: ";
        getline(cin, username);

        cout << "Password: ";
        getline(cin, password);

        cout << "Role (student/teacher/admin): ";
        getline(cin, role);

        cout << "Department: ";
        getline(cin, department);

        cout << "Email: ";
        getline(cin, email);

        users.registerUser(username, password, role, department, email);
    }

    // -----------------------------------------------------------
    // 2. Add Buildings from User Input
    // -----------------------------------------------------------
    cout << "\nHow many buildings do you want to add? ";
    cin >> n;
    cin.ignore(); // clear newline buffer

    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter name of Building " << i + 1 << ": ";
        string b;
        getline(cin, b);

        campus.addBuilding(b);
    }

    // -----------------------------------------------------------
    // 3. Add Paths Between Buildings (User Input)
    // -----------------------------------------------------------
    cout << "\nHow many paths do you want to add? ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++)
    {
        cout << "\n--- Enter Path " << i + 1 << " Details ---\n";

        string b1, b2, type, status;
        int dist;

        cout << "From Building: ";
        getline(cin, b1);

        cout << "To Building: ";
        getline(cin, b2);

        cout << "Distance (meters): ";
        cin >> dist;
        cin.ignore();

        cout << "Path type (road/walkway): ";
        getline(cin, type);

        cout << "Status (open/closed): ";
        getline(cin, status);

        if (campus.addPath(b1, b2, dist, type, status) == false)
        {
            i = i - 1;
            continue;
        }
    }

    // -----------------------------------------------------------
    // Start Interactive Main Menu
    // -----------------------------------------------------------
    cout << "\nAll data recorded successfully!\n"
         << endl;

    mainMenu(users, campus, complaints);

    cout << "\nExiting system... Goodbye!\n";
    return 0;
}