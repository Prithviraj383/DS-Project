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
    int priority;        
    char description[200];
    int status;          
} Complaint;

/* -------------------- LINKED LIST NODE -------------------- */
typedef struct Node {
    Complaint data;
    struct Node *next;
} Node;

Node *head = NULL;
Node *tail = NULL;

/* -------------------- QUEUE STRUCT -------------------- */
typedef struct {
    int items[MAX_QUEUE];
    int front, rear;
} Queue;

/* -------------------- STACK STRUCT -------------------- */
typedef struct {
    int items[MAX_STACK];
    int top;
} Stack;

/* Priority Queues */
Queue highQ, mediumQ, lowQ;
Stack resolvedStack;

int ticketCounter = 1000;

/* -------------------- FUNCTION DECLARATIONS -------------------- */
void initQueue(Queue *q);
void initStack(Stack *s);

void enqueue(Queue *q, int id);
int dequeue(Queue *q);
int isQueueEmpty(Queue *q);

void push(Stack *s, int id);
int pop(Stack *s);

int generateTicketID();
Node* findComplaintByID(int id);

void registerComplaint();
void processNextComplaint();
void reopenLastResolved();
void displayAllComplaints();

/* -------------------- MAIN -------------------- */
int main() {
    int choice;

    initQueue(&highQ);
    initQueue(&mediumQ);
    initQueue(&lowQ);
    initStack(&resolvedStack);

    do {
        printf("\n--- Campus Complaint Registration & Resolution System ---\n");
        printf("1. Register New Complaint\n");
        printf("2. Process Next Complaint (Priority Based)\n");
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
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

/* -------------------- INITIALIZATION -------------------- */
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

void initStack(Stack *s) {
    s->top = -1;
}

/* -------------------- QUEUE OPERATIONS -------------------- */
void enqueue(Queue *q, int id) {
    if (q->rear == MAX_QUEUE - 1) return;

    if (q->front == -1)
        q->front = 0;

    q->items[++q->rear] = id;
}

int dequeue(Queue *q) {
    if (q->front == -1) return -1;

    int id = q->items[q->front++];

    if (q->front > q->rear)
        q->front = q->rear = -1;

    return id;
}

int isQueueEmpty(Queue *q) {
    return q->front == -1;
}

/* -------------------- STACK OPERATIONS -------------------- */
void push(Stack *s, int id) {
    if (s->top == MAX_STACK - 1) return;
    s->items[++s->top] = id;
}

int pop(Stack *s) {
    if (s->top == -1) return -1;
    return s->items[s->top--];
}

/* -------------------- CORE LOGIC -------------------- */
int generateTicketID() {
    return ++ticketCounter;
}

/* FIFO Linked List Insertion */
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

    printf("Select Priority (0-Low, 1-Medium, 2-High): ");
    scanf("%d", &c.priority);

    printf("Enter Description: ");
    scanf(" %[^\n]", c.description);

    c.status = 0;

    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = c;
    newNode->next = NULL;

    if (!head)
        head = tail = newNode;
    else {
        tail->next = newNode;
        tail = newNode;
    }

    /* Enqueue based on priority */
    if (c.priority == 2)
        enqueue(&highQ, c.id);
    else if (c.priority == 1)
        enqueue(&mediumQ, c.id);
    else
        enqueue(&lowQ, c.id);

    printf("Complaint registered successfully.\n");
    printf("Ticket ID: %d\n", c.id);
}

/* Priority-based processing */
void processNextComplaint() {
    int id;

    if (!isQueueEmpty(&highQ))
        id = dequeue(&highQ);
    else if (!isQueueEmpty(&mediumQ))
        id = dequeue(&mediumQ);
    else if (!isQueueEmpty(&lowQ))
        id = dequeue(&lowQ);
    else {
        printf("No pending complaints.\n");
        return;
    }

    Node *temp = findComplaintByID(id);
    if (temp) {
        temp->data.status = 1;
        push(&resolvedStack, id);
        printf("Complaint ID %d resolved.\n", id);
    }
}

void reopenLastResolved() {
    int id = pop(&resolvedStack);
    if (id == -1) {
        printf("No resolved complaints to reopen.\n");
        return;
    }

    Node *temp = findComplaintByID(id);
    if (temp) {
        temp->data.status = 0;

        if (temp->data.priority == 2)
            enqueue(&highQ, id);
        else if (temp->data.priority == 1)
            enqueue(&mediumQ, id);
        else
            enqueue(&lowQ, id);

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
        printf("\nPriority: %s",
            temp->data.priority == 2 ? "High" :
            temp->data.priority == 1 ? "Medium" : "Low");
        printf("\nStatus: %s\n",
            temp->data.status == 0 ? "Pending" : "Resolved");
        temp = temp->next;
    }
}
