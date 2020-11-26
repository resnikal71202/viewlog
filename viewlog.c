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

struct dynamic_colum_array
{
    struct colum *array;
    unsigned long long used;
    unsigned long long size;
};

int init_dyca(struct dynamic_colum_array *a, unsigned long long size)
{
    a->array = malloc(size * sizeof(struct colum));
    a->used = 0;
    a->size = size;
    return 0;
}

int insert_dyca_element(struct dynamic_colum_array *a, struct colum element_to_insert)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(struct dynamic_colum_array));
    }
    a->array[a->used++] = element_to_insert;
    return 0;
}

int free_dyca(struct dynamic_colum_array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
    return 0;
}

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

int interpret_colum(const char *colum, int lenght, int working_colum, struct colum *arr)
{
    char *endptr;

    arr->time = strtol(colum, &endptr, 10);
    // printf("%d", arr[working_colum].time);
    char availible_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    char *action_ptr;
    action_ptr = strpbrk(endptr, availible_chars);
    arr->action = *action_ptr;
    // printf(" %c", arr[working_colum].action);
    arr->task = strtol(++(action_ptr), &endptr, 10);
    // printf(" %d\n", arr[working_colum].task);
    // if (endptr > colum + lenght || working_task > MAX || time_tick > MAX)
    // {
    //     return 1;
    // }
    return 0;
}

int interpret_log(FILE **fileptr, struct dynamic_colum_array *arr)
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
            struct colum temp_col;
            interpret_colum(colum, i, j, &temp_col);
            insert_dyca_element(arr, temp_col);
            // printf("log: %d %c %d\n", arr->array[j].time, arr->array[j].action, arr->array[j].task);
            memset(colum, 0, sizeof(colum));
            i = 0;
            j++;
        }
        else
        {
            colum[i++] = c;
        }
    }
    // printf("used: %d\n", arr->used);
    // printf("size: %d\n", arr->size);
    arr->array[j].time = -1;
    return 0;
}

int print_log(struct dynamic_colum_array *arr)
{
    int j = 0;
    while (arr->array[j].time != -1 && j < arr->used)
    {
        printf("%d %c %d\n", arr->array[j].time, arr->array[j].action, arr->array[j].task);
        j++;
    }
    return 0;
}

void print_task_schedule(struct dynamic_colum_array *arr, int task, int lengh_arr, int max_time_ticks)
{
    printf("T%d ", task);
    int i = 0;
    char cached_schedul[MAX];
    int temp = 0;
    memset(cached_schedul, ' ', MAX - 1);
    while (i < arr->used)
    {
        if ((arr->array[i].action == 'S' && arr->array[i].task == task) || temp)
        {
            cached_schedul[arr->array[i].time] = '-';
            temp = 1;
        }
        if (arr->array[i].action == 'S' && arr->array[i].task != task)
        {
            cached_schedul[arr->array[i].time] = ' ';
            temp = 0;
        }
        i++;
    }
    printf("\t");
    for (i = 0; i <= max_time_ticks; i++)
    {
        printf("%c", cached_schedul[i]);
    }
    printf("\n");
}

char find_action(struct dynamic_colum_array *arr, int time, int ignore, int task, int lengh_arr)
{
    char c = 'F';
    int ignore_cnt = 1;
    for (int i = 0; i <= lengh_arr; i++)
    {
        if (arr->array[i].time == time && arr->array[i].task == task && ignore_cnt == ignore)
        {
            c = arr->array[i].action;
        }
        if (arr->array[i].time == time && arr->array[i].task == task)
        {
            ignore_cnt++;
        }
    }
    return c;
}

void print_task_action(struct dynamic_colum_array *arr, int task, int max_time_ticks, int lengh_arr)
{
    int action_per_tick[MAX];
    memset(action_per_tick, 0, MAX - 1);
    int max_action_per_tick = 0;

    for (int i = 0; i < lengh_arr; i++)
    {
        int time = arr->array[i].time;
        if (arr->array[i].task == task)
        {
            action_per_tick[time]++;
            if (action_per_tick[time] > max_action_per_tick)
            {
                max_action_per_tick = action_per_tick[time];
            }
        }
    }

    for (int col = max_action_per_tick; col > 0; col--)
    {
        printf("\t");
        for (int time = 0; time <= max_time_ticks; time++)
        {
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

int print_viewlog(struct dynamic_colum_array *arr)
{
    int lengh_arr = 0;
    int max_time_ticks = 0;
    int max_actions_per_task[MAX];
    memset(max_actions_per_task, 0, MAX - 1);
    int activ_task[MAX];
    memset(activ_task, -1, MAX - 1);
    int i = 0;
    while (arr->array[i].time >= 0 && i <= arr->used)
    {
        // printf("%d %c %d\n", arr->array[i].time, arr->array[i].action, arr->array[i].task);
        if (arr->array[i].time > max_time_ticks)
        {
            max_time_ticks = arr->array[i].time;
        }
        if (activ_task[arr->array[i].task] <= 0)
        {
            activ_task[arr->array[i].task] = 1;
        }
        // if (arr[i].time == arr[i-1].time && arr[i].task == arr[i-1].task)
        // {
        //     max_actions_per_task[arr[i].task]++;
        // }
        i++;
    }
    lengh_arr = i;
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
    struct dynamic_colum_array dyca;

    init_dyca(&dyca, 2);

    FILE *fileptr = NULL;
    if (open_file_r(&fileptr, "log.txt"))
    {
        printf("failed opening file\nexiting...");
        return EXIT_FAILURE;
    }
    interpret_log(&fileptr, &dyca);
    // print_log(dyca);
    if (close_file(&fileptr))
    {
        printf("failed to close file\nexiting...");
        return EXIT_FAILURE;
    }
    print_viewlog(&dyca);
    free_dyca(&dyca);
    return EXIT_SUCCESS;
}