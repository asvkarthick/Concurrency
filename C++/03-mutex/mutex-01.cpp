#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int isDataAvailable = 0;
std::mutex data_available_mutex;

void* Producer()
{
    std::cout << __func__ << ": entering" << std::endl;

    while(true)
    {
        data_available_mutex.lock();
        std::cout << __func__ << ": done" << std::endl;

        isDataAvailable++;
        std::cout << __func__ << ": data ready " << isDataAvailable << std::endl;

        data_available_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return NULL;
}

void* Consumer()
{
    printf("%s entering...", __func__);

    while(true)
    {
        data_available_mutex.lock();
        std::cout << __func__ << ": done" << std::endl;

        std::cout << __func__ << ": checking if data is available." << std::endl;
        while(isDataAvailable > 0)
        {
            std::cout << __func__ << ": data read " << isDataAvailable << std::endl;
            isDataAvailable--;
        }

        data_available_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return NULL;
}

int main(void)
{
    std::thread producer_thread { Producer };
    std::thread consumer_thread { Consumer };

    producer_thread.join();
    consumer_thread.join();
    return 0;
}
