#include <iostream>
#include <thread>

void thread_fn()
{
    std::cout << "This is thread function" << std::endl;
}

int main(void)
{
    std::thread thr { thread_fn };
    std::cout << "This is main thread" << std::endl;
    thr.join();
    return 0;
}
