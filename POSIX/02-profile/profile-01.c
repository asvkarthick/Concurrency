#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void CreateImage(unsigned char* buf, int width, int height)
{
    int row;

    for(row = 0; row < height; row++)
    {
        memset(&buf[row * width], row, width);
    }
}

#define WIDTH 256
#define HEIGHT 256
unsigned char image_buffer[WIDTH * HEIGHT];

int main(void)
{
    struct timeval start, end;
    long seconds, useconds;

    // Profile time to fill the image buffer
    gettimeofday(&start, 0);
    CreateImage(image_buffer, WIDTH, HEIGHT);
    gettimeofday(&end, 0);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    printf("Time taken = %ld micro seconds\n", useconds);

    return 0;
}
