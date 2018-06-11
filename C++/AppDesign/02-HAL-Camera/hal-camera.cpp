#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class HAL
{
public:

    HAL() :  num_requests { 0 }, is_working { true }, num_results { 0 }
    {
        result_thread = std::thread { &HAL::ProcessCaptureResult, this };
    }

    ~HAL()
    {
        {
            std::lock_guard<std::mutex> mtx(request_mutex);
            num_requests = 1;
        }
        cond.notify_all();
        if(result_thread.joinable())
            result_thread.join();
    }

    void ProcessCaptureRequest()
    {
        std::lock_guard<std::mutex> mtx(request_mutex);
        ++num_requests;
        cond.notify_one();
    }

    void ProcessCaptureResult()
    {
        std::cout << "KK: " << __func__ << " " << __LINE__ << std::endl;
        while(is_working)
        {
            std::unique_lock<std::mutex> mtx(request_mutex);
            cond.wait(mtx, [this]() { return num_requests > 0; } );
            while(num_requests > 0)
            {
                if(callback)
                    callback();
                num_requests--;
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void Stop()
    {
        is_working = false;
    }

    void RegisterCallbacks(void (*func)())
    {
        callback = func;
    }

private:
    int num_results;
    int num_requests;
    std::mutex request_mutex;
    std::condition_variable cond;
    bool is_working;
    std::thread result_thread;
    void (*callback)();
};

int num_callbacks = 0;
void FrameCallback()
{
    ++num_callbacks;
    std::cout << __func__ << " " << num_callbacks << std::endl;
}

class Camera : public HAL
{
public:
    Camera()
    {
        RegisterCallbacks(FrameCallback);
    }

    ~Camera()
    {
        Stop();
    }

    void Request()
    {
        ProcessCaptureRequest();
    }

    void Stop()
    {
        HAL::Stop();
    }

private:
    // int num_callbacks;
};

int main(int argc, char* argv[])
{
    Camera c;
    int num_requests;

    num_requests = atoi(argv[1]);

    while(num_requests > 0)
    {
        num_requests--;
        c.Request();
    }

    getchar();

    return 0;
}
