#include <iostream>
#include <string>
#include <thread>

#include <Chip_PCA9685.h>
#include <Drivetrain.h>
#include <Sonar.h>
#include <Servo.h>


using namespace std;

    int Sonar_front_trigger_pin = 7;
    int Sonar_front_echo_pin = 4;
    int chip_PCA9685_address = 0x40;

//The function we want to make the thread run.
void constant_distance_measure(Sonar snr, int * result)
{
    while (true)
    {
        *result = snr.get_distance();
        delay(25);
    }
}



int main()
{


    int front_distance = 0;

    Sonar Sonar_front (Sonar_front_trigger_pin, Sonar_front_echo_pin);

    thread t1(constant_distance_measure, Sonar_front, &front_distance);

    Chip_PCA9685 pwm_chip(chip_PCA9685_address);

    Servo Servo_front(&pwm_chip, 11);

    Drivetrain drv;
    drv.set_speed(0.12);



   while (true)
   {
       cout << "measure = " << distance << endl;
       drv.drive(0.25, FORWARD);
       if ((distance < 20) && (distance != 0) )
       {
           cout << "finish = " << endl;
           break;
       }
       //delay(100);
   }

    return 0;
}
