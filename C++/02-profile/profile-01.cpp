#include <iostream>
#include <chrono>
#include <string.h>

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
    // Profile time to fill the image buffer
    auto start = std::chrono::steady_clock::now();
    CreateImage(image_buffer, WIDTH, HEIGHT);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken = " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << " microseconds" << std::endl;

    return 0;
}
