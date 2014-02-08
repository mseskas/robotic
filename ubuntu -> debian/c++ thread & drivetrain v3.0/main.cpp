#include <iostream>
#include <string>
#include <thread>
#include <wiringPi.h>

#include <pwm_chip.h>
#include <drivetrain.h>
#include <sonar.h>
#include <servo.h>


using namespace std;


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

    sonar sonar_front (PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);
    std::thread t1(constant_distance_measure, sonar_front, &front_distance);
    pwm_chip chip_16pwm(PWM_CHIP_ADDR);

    servo servo_front(&chip_16pwm, PIN_SERVO);

    drivetrain drv(&chip_16pwm);

    drv.a_drive(0.5, FORWARD);
    cout << "start : " << endl;
    delay(50);
    cout << "kill " << endl;
    drv.force_stop();
    cout << "fin " << endl;
    delay(8000);

  /*  while (true)
   {
       cout << "measure = " << front_distance << endl;
       //drv.drive(0.25, FORWARD);
       if (front_distance != 0)
       {
           if (front_distance > 50) front_distance =50;
           servo_front.set_angle((float)(front_distance/50.0));
       }
       delay(100);
   }
*/
    return 0;
}
