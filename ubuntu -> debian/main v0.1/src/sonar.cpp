#include "sonar.h"



using namespace std;

sonar::sonar(int pin_trigger, int pin_echo)
{
    wiringPiSetup () ;
    trigger = pin_trigger;
    echo = pin_echo;
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    _drv = NULL;
    _stop_execution = false;
    _execution_thread = new thread (&sonar::constant_distance_measure, this);
    _last_distance = 0;

}
sonar::~sonar()
{
    force_stop();
}

void sonar::stop()
{
    digitalWrite(trigger, LOW);
    digitalWrite(echo, LOW);
}

void sonar::wait_to_finish(int timeout_ms)
{
    while (true)
    {
        if (!_is_executing ) break;
        delay(5);
    }
}

void sonar::force_stop()
{
    _stop_execution = true;
    wait_to_finish(0);
    _stop_execution = false;
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
        if (now.tv_sec > start.tv_sec) micros = 1000000L;
        else micros = 0;
        micros = micros + (now.tv_usec - start.tv_usec);
        if (micros > timeout_uS) done=1;
        if (digitalRead(echo) == pin_value) done = 1;
    }
    return micros;
}

void sonar::set_drivetrain(drivetrain * drv)
{
    _drv = drv;
}

void sonar::constant_distance_measure()
{
    _is_executing = true;
    while (true)
    {
        measure_distance();
        if ((_last_distance < stop_drivetrain_when_distance_to_front) && (_last_distance != 0))
        {
            if (_drv != NULL)
            {
                _drv->force_stop();
            }

        }
        if (_stop_execution) break;
        delay(30);
    }
    stop();
    _stop_execution = false;
    _is_executing = false;
}

void sonar::measure_distance()
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
    _last_distance = l;
}

int sonar::get_distance ()
{
    return _last_distance;
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

