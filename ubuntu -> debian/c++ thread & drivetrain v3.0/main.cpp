#include <iostream>
#include <string>
#include <thread>
#include <wiringPi.h>

#include <pwm_chip.h>
#include <drivetrain.h>
#include <sonar.h>
#include <servo.h>


using namespace std;

    int sonar_front_trigger_pin = 7;
    int sonar_front_echo_pin = 4;
    int chip_PCA9685_address = 0x40;

//The function we want to make the thread run.
void constant_distance_measure(sonar snr, int * result)
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

    sonar sonar_front (sonar_front_trigger_pin, sonar_front_echo_pin);

    //thread t1(constant_distance_measure, sonar_front, &front_distance);

    pwm_chip chip_16pwm(chip_PCA9685_address);

    servo servo_front(chip_16pwm, 11);

    cout << servo_front._max_ticks << " - max ticks\n" << servo_front._min_ticks << " - min ticks\n";

   // drivetrain drv;
    //drv.drive(5, FORWARD);
   // drv.set_speed(0.12);



 /*  while (true)
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
*/
    return 0;
}
