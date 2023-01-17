//Name: Achyutha Santhoshi
//CWID: A2031428
//CS 5323 Operating Systems 2
//Programming Assignment 1
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>
#include<string.h>
//Creating a struct to store id and line
typedef struct{

    int Threadid; 
    char *line;

}THREADSTRUCT; 

void* Frequency(void *p){ // our function that the threads will start

     THREADSTRUCT *Value = (THREADSTRUCT *) p; // pass the pointer p to the Value pointer
     char achyu[100];
     int c = 0, count[26] = {0}, x;
     //Displaying each line with its histogram
     printf("Line %d, %s\n", Value->Threadid, Value->line);
     int i =0;
     //Creating temporary string to store and calculate histogram
     strcpy(achyu, Value->line);
     while (achyu[c] != '\0') {
      //Calculating histogram
      if (achyu[c] >= 'a' && achyu[c] <= 'z') {
         x = achyu[c] - 'a';
         count[x]++;
      }

      c++;
   }
    printf("{");
   for (c = 0; c < 26; c++){
         printf(" '%c' : %d,", c + 'a', count[c]);
   }
    printf("}\n");
     //Operations for memory
     fflush(stdout);
     free(Value->line); // free memory
     pthread_exit(0);
     asm("push 0xdead"); //To Trash the program
     asm("ret");
}

int main(int argc, char **argv){
     int no_of_threads = atoi(argv[1]); //Read no of threads through command line prompt
     if(no_of_threads!=3){//Pop up error if its not greater than 2 and less than 4 and exit from propgram
     printf("Please input no of threads which should be greater than 2 and less than 4\n");
     exit(0);
     }
     THREADSTRUCT *Value =(THREADSTRUCT *) malloc(sizeof(int) + sizeof(char *)); // create a temporary struct

     char *filename = (char *) malloc(1000); //to store the filename given by user

     printf("Enter a file to read : ");

     fgets(filename, 1000, stdin); // reads up to 1000 chars 

     for(int i = 0; i < 1000; i++) // clears the \n from the input string
         if(filename[i] == '\n')
             filename[i] = 0;

     FILE * input = fopen(filename , "r"); // fopen to read the file

     int count = 0, i = 0; // count : count the bytes to be allocate, i is an index to count where we are on the line
     int start = 0; // start : remainder of where our line start
     char s = 0; // a temporary s  to store our input
     Value->Threadid = 1; // start from 1

     pthread_t threads[1000]; // allocating 10000 spaces to store our threads

     while(s != EOF){ // read until we reach the End-Of-File

        while(s != '\n'){ // Read the line
            s = fgetc(input);
            count++; // count the line's length
        }

        Value->line = (char *) malloc (count-1);
      fseek(input, start, 0); // go back to the start of the line
       start += count; // remember the end of the line
        s = 1;
        count = 0; // set count to 0

        while(s != '\n'){
            s = fgetc(input); // read from the file
            if(s != '\n') // if our input != \n
                 Value->line[i] = (char) s; // store it to Value->line(pointer) + i(offset)
            i++; // increase our index
        }
        i = 0; // zero our index
        s = 1; // delete the \n
        
        pthread_create(&threads[Value->Threadid], NULL, &Frequency,(void *) Value); // creating the thread

        sleep(1); //creating delay so that threads wont overlap
        s = fgetc(input); 
        fseek(input, start, 0); 

        Value->Threadid++; // increase the id index
}
        for(int i=0;i<no_of_threads;i++)
            pthread_join(threads[i], NULL);

}