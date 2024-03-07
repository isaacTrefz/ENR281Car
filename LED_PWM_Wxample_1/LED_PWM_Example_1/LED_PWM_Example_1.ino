// Include the ESP32 PWM library
//#include <ledc.h>

// Define the PWM settings
#define PWM_CHANNEL 0
#define PWM_FREQ    50 // 50Hz
#define PWM_RESOLUTION 10 // 10-bit resolution for duty cycle

// Define the pin for PWM output
#define PWM_PIN 22

void setup() {
  // Configure PWM channel
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  
  // Attach the PWM channel to the GPIO pin
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
}

void loop() {
  // Set duty cycle to 10%
  int dutyCycle = 1023 * 0.1; // 10% of the maximum duty cycle (1023 for 10-bit resolution)
  
  // Write duty cycle value to the PWM channel
  ledcWrite(PWM_CHANNEL, dutyCycle);
}




















/*// the number of the LED pin
const int ledPin = 22;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 50;
const int ledChannel = 0;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}
 
void loop(){
  // increase the LED brightness
  //for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
  //  ledcWrite(ledChannel, dutyCycle);
   // delay(15);
  //}

ledcWrite(ledChannel, 25);

  // decrease the LED brightness
//  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
  //  ledcWrite(ledChannel, dutyCycle);   
    //delay(15);
  //}
}*/
