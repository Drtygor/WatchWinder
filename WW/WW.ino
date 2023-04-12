// define constants for the pins used
const int motorPin = 13;  // Motor Pin that acts like a trigger to transistor
const int sensorIn = 33;  // Interupt Sensor pin (Input)
const int buttonPin = 12;
const int redPin = 14;
const int greenPin = 32;
const int bluePin = 15;

bool lastButtonState = HIGH; // false = 0 = LOW
bool buttonState = HIGH;
bool motorState = LOW;
bool sensorState = LOW;
bool lastSensorState = LOW;
bool turnOff = LOW;

unsigned long currentMillis = 0;
unsigned long startTime = 0; 
const long oneHour = 3600000; // 1 hour in millis
const int MAX_ANALOG_VAL = 4095;
const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2

int counter = 0;
int lastsensorState = 0;
int revolutionsPerDay = 800; // User watch info

//Testing
const long oneMin = 60000; // 1 hour in millis
const long halfMin = 30000; // 1 hour in millis
int revolutionsTest = 72; // test

void setup() {
  // put your setup code here, to run once:
 
  pinMode(motorPin, OUTPUT);
  pinMode(sensorIn, INPUT);  // set pin 2 as input
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  Serial.begin(9600);
  Serial.println("");
  Serial.println("Ready to count");
}


void loop() {
  // put your main code here, to run repeatedly:

  currentMillis = millis();

  buttonState = digitalRead(buttonPin); // Read the switch state  
  if (buttonState == LOW && buttonState != lastButtonState && motorState == LOW) {  // If the switch is ON
    analogWrite(motorPin, 90); // Start the motor (set speed)
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    lastButtonState = buttonState;
    motorState = HIGH;
    startTime = currentMillis;
    counter = 0;

    delay (40);

  } else if (buttonState == LOW && buttonState != lastButtonState && motorState == HIGH){
   lastButtonState = buttonState;
   turnOff = HIGH;
   digitalWrite(redPin, HIGH);
   digitalWrite(greenPin, LOW);
   analogWrite(motorPin,150);
   delay (40);

  }

  sensorState = digitalRead(sensorIn); // Read the interrupt Sensor
   if (sensorState == HIGH && turnOff == HIGH){
     analogWrite(motorPin, 0); // Turn the motor OFF
     motorState = LOW;
     turnOff = LOW;
     startTime = millis();

  }

  if (sensorState == HIGH && sensorState != lastsensorState){
    digitalWrite(bluePin, HIGH);
    counter++; // add 1 to the counter
    Serial.print("Revolutions: "); 
    Serial.println(counter); // print the counter
  }else{
    digitalWrite(bluePin, LOW);
  }

  if (motorState == HIGH && counter == revolutionsPerDay/24){
    analogWrite(motorPin, 0); // Turn the motor OFF
  }

  if (motorState == HIGH && currentMillis - startTime >= oneHour){
    analogWrite(motorPin, 90);
    counter = 0;
    startTime = currentMillis;
  }

  // int rawValue = analogRead(A13);
  // // float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3; // calculate voltage level
  // float voltageLevel = 3.5;
  // float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  
  // if (motorState == LOW && voltageLevel <= 3.6){
  //    digitalWrite(redPin, LOW);
  //    digitalWrite(redPin, HIGH);
  // } else if (motorState == HIGH && voltageLevel <= 3.7 ){
  //    digitalWrite(greenPin, LOW);
  //    delay (1000);
  //    digitalWrite(greenPin, HIGH);
  // }

  delay(10);
  lastButtonState = buttonState;
  lastsensorState = sensorState;
