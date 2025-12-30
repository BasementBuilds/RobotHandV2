#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>


uint8_t broadcastAddress[] = {0x6C, 0xC8, 0x40, 0x8A, 0x89, 0x58};

typedef struct struct_message {

    float Adest;
    float Bdest;
    float Cdest;
    float Ddest;
    float Edest;
    float Fdest;


} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


// thresholds for open close glove
int PointerOpenGlv = 2100,  MiddleOpenGlv = 2000, RingOpenGlv = 1900,  FoldOpenGlv = 3000,  DigitsOpenGlv = 1900;
int PointerCloseGlv = 900, MiddleCloseGlv = 150, RingCloseGlv = 450, FoldCloseGlv = 2800, DigitsCloseGlv = 1000;

float AOpen  = 10.6,  BOpen  = 10.2,  COpen  = 10.3, DOpen  = 9.5, EOpen  = 10.2, FOpen  = 0.8;
float AMid   = 9.5, BMid   = 8.9, CMid   = 11.4, DMid   = 10.5, EMid   = 9.8, FMid   = 0.5;
float AClose = 8.5, BClose = 7.9, CClose = 12.5,  DClose = 11.6, EClose = 9.5, FClose = 0.2;






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(25,OUTPUT);




  WiFi.mode(WIFI_STA);
  //initialize espNOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
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
  // put your main code here, to run repeatedly:
  float Adest = AOpen;
  float Bdest = BOpen; 
  float Cdest = COpen;
  float Ddest = DOpen;
  float Edest = EOpen;
  float Fdest = FOpen;

  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(14,HIGH);
  digitalWrite(27,HIGH);
  digitalWrite(25,HIGH);

  delay(2);
  //need to find actual pin nums
  int GlvMiddle = analogRead(32);
  int GlvPointer = analogRead(33);
  int GlvRing= analogRead(35);
  int GlvFold = analogRead(39);
  int GlvDigits = analogRead(34);

  delay(1);

  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  digitalWrite(14,LOW);
  digitalWrite(27,LOW);
  digitalWrite(25,LOW);

  //need to find which letter motors are which fingers
  if(GlvPointer >= PointerOpenGlv) { Bdest = BOpen ;} else if(GlvPointer >= PointerCloseGlv) { Bdest = BMid; } else { Bdest = BClose;} 
  if(GlvMiddle  >= MiddleOpenGlv)  { Adest = AOpen ;} else if(GlvMiddle  >= MiddleCloseGlv)  { Adest = AMid; } else { Adest = AClose;} 
  if(GlvRing >= RingOpenGlv) { Cdest = COpen; Ddest = DOpen;} else if(GlvRing >= RingCloseGlv) { Cdest = CMid; Ddest = DMid;} else { Cdest = CClose; Ddest = DClose;} 
  if(GlvFold >= FoldOpenGlv)       { Edest = EOpen ;} else if(GlvFold >= FoldCloseGlv)       { Edest = EMid; } else { Edest = EClose;} 
  if(GlvDigits >= DigitsOpenGlv)   { Fdest = FOpen ;} else if(GlvDigits >= DigitsCloseGlv)   { Fdest = FMid; } else { Fdest = FClose;} 

  Serial.print(GlvDigits);
  Serial.print( "   ");
  Serial.println(GlvFold);

  /*
  myData.glvFold = GlvFold;
  myData.glvDigits = GlvDigits;
  myData.glvPoint = GlvPointer;
  myData.glvMiddle = GlvMiddle;
  myData.glvRing = GlvRing;
  */
  myData.Adest = Adest;
  myData.Bdest = Bdest;
  myData.Cdest = Cdest;
  myData.Ddest = Ddest;
  myData.Edest = Edest;
  myData.Fdest = Fdest;

  if (Edest == EOpen){Serial.println("Open");} else if (Edest == EMid){Serial.println("mid");} else if (Edest == EClose){Serial.println("close");}
  //if (Fdest == FOpen){Serial.println("Open");} else if (Fdest == FMid){Serial.println("mid");} else if (Fdest == FClose){Serial.println("close");}


  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {  Serial.println("Sent with success");  }
  //else { Serial.println("Error sending the data");  }

  delay(90);
}
