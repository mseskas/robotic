#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h> // system()

#include <wiringPi.h>

#define switch_pin 1
#define led_pin 0

using namespace std;

void constant_check ()
{
    cout << "Switch checking started" << endl;
    while (true)
    {
        if (digitalRead(switch_pin) == 1) // switch is on
            digitalWrite(led_pin, 1);
        delay(5000);
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
