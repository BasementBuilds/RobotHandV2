#include <esp_now.h>
#include <WiFi.h>

bool printArray = false;
int gestureNum = 0;

typedef struct struct_message {
    int arrayValues[16];
    int Paverage;
    int knnPrediction;
} struct_message;

// Create a struct_message called myData
struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");

}

void setup() {
  pinMode(2,OUTPUT);  pinMode(15,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}

void loop() {
  printArray = false;
  if (Serial.available() > 0) {
    gestureNum = Serial.parseInt();
    printArray = true;
  }
  // put your main code here, to run repeatedly:
  int avg = myData.Paverage;


  if(printArray && gestureNum>0){
  
    for(int i = 0; i<16;i++){
      Serial.print(myData.arrayValues[i]);
      Serial.print(",");
    }
    //Serial.print("        ");
    Serial.println(gestureNum);
    //Serial.print(",     ");
    //Serial.println(avg);

  }
/*
  int pred = myData.knnPrediction;
  if (pred == 1){ Serial.println("Open");}
  if (pred == 2){ Serial.println("Close");}
  if (pred == 3){ Serial.println("Point");}
  if (pred == 4){ Serial.println("Pinch");}

  
  //change LEDs based on gesture
  if (pred == 1){ digitalWrite(15,LOW); digitalWrite(2,LOW);}    //open,  both off
  if (pred == 2){ digitalWrite(15,HIGH); digitalWrite(2,HIGH);}  // close, both on
  if (pred == 3){ digitalWrite(15,LOW); digitalWrite(2,HIGH);}   //point, blue on
  if (pred == 4){ digitalWrite(15,HIGH); digitalWrite(2,LOW);}   // pinch, red on

*/

  delay(150);

}
