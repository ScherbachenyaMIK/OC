#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <random>
#include "Header.h"
#include "Variables.h"

void Work(std::vector<double>& arr, double x, int t)
{
    std::unique_lock<std::mutex> start(start_event_m1);
    start_event.wait(start);
    ++threads_started;
    {
        std::lock_guard<std::mutex> lock1(work_finish_event_m1);
        std::lock_guard<std::mutex> lock2(work_finish_event_m2);
    }
    int n = arr.size();
    std::vector<double> b(n);
    int j = 0, k = n - 1;
    for (int i = 0; i < n; ++i)             //copy all elements equals to x at
    {                                       //beginning of array
        if (std::abs(arr[i] - x) < 1e-5)
        {
            b[j] = arr[i];
            ++j;
        }
        else                                //others at the end
        {
            b[k] = arr[i];
            --k;
        }
    }
    std::swap(arr, b);
    std::cout << "From work:\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << arr[i] << ' ';
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
    }
    std::cout << "\n\n";
    while (threads_started != 2)
    {
        work_finish_event.notify_all();     //set event that 'work' finish their work
    }
}

void CountElement(const std::vector<double>& arr, double x, int& result)
{
    main_count_element_cs.lock();
    std::unique_lock<std::mutex> start(start_event_m2);
    start_event.wait(start);
    ++threads_started;
    std::unique_lock<std::mutex> work_finish(work_finish_event_m1);
    work_finish_event.wait(work_finish);
    ++threads_started;
    if (std::abs(arr[0] - x) > 1e-5)        //if first element not equals return 0
    {
        result = arr.size();
        main_count_element_cs.unlock();
        return;
    }
    int count = 0;
    int n = arr.size();
    for (int i = 0; i < n; ++i)             //while vector[i] equals to x count elements
    {
        if (std::abs(arr[i] - x) > 1e-5)
        {
            break;
        }
        ++count;
    }
    result = n - count;
    main_count_element_cs.unlock();
}

int main()
{
    std::vector<double> a;
    int t;
    double x;
    int result = -1;
    int n;
    std::cout << "Enter array size: ";
    std::cin >> n;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 30);
    for (int i = 0; i < n; ++i)             //enter array using new randomizer
    {
        a.push_back(dis(gen));
        /*double y;
        std::cin >> y;
        a.push_back(y);*/
    }
    std::cout << "Array: ";
    for (int i = 0; i < n; ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << '\n';
    std::cout << "Enter time to sleep after processing one element (ms): ";
    std::cin >> t;
    std::cout << "Enter element for searching: ";
    std::cin >> x;

    {
        std::lock_guard<std::mutex> lock1(start_event_m1);
        std::lock_guard<std::mutex> lock2(start_event_m2);
    }

    std::thread workThread(Work, std::ref(a), x, t);                                //definitely use ref to trasfer reference
    std::thread countElementThread(CountElement, std::cref(a), x, std::ref(result));//cref to constant reference

    while (threads_started != 2)
    {
        start_event.notify_all();                                                   //set event to start threads
    }
    threads_started = 0;

    std::unique_lock<std::mutex> work_finish(work_finish_event_m2);
    work_finish_event.wait(work_finish);                                            //waiting while thread 'work' finishes
    ++threads_started;

    std::cout << "From main:\n";
    for (auto i : a)
    {
        std::cout << i << ' ';                                                      //waiting while 'count' thread enter sections
    }
    std::cout << '\n';

    main_count_element_cs.lock();							                        //waiting while thread 'count' finishes
    std::cout << "\nCount of elements not equals to X: " << result << '\n';
    main_count_element_cs.unlock();
    workThread.join();                                                              //waiting for end of threads
    countElementThread.join();
    return 0;
}
