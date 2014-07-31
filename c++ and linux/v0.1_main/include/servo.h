#include <pwm_chip.h>

#include <project_constants.h>

class servo
{
public:
    ~servo();
    servo(pwm_chip * chip, int pin_no);
    void set_angle(float new_angle);
    float get_angle();
    void turn(float angle);
    int _max_ticks;
    int _min_ticks;
protected:
private:
    pwm_chip * _chip_PCA9685;
    int _pin;
    float _angle;

};
