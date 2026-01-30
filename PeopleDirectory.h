#ifndef PEOPLE_DIRECTORY_H
#define PEOPLE_DIRECTORY_H

#include <iostream>
#include <string>

using namespace std;

// Hash Table Entry Node
struct HashEntry
{
    string userName; // unique key
    string password;
    string role; // student | teacher | admin
    string department;
    string email;
    string lastBookedRoom; // For integration with MODULE C
    HashEntry *next;       // chaining for collision handling

    HashEntry(string un, string pw, string r, string dept, string em)
    {
        userName = un;
        password = pw;
        role = r;
        department = dept;
        email = em;
        lastBookedRoom = "";
        next = NULL;
    }
};

class PeopleDirectory
{
private:
    HashEntry **table;
    int tableSize;
    int itemCount;
    const double LOAD_FACTOR_THRESHOLD = 0.75;

    // Hash function
    int hashFunction(string key)
    {
        int hash = 0;
        for (int i = 0; i < key.length(); i++)
        {
            hash = (hash * 31 + key[i]) % tableSize;
        }
        return hash;
    }

    // Check if number is prime
    bool isPrime(int n)
    {
        if (n <= 1)
        {
            return false;
        }
        if (n <= 3)
        {
            return true;
        }
        if (n % 2 == 0 || n % 3 == 0)
        {
            return false;
        }
        for (int i = 5; i * i <= n; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Get next prime number
    int getNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    // Resize and rehash table if full
    void resize()
    {
        int oldSize = tableSize;
        HashEntry **oldTable = table;

        // Double size and find next prime
        tableSize = getNextPrime(tableSize * 2);
        table = new HashEntry *[tableSize];

        // Initialize new table
        for (int i = 0; i < tableSize; i++)
        {
            table[i] = NULL;
        }

        itemCount = 0;

        // Rehash all entries
        for (int i = 0; i < oldSize; i++)
        {
            HashEntry *current = oldTable[i];
            while (current != NULL)
            {
                HashEntry *next = current->next;

                // Rehash into new table
                int newIndex = hashFunction(current->userName);
                current->next = table[newIndex];
                table[newIndex] = current;
                itemCount++;

                current = next;
            }
        }

        delete[] oldTable;
        cout << "Hash table resized to " << tableSize << endl;
    }

    // Check if resize is needed
    void checkLoadFactor()
    {
        double loadFactor = (double)itemCount / tableSize;
        if (loadFactor > LOAD_FACTOR_THRESHOLD)
        {
            resize();
        }
    }

public:
    PeopleDirectory(int size = 11)
    {
        tableSize = getNextPrime(size);
        table = new HashEntry *[tableSize];
        itemCount = 0;

        for (int i = 0; i < tableSize; i++)
        {
            table[i] = NULL;
        }
    }

    ~PeopleDirectory()
    {
        for (int i = 0; i < tableSize; i++)
        {
            HashEntry *current = table[i];
            while (current != NULL)
            {
                HashEntry *temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // Register new user
    bool registerUser(string userName, string password, string role, string department, string email)
    {
        // Check if username already exists then not add it
        if (searchUser(userName) != NULL)
        {
            cout << "Username '" << userName << "' already exists!" << endl;
            return false;
        }

        // Validate role
        if (role != "student" && role != "teacher" && role != "admin")
        {
            cout << "Invalid role! Must be student, teacher, or admin." << endl;
            return false;
        }

        int index = hashFunction(userName);
        HashEntry *newEntry = new HashEntry(userName, password, role, department, email);

        // Insert at beginning of chain
        newEntry->next = table[index];
        table[index] = newEntry;
        itemCount++;

        cout << "User '" << userName << "' registered successfully as " << role << "!" << endl;

        checkLoadFactor();
        return true;
    }

    // Login authentication
    bool login(string userName, string password)
    {
        HashEntry *user = searchUser(userName);

        if (user == NULL)
        {
            cout << "User '" << userName << "' not found!" << endl;
            return false;
        }

        if (user->password != password)
        {
            cout << "Incorrect password!" << endl;
            return false;
        }

        cout << "Login successful! Welcome " << userName << " (" << user->role << ")" << endl;
        return true;
    }

    // Search for user
    HashEntry *searchUser(string userName)
    {
        int index = hashFunction(userName);
        HashEntry *current = table[index];

        while (current != NULL)
        {
            if (current->userName == userName)
            {
                return current;
            }
            current = current->next;
        }

        return NULL;
    }

    // Update user profile
    bool updateProfile(string userName, string newPassword, string newDepartment, string newEmail)
    {
        HashEntry *user = searchUser(userName);

        if (user == NULL)
        {
            cout << "User '" << userName << "' not found!" << endl;
            return false;
        }

        if (newPassword != "")
        {
            user->password = newPassword;
        }
        if (newDepartment != "")
        {
            user->department = newDepartment;
        }
        if (newEmail != "")
        {
            user->email = newEmail;
        }

        cout << "Profile updated successfully for " << userName << "!" << endl;
        return true;
    }

    // Delete user
    bool deleteUser(string userName)
    {
        int index = hashFunction(userName);
        HashEntry *current = table[index];
        HashEntry *prev = NULL;

        while (current != NULL)
        {
            if (current->userName == userName)
            {
                if (prev == NULL)
                {
                    table[index] = current->next;
                }
                else
                {
                    prev->next = current->next;
                }

                delete current;
                itemCount--;
                cout << "User '" << userName << "' deleted successfully!" << endl;
                return true;
            }
            prev = current;
            current = current->next;
        }

        cout << "User '" << userName << "' not found!" << endl;
        return false;
    }

    // Display all users (show buckets)
    void displayAllUsers()
    {
        if (itemCount == 0)
        {
            cout << "\nNo users in the system!" << endl;
            return;
        }

        cout << "\n========== All Users (Hash Table) ==========" << endl;
        cout << "Table Size: " << tableSize << " | Total Users: " << itemCount << endl;
        cout << "Load Factor: " << (double)itemCount / tableSize << endl;
        cout << "============================================" << endl;

        for (int i = 0; i < tableSize; i++)
        {
            if (table[i] != NULL)
            {
                cout << "\nBucket [" << i << "]: ";
                HashEntry *current = table[i];
                int count = 0;
                while (current != NULL)
                {
                    count++;
                    cout << "\n  " << count << ". Username: " << current->userName
                         << " | Role: " << current->role
                         << " | Dept: " << current->department
                         << " | Email: " << current->email;
                    if (current->lastBookedRoom != "")
                    {
                        cout << " | Last Booked: " << current->lastBookedRoom;
                    }
                    current = current->next;
                }
            }
        }
        cout << "\n============================================\n"
             << endl;
    }

    // Display user details
    void displayUserDetails(string userName)
    {
        HashEntry *user = searchUser(userName);

        if (user == NULL)
        {
            cout << "User '" << userName << "' not found!" << endl;
            return;
        }

        cout << "\n========== User Details ==========" << endl;
        cout << "Username: " << user->userName << endl;
        cout << "Role: " << user->role << endl;
        cout << "Department: " << user->department << endl;
        cout << "Email: " << user->email << endl;
        cout << "Last Booked Room: " << (user->lastBookedRoom != "" ? user->lastBookedRoom : "None") << endl;
        cout << "==================================\n"
             << endl;
    }

    // Update last booked room (for MODULE C integration)
    bool updateLastBookedRoom(string userName, string roomInfo)
    {
        HashEntry *user = searchUser(userName);

        if (user == NULL)
        {
            return false;
        }

        user->lastBookedRoom = roomInfo;
        return true;
    }

    // Check if user exists
    bool userExists(string userName)
    {
        return searchUser(userName) != NULL;
    }

    // Get user role
    string getUserRole(string userName)
    {
        HashEntry *user = searchUser(userName);
        if (user == NULL)
            return "";
        return user->role;
    }

    // Get table statistics
    void displayStatistics()
    {
        cout << "\n========== Hash Table Statistics ==========" << endl;
        cout << "Table Size: " << tableSize << endl;
        cout << "Total Users: " << itemCount << endl;
        cout << "Load Factor: " << (double)itemCount / tableSize << endl;

        int emptyBuckets = 0;
        int maxChainLength = 0;

        for (int i = 0; i < tableSize; i++)
        {
            if (table[i] == NULL)
            {
                emptyBuckets++;
            }
            else
            {
                int chainLength = 0;
                HashEntry *current = table[i];
                while (current != NULL)
                {
                    chainLength++;
                    current = current->next;
                }
                if (chainLength > maxChainLength)
                {
                    maxChainLength = chainLength;
                }
            }
        }

        cout << "Empty Buckets: " << emptyBuckets << endl;
        cout << "Used Buckets: " << (tableSize - emptyBuckets) << endl;
        cout << "Max Chain Length: " << maxChainLength << endl;
        cout << "==========================================\n"
             << endl;
    }
};

#endif // PEOPLE_DIRECTORY_H