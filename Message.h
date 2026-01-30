#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class Message
{
public:
    string fromUser;
    string toUser;
    string messageText;
    string timestamp;
    Message *next;
    Message(string fu, string tu, string mt)
    {
        fromUser = fu;
        toUser = tu;
        messageText = mt;

        // Get current timestamp
        time_t now = time(nullptr);
        tm *tinfo = localtime(&now);

        // Format timestamp in a portable way
        char buffer[50];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tinfo);

        timestamp = buffer; // assign to std::string

        next = NULL;
    }
};

// Each conversation has its own stack
class ConversationNode
{
public:
    string friendName;
    Message *top;           // Stack top for messages
    ConversationNode *next; // Next conversation in linked list

    ConversationNode(string fn)
    {
        friendName = fn;
        top = NULL;
        next = NULL;
    }
};

class MessageManager
{
private:
    string userName;
    ConversationNode *head; // Head of conversation list

    // Find or create conversation with a friend
    ConversationNode *getConversation(string friendName)
    {
        ConversationNode *temp = head;

        // Search for existing conversation
        while (temp != NULL)
        {
            if (temp->friendName == friendName)
            {
                return temp;
            }
            temp = temp->next;
        }

        // Create new conversation if not found
        ConversationNode *newNode = new ConversationNode(friendName);
        newNode->next = head;
        head = newNode;
        return newNode;
    }

public:
    MessageManager(string username = "")
    {
        this->userName = username;
        head = NULL;
    }

    ~MessageManager()
    {
        ConversationNode *convTemp = head;
        while (convTemp != NULL)
        {
            // Delete all messages in this conversation
            Message *msgTemp = convTemp->top;
            while (msgTemp != NULL)
            {
                Message *toDelete = msgTemp;
                msgTemp = msgTemp->next;
                delete toDelete;
            }

            ConversationNode *convToDelete = convTemp;
            convTemp = convTemp->next;
            delete convToDelete;
        }
    }

    void setUserName(string username)
    {
        userName = username;
    }

    string getUserName()
    {
        return userName;
    }

    // Send message (Push to stack)
    void sendMessage(string fromUser, string toUser, string messageText)
    {
        // TODO: When MODULE A implemented, validate both users exist

        if (messageText.empty())
        {
            cout << "Cannot send empty message!" << endl;
            return;
        }

        Message *newMessage = new Message(fromUser, toUser, messageText);
        ConversationNode *conv = getConversation(toUser);

        // Push to stack (add at beginning)
        newMessage->next = conv->top;
        conv->top = newMessage;

        cout << "Message sent to " << toUser << " successfully!" << endl;
    }

    // View latest message without removing (Peek)
    Message *viewLatestMessage(string friendName)
    {
        ConversationNode *conv = getConversation(friendName);

        if (conv->top == NULL)
        {
            cout << "No conversation started yet with " << friendName << endl;
            return NULL;
        }

        cout << "\n========== Latest Message ==========" << endl;
        cout << "From: " << conv->top->fromUser << endl;
        cout << "To: " << conv->top->toUser << endl;
        cout << "Message: " << conv->top->messageText << endl;
        cout << "Time: " << conv->top->timestamp << endl;
        cout << "====================================\n"
             << endl;

        return conv->top;
    }

    // Pop latest message (Remove from stack)
    Message *popLatestMessage(string friendName)
    {
        ConversationNode *conv = getConversation(friendName);

        if (conv->top == NULL)
        {
            cout << "No messages to pop with " << friendName << endl;
            return NULL;
        }

        Message *temp = conv->top;
        conv->top = conv->top->next;

        cout << "Latest message popped from conversation with " << friendName << endl;

        // Note: Caller should delete the returned message
        return temp;
    }

    // Display entire conversation (recent messages first - LIFO)
    void displayConversation(string friendName)
    {
        cout << "\n========== Conversation: " << userName << " <-> " << friendName << " ==========" << endl;

        ConversationNode *conv = getConversation(friendName);
        Message *temp = conv->top;

        if (temp == NULL)
        {
            cout << "No messages yet." << endl;
            cout << "=====================================================\n"
                 << endl;
            return;
        }

        int count = 1;
        while (temp != NULL)
        {
            cout << "\n--- Message " << count << " (Most Recent) ---" << endl;
            cout << "From: " << temp->fromUser << endl;
            cout << "To: " << temp->toUser << endl;
            cout << "Message: " << temp->messageText << endl;
            cout << "Time: " << temp->timestamp << endl;

            temp = temp->next;
            count++;
        }
        cout << "\n=====================================================\n"
             << endl;
    }

