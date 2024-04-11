#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xE8, 0x6B, 0xEA, 0xCF, 0x22, 0x94};
//B0:A7:32:14:BC:BC
//0x54, 0x43, 0xB2, 0xFB, 0xAE, 0x10   E8:6B:EA:CF:AE:EC



// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int right;
  int left;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void swapInt(int*a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}


class buttonCounter {

  private:
    int leftPreviousState = 1;
    int rightPreviousState = 1;
    
  public:
    int triming = 0;

    void updateTriming(){
      if(digitalRead(33)== 0 && rightPreviousState == 1){
        triming += 5;
      }

      if(digitalRead(32)== 0 && leftPreviousState == 1){
        triming -= 5;
      }

      rightPreviousState = digitalRead(33);
      leftPreviousState = digitalRead(32);
        
     }
};

  buttonCounter triming;







 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(33, INPUT);
  pinMode(32, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);


  analogReadResolution(12);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
  
void loop() {
  float dampingFactor = float(analogRead(34))/float(4095);
  int devisor = pow(dampingFactor*2047,2);
  // Set values to send

  
  if(digitalRead(19)==1){                           //taank drive mode
  myData.right = (pow(dampingFactor*(analogRead(39)-2047),3)/devisor) + 2047;
  myData.left =  (pow(dampingFactor*(analogRead(36)-2047),3)/devisor) + 2047;
  }


  

  else{      //mario kart mode

    int fowardBackShifted = (analogRead(36)-2047)/2;
    int leftRightShifted = (analogRead(35)-2047)/2;
    float a = (0.5*dampingFactor)+0.5;
    float b = 2*(1-(float(b)/2));


    /*if(leftRightShifted > 2047){
          myData.right = (1.3*fowardBackShifted) + (0.7*leftRightShifted) + 2047;
          myData.left = (1.3*fowardBackShifted) - (0.7*leftRightShifted) + 2047;
    }*/

    Serial.print("                    ");
    Serial.println(a);
    Serial.print("                  ");
    Serial.println(b);
    myData.right = dampingFactor*((a*1.3*fowardBackShifted) + (b*0.7*leftRightShifted)) + 2047;
    myData.left = dampingFactor*((a*1.3*fowardBackShifted) - (b*0.7*leftRightShifted)) + 2047;
    

    
      
    }
    



  if(digitalRead(18)==0){
    swapInt(&myData.right, &myData.left);
  }

  if(myData.right > 2047 && myData.left > 2047){
    if(triming.triming > 0){
      myData.right -= triming.triming;
    }
    else {
      myData.left += triming.triming;
    }
  }
  
  triming.updateTriming();
  Serial.print("Trim: ");
  Serial.println(triming.triming);

  Serial.print("Right: ");
  Serial.println(myData.right);\
  Serial.print("Left: ");
  Serial.println(myData.left);
  
  /*Serial.print("Left Right Data: ");
  Serial.println(analogRead(35));

  Serial.print("Button 1: ");
  Serial.println(digitalRead(33));

  Serial.print("Button 2: ");
  Serial.println(digitalRead(32));

  Serial.print("Switch 1: ");
  Serial.println(digitalRead(19));

  Serial.print("Switch 2: ");
  Serial.println(digitalRead(18));*/


  
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }
  delay(50);
}
