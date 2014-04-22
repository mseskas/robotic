#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h> // system()

#include <wiringPi.h>

#define switch_pin 5
#define led_pin 0

using namespace std;

void constant_check ()
{
    cout << "Switch checking started" << endl;
    while (true)
    {
        //cout << digitalRead(switch_pin) << endl;
        if (digitalRead(switch_pin) == 1) // switch is on
        {
            digitalWrite(led_pin, 1);//cout << "on" << endl; //digitalWrite(led_pin, 1);

        }
        else
        {
            digitalWrite(led_pin, 0);//cout << "0ff" << endl; //digitalWrite(led_pin, 0);
        }
        delay(100);
    }
}

int main()
{
    cout << "Guard : process started" << endl;
    wiringPiSetup();

    pinMode(switch_pin, INPUT);
    pinMode(led_pin, OUTPUT);

    std::thread t(constant_check);

    if (t.joinable()) t.join();
    cout << "Guard : process stopped" << endl;
    return 0;
}
