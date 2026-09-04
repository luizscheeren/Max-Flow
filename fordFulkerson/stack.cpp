#include <iostream>

using namespace std;

typedef struct tagNodeS
{
    int vertice;
    struct tagNodeS* next;
} nodeS;

typedef struct tagStack
{
    nodeS* top;
} stack;

stack* createStack()
{
    stack* s = (stack*)malloc(sizeof(stack));

    s->top = NULL;
    return s;
}

void push(stack* s, int vertice)
{
    nodeS* newNode = (nodeS*)malloc(sizeof(nodeS));
    newNode->vertice = vertice;
    newNode->next = s->top;
    s->top = newNode;
}

int pop(stack* s)
{
    if (s->top == NULL)
    {
        cout << "Stack is empty!" << endl;
        return -1;
    }

    nodeS* temp = s->top;
    int vertice = temp->vertice;
    s->top = s->top->next;
    free(temp);
    return vertice;
}

void freeStack(stack* s)
{
    while (s->top != NULL)
    {
        pop(s);
    }
    free(s);
}