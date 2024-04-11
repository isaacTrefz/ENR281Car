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

int rightDirection = 1;
int leftDirection = 1;
int dutyCycleLeft;
int dutyCycleRight;



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
  Serial.println(myData.left);




  if(myData.left < 1000){                               //left wheel reverse restart
    if(leftDirection == 1){
          dutyCycleLeft = (myData.left * 0.04) +205;
          ledcWrite(PWM_CHANNEL2, dutyCycleLeft);
          delay(50);
          dutyCycleLeft = 286;
          ledcWrite(PWM_CHANNEL2, dutyCycleLeft);
          delay(50);
          leftDirection = 0;
    }
    else{
      int dutyCycleLeft = (myData.left * 0.04) +205;
      ledcWrite(PWM_CHANNEL2, dutyCycleLeft);
    }
  }
  else{                                                 //left wheel standard mode
    leftDirection = 1;
    int dutyCycleLeft = (myData.left * 0.04) +205;
    ledcWrite(PWM_CHANNEL2, dutyCycleLeft);
    Serial.println(dutyCycleLeft);
  }



  if(myData.right < 1000){                               //right wheel reverse restart
    if(rightDirection == 1){
          dutyCycleRight = (myData.right * 0.04) +205;
          ledcWrite(PWM_CHANNEL1, dutyCycleRight);
          Serial.println("you are here");
          delay(50);
          dutyCycleRight = 286;
          ledcWrite(PWM_CHANNEL1, dutyCycleLeft);
          delay(50);
          rightDirection = 0;
    }
    else{
      int dutyCycleRight = (myData.right * 0.04) +205;
      ledcWrite(PWM_CHANNEL1, dutyCycleRight);
    }
  }
  else{                                                 //right wheel standard mode
    rightDirection = 1;
    int dutyCycleRight = (myData.right * 0.04) +205;
    ledcWrite(PWM_CHANNEL1, dutyCycleRight);
  }
 
 // int dutyCycleRight = (myData.right * 0.04) + 205; // 10% of the maximum duty cycle (12-bit resolution)
  
  // Write duty cycle value to the PWM channel
  //ledcWrite(PWM_CHANNEL1, dutyCycleRight);

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
