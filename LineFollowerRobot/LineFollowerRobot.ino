// Options
#include "Config.h"

int DEBUG_PRINT_SENTINEL=0;

// Speeds
#define MOTOR_SPEED 100 // Reverse sign as needed.

// Pin Numbers
#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12

//Right motor
int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;


void setup()
{
    if (DEBUG_PRINT) {
        Serial.begin(9600); 
    }
    //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
    //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
    //For that we need to increase the frequency of analogWrite.
    //Below line is important to change the frequency of PWM signal on pin D5 and D6
    //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
    //This sets frequency as 7812.5 hz.
    //TCCR0B = TCCR0B & B11111000 | B00000010; // Magic line; we had better results with it disabled.

    // put your setup code here, to run once:
    pinMode(enableRightMotor, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);

    pinMode(enableLeftMotor, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);

    pinMode(IR_SENSOR_RIGHT, INPUT);
    pinMode(IR_SENSOR_LEFT, INPUT);
    rotateMotors(0,0);   
}


void loop()
{

    int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
    int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

    if (DEBUG_PRINT) {
        if (DEBUG_PRINT_SENTINEL == 0) {
            Serial.print("rightIRSensorValue = ");
            Serial.println(rightIRSensorValue, DEC);
            Serial.print("leftIRSensorValue = ");
            Serial.print(leftIRSensorValue, DEC);
            Serial.println();
        }

        if (DEBUG_PRINT_SENTINEL >= DEBUG_PRINT_INTERVAL) { // This is carefully constructed so that all users of the sentinel can simply check for equality to zero and do not need to consider incrementation/reset and the cycle will begin on the first run of the loop instead of skipping until after the first interval.
            DEBUG_PRINT_SENTINEL = 0;
        } else {
            DEBUG_PRINT_SENTINEL += 1;
        }
    }

    if (DEBUG_MOTORS) {
        rotateMotors(MOTOR_SPEED, MOTOR_SPEED);
        return;
    }

    //If none of the sensors detects black line, then go straight
    if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
    {
        rotateMotors(MOTOR_SPEED, MOTOR_SPEED);
    }
    //If right sensor detects black line, then turn right
    else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
    {
        rotateMotors(-MOTOR_SPEED, MOTOR_SPEED); 
    }
    //If left sensor detects black line, then turn left  
    else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
    {
        rotateMotors(MOTOR_SPEED, -MOTOR_SPEED); 
    } 
    //If both the sensors detect black line, then stop 
    else 
    {
        rotateMotors(0, 0);
    }
}


void rotateMotors(int rightMotorSpeed, int leftMotorSpeed)
{
    rotateMotor(rightMotorSpeed, rightMotorPin1, rightMotorPin2, enableRightMotor);
    rotateMotor(leftMotorSpeed, leftMotorPin1, leftMotorPin2, enableLeftMotor);
}

void rotateMotor(int speed, int pin1, int pin2, int enable)
{
    if (DEBUG_PRINT && DEBUG_PRINT_SENTINEL == 0) {
        Serial.print("Turning motor with pin1 = ");
        Serial.print(pin1, DEC);
        Serial.print(" , pin2 = ");
        Serial.print(pin2, DEC);
        Serial.print(" , and enable pin = ");
        Serial.print(enable, DEC);
        //Serial.print(" (" + (enable == enableLeftMotor) ? "left" : (enable == enableRightMotor) ? "right" : "unknown" + ")"); // Concatenation/Type-Strictness issues, my beloved.
        Serial.print(" with speed = ");
        Serial.print(speed, DEC);
        Serial.println();
    }

    if (speed < 0)
    {
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,HIGH);    
    }
    else if (speed > 0)
    {
        digitalWrite(pin1,HIGH);
        digitalWrite(pin2,LOW);      
    }
    else
    {
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);      
    }

    analogWrite(enable, abs(speed));
}

