#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 100
#define MAX_STACK 100
#define MAX_CATEGORIES 5

/* -------------------- CATEGORY ARRAY -------------------- */
char categories[MAX_CATEGORIES][20] = { 
    "WiFi",
    "Electricity",
    "Plumbing",
    "Cleanliness",
    "Other"
};

/* -------------------- COMPLAINT STRUCT -------------------- */
typedef struct {
    int id;
    char name[50];
    char room[10];
    int category;
    char description[200];
    int status;     // 0 = Pending, 1 = Resolved
} Complaint;

/* -------------------- LINKED LIST NODE -------------------- */
typedef struct Node {
    Complaint data;
    struct Node *next;
} Node;

/* Head and Tail pointers */
Node *head = NULL;
Node *tail = NULL;

/* -------------------- QUEUE -------------------- */
typedef struct {
    int items[MAX_QUEUE];
    int front, rear;
} Queue;

/* -------------------- STACK -------------------- */
typedef struct {
    int items[MAX_STACK];
    int top;
} Stack;

Queue pendingQueue;
Stack resolvedStack;

int ticketCounter = 1000;

/* -------------------- FUNCTION DECLARATIONS -------------------- */
void initQueue();
void initStack();

void enqueue(int id);
int dequeue();

void push(int id);
int pop();

void registerComplaint();
void processNextComplaint();
void reopenLastResolved();
void displayAllComplaints();

Node* findComplaintByID(int id);
int generateTicketID();

/* -------------------- MAIN -------------------- */
int main() {
    int choice;

    initQueue();
    initStack();

    do {
        printf("\n--- Campus Complaint Registration & Resolution System ---\n");
        printf("1. Register New Complaint\n");
        printf("2. Process Next Pending Complaint\n");
        printf("3. Reopen Last Resolved Complaint\n");
        printf("4. View All Complaints\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerComplaint(); break;
            case 2: processNextComplaint(); break;
            case 3: reopenLastResolved(); break;
            case 4: displayAllComplaints(); break;
            case 0: printf("Exiting system...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

/* -------------------- INITIALIZATION -------------------- */
void initQueue() {
    pendingQueue.front = pendingQueue.rear = -1;
}

void initStack() {
    resolvedStack.top = -1;
}

/* -------------------- QUEUE FUNCTIONS -------------------- */
void enqueue(int id) {
    if (pendingQueue.rear == MAX_QUEUE - 1)
        return;

    if (pendingQueue.front == -1)
        pendingQueue.front = 0;

    pendingQueue.items[++pendingQueue.rear] = id;
}

int dequeue() {
    if (pendingQueue.front == -1)
        return -1;

    int id = pendingQueue.items[pendingQueue.front++];

    if (pendingQueue.front > pendingQueue.rear)
        pendingQueue.front = pendingQueue.rear = -1;

    return id;
}

/* -------------------- STACK FUNCTIONS -------------------- */
void push(int id) {
    if (resolvedStack.top == MAX_STACK - 1)
        return;

    resolvedStack.items[++resolvedStack.top] = id;
}

int pop() {
    if (resolvedStack.top == -1)
        return -1;

    return resolvedStack.items[resolvedStack.top--];
}

/* -------------------- CORE LOGIC -------------------- */
int generateTicketID() {
    return ++ticketCounter;
}

/* FIFO insertion using tail pointer */
void registerComplaint() {
    Complaint c;
    c.id = generateTicketID();

    printf("Enter Name: ");
    scanf(" %[^\n]", c.name);

    printf("Enter Room No: ");
    scanf("%s", c.room);

    printf("Select Category:\n");
    for (int i = 0; i < MAX_CATEGORIES; i++)
        printf("%d. %s\n", i, categories[i]);

    scanf("%d", &c.category);

    printf("Enter Description: ");
    scanf(" %[^\n]", c.description);

    c.status = 0;

    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = c;
    newNode->next = NULL;

    if (head == NULL) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }

    enqueue(c.id);

    printf("Complaint registered successfully.\n");
    printf("Ticket ID: %d\n", c.id);
}

void processNextComplaint() {
    int id = dequeue();
    if (id == -1) {
        printf("No pending complaints.\n");
        return;
    }

    Node *temp = findComplaintByID(id);
    if (temp) {
        temp->data.status = 1;
        push(id);
        printf("Complaint ID %d resolved.\n", id);
    }
}

void reopenLastResolved() {
    int id = pop();
    if (id == -1) {
        printf("No resolved complaints to reopen.\n");
        return;
    }

    Node *temp = findComplaintByID(id);
    if (temp) {
        temp->data.status = 0;
        enqueue(id);
        printf("Complaint ID %d reopened.\n", id);
    }
}

Node* findComplaintByID(int id) {
    Node *temp = head;
    while (temp) {
        if (temp->data.id == id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void displayAllComplaints() {
    if (!head) {
        printf("No complaints found.\n");
        return;
    }

    Node *temp = head;
    while (temp) {
        printf("\nTicket ID: %d", temp->data.id);
        printf("\nName: %s", temp->data.name);
        printf("\nRoom: %s", temp->data.room);
        printf("\nCategory: %s", categories[temp->data.category]);
        printf("\nDescription: %s", temp->data.description);
        printf("\nStatus: %s\n",
               temp->data.status == 0 ? "Pending" : "RESOLVED");
        temp = temp->next;
    }
}
