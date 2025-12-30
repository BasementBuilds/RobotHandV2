#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    float Adest;
    float Bdest;
    float Cdest;
    float Ddest;
    float Edest;
    float Fdest;

} struct_message;
struct_message myData;
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");
}



const int freq = 1000;
const int res = 8;
// motor PID control variables
int motorLimit = 240;
int kp = 450;
int kd = 300;
int kpT = 450;
int kdT = 2100;

// A = middle finger B = pointer finger C = ring finger D = pinky

float AcalculatedPos = 10.6;
int ArawPotValue = 0;
int ArawPotValuePrev = 0;
int Aoffset = 2000;
int Aadjusted = 0;
int AadjustedPrev = 0;

float BcalculatedPos = 10.0;
int BrawPotValue = 0;
int BrawPotValuePrev = 0;
int Boffset = 2000;
int Badjusted = 0;
int BadjustedPrev = 0;

float CcalculatedPos = 10.0;
int CrawPotValue = 0;
int CrawPotValuePrev = 0;
int Coffset = 2000;
int Cadjusted = 0;
int CadjustedPrev = 0;

float DcalculatedPos = 10.0;
int DrawPotValue = 0;
int DrawPotValuePrev = 0;
int Doffset = 2000;
int Dadjusted = 0;
int DadjustedPrev = 0;


float EcalculatedPos = 10.0;
int ErawPotValue = 0;
int ErawPotValuePrev = 0;
int Eoffset = 2000;
int Eadjusted = 0;
int EadjustedPrev = 0;

float FcalculatedPos = 10.0;
int FrawPotValue = 0;
int FrawPotValuePrev = 0;
int Foffset = 2000;
int Fadjusted = 0;
int FadjustedPrev = 0;

//  A is middle, B is pointer, C is Ring, D is pinky, E is thumb fold, F is thumb digits

float Agoal = 10.6;
float Aerror, AprevError;
int Apin1 = 2, Apin2 = 4;                   // motor signal pins
const int Achan1 =0, Achan2 = 1;            // channells
int Aout1 = 0, Aout2 = 0;   

float Bgoal = 10.2;
float Berror, BprevError;
int Bpin1 = 17, Bpin2 = 16;                   // motor signal pins
const int Bchan1 =2, Bchan2 = 3;            // channells
int Bout1 = 0, Bout2 = 0; 

float Cgoal = 10.3;
float Cerror, CprevError;
int Cpin1 = 13, Cpin2 = 12;                   // motor signal pins
const int Cchan1 =4, Cchan2 = 5;            // channells
int Cout1 = 0, Cout2 = 0; 

float Dgoal = 9.5;
float Derror, DprevError;
int Dpin1 = 5, Dpin2 = 18;                   // motor signal pins
const int Dchan1 =6, Dchan2 = 7;            // channells
int Dout1 = 0, Dout2 = 0; 

float Egoal = 10.2;
float Eerror, EprevError;
int Epin1 = 23, Epin2 = 22;                   // motor signal pins
const int Echan1 =8, Echan2 = 9;            // channells
int Eout1 = 0, Eout2 = 0; 

float Fgoal = 0.8;
float Ferror, FprevError;
int Fpin1 = 21, Fpin2 = 19;                   // motor signal pins
const int Fchan1 =10, Fchan2 = 11;            // channells
int Fout1 = 0, Fout2 = 0; 



void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));


  // Motor A setup
  ledcSetup(Achan1,freq,res);  ledcSetup(Achan2,freq,res);
  ledcAttachPin(Apin1,Achan1); ledcAttachPin(Apin2,Achan2);
  ledcWrite(Achan1, 0); ledcWrite(Achan2, 0);

  ledcSetup(Bchan1,freq,res);  ledcSetup(Bchan2,freq,res);
  ledcAttachPin(Bpin1,Bchan1); ledcAttachPin(Bpin2,Bchan2);
  ledcWrite(Bchan1, 0); ledcWrite(Bchan2, 0);

  ledcSetup(Cchan1,freq,res);  ledcSetup(Cchan2,freq,res);
  ledcAttachPin(Cpin1,Cchan1); ledcAttachPin(Cpin2,Cchan2);
  ledcWrite(Cchan1, 0); ledcWrite(Cchan2, 0);

  ledcSetup(Dchan1,freq,res);  ledcSetup(Dchan2,freq,res);
  ledcAttachPin(Dpin1,Dchan1); ledcAttachPin(Dpin2,Dchan2);
  ledcWrite(Dchan1, 0); ledcWrite(Dchan2, 0);

  ledcSetup(Echan1,freq,res);  ledcSetup(Echan2,freq,res);
  ledcAttachPin(Epin1,Echan1); ledcAttachPin(Epin2,Echan2);
  ledcWrite(Echan1, 0); ledcWrite(Echan2, 0);

  ledcSetup(Fchan1,freq,res);  ledcSetup(Fchan2,freq,res);
  ledcAttachPin(Fpin1,Fchan1); ledcAttachPin(Fpin2,Fchan2);
  ledcWrite(Fchan1, 0); ledcWrite(Fchan2, 0);

  pinMode(32,INPUT); pinMode(33,INPUT);pinMode(34,INPUT);pinMode(35,INPUT);pinMode(36,INPUT);pinMode(39,INPUT);


}

