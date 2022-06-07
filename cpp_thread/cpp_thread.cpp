#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <omp.h>

// 0 for stop, 1 for run, 2 for pause
volatile std::atomic_int status = 0;

void work_thread() {
    using namespace std::chrono_literals;
    while (status != 0) {
        if (status == 1) {
            std::cout << "Work thread running..." << std::endl;
            std::this_thread::sleep_for(1s);
        } else if (status == 2) {
            std::cout << "Work thread paused" << std::endl;
            while (status == 2);
            if (status == 1) std::cout << "Work thread resumed" << std::endl;
        }
    }
    std::cout << "Work thread exit." << std::endl;
}

void control_thread() {
    using namespace std::chrono_literals;
    char op;
    while (std::cin >> op) {
        switch (op) {
            case 'r':
                if (status == 0) {
                    status = 1;
                    std::cout << "Creating work thread..." << std::endl;
                    std::thread t(work_thread);
                    t.detach();
                } else if (status == 2) {
                    status = 1;
                } else {
                    std::cout << "Work thread already running" << std::endl;
                }
                break;
            case 's':
                if (status == 1 || status == 2) {
                    std::cout << "Sending stop signal..." << std::endl;
                    status = 0;
                } else {
                    std::cout << "Work thread already stopped." << std::endl;
                }
                break;
            case 'p':
                switch (status) {
                    case 0:
                        std::cout << "Work thread not running" << std::endl; break;
                    case 1:
                        std::cout << "Sending pause signal..." << std::endl;
                        status = 2;
                        break;
                    case 2:
                        std::cout << "Work thread already paused" << std::endl;
                        break;
                }
                break;
            default:
                ;
        }
    }
    status = 0;
    std::this_thread::sleep_for(1s);
}

int main() {
    omp_set_num_threads(1);
    std::cout << omp_get_max_threads() << std::endl;
    std::thread _([]() {
        std::cout << omp_get_max_threads() << std::endl;
    });
    _.join();
    return 0;
}