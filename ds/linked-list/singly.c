#include <stdio.h>
#include <stdlib.h>
typedef struct Node Node;

struct Node
{
    int value;
    struct Node *next;
};

Node *create(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

Node *insert(Node *head, int value, int pos)
{
    if (pos == 0)
    {
        Node *newNode = create(value);
        newNode->next = head;
        return newNode;
    }
    Node *temp = head;
    int cnt = 0;
    while (cnt < pos - 1)
    {
        if (temp == NULL || temp->next == NULL)
        {
            printf("ERROR: Position does not Exists\n");
            return head;
        }
        temp = temp->next;
        cnt++;
    }
    Node *newNode = create(value);
    newNode->next = temp->next;
    temp->next = newNode;
    return head;
}

void append(Node *head, int value)
{
    Node *temp = head;
    Node *newNode = create(value);
    if (head == NULL)
    {
        printf("ERROR: Head is NULL\n");
        return;
    }

    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
    return;
}

void print(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        printf(" %d ", temp->value);
        if (temp->next != NULL)
        {
            printf("->");
        }
        temp = temp->next;
    }
    printf("\n");
}

void delete(Node *head, int pos)
{
    int cnt = 0;
    Node *temp = head;
    while (cnt < pos - 1)
    {
        if (temp == NULL || temp->next == NULL)
        {
            printf("Position does not exists\n");
        }
        temp = temp->next;
        cnt++;
    }
    if (temp->next == NULL)
    {
        printf("Position does not exists\n");
        return;
    }
    Node *nodetodel = temp->next;
    temp->next = nodetodel->next;
    free(nodetodel);
}

int main()
{
    Node *head = NULL;
    head = create(45);
    insert(head, 34, 1);
    append(head, 3);
    append(head, 23);
    print(head);
    delete(head, 2);
    print(head);
    return 0;
}