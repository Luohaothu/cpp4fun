#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    // 模拟昂贵操作
    std::cout << "foo called\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void bar()
{
    // 模拟昂贵操作
    std::cout << "bar called\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::cout << "starting first helper...\n";
    std::thread helper1(foo);

    std::cout << "starting second helper...\n";
    std::thread helper2(bar);

    std::cout << "waiting for helpers to finish..." << std::endl;
    helper1.join();
    helper2.join();

    std::cout << "done!\n";
}