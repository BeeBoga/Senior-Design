#define LAT 7
#define SCLK 6
#define SIN 5
#define GSCLK 4

//769 bit register
//768th bit is control bit 0 sets grayscale
//1 sets dc,mc,bc,fc
//control latch is 371 bits long
//DC data latch is 336 bits long

//bits shift in from 0 towards 769
//in GS each color is 16 bits wide with bit 0 being OUTR0 bit 0 and bit 15 being OUTR0 bit 15

//msb == 1 stores here
//8 bit decoder 767:760
//bits 370:366 FC function control
//bits 365:345 BC global Bright starts at Rn,Gn,then Bn
//bits 344:336 MC max current starts at Rn, Gn, then Bn
//bits 335:0 DC dot correction 7 bits wide each first six are OUTR0



void setup() {
  // put your setup code here, to run once:

  int i, temp;
  int GS [49];
  int settings[769];
  int count =0;
  int times=0;
  int timetemp;
  int FC[5] = {0,0,1,1,1};

  Serial.begin(9600);

  GS[48]=0;

  for(i=0; i <49;i++)
  {
    if((i%3)==0)
      GS[i]=10000;
    else
      GS[i]=10000;
  }
  
  pinMode(LAT, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SIN, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  
  digitalWrite(SCLK, LOW);

  digitalWrite(SIN, HIGH);
  digitalWrite(LAT, LOW);

////////////////////////////////////////////////////////////////////////

  shiftOut(SIN, SCLK, MSBFIRST, 1);

    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 0);
    shiftOut(SIN, SCLK, MSBFIRST, 0);
    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 0);
    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 0);
  
  for(i=759;i>370;i--)
  {
    shiftOut(SIN, SCLK, MSBFIRST, 1);
  }

  
    shiftOut(SIN, SCLK, MSBFIRST, 0);
    shiftOut(SIN, SCLK, MSBFIRST, 0);
    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 1);
    shiftOut(SIN, SCLK, MSBFIRST, 1);

  for(i=365;i>=0;i--)
  {
    shiftOut(SIN, SCLK, MSBFIRST, 1);
  }


  digitalWrite(LAT, HIGH);

  digitalWrite(LAT, LOW);
 
  //////////////////////////////////////////////////////////////////////

  shiftOut(SIN, SCLK, MSBFIRST, GS[0]);

  times=millis();
  Serial.println(times);
  Serial.println(0);

 
  
  for(i=1;i<49;i++)
  {
    for(int x=15; x>=0; x--)
    {
    temp=bitRead(GS[i],x);
    shiftOut(SIN, SCLK, MSBFIRST, temp);
    //Serial.print(temp);
     //Serial.println(micros());
    count++;
    }
    //Serial.println("");
  }
  
  Serial.println(count+1);

  digitalWrite(LAT, HIGH);

  digitalWrite(LAT, LOW);

  

  timetemp=millis();
  Serial.println(timetemp);
  timetemp=timetemp-times;

  Serial.println(timetemp);
  
  analogWrite(GSCLK,127);
  

  
/*
  for(int x=0; x<769; x++)
  {
    digitalWrite(SCLK,HIGH);
    digitalWrite(SCLK,LOW);
  }

  digitalWrite(LAT, HIGH);
  delay(100);
  digitalWrite(LAT, LOW);

  digitalWrite(SIN, LOW);
  digitalWrite(SCLK, HIGH);
  digitalWrite(SCLK, LOW);
  digitalWrite(SIN, HIGH);

  for(int x=0; x<768; x++)
  {
    digitalWrite(SCLK,HIGH);
    digitalWrite(SCLK,LOW);
  }

  digitalWrite(LAT, HIGH);
  digitalWrite(LAT, LOW);

  analogWrite(GSCLK,512);
  */

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LAT, HIGH);

  //digitalWrite(LAT, LOW);

}
