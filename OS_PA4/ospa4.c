//Name: Achyutha Santhoshi 
//CWID: A20314248
//CS 5323 Operating Systems 2 
//Programming Assignment 4
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>
bool dirty;
struct Page
{
    unsigned long value;
    struct Page *next;
};

struct Page *head = NULL;
struct Page *current = NULL;

struct Page* search_list(unsigned long value, struct Page **previous)
{
    struct Page *ptr = head;
    struct Page *temp = NULL;
    while (ptr != NULL)
    {
        if (ptr->value == value)
        {
	     if(previous)
            	*previous = temp;

	     return ptr;
        }
        else
        {
            temp = ptr;
            ptr = ptr->next;
        }
    }

    return NULL;
}

// create list a list
struct Page* create_list(unsigned long value)
{
    struct Page *ptr = (struct Page*) malloc(sizeof(struct Page));
    if (ptr == NULL)
    {
        fprintf(stderr, "Page creation failed!");
        return NULL;
    }
    ptr->value = value;
    ptr->next = NULL;
    head = current = ptr;
    return ptr;
}

struct Page* insert_front(unsigned long value)
{
    if(head == NULL)
    {
        return (create_list(value));
    }

    struct Page *ptr = (struct Page*)malloc(sizeof(struct Page));
    if (ptr == NULL)
    {
        fprintf(stderr, "Page creation failed!");
        return NULL;
    }
    ptr->value = value;
    ptr->next = head;
    head = ptr;
    return ptr;
}


struct Page* insert_tail(unsigned long value)
{
    if(head == NULL)
    {
        return (create_list(value));
    }

    struct Page *ptr = (struct Page*)malloc(sizeof(struct Page));
    if (ptr == NULL)
    {
        fprintf(stderr, "Page creation failed!");
        return NULL;
    }
    ptr->value = value;
    ptr->next = NULL;
    current->next = ptr;
    current = ptr;
    return ptr;
}

int remove_list(unsigned long value)
{
    struct Page *delet = NULL;
    struct Page *previous = NULL;

    delet = search_list(value, &previous);

    if(delet == NULL)
        return -1;

    else
    {
        if(previous != NULL)
            previous->next = delet->next;
        if(delet == current)
            current = previous;
        if (delet == head)
            head = delet->next;
    }

    free(delet);
    delet = NULL;
    return 0;
}

int fifo(int table_size, unsigned long page_request)
{
    struct Page *ptr = NULL;
    struct Page *previous = NULL; 
    int count = 0;
    ptr = search_list(page_request, &previous);
   
    // page fault
    if (ptr == NULL)
    {
        ptr = head;
	// Track count of size
        while (ptr != NULL)
        {
            ptr = ptr->next;
            count++;
        }

        if (count == table_size)
        {
            ptr = head;
	    insert_tail(page_request);
            remove_list(ptr->value);
            
        }
        // Insert at back
        else
        {
            insert_tail(page_request);
        }
	
	return 1;
    }

    return 0;
}

int lru(int table_size, unsigned long page_request)
{
    struct Page *ptr = NULL;
    struct Page *previous = NULL;
    ptr = search_list(page_request, &previous);
    int count = 0;

    if (ptr == NULL)
    {
        ptr = head;
        while (ptr != NULL)
        {
            ptr = ptr->next;
            count++;
        }
        while (count >= table_size)
        {
            remove_list(current->value);
            count--;
        }
        insert_front(page_request);

        return 1;
    }
    else
    {
        remove_list(page_request);
        insert_front(page_request);
    }
    return 0;
}





int main(int argc, char *argv[])
{
    unsigned long bit;
    unsigned long one;
    unsigned long two;
    unsigned long three;
    int countn = 0;
    char rw;
    int size=10;
    int table_size = atoi(argv[2]);
    int page_request;
    int num_requests = 0;
    unsigned long frame[50];
    int i = 0;
    FILE *fp = fopen(argv[1], "r");
    char buff[size];
    char str[128];
    int c =0, r=0, w=0;
    char *op;
    while(!feof(fp))
    {
        fscanf(fp,"%lx %c",&bit,&rw);
        //printf("%lx\n",bit);
        //printf ("%s\n", buff);
        //strcpy (str, buff);
        //strtok(str, " ");
        //printf("%s",str);
        //int buff1 = atoi(buff);
        //printf("%d\n", buff1);
        if(strcmp(argv[3],"fifo")==0){
        if(fifo(table_size,bit)) 
        {
              //printf("%lx\t", bit);
              if(countn==0)
              {
                  one=bit;
                  //printf("%lx this is one\n", one);
                  
              }
              if(countn==1)
              {
                  two=bit;
                  //printf("%lx is two\n", two);
              }
              if(countn==2)
              {
                  three=bit;
                  //printf("%lx is three\n", three);
              }
              countn++;
              if(countn==3)
              {
                  countn =0;
              }
              if(rw=='R')
              {
                  dirty =0;
                  r++;
              }
              if(rw=='W')
              {
                  dirty = 1;
                  w++;
              }
        }}
    
    if(strcmp(argv[3],"lru")==0){
    if(lru(table_size,bit)) 
        {
              //printf("%lx\t", bit);
              if(countn==0)
              {
                  one=bit;
                  //printf("%lx this is one\n", one);
                  
              }
              if(countn==1)
              {
                  two=bit;
                  //printf("%lx is two\n", two);
              }
              if(countn==2)
              {
                  three=bit;
                  //printf("%lx is three\n", three);
              }
              countn++;
              if(countn==3)
              {
                  countn =0;
              }
              if(rw=='R')
              {
                  r++;
              }
              if(rw=='W')
              {
                  w++;
              }
        }}
        
        
    }
    printf("Contents of page frames\n");
    printf("%lx\t%lx\t%lx",one, two, three);
    //printf("%d",countn);
    //printf("%d",(char)frame[i-1]);
    //printf("%s")
    printf("\nNumber of Reads: %d\n",r);
    printf("Number of Writes: %d",w);
    return 0;

}