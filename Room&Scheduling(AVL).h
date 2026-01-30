#ifndef ROOMS_SCHEDULING_H
#define ROOMS_SCHEDULING_H

#include <iostream>
#include <string>

using namespace std;

struct AVLNode
{
    string roomID;
    int floorNumber;
    string roomType; // classroom/lab/office
    bool isReserved;
    string reservedBy; // userName who reserved it
    AVLNode *left;
    AVLNode *right;
    int height; // Height of node for AVL balancing

    AVLNode(string id, int floor, string type)
    {
        roomID = id;
        floorNumber = floor;
        roomType = type;
        isReserved = false;
        reservedBy = "";
        left = NULL;
        right = NULL;
        height = 1;
    }
};

class RoomsAVL
{
private:
    AVLNode *root;
    string buildingName; // name of building which the AVL belongs to

    int height(AVLNode *node)
    {
        if (node == NULL)
            return 0;
        return node->height;
    }

    int getBalance(AVLNode *node)
    {
        if (node == NULL)
            return 0;
        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLNode *node)
    {
        if (node == NULL)
            return;
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    AVLNode *rotateRight(AVLNode *y)
    {
        AVLNode *x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLNode *rotateLeft(AVLNode *x)
    {
        AVLNode *y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLNode *insertHelper(AVLNode *node, string roomID, int floorNumber, string roomType)
    {
        if (node == NULL)
        {
            return new AVLNode(roomID, floorNumber, roomType);
        }

        if (roomID < node->roomID)
        {
            node->left = insertHelper(node->left, roomID, floorNumber, roomType);
        }
        else if (roomID > node->roomID)
        {
            node->right = insertHelper(node->right, roomID, floorNumber, roomType);
        }
        else
        {
            cout << "Room '" << roomID << "' already exists!" << endl;
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && roomID < node->left->roomID)
        {
            return rotateRight(node);
        }
        if (balance < -1 && roomID > node->right->roomID)
        {
            return rotateLeft(node);
        }
        if (balance > 1 && roomID > node->left->roomID)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && roomID < node->right->roomID)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode *minValueNode(AVLNode *node)
    {
        AVLNode *current = node;
        while (current->left != NULL)
        {
            current = current->left;
        }
        return current;
    }

    AVLNode *deleteHelper(AVLNode *node, string roomID)
    {
        if (node == NULL)
        {
            return node;
        }

        if (roomID < node->roomID)
        {
            node->left = deleteHelper(node->left, roomID);
        }
        else if (roomID > node->roomID)
        {
            node->right = deleteHelper(node->right, roomID);
        }
        else
        {

            if (node->left == NULL || node->right == NULL)
            {
                AVLNode *temp = node->left ? node->left : node->right;

                if (temp == NULL)
                {
                    // No child case
                    temp = node;
                    node = NULL;
                }
                else
                {
                    // One child case
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                // Node with two children
                AVLNode *temp = minValueNode(node->right);

                node->roomID = temp->roomID;
                node->floorNumber = temp->floorNumber;
                node->roomType = temp->roomType;
                node->isReserved = temp->isReserved;
                node->reservedBy = temp->reservedBy;

                // inorder successor deletion
                node->right = deleteHelper(node->right, temp->roomID);
            }
        }

        if (node == NULL)
            return node;

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0)
        {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode *searchHelper(AVLNode *node, string roomID)
    {
        if (node == NULL || node->roomID == roomID)
            return node;

        if (roomID < node->roomID)
            return searchHelper(node->left, roomID);

        return searchHelper(node->right, roomID);
    }

    void searchByTypeHelper(AVLNode *node, string type)
    {
        if (node == NULL)
            return;

        searchByTypeHelper(node->left, type);

        if (node->roomType == type)
        {
            cout << "  Room ID: " << node->roomID
                 << " | Floor: " << node->floorNumber
                 << " | Type: " << node->roomType
                 << " | Reserved: " << (node->isReserved ? "Yes (by " + node->reservedBy + ")" : "No")
                 << endl;
        }

        searchByTypeHelper(node->right, type);
    }

    void inorderHelper(AVLNode *node)
    {
        if (node == NULL)
            return;

        inorderHelper(node->left);
        cout << "  Room ID: " << node->roomID
             << " | Floor: " << node->floorNumber
             << " | Type: " << node->roomType
             << " | Reserved: " << (node->isReserved ? "Yes (by " + node->reservedBy + ")" : "No")
             << endl;
        inorderHelper(node->right);
    }

    void preorderHelper(AVLNode *node)
    {
        if (node == NULL)
            return;

        cout << "  Room ID: " << node->roomID
             << " | Floor: " << node->floorNumber
             << " | Type: " << node->roomType
             << " | Reserved: " << (node->isReserved ? "Yes (by " + node->reservedBy + ")" : "No")
             << endl;
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorderHelper(AVLNode *node)
    {
        if (node == NULL)
            return;

        postorderHelper(node->left);
        postorderHelper(node->right);
        cout << "  Room ID: " << node->roomID
             << " | Floor: " << node->floorNumber
             << " | Type: " << node->roomType
             << " | Reserved: " << (node->isReserved ? "Yes (by " + node->reservedBy + ")" : "No")
             << endl;
    }

    int countLeavesHelper(AVLNode *node)
    {
        if (node == NULL)
            return 0;
        if (node->left == NULL && node->right == NULL)
            return 1;
        return countLeavesHelper(node->left) + countLeavesHelper(node->right);
    }

    int countInternalNodesHelper(AVLNode *node)
    {
        if (node == NULL || (node->left == NULL && node->right == NULL))
            return 0;
        return 1 + countInternalNodesHelper(node->left) + countInternalNodesHelper(node->right);
    }

    void deleteAllNodes(AVLNode *node)
    {
        if (node == NULL)
            return;
        deleteAllNodes(node->left);
        deleteAllNodes(node->right);
        delete node;
    }

    void cancelAllReservationsByUserHelper(AVLNode *node, string userName)
    {
        if (node == NULL)
            return;

        cancelAllReservationsByUserHelper(node->left, userName);

        if (node->isReserved && node->reservedBy == userName)
        {
            node->isReserved = false;
            node->reservedBy = "";
        }

        cancelAllReservationsByUserHelper(node->right, userName);
    }

public:
    RoomsAVL(string building = "")
    {
        root = NULL;
        buildingName = building;
    }

    ~RoomsAVL()
    {
        deleteAllNodes(root);
    }

    void setBuildingName(string building)
    {
        buildingName = building;
    }

    string getBuildingName()
    {
        return buildingName;
    }

    void insertRoom(string roomID, int floorNumber, string roomType)
    {
        root = insertHelper(root, roomID, floorNumber, roomType);
        cout << "Room '" << roomID << "' added to " << buildingName << " successfully!" << endl;
    }

    void deleteRoom(string roomID)
    {
        AVLNode *node = searchHelper(root, roomID);
        if (node == NULL)
        {
            cout << "Room '" << roomID << "' not found in " << buildingName << "!" << endl;
            return;
        }

        root = deleteHelper(root, roomID);
        cout << "Room '" << roomID << "' deleted from " << buildingName << " successfully!" << endl;
    }

    AVLNode *searchRoom(string roomID)
    {
        return searchHelper(root, roomID);
    }

    void searchRoomsByType(string type)
    {
        if (root == NULL)
        {
            cout << "No rooms in " << buildingName << "!" << endl;
            return;
        }

        cout << "\n========== Rooms of Type: " << type << " in " << buildingName << " ==========" << endl;
        searchByTypeHelper(root, type);
        cout << "========================================================\n"
             << endl;
    }

    // Reserve a room
    bool reserveRoom(string roomID, string userName)
    {
        AVLNode *node = searchHelper(root, roomID);

        if (node == NULL)
        {
            cout << "Room '" << roomID << "' not found in " << buildingName << "!" << endl;
            return false;
        }

        if (node->isReserved)
        {
            cout << "Room '" << roomID << "' is already reserved by " << node->reservedBy << "!" << endl;
            return false;
        }

        node->isReserved = true;
        node->reservedBy = userName;
        cout << "Room '" << roomID << "' in " << buildingName << " reserved successfully by " << userName << "!" << endl;
        return true;
    }

    // Cancel reservation
    bool cancelReservation(string roomID)
    {
        AVLNode *node = searchHelper(root, roomID);

        if (node == NULL)
        {
            cout << "Room '" << roomID << "' not found in " << buildingName << "!" << endl;
            return false;
        }

        if (!node->isReserved)
        {
            cout << "Room '" << roomID << "' is not reserved!" << endl;
            return false;
        }

        string previousUser = node->reservedBy;
        node->isReserved = false;
        node->reservedBy = "";
        cout << "Reservation cancelled for room '" << roomID << "' (was reserved by " << previousUser << ")" << endl;

        return true;
    }

    // Cancel all reservations by a specific user (for MODULE F integration)
    void cancelAllReservationsByUser(string userName)
    {
        if (root == NULL)
            return;
        cancelAllReservationsByUserHelper(root, userName);
    }

    // Print inorder traversal
    void printInorder()
    {
        if (root == NULL)
        {
            cout << "No rooms in " << buildingName << "!" << endl;
            return;
        }

        cout << "\n========== Inorder Traversal (" << buildingName << ") ==========" << endl;
        inorderHelper(root);
        cout << "===================================================\n"
             << endl;
    }

    // Print preorder traversal
    void printPreorder()
    {
        if (root == NULL)
        {
            cout << "No rooms in " << buildingName << "!" << endl;
            return;
        }

        cout << "\n========== Preorder Traversal (" << buildingName << ") ==========" << endl;
        preorderHelper(root);
        cout << "====================================================\n"
             << endl;
    }

    // Print postorder traversal
    void printPostorder()
    {
        if (root == NULL)
        {
            cout << "No rooms in " << buildingName << "!" << endl;
            return;
        }

        cout << "\n========== Postorder Traversal (" << buildingName << ") ==========" << endl;
        postorderHelper(root);
        cout << "=====================================================\n"
             << endl;
    }

    // Get height of tree
    int getHeight()
    {
        return height(root);
    }

    // Count leaf nodes
    int countLeaves()
    {
        return countLeavesHelper(root);
    }

    // Count internal nodes
    int countInternalNodes()
    {
        return countInternalNodesHelper(root);
    }

    // Check if room exists
    bool roomExists(string roomID)
    {
        return searchHelper(root, roomID) != NULL;
    }

    // Display room details
    void displayRoomDetails(string roomID)
    {
        AVLNode *node = searchHelper(root, roomID);

        if (node == NULL)
        {
            cout << "Room '" << roomID << "' not found in " << buildingName << "!" << endl;
            return;
        }

        cout << "\n========== Room Details ==========" << endl;
        cout << "Building: " << buildingName << endl;
        cout << "Room ID: " << node->roomID << endl;
        cout << "Floor Number: " << node->floorNumber << endl;
        cout << "Room Type: " << node->roomType << endl;
        cout << "Status: " << (node->isReserved ? "Reserved" : "Available") << endl;
        if (node->isReserved)
        {
            cout << "Reserved By: " << node->reservedBy << endl;
        }
        cout << "==================================\n"
             << endl;
    }

    // Get root (for testing/debugging)
    AVLNode *getRoot()
    {
        return root;
    }
};

#endif