    // Display all conversations
    void displayAllConversations()
    {
        if (head == NULL)
        {
            cout << "\nNo conversations yet!" << endl;
            return;
        }

        cout << "\n========== All Conversations for " << userName << " ==========" << endl;
        ConversationNode *temp = head;
        int count = 1;

        while (temp != NULL)
        {
            int messageCount = 0;
            Message *msg = temp->top;
            while (msg != NULL)
            {
                messageCount++;
                msg = msg->next;
            }

            cout << count << ". " << temp->friendName
                 << " (" << messageCount << " messages)" << endl;

            temp = temp->next;
            count++;
        }
        cout << "========================================================\n"
             << endl;
    }

    // Delete all messages in a conversation
    void deleteConversation(string friendName)
    {
        ConversationNode *current = head;
        ConversationNode *prev = NULL;

        while (current != NULL)
        {
            if (current->friendName == friendName)
            {
                // Delete all messages
                Message *msgTemp = current->top;
                while (msgTemp != NULL)
                {
                    Message *toDelete = msgTemp;
                    msgTemp = msgTemp->next;
                    delete toDelete;
                }

                // Remove conversation node
                if (prev == NULL)
                {
                    head = current->next;
                }
                else
                {
                    prev->next = current->next;
                }

                delete current;
                cout << "Conversation with " << friendName << " deleted!" << endl;
                return;
            }
            prev = current;
            current = current->next;
        }

        cout << "No conversation found with " << friendName << endl;
    }

    // Get message count in a conversation
    int getMessageCount(string friendName)
    {
        ConversationNode *conv = getConversation(friendName);
        int count = 0;
        Message *temp = conv->top;

        while (temp != NULL)
        {
            count++;
            temp = temp->next;
        }

        return count;
    }

    // Check if conversation exists and has messages
    bool hasMessages(string friendName)
    {
        ConversationNode *temp = head;

        while (temp != NULL)
        {
            if (temp->friendName == friendName && temp->top != NULL)
            {
                return true;
            }
            temp = temp->next;
        }

        return false;
    }
};

// Notification Stack (separate from conversations)
class NotificationNode
{
public:
    string message;
    string timestamp;
    NotificationNode *next;
    NotificationNode(string msg)
    {
        message = msg;

        // Get current timestamp
        time_t now = time(nullptr);
        tm *tinfo = localtime(&now);

        // Format timestamp safely and portably
        char buffer[50];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tinfo);

        timestamp = buffer; // store formatted timestamp

        next = NULL;
    }
};

class NotificationManager
{
private:
    NotificationNode *top;
    string userName;

public:
    NotificationManager(string username = "")
    {
        top = NULL;
        userName = username;
    }

    ~NotificationManager()
    {
        while (top != NULL)
        {
            NotificationNode *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void setUserName(string username)
    {
        userName = username;
    }

    // Push notification
    void pushNotification(string message)
    {
        NotificationNode *newNode = new NotificationNode(message);
        newNode->next = top;
        top = newNode;
    }

    // Pop notification
    string popNotification()
    {
        if (top == NULL)
        {
            return "";
        }

        NotificationNode *temp = top;
        string message = temp->message;
        top = top->next;
        delete temp;

        return message;
    }

    // View latest notification
    void viewLatestNotification()
    {
        if (top == NULL)
        {
            cout << "No notifications!" << endl;
            return;
        }

        cout << "\n========== Latest Notification ==========" << endl;
        cout << "Message: " << top->message << endl;
        cout << "Time: " << top->timestamp << endl;
        cout << "=========================================\n"
             << endl;
    }

    // Display all notifications
    void displayAllNotifications()
    {
        if (top == NULL)
        {
            cout << "\nNo notifications for " << userName << "!" << endl;
            return;
        }

        cout << "\n========== Notifications for " << userName << " ==========" << endl;
        NotificationNode *temp = top;
        int count = 1;

        while (temp != NULL)
        {
            cout << count << ". " << temp->message << " (" << temp->timestamp << ")" << endl;
            temp = temp->next;
            count++;
        }
        cout << "==================================================\n"
             << endl;
    }

    bool isEmpty()
    {
        return top == NULL;
    }
};

#endif // MESSAGE_H