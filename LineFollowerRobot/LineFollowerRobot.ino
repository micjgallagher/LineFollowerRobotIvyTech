// Options
#define DEBUG true

// Speeds
#define MOTOR_SPEED -180 // Reverse sign as needed.

// Pin Numbers
#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12

//Right motor
int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

int rightMotorSpeed=MOTOR_SPEED;

//Left motor
int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;

int leftMotorSpeed=MOTOR_SPEED;


void setup()
{
  Serial.begin(9600); 
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequency as 7812.5 hz.
  //TCCR0B = TCCR0B & B11111000 | B00000010;
  
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

  if (DEBUG) {
    Serial.print("rightIRSensorValue = ");
    Serial.println(rightIRSensorValue, DEC);
    Serial.print("leftIRSensorValue = ");
    Serial.println(leftIRSensorValue, DEC);
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
  if (DEBUG) {
    Serial.print("Turning motor with enable pin ");
    Serial.print(enable, DEC);
    Serial.print(" with speed ");
    Serial.print(speed, DEC);
    Serial.println();
  }

  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  analogWrite(enable, abs(speed));
}

