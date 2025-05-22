#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

list createlist()
{
    list newnode = (list)malloc(sizeof(node));
    assert(newnode != NULL);
    newnode->data = NULL;

    newnode->next = NULL;
    return newnode;
}

queue createqueue()
{
    queue newqueue = (queue)malloc(sizeof(stqueue));
    assert(newqueue != NULL);
    newqueue->front = NULL;
    newqueue->rear = NULL;
    newqueue->size = 0;
    return newqueue;
}

queue enqueue(queue q, char *x)
{
    list newnode = (list)malloc(sizeof(node));
    assert(newnode != NULL);
    newnode->data = (char *)malloc(sizeof(char) * 4096);
    assert(newnode->data != NULL);
    strcpy(newnode->data, x);
    if (q->size == 0)
    {
        q->front = newnode;
        q->rear = newnode;
    }
    else
    {
        q->rear->next = newnode;
        q->rear = newnode;
    }
    q->size++;

    return q;
}

queue dequeue(queue q)
{
    if (q->size == 0)
    {
        printf("\033[1;31mNo Activities to dequeue!\n\033[0m");
        return q;
    }

    list tempnode = q->front;

    if (q->size == 1)
    {
        q->front = NULL;
        q->rear = NULL;
    }
    else
    {
        q->front = q->front->next;
    }
    q->size--;
    free(tempnode);

    return q;
}

void printqueue(queue q)
{
    if (q->size == 0)
    {
        printf("\033[1;31mNo Activities to display!\n\033[0m");
        return;
    }
    else
    {
        list temp = q->front;
        list temp2 = q->rear;
        while (temp != temp2)
        {
            printf("%s\n", temp->data);
            temp = temp->next;
        }
        printf("%s\n", temp->data);

        return;
    }
}

int givesize(queue q)
{
    return q->size;
}

void file_write(queue q,char * file_path)
{
    FILE *file = fopen(file_path, "w");
    if (file == NULL)
    {
        printf("\033[1;31mFailed to write in file!\n\033[0m");
        return;
    }
    list temp = q->front;
    list temp2 = q->rear;
    while (temp != temp2)
    {
        fprintf(file, "%s\n", temp->data);
        fflush(file);
        temp = temp->next;
    }

    fprintf(file, "%s\n", temp->data);
    fflush(file);
    fclose(file);
    return ;
}

void destroyqueue(queue q)
{
    list temp = q->front;
    while (temp != NULL)
    {
        list tmp = temp;
        temp = temp->next;
        free(tmp);
    }
    free(q);
}
