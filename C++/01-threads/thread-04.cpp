/* Program: Lambda as thread function
 * Author : Karthick Kumaran
 * E-mail : asvkarthick@gmail.com
 */

#include <iostream>
#include <thread>

int main(void)
{
    auto lambda = []() {
        std::cout << "Lambda thread" << std::endl;
    };

    std::thread t(lambda);
    t.join();

    return 0;
}
