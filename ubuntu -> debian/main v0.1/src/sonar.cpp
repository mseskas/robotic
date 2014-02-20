#include "sonar.h"



using namespace std;

sonar::sonar(int pin_trigger, int pin_echo)
{
    wiringPiSetup () ;
    trigger = pin_trigger;
    echo = pin_echo;
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
}
sonar::~sonar()
{
    digitalWrite(trigger, LOW);
    digitalWrite(echo, LOW);
}

int sonar::waitforpin(int pin_value, int timeout_uS)
 {
   struct timeval now, start;
   int done;
   int micros;
   gettimeofday(&start, NULL);
   micros = 0;
   done=0;
   while (!done)
   {
    gettimeofday(&now, NULL);
    if (now.tv_sec > start.tv_sec) micros = 1000000L; else micros = 0;
    micros = micros + (now.tv_usec - start.tv_usec);
    if (micros > timeout_uS) done=1;
    if (digitalRead(echo) == pin_value) done = 1;
   }
   return micros;
 }

 int sonar::get_distance ()
{
    int l=0;

    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    waitforpin(HIGH, 5000);

    if (digitalRead(echo) ==HIGH)
    {
        l = waitforpin(LOW, 30000);

        if (digitalRead(echo) == LOW)
        {
            l = l/ 58 + 1;
        }
    }
    return l;
}

int sonar::get_trigger()
{
    return trigger;
}
void sonar::set_trigger(int pin_trigger)
{
    trigger = pin_trigger;
}
int sonar::get_echo()
{
    return echo;
}
void sonar::set_echo(int pin_echo)
{
    echo = pin_echo;
}

