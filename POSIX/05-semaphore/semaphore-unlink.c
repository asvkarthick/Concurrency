#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    sem_unlink("karthick");

    return 0;
}
