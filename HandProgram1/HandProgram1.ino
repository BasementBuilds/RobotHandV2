const int freq = 1000;
const int res = 8;
// motor PID control variables
int motorLimit = 240;
int kp = 450;
int kd = 300;

// A = middle finger B = pointer finger C = ring finger D = pinky

float AcalculatedPos = 10.0;
int ArawPotValue = 0;
int ArawPotValuePrev = 0;

float BcalculatedPos = 10.0;
int BrawPotValue = 0;
int BrawPotValuePrev = 0;

float CcalculatedPos = 10.0;
int CrawPotValue = 0;
int CrawPotValuePrev = 0;

float DcalculatedPos = 10.0;
int DrawPotValue = 0;
int DrawPotValuePrev = 0;

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



float Agoal = 9.8;
float Aerror, AprevError;
int Apin1 = 2, Apin2 = 4;                   // motor signal pins
const int Achan1 =0, Achan2 = 1;            // channells
int Aout1 = 0, Aout2 = 0;   

float Bgoal = 10.2;
float Berror, BprevError;
int Bpin1 = 17, Bpin2 = 16;                   // motor signal pins
const int Bchan1 =2, Bchan2 = 3;            // channells
int Bout1 = 0, Bout2 = 0; 

float Cgoal = 9.8;
float Cerror, CprevError;
int Cpin1 = 13, Cpin2 = 12;                   // motor signal pins
const int Cchan1 =4, Cchan2 = 5;            // channells
int Cout1 = 0, Cout2 = 0; 

float Dgoal = 9.6;
float Derror, DprevError;
int Dpin1 = 5, Dpin2 = 18;                   // motor signal pins
const int Dchan1 =6, Dchan2 = 7;            // channells
int Dout1 = 0, Dout2 = 0; 

float Egoal = 10.2;
float Eerror, EprevError;
int Epin1 = 23, Epin2 = 22;                   // motor signal pins
const int Echan1 =8, Echan2 = 9;            // channells
int Eout1 = 0, Eout2 = 0; 

float Fgoal = 10.5;
float Ferror, FprevError;
int Fpin1 = 21, Fpin2 = 19;                   // motor signal pins
const int Fchan1 =10, Fchan2 = 11;            // channells
int Fout1 = 0, Fout2 = 0; 



void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);

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

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (Serial.available() > 0) {  // Check if data is available to read
    Fgoal = Serial.parseFloat();  // Read and parse the float value
    while (Serial.available() > 0) { Serial.read(); }
  }


  //find motor positions
  ArawPotValue = analogRead(34);
  AcalculatedPos = getMotorPos(AcalculatedPos,ArawPotValue,ArawPotValuePrev);
  BrawPotValue = analogRead(35);
  BcalculatedPos = getMotorPos(BcalculatedPos,BrawPotValue,BrawPotValuePrev);
  CrawPotValue = analogRead(32);
  CcalculatedPos = getMotorPos(CcalculatedPos,CrawPotValue,CrawPotValuePrev);
  DrawPotValue = analogRead(33);
  DcalculatedPos = getMotorPos(DcalculatedPos,DrawPotValue,DrawPotValuePrev);
 
  ErawPotValue = analogRead(26);
  if(ErawPotValue>=Eoffset){Eadjusted = ErawPotValue - Eoffset;}  else{ Eadjusted = ErawPotValue + (4095 - Eoffset); }
  EcalculatedPos = getThumbMotorPos(EcalculatedPos,Eadjusted,EadjustedPrev);
  FrawPotValue = analogRead(14);
  if(FrawPotValue>=Foffset){Fadjusted = FrawPotValue - Foffset;}  else{ Fadjusted = FrawPotValue + (4095 - Foffset); }
  FcalculatedPos = getThumbMotorPos(FcalculatedPos,Fadjusted,FadjustedPrev);
  
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
  pidController(Ferror, FprevError, Fout1, Fout2);
  ledcWrite(Fchan1, Fout1); ledcWrite(Fchan2, Fout2);



  // update previous pot values and errors
  ArawPotValuePrev = ArawPotValue;
  AprevError = Aerror;
  BrawPotValuePrev = BrawPotValue;
  BprevError = Berror;
  CrawPotValuePrev = CrawPotValue;
  CprevError = Cerror;
  DrawPotValuePrev = DrawPotValue;
  DprevError = Derror;

  ErawPotValuePrev = ErawPotValue;
  EadjustedPrev = Eadjusted;
  EprevError = Eerror;
  FrawPotValuePrev = FrawPotValue;
  FadjustedPrev = Fadjusted;
  FprevError = Ferror;


  Serial.println(FcalculatedPos); 
  //Serial.println(CrawPotValue) ;
  //Serial.println(analogRead(33));


  delay (5);
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


float getThumbMotorPos (float calculatedPos, int rawPotVal, int prevRawPotVal){

  float UpdatedMotorPos = 10;
  int totalSpins = int(calculatedPos);

  float fraction = (rawPotVal*1.0)/4097.0;

  if(fraction>0.99){fraction = .99;}


  if(rawPotVal<300 && prevRawPotVal>3800){ totalSpins +=1; }

  if(rawPotVal>3800 && prevRawPotVal<300){ totalSpins -=1;}

  UpdatedMotorPos = totalSpins*1.0+fraction*1.0;
  
  return UpdatedMotorPos;
  
}
