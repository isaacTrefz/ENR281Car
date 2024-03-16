#include <esp_now.h>
#include <WiFi.h>

// Define the PWM settings
#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1
#define PWM_FREQ    50 // 50Hz
#define PWM_RESOLUTION 12 // 12-bit resolution for duty cycle
// Define the pin for PWM output
#define PWM_PIN1 22
#define PWM_PIN2 23




// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int right;
    int left;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Int 1: ");
  Serial.println(myData.right);
  Serial.print("Int 2: ");
  Serial.print(myData.left);
  int dutyCycleRight = myData.right * 0.1; // 10% of the maximum duty cycle (12-bit resolution)
  int dutyCycleLeft = myData.left * 0.1;
  // Write duty cycle value to the PWM channel
  ledcWrite(PWM_CHANNEL1, dutyCycleRight);
  ledcWrite(PWM_CHANNEL2, dutyCycleLeft);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

 // Configure PWM channel
  ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL2, PWM_FREQ, PWM_RESOLUTION);
  
  // Attach the PWM channel to the GPIO pin
  ledcAttachPin(PWM_PIN1, PWM_CHANNEL1);
  ledcAttachPin(PWM_PIN2, PWM_CHANNEL2);

  
  
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
