#ifndef COMPLAINTS_H
#define COMPLAINTS_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class Complaint
{
public:
    int ticketID;
    string raisedBy;
    string building;
    string roomID;
    string description;
    string timestamp;
    Complaint *next;

    Complaint()
    {
        ticketID = 0;
        raisedBy = "";
        building = "";
        roomID = "";
        description = "";
        timestamp = "";
        next = NULL;
    }
    Complaint(int tid, string rb, string b, string ri, string d)
    {
        ticketID = tid;
        raisedBy = rb;
        building = b;
        roomID = ri;
        description = d;

        // Get current time
        time_t now = time(nullptr);

        // Convert to tm structure (local time)
        tm *tinfo = localtime(&now);

        // Format time safely (works in ALL compilers)
        char buffer[50];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tinfo);

        timestamp = buffer; // Assign formatted string
        next = NULL;
    }
};

class ComplaintManager
{
private:
    Complaint *head;
    Complaint *tail;
    int complaintCount;
    int nextTicketID;

public:
    ComplaintManager()
    {
        head = NULL;
        tail = NULL;
        complaintCount = 0;
        nextTicketID = 1001; // Starting ticket ID
    }

    ~ComplaintManager()
    {
        while (head != NULL)
        {
            Complaint *temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Enqueue - Add new complaint
    void enqueue(string raisedBy, string building, string roomID, string description)
    {
        // TODO: When MODULE A implemented, validate user exists
        // TODO: When MODULE B implemented, validate building exists
        // TODO: When MODULE C implemented, validate room exists

        Complaint *newComplaint = new Complaint(nextTicketID++, raisedBy, building, roomID, description);

        if (head == NULL)
        {
            head = tail = newComplaint;
        }
        else
        {
            tail->next = newComplaint;
            tail = newComplaint;
        }

        complaintCount++;
        cout << "Complaint registered successfully!" << endl;
        cout << "Ticket ID: " << newComplaint->ticketID << endl;
        cout << "Timestamp: " << newComplaint->timestamp << endl;
    }

    // Dequeue - Process next complaint
    Complaint *dequeue()
    {
        if (head == NULL)
        {
            cout << "No complaints to process!" << endl;
            return NULL;
        }

        Complaint *temp = head;
        head = head->next;

        if (head == NULL)
        {
            tail = NULL;
        }

        complaintCount--;

        cout << "\n========== Processing Complaint ==========" << endl;
        cout << "Ticket ID: " << temp->ticketID << endl;
        cout << "Raised By: " << temp->raisedBy << endl;
        cout << "Building: " << temp->building << endl;
        cout << "Room ID: " << temp->roomID << endl;
        cout << "Description: " << temp->description << endl;
        cout << "Timestamp: " << temp->timestamp << endl;
        cout << "Complaint processed and resolved!" << endl;
        cout << "=========================================\n"
             << endl;

        // Note: Caller should delete the returned complaint
        return temp;
    }

    // Peek - View next complaint without removing
    void peek()
    {
        if (head == NULL)
        {
            cout << "No complaints in queue!" << endl;
            return;
        }

        cout << "\n========== Next Complaint ==========" << endl;
        cout << "Ticket ID: " << head->ticketID << endl;
        cout << "Raised By: " << head->raisedBy << endl;
        cout << "Building: " << head->building << endl;
        cout << "Room ID: " << head->roomID << endl;
        cout << "Description: " << head->description << endl;
        cout << "Timestamp: " << head->timestamp << endl;
        cout << "====================================\n"
             << endl;
    }

    // Display all pending complaints
    void displayQueue()
    {
        if (head == NULL)
        {
            cout << "\nNo pending complaints!" << endl;
            return;
        }

        cout << "\n========== All Pending Complaints ==========" << endl;
        Complaint *temp = head;
        int count = 1;

        while (temp != NULL)
        {
            cout << "\n--- Complaint " << count << " ---" << endl;
            cout << "Ticket ID: " << temp->ticketID << endl;
            cout << "Raised By: " << temp->raisedBy << endl;
            cout << "Building: " << temp->building << endl;
            cout << "Room ID: " << temp->roomID << endl;
            cout << "Description: " << temp->description << endl;
            cout << "Timestamp: " << temp->timestamp << endl;

            temp = temp->next;
            count++;
        }
        cout << "\nTotal Pending: " << complaintCount << endl;
        cout << "============================================\n"
             << endl;
    }

    // Display complaint statistics
    void displayStatistics()
    {
        cout << "\n========== Complaint Statistics ==========" << endl;
        cout << "Total Pending Complaints: " << complaintCount << endl;
        cout << "Next Ticket ID: " << nextTicketID << endl;
        cout << "Total Complaints Registered: " << (nextTicketID - 1001) << endl;
        cout << "Total Complaints Processed: " << (nextTicketID - 1001 - complaintCount) << endl;
        cout << "==========================================\n"
             << endl;
    }

    // Remove complaints by building (for MODULE B integration)
    void removeComplaintsByBuilding(string buildingName)
    {
        if (head == NULL)
            return;

        // Remove from head
        while (head != NULL && head->building == buildingName)
        {
            Complaint *temp = head;
            head = head->next;
            delete temp;
            complaintCount--;
        }

        if (head == NULL)
        {
            tail = NULL;
            return;
        }

        // Remove from middle/end
        Complaint *current = head;
        while (current->next != NULL)
        {
            if (current->next->building == buildingName)
            {
                Complaint *temp = current->next;
                current->next = temp->next;

                if (temp == tail)
                {
                    tail = current;
                }

                delete temp;
                complaintCount--;
            }
            else
            {
                current = current->next;
            }
        }

        cout << "All complaints for building '" << buildingName << "' removed." << endl;
    }

    // Remove complaints by user (for MODULE A integration)
    void removeComplaintsByUser(string userName)
    {
        if (head == NULL)
            return;

        // Remove from head
        while (head != NULL && head->raisedBy == userName)
        {
            Complaint *temp = head;
            head = head->next;
            delete temp;
            complaintCount--;
        }

        if (head == NULL)
        {
            tail = NULL;
            return;
        }

        // Remove from middle/end
        Complaint *current = head;
        while (current->next != NULL)
        {
            if (current->next->raisedBy == userName)
            {
                Complaint *temp = current->next;
                current->next = temp->next;

                if (temp == tail)
                {
                    tail = current;
                }

                delete temp;
                complaintCount--;
            }
            else
            {
                current = current->next;
            }
        }
    }

    // Check if queue is empty
    bool isEmpty()
    {
        return head == NULL;
    }

    // Get complaint count
    int getComplaintCount()
    {
        return complaintCount;
    }
};

#endif // COMPLAINTS_H