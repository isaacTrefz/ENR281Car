/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// Define the PWM settings
#define PWM_CHANNEL 0
#define PWM_FREQ    50 // 50Hz
#define PWM_RESOLUTION 12 // 10-bit resolution for duty cycle
// Define the pin for PWM output
#define PWM_PIN 22




// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Int: ");
  Serial.println(myData.a);
  Serial.println("You are here");
  int dutyCycle = myData.a * 0.1; // 10% of the maximum duty cycle (1023 for 10-bit resolution)
  
  // Write duty cycle value to the PWM channel
  ledcWrite(PWM_CHANNEL, dutyCycle);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

 // Configure PWM channel
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  
  // Attach the PWM channel to the GPIO pin
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);

  
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
