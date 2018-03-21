//
// Created by alex_pshe on 21.03.18.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
 
#define NUM_THREADS 50
 
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
 
struct Node
{
        void *data;
        struct Node *next;
};
 
struct Stack
{
        struct Node *head;
        int size;
        int finish;
};
 
struct Stack *CreateStack()
{
        struct Stack *res = (struct Stack *)malloc(sizeof(struct Stack));
        
        res->size = 0;
        res->head = NULL;
        res->finish = 0;
        printf("init stack\n");
        return res;
}
 
void Stack_Add(struct Stack *stack, void *data)
{
        struct Node *old_node = stack->head;
        struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
        new_node->data = data;
        new_node->next = old_node;
        stack->head = new_node;
        stack->size++;
}
 
void *Stack_Get(struct Stack *stack)
{
        void *res;
        if (stack->size == 0)
            {
                    printf("Stack is empty !!!11111\n");
                    return NULL;
                }
        res = stack->head->data;
        struct Node *old_node = stack->head;
        stack->head = stack->head->next;
        free(old_node);
        stack->size--;
        return res;
}
 
struct ThreadTask
{
        char **items;
        int i;
        int gap;
};
 
struct ThreadTask *ptt(char **items, int i, int gap)
{
        struct ThreadTask *res = (struct ThreadTask *)malloc(sizeof(struct ThreadTask));
        res->items = items;
        res->i = i;
        res->gap = gap;
        return res;
}
 
void splitedwork(struct ThreadTask *task)
{
        int j;
        char *x = task->items[task->i];
        for(j=task->i-task->gap; ((j >= 0) && strcmp(x, task->items[j]) < 0) ; j=j-task->gap) {
                task->items[j+task->gap] = task->items[j];
            }
        task->items[j+task->gap] = x;
     
}
 
void *threadwork(void *data)
{
        struct Stack *stack = (struct Stack *)data;
        //printf("loop");
        while (stack->finish == 0)
            {
                    while (stack->size > 0)
                        {
                                //printf("Ss %i \n", stack->size);//Если раскомментировать строку, кол-во ошибок резко уменьшается
                                struct ThreadTask *tmp =NULL;
                                pthread_mutex_lock( &mutex1 );
                                if (stack->size > 0)
                                    {
                                            tmp = (struct ThreadTask *)Stack_Get(stack);
                                        }
                                else
                                    {
                                            tmp = NULL;
                                        }
                                pthread_mutex_unlock( &mutex1 );
                                
                                if (tmp != NULL)
                                    {
                                            splitedwork(tmp);
                                        }
                            }
                    sleep(0);
                }
}
 
void shell(char **items, int count, int num_of_threads)//
{
        printf("start sort\n");
        struct Stack *stack = CreateStack();
        printf("Stack created\n");
        int i, j, gap, k;
        int a[4];
        char *x;
        printf("Creating thread arr\n");
        pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_of_threads);
        printf("thread arr created\n");
        struct ThreadTask *task;
     
        int t = 0;
        int rc;
     
        a[0]=5; a[1]=3; a[2]=2; a[3]=1;
        printf("init finish\n");
        stack->finish = 0;
     
        printf("Creating thread\n");
        //rc = pthread_create(&threads[0], NULL, threadwork, stack);
        for (; t < NUM_THREADS; t++)
            {
                    rc = pthread_create(&threads[t], NULL, threadwork, (void *)stack);
                    if (rc)
                        {
                                printf("error!");
                            }
                }
        //sleep(0);
        printf("thread created\n");
        for(k=0; k < 4; k++) {
                printf("Inside loop\n");
                gap = a[k];
                printf("gap=%i", gap);
                for(i=gap; i < count; ++i) {
                        //printf("stack size: %i\n", stack->size);
                        Stack_Add(stack, (void *)ptt(items, i, gap));
                        }
                    while (stack->size > 0)
                        {
                                sleep(0);
                            }
                    
            }
        stack->finish = 1;
        for (t = 0; t < NUM_THREADS; t++)
            {
                    pthread_join(threads[t], NULL);
                }
     
}
 
char **readfromfile(int *count)
{
        FILE *f = fopen("input", "r");
        char **str = NULL;
        char c = ' ';
        int i = 0;
        char *s;
        while( !feof(f) ) {
                s = realloc(NULL, 1);
                s[0] = 0;
                int j = 1;
                while (( !feof(f) ) && (c != '\n'))
                    {
                            j++;
                                c = fgetc(f);
                                s = realloc(s, j);
                                s[j - 2] = c;
                                s[j - 1] = 0;
                            }
                if (!feof(f)) {
                        c = ' ';
                        i++;
                        str = realloc(str, sizeof(char*) * i);
                        //printf("%i\n", i);
                        str[i - 1] = s;
                        }
                }
        fclose(f);
        *count = i;
        return str;
}
 
void printarr(char **str, int count)
{
        int i = 0;
        for (;i < count; i++)
                printf("%i %s", i, str[i]);
        //printf("\n");
}
 
int main(int argc, char** argv)
{
        int count = 0;
        char **str = readfromfile(&count);
        printarr(str, count);
        shell(str, count, NUM_THREADS);
        printf("end sort\n");
        printarr(str, count);
        
        return 0;
}
