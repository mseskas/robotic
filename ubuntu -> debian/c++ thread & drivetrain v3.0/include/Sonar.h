#include <iostream>
#include <stdio.h>
#include <wiringPi.h>

#include <sys/time.h>

#include <project_constants.h>

class Sonar
{
    public:  Sonar(int pin_trigger, int pin_echo);
    public:  ~Sonar();
    public:  int get_distance();
    public:  int get_trigger();
    public:  void set_trigger(int pin_trigger);
    public:  int get_echo();
    public:  void set_echo(int pin_echo);

    private: int trigger;
    private: int echo;
    private: int waitforpin(int pin_value, int timeout_uS);
};
