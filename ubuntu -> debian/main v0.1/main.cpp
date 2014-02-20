#include <iostream>
#include <string>
#include <thread>

#include <project_constants.h>
#include <pwm_chip.h>
#include <drivetrain.h>
#include <servo.h>
#include <sonar.h>
#include <camera.h>

#include "activities.h"

using namespace std;


int main()
{

    sonar sonar_front (PIN_SONAR_FRONT_TRIGGER, PIN_SONAR_FRONT_ECHO);

    pwm_chip chip_16pwm(PWM_CHIP_ADDR);

    servo servo(&chip_16pwm, PIN_SERVO);

    drivetrain drv(&chip_16pwm);

    sonar_front.set_drivetrain( &drv);

    drv.a_drive(10, FORWARD);
    cout << "start : " << endl;
   // delay(50);
    //cout << "kill " << endl;
   // drv.force_stop();
    //cout << "fin " << endl;
    delay(8000);




    return 0;
}
