#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int n, i;
    const char *string = "Karthick Kumaran is writing some concurrent programs in Linux Operating System";
    sem_t s;

    printf("Enter n : ");
    scanf("%d", &n);

    if(sem_init(&s, 1, 1) < 0) {
        fprintf(stderr, "Error: Initializing semaphore.\n");
        exit(1);
    }

    for(i = 0; i < n; i++)
        if(fork())
            break;

    if(sem_wait(&s) == -1) {
        fprintf(stderr, "Error: Waiting semaphore invalid\n");
        exit(1);
    }

    while(*string)
        putc(*string++, stdout);

    printf("\n");

    if(sem_post(&s) == -1) {
        fprintf(stderr, "Error: Signalling semaphore\n");
        return -1;
    }

    return 0;
}