void loop() {
  
  if (Serial.available() > 0) {  // Check if data is available to read
    Fgoal = Serial.parseFloat();  // Read and parse the float value
    while (Serial.available() > 0) { Serial.read(); }
  }

  //receive comands from glove

  Agoal = constrain(myData.Adest,8.5,10.6);
  Bgoal = constrain(myData.Bdest,7.9,10.2);
  Cgoal = constrain(myData.Cdest,10.3,12.5);
  Dgoal = constrain(myData.Ddest,9.5,11.7);
  Egoal = constrain(myData.Edest,9.5,10.2);
  Fgoal = constrain(myData.Fdest,0.2,0.8);



  Agoal = constrain(Agoal,8.5,10.6);
  Bgoal = constrain(Bgoal,7.9,10.2);
  Cgoal = constrain(Cgoal,10.3,12.5);
  Dgoal = constrain(Dgoal,9.5,11.7);
  Egoal = constrain(Egoal,9.5,10.2);
  Fgoal = constrain(Fgoal,0.2,0.8);

  //find motor positions
  ArawPotValue = analogRead(34);
  if(ArawPotValue>=Aoffset){Aadjusted = ArawPotValue - Aoffset;}  else{ Aadjusted = ArawPotValue + (4095 - Aoffset); }
  AcalculatedPos = getMotorPos(AcalculatedPos,Aadjusted,AadjustedPrev);

  BrawPotValue = analogRead(35);
  if(BrawPotValue>=Boffset){Badjusted = BrawPotValue - Boffset;}  else{ Badjusted = BrawPotValue + (4095 - Boffset); }
  BcalculatedPos = getMotorPos(BcalculatedPos,Badjusted,BadjustedPrev);

  CrawPotValue = analogRead(32);
  if(CrawPotValue>=Coffset){Cadjusted = CrawPotValue - Coffset;}  else{ Cadjusted = CrawPotValue + (4095 - Coffset); }
  CcalculatedPos = getMotorPos(CcalculatedPos,Cadjusted,CadjustedPrev);
  
  DrawPotValue = analogRead(33);
  if(DrawPotValue>=Doffset){Dadjusted = DrawPotValue - Doffset;}  else{ Dadjusted = DrawPotValue + (4095 - Doffset); }
  DcalculatedPos = getMotorPos(DcalculatedPos,Dadjusted,DadjustedPrev);
 
  ErawPotValue = analogRead(36);
  if(ErawPotValue>=Eoffset){Eadjusted = ErawPotValue - Eoffset;}  else{ Eadjusted = ErawPotValue + (4095 - Eoffset); }
  EcalculatedPos = getMotorPos2(EcalculatedPos,Eadjusted,EadjustedPrev);

  FrawPotValue = analogRead(39);
  if(FrawPotValue>=Foffset){Fadjusted = FrawPotValue - Foffset;}  else{ Fadjusted = FrawPotValue + (4095 - Foffset); }
  FcalculatedPos = getThumbMotorPos(Fadjusted);
  
  
  //give motor power to move towards the goal
  Aerror = Agoal - AcalculatedPos;
  pidController(Aerror, AprevError, Aout1, Aout2);
  ledcWrite(Achan1, Aout1); ledcWrite(Achan2, Aout2);
  Berror = Bgoal - BcalculatedPos;
  pidController(Berror, BprevError, Bout1, Bout2);
  ledcWrite(Bchan1, Bout1); ledcWrite(Bchan2, Bout2);
  Cerror = Cgoal - CcalculatedPos;
  pidController(Cerror, CprevError, Cout1, Cout2);
  ledcWrite(Cchan1, Cout1); ledcWrite(Cchan2, Cout2);
  Derror = Dgoal - DcalculatedPos;
  pidController(Derror, DprevError, Dout1, Dout2);
  ledcWrite(Dchan1, Dout1); ledcWrite(Dchan2, Dout2);
  Eerror = Egoal - EcalculatedPos;
  pidController(Eerror, EprevError, Eout1, Eout2);
  ledcWrite(Echan1, Eout1); ledcWrite(Echan2, Eout2);
  Ferror = Fgoal - FcalculatedPos;
  pidControllerThumb(Ferror, FprevError, Fout1, Fout2);
  ledcWrite(Fchan1, Fout1); ledcWrite(Fchan2, Fout2);



  // update previous pot values and errors
  ArawPotValuePrev = ArawPotValue;
  AadjustedPrev = Aadjusted;
  AprevError = Aerror;

  BrawPotValuePrev = BrawPotValue;
  BadjustedPrev = Badjusted;
  BprevError = Berror;

  CrawPotValuePrev = CrawPotValue;
  CadjustedPrev = Cadjusted;
  CprevError = Cerror;

  DrawPotValuePrev = DrawPotValue;
  DadjustedPrev = Dadjusted;
  DprevError = Derror;

  ErawPotValuePrev = ErawPotValue;
  EadjustedPrev = Eadjusted;
  EprevError = Eerror;

  FrawPotValuePrev = FrawPotValue;
  FadjustedPrev = Fadjusted;
  FprevError = Ferror;


  Serial.print(Fgoal);
  Serial.print("    ");
  Serial.println(FcalculatedPos);



  delay (3);
}

