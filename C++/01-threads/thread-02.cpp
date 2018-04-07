#include <iostream>
#include <thread>

void thread_fn()
{
    std::cout << "This is thread function" << std::endl;
}

int main(void)
{
    std::thread thr { thread_fn };
    // No need to join the thread when we detach
    thr.detach();
    std::cout << "This is main thread" << std::endl;
    return 0;
}
