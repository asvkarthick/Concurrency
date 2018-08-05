#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    sem_t *sem;
    int value;

    sem = sem_open("karthick", 0);

    sem_post(sem);
    sem_getvalue(sem, &value);
    printf("Value = %d\n", value);

    sem_close(sem);

    return 0;
}
