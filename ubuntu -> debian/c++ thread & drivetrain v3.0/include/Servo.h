#include <Chip_PCA9685.h>

#include <project_constants.h>

class Servo
{
    public:
        ~Servo();
        Servo(Chip_PCA9685 * chip, int pin_no);
        void set_angle(float new_angle);
        float get_angle();
        float get_max_angle();
        void turn(int angle);
         int _max_ticks;
        int _min_ticks;
    protected:
    private:
        Chip_PCA9685 * _chip_PCA9685;
        int _pin;
        float _angle;
        float _max_angle;

};
