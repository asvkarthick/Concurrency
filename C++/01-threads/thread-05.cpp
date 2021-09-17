/* Program: Functor as thread function
 * Author : Karthick Kumaran
 * E-mail : asvkarthick@gmail.com
 */

#include <iostream>
#include <thread>

class FunctorClass
{
public:
    void operator()()
    {
        std::cout << "Functor thread" << std::endl;
    }
};

int main(void)
{
    FunctorClass f;
    std::thread thr(f);
    thr.join();
    return 0;
}
