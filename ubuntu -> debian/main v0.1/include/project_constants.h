#define TURN_LEFT  0
#define TURN_RIGHT 1
#define FORWARD 0
#define BACKWARD 1

#define PWM_CHIP_ADDR 0x40

#define PWM_CHIP_HZ 50
#define USB_FRONT_CAMERA_NO 0

// default values

#define default_drivetrain_drive_speed 0.175
#define default_drivetrain_turn_speed 0.275

// ============= Used Pins =================

// 1. Raspberry Pi
#define PIN_SONAR_FRONT_TRIGGER  7
#define PIN_SONAR_FRONT_ECHO     4

// 2. PWM chip
#define PIN_WHEEL_LEFT_BACKWARD  0
#define PIN_WHEEL_LEFT_FORWARD   1
#define PIN_WHEEL_RIGHT_FORWARD  2
#define PIN_WHEEL_RIGHT_BACKWARD 3

#define PIN_SERVO                11
