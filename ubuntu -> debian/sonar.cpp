
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>

#include <sys/time.h>

int waitforpin(int pin_no, int pin_value, int timeout_uS)
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
    if (digitalRead(pin_no) == pin_value) done = 1;
   }
   return micros;
 }

int get_distance_sonar (int pin_trigger, int pin_echo)
{
    pinMode(pin_trigger, OUTPUT);
    pinMode(pin_echo, INPUT);

    int l=0;

    digitalWrite(pin_trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(pin_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_trigger, LOW);

    waitforpin(pin_echo, HIGH, 5000);

    if (digitalRead(pin_echo) ==HIGH)
    {
        l = waitforpin(pin_echo, LOW, 30000);

        if (digitalRead(pin_echo) == LOW)
        {
            l = l/ 58 + 1;
        }
        else
        {
            l = 0;
        }
    }
    else
    {
        printf("sensor didn't fired\n");
    }
    return l;
}

using namespace std;

int main (int argc, char * argv[] )
{
  wiringPiSetup () ;
  setbuf(stdout, NULL);  // disable buffered std ( to don't use \n at end

while (1)
  {
    int dist = get_distance_sonar(7,4);
    
    printf("distance in cm - %i\n", dist);
    delay (500) ;
  }
  return 0 ;
}
