/* Program: Finding number of hardware threads
 * Author : Karthick Kumaran
 * E-mail : asvkarthick@gmail.com
 */

#include <iostream>
#include <thread>

int main(void)
{
    std::thread t;
    std::cout << "Number of hardware threads = " << std::thread::hardware_concurrency() << std::endl;
    return 0;
}
