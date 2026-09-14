#include <iostream>

using namespace std;

typedef struct tagNodeQ
{
    int vertice;
    struct tagNodeQ* next;
} nodeQ;

typedef struct tagQueue
{
    nodeQ* head;
} queue;

queue* createQueue()
{
    queue* q = (queue*)malloc(sizeof(queue));

    q->head = NULL;
    return q;
}

void push(queue* q, int vertice)
{
    nodeQ* newNode = (nodeQ*)malloc(sizeof(nodeQ));
    newNode->vertice = vertice;
    newNode->next = NULL;

    if (q->head == NULL)
    {
        q->head = newNode;
    }
    else
    {
        nodeQ* temp = q->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

int pop(queue* q)
{
    if (q->head == NULL)
    {
        cout << "Queue is empty!" << endl;
        return -1;
    }

    nodeQ* temp = q->head;
    int vertice = temp->vertice;
    q->head = q->head->next;
    free(temp);
    return vertice;
}

void freeQueue(queue* q)
{
    while (q->head != NULL)
    {
        pop(q);
    }
    free(q);
}