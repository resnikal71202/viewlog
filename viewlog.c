#include <stdio.h>
#include <stdlib.h>
#include <String.h>

#define MAX 1000

struct colum
{
    int task;
    int time;
    char action;
};

int open_file_r(FILE **fileptr, const char file_name[])
{
    *fileptr = fopen(file_name, "r");
    if (fileptr == NULL)
    {
        return 1;
    }
    return 0;
}

int close_file(FILE **fileptr)
{
    if (fclose(*fileptr) != 0)
    {
        return 1;
    }
    return 0;
}

int interpret_colum(char *colum, int lenght, int working_colum, struct colum *arr)
{
    char *endptr;

    arr[working_colum].time = strtol(colum, &endptr, 10);
    // printf("%d", arr[working_colum].time);
    char availible_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    char *action_ptr;
    action_ptr = strpbrk(endptr, availible_chars);
    arr[working_colum].action = *action_ptr;
    // printf(" %c", arr[working_colum].action);
    arr[working_colum].task = strtol(++(action_ptr), &endptr, 10);
    // printf(" %d\n", arr[working_colum].task);
    // if (endptr > colum + lenght || working_task > MAX || time_tick > MAX)
    // {
    //     return 1;
    // }
    return 0;
}

int interpret_log(FILE **fileptr, struct colum *arr)
{
    int j = 0;
    int i = 0;
    char colum[40];
    while (1)
    {
        if (feof(*fileptr))
        {
            break;
        }
        char c = fgetc(*fileptr);
        if (c == '\r')
        {
            interpret_colum(colum, i, j, arr);
            // printf("log: %d %c %d\n", arr[j].time, arr[j].action, arr[j].task);
            memset(colum, 0, sizeof(colum));
            i = 0;
            j++;
        }
        else
        {
            colum[i++] = c;
        }
    }
    arr[j].time = -1;
    return 0;
}

int print_log(struct colum *arr)
{
    int j = 0;
    while (arr[j].time != -1 && j <= MAX)
    {
        printf("%d %c %d\n", arr[j].time, arr[j].action, arr[j].task);
        j++;
    }
    return 0;
}

void print_task_schedule(struct colum *arr, int task, int lengh_arr, int max_time_ticks)
{
    printf("T%d ", task);
    int i = 0;
    char cached_schedul[MAX];
    int temp = 0;
    memset(cached_schedul, ' ', MAX - 1);
    while (i <= lengh_arr)
    {
        if ((arr[i].action == 'S' && arr[i].task == task) || temp)
        {
            cached_schedul[arr[i].time] = '-';
            temp = 1;
        }
        if (arr[i].action == 'S' && arr[i].task != task)
        {
            cached_schedul[arr[i].time] = ' ';
            temp = 0;
        }
        i++;
    }
    printf("\t");
    for (i = 0; i<= max_time_ticks; i++){
        printf("%c", cached_schedul[i]);
    }
        printf("\n");
}

char find_action(struct colum *arr, int time, int ignore, int task, int lengh_arr)
{
    char c = 'F';
    int ignore_cnt = 1;
    for (int i = 0; i <= lengh_arr;i++){
        if (arr[i].time == time && arr[i].task == task && ignore_cnt == ignore){
            c = arr[i].action;
        }
        if (arr[i].time == time && arr[i].task == task)
        {
            ignore_cnt++;
        }
    }
    return c;
}

    void
    print_task_action(struct colum *arr, int task, int max_time_ticks, int lengh_arr)
{
    int action_per_tick[MAX];
    memset(action_per_tick, 0, MAX - 1);
    int max_action_per_tick = 0;

    for (int i = 0; i < lengh_arr; i++){
        int time = arr[i].time;
        if(arr[i].task == task){
            action_per_tick[time]++;
            if(action_per_tick[time]>max_action_per_tick){
                max_action_per_tick = action_per_tick[time];
            }
        }
    }

    for (int col = max_action_per_tick; col > 0;col--){
        printf("\t");
        for (int time = 0; time < max_time_ticks; time++){
            if (action_per_tick[time] >= col)
            {
                printf("%c", find_action(arr, time, col, task, lengh_arr));
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int print_viewlog(struct colum *arr)
{
    int lengh_arr = 0;
    int max_time_ticks = 0;
    int max_actions_per_task[MAX];
    memset(max_actions_per_task, 0, MAX - 1);
    int activ_task[MAX];
    memset(activ_task, -1, MAX - 1);
    int i = 0;
    while (arr[i].time >= 0 && i <= MAX)
    {
        // printf("%d %c %d\n", arr[i].time, arr[i].action, arr[i].task);
        if (arr[i].time > max_time_ticks)
        {
            max_time_ticks = arr[i].time;
        }
        if (activ_task[arr[i].task] <= 0)
        {
            activ_task[arr[i].task] = 1;
        }
        // if (arr[i].time == arr[i-1].time && arr[i].task == arr[i-1].task)
        // {
        //     max_actions_per_task[arr[i].task]++;
        // }
        i++;
    }
    lengh_arr = i - 1;
    // printf("%d\n", max_actions_per_task[0]);
    int n = max_time_ticks;
    
    while (n >= 0)
    {
        if (activ_task[n] > 0)
        {
            print_task_action(arr, n, max_time_ticks, lengh_arr);
            print_task_schedule(arr, n, lengh_arr, max_time_ticks);
        }
        n--;
    }

    return 0;
}

int main(void)
{
    struct colum arr[MAX];

    FILE *fileptr = NULL;
    if (open_file_r(&fileptr, "log.txt"))
    {
        printf("failed opening file\nexiting...");
        return EXIT_FAILURE;
    }
    interpret_log(&fileptr, arr);
    // print_log(arr);
    if (close_file(&fileptr))
    {
        printf("failed to close file\nexiting...");
        return EXIT_FAILURE;
    }
    // print_log(arr);
    print_viewlog(arr);
    return EXIT_SUCCESS;
}