#include <iostream>
#include <thread>
#include <vector>

class ThreadClass
{
public:

    ThreadClass() : thread_count {0} {}

    void CreateThread()
    {
        threads.push_back(std::thread(&ThreadClass::ThreadFn, this));
    }

    void ThreadFn()
    {
        thread_count++;
        std::cout << "Thread created: " << thread_count << std::endl;
    }

    void JoinThread()
    {
        for(auto& x : threads)
            x.join();
    }

private:
    std::vector<std::thread> threads;
    int thread_count;
};

int main()
{
    ThreadClass thread;

    thread.CreateThread();
    thread.CreateThread();

    thread.JoinThread();

    return 0;
}
