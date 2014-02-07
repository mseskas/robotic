#include <iostream>
#include <string>
#include <thread>
#include <wiringPi.h>


#include <drivetrain.h>
#include <sonar.h>


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

    //thread t2(task1, "antras");
    //t1.join();
    //t2.join();


    int sonar_front_trigger = 7;
    int sonar_front_echo = 4;

    int distance = 0;

    sonar sonar_front (sonar_front_trigger, sonar_front_echo);

    thread t1(constant_distance_measure, sonar_front, &distance);

    drivetrain drv;
    drv.drive(5, FORWARD);
    drv.set_speed(0.12);



   while (true)
   {
       cout << "measure = " << distance << endl;
       drv.drive(5, FORWARD);
       if ((distance < 20) && (distance != 0) )
       {
           cout << "finish = " << endl;
           break;
       }
       //delay(100);
   }

    return 0;
}
