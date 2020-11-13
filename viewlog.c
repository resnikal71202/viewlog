#include<stdio.h>
#include<stdlib.h>
#include<String.h>

#define MAX 1000

int open_file_r(FILE **fileptr, const char file_name[])
{
    *fileptr = fopen(file_name, "r");
    if(fileptr == NULL){
        return 1;
    }
    return 0;
}

int close_file(FILE **fileptr){
    if(fclose(*fileptr)!=0){
        return 1;
    }
    return 0;
}

int interpret_colum(char *colum, int size){
    // for (int i = 0; i <= size; i++)
    //     printf("%d", colum[i]);

    int time_tick;
    char *endptr;
    int working_task;

    time_tick = strtol(colum, &endptr, 10);
    // printf("%d", time_tick);
    char availible_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char *action;
    action = strpbrk(endptr, availible_chars);
    // printf(" %c", *action);
    working_task = strtol(++action, &endptr, 10);
    // printf(" %d\n", working_task);
    return 0;
}

int interpret_log(FILE **fileptr, char *action, int *time, int *task){
    int j = 0;
    int i = 0;
    char colum[40];
    while(1){
        if(feof(*fileptr)){
            break;
        }
        char c = fgetc(*fileptr);
        if(c == '\r'){
            interpret_colum(colum, i);
            memset(colum, 0, sizeof(colum));
            i = 0;
            j++;
        }
        else
        {
            colum[i++] = c;
        }
    }
    return 0;
}


int main(void)
{
    char action[MAX];
    int time[MAX];
    int task[MAX];

    FILE *fileptr = NULL;
    if (open_file_r(&fileptr, "log.txt"))
    {
        printf("failed opening file\nexiting...");
        return EXIT_FAILURE;
    }
    interpret_log(&fileptr, action, time, task);

    if(close_file(&fileptr)){
        printf("failed to close file\nexiting...");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}