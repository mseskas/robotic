#include <iostream>
#include <string>
#include <thread>
#include <wiringPi.h>


#include <drivetrain.h>
#include <sonar.h>


using namespace std;

//The function we want to make the thread run.
void task1(string str)
{
    for (int i = 0; i < 10; i++)
    {
        delay(10);
        string rez = "task output - " + str + " " + to_string(i) + "\n";
        cout << rez;
    }
}

int main()
{
    /*thread t1(task1, "pirmas");
    thread t2(task1, "antras");
    t1.join();
    t2.join();*/

    wiringPiI2CSetup (40) ;

    int sonar_front_trigger = 7;
    int sonar_front_echo = 4;


    sonar sonar_front (sonar_front_trigger, sonar_front_echo);
    drivetrain drv;

    int step_angle = 15;

    for (int i = 0; i < 360; i += step_angle)
    {
        sonar_front.get_distance();
        drv.turn(DIRECTION_LEFT, 5); // creates new execution thread

    }






    return 0;
}
