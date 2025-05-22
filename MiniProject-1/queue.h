#ifndef _queue_h_
#define _queue_h_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct process
{
    char name[4096];
    int id;
}process;

typedef struct node
{
    char *data;
    struct node *next;
} node;

typedef node *list;

typedef struct stqueue
{
    list front;
    list rear;
    int size;
} stqueue;

typedef struct stqueue *queue;

extern list createlist();
extern queue createqueue();
extern queue enqueue(queue q, char* x);
extern queue dequeue(queue q);
extern void printqueue(queue q);
extern void destroyqueue(queue q);
extern int givesize(queue q);
extern void file_write(queue q,char *file_path);

#endif
