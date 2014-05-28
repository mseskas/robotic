#include <iostream>
#include <string>
#include <thread>
#include <wiringPi.h>

using namespace std;

//The function we want to make the thread run.
void task1(string str)
{
    for (int i = 0; i < 10; i++)
    {
        delay(10);
        string rez = "task output - " + str + "\n";
        cout << rez;
    }
}

int main()
{
    thread t1(task1, "pirmas");
    thread t2(task1, "antras");

    t1.join();
    t2.join();
}
