#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    sem_t *sem;

    sem = sem_open("karthick", O_CREAT | O_RDWR, 0777, 8 /* value */);

    sem_close(sem);

    return 0;
}
