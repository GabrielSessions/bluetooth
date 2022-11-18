#include <Adafruit_MotorShield.h>

#define CHANNEL1 11 // Right stick - Horizontal
#define CHANNEL2 10 // Right stick - Vertical
#define CHANNEL3 9 // Left stick - Vertical
#define CHANNEL4 5 // SWA
#define CHANNEL5 6 // SWB

// int variables for the values being read from the right stick
int verticalValue;
int horizontalValue;
int spinnerValue;

// Check upper left buttons, don't run spinner if not activated
int leftSpinCheck;
int rightSpinCheck;

// defining the deadzone for driving control
int deadZone = 30;

// int variable for the speed of a motor
int motorSpeed;

// Create the motor shield object 
Adafruit_MotorShield MotorShield = Adafruit_MotorShield();

// Select which 'ports' M1, M2, M3 or M4. In this case, M1 and M2
Adafruit_DCMotor *leftMotor = MotorShield.getMotor(1);
Adafruit_DCMotor *rightMotor = MotorShield.getMotor(2);
Adafruit_DCMotor *spinner = MotorShield.getMotor(3);

// Safety turn off, if counter is sufficiently high, turn off everything


// Read the number of a specified channel and map the reading to the provided range
// If the channel is off, return the default value
int readChannel(int channelInput, int mapLower, int mapUpper, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000); // Gets length of HIGH pulse, times out after 30000 useconds
  if (ch < 100) return defaultValue; // If less than 100 microseconds, not a significant pulse
  return map(ch, 1000, 2000, mapLower, mapUpper); // Map the vaule to the provided min/max
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setting Up!");
  if (!MotorShield.begin()) {   // if the MotorShield hasn't been detected      
    Serial.println("Could not find Motor Shield. Check wiring.");
    // while (1); // hang
  }
  Serial.println("Motor Shield found.");

  // Set up the pins for the two channels for the right joystick
  pinMode(CHANNEL1, INPUT);
  pinMode(CHANNEL2, INPUT);
  pinMode(CHANNEL3, INPUT);
  pinMode(CHANNEL4, INPUT);
  pinMode(CHANNEL5, INPUT);
  
  Serial.println("Setup complete!");
}

void loop() {
  // Read the values from the two channels, map them all from -255 to 255 with default value of 0
  // Only using the right stick to drive
  horizontalValue = readChannel(CHANNEL1, -255, 255, 0);
  verticalValue = readChannel(CHANNEL2, -255, 255, 0);
  spinnerValue = readChannel(CHANNEL3, -255, 255, 0);
  rightSpinCheck = readChannel(CHANNEL4, -1, 1 , 0);
  leftSpinCheck = readChannel(CHANNEL5, -1, 1, 0);
  motorSpeed = sqrt(verticalValue*verticalValue + horizontalValue*horizontalValue)/sqrt(2);

  // Moving the joystick to the left and down are negative values
  
  //forward right
  if (verticalValue > deadZone && horizontalValue > deadZone)
  {
    Serial.println("Forward right");
    rightMotor->setSpeed(motorSpeed - horizontalValue);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);     
  }
  
  // forward left
  else if (verticalValue > deadZone && horizontalValue < -deadZone)
  {
    Serial.println("Forward left");
    rightMotor->setSpeed(motorSpeed);
    leftMotor->setSpeed(motorSpeed + horizontalValue);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);  
  }
  
  // forward
  else if (verticalValue > deadZone && horizontalValue > -deadZone && horizontalValue < deadZone)
  {

    Serial.println("Forward");
    rightMotor->setSpeed(motorSpeed);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD);  
  }

  //back right
  else if (verticalValue < -deadZone && horizontalValue > deadZone)
  {
    Serial.println("Back right");
    rightMotor->setSpeed(motorSpeed - horizontalValue);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->run(BACKWARD);
    leftMotor->run(FORWARD);
  }
  
  // back left
  else if (verticalValue < -deadZone && horizontalValue < -deadZone)
  {
    Serial.println("Back left");
    rightMotor->setSpeed(motorSpeed);
    leftMotor->setSpeed(motorSpeed + horizontalValue);
    rightMotor->run(BACKWARD);
    leftMotor->run(FORWARD);  
  }

    // backwards
  else if (verticalValue < -deadZone && abs(horizontalValue) < deadZone)
  {
    Serial.println("Backwards");
    rightMotor->setSpeed(motorSpeed);
    leftMotor->setSpeed(motorSpeed);
    rightMotor->run(FORWARD);
    leftMotor->run(BACKWARD); 
  }

  // turn left
  else if (abs(verticalValue) < deadZone && horizontalValue < -deadZone) {
    Serial.println("Turn left");
    rightMotor->setSpeed(motorSpeed);
    rightMotor->run(FORWARD);
    leftMotor->run(RELEASE);      
  }

  //turn right
  else if (abs(verticalValue) < deadZone && horizontalValue > deadZone) {
    Serial.println("Turn right");
    leftMotor->setSpeed(motorSpeed);
    rightMotor->run(RELEASE);
    leftMotor->run(BACKWARD);  
  }
  else // in the deadZone, don't move
  {
    Serial.println("Stop");
    rightMotor->run(RELEASE);
    leftMotor->run(RELEASE); 
  }

  // timeout after 15 seconds
  if (abs(spinnerValue) > deadZone and leftSpinCheck == 0 and rightSpinCheck == 0) {
    spinner->setSpeed(abs(spinnerValue));
    if (spinnerValue < 0) {
      spinner->run(FORWARD);
    }
    else {
      spinner->run(BACKWARD);
    }
    
  }
  else {
    spinner->setSpeed(0);
    spinner->run(RELEASE);
  }

  //Serial.println("Motor Vertical: ");
  //Serial.println(verticalValue);
  //Serial.println("Motor Horizontal: ");
  //Serial.println(horizontalValue);
  
  
  delay(50); 
}