float getMotorPos (float calculatedPos, int rawPotVal, int prevRawPotVal){

  float UpdatedMotorPos = 10;
  int totalSpins = int(calculatedPos);

  float fraction = (rawPotVal*1.0)/4097.0;
  if(fraction>0.99){fraction = .99;}

  // figure out if we completed a full rotation, and in what direction
  if(rawPotVal<500 && prevRawPotVal>3600){ totalSpins +=1; }
  if(rawPotVal>3600 && prevRawPotVal<500){ totalSpins -=1;}


  UpdatedMotorPos = totalSpins*1.0+fraction*1.0;
  
  if(UpdatedMotorPos-calculatedPos>.5){
    totalSpins-=1;
    UpdatedMotorPos = totalSpins*1.0+fraction*1.0;
    //Serial.println("problemmmmmmmmmmmmmmmmmmmmmmmmmm");
  }
   
     
  if(UpdatedMotorPos-calculatedPos<-.5){
    totalSpins+=1;
    UpdatedMotorPos = totalSpins*1.0+fraction*1.0;
    //Serial.println("problemmmmmmmmmmmmmmmmmmmmmmmmmm");
  }



  return UpdatedMotorPos;
  
}

void pidController (float error, float prevError, int & out1, int & out2){

  int pwmOut = 0;
  float errorDerriv = (error-prevError);
  pwmOut = kp*error + kd*errorDerriv;     // implement pd controller
  if(abs(error)<.07){pwmOut = 0;}               //prevents vibrations and power draw at low errrors
  if(abs(pwmOut)<35){pwmOut = 0;}

  pwmOut = int(abs(pwmOut));                   //must be positive
  pwmOut = constrain(pwmOut,0,motorLimit);// constrain maximum speed

  if (error<0){         // switch motor direction based on error.
    out1 = pwmOut;
    out2 = 0;
    }
  else{
    out1 = 0;
    out2 = pwmOut;
  }
  //Serial.println(pwmOut);
}

void pidControllerThumb (float error, float prevError, int & out1, int & out2){

  int pwmOut = 0;
  float errorDerriv = (error-prevError);
  pwmOut = kpT*error + kdT*errorDerriv;     // implement pd controller
  if(abs(error)<0.04){pwmOut = 0;}               //prevents vibrations and power draw at low errrors
  if(abs(pwmOut)<35){pwmOut = 0;}

  pwmOut = int(abs(pwmOut));                   //must be positive
  pwmOut = constrain(pwmOut,0,motorLimit);// constrain maximum speed

  if (error<0){         // switch motor direction based on error.
    out1 = pwmOut;
    out2 = 0;
    }
  else{
    out1 = 0;
    out2 = pwmOut;
  }
}



float getThumbMotorPos (int adjusted){

  float UpdatedMotorPos;
  
  UpdatedMotorPos = (adjusted*1.0)/4097.0;

  return UpdatedMotorPos;  
}


float getMotorPos2 (float calculatedPos, int adjusted, int adjustedPrev){

  float UpdatedMotorPos;
  int totalSpins = int(calculatedPos);

  float fraction = (adjusted*1.0)/4097.0;
  float pFraction =(adjustedPrev*1.0)/4097.0;

  if(fraction>0.99){fraction = .99;}
  if(pFraction>0.99){pFraction = .99;}


  if(fraction-pFraction > .96){ totalSpins -=1; }
  if(fraction-pFraction < -.96){ totalSpins +=1;}

  UpdatedMotorPos = totalSpins*1.0+fraction*1.0;
  
  return UpdatedMotorPos;
  
}
