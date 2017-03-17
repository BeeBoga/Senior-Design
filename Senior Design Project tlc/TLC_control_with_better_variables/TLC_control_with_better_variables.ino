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
  int GS [48];//actual color assigned to each LED

  
  int FC=0;//settings of repeat...

  
  int BC[3];//global brightness of each color
  int BC_B = 0;//7-bits
  int BC_R = 0;
  int BC_G = 0;

  
  int MC[3];//Maximum current allowed
  int MC_B = 0;//3-bits
  int MC_R = 0;
  int MC_G = 0;

  int DC[48];

  
  int settings[769];
  int count =0;
  int times=0;
  int timetemp;
  int FC[5] = {0,0,1,1,1};

  pinMode(2, OUTPUT);

  //analogWrite(2, 128);

  Serial.begin(9600);

  /*
  GS[48]=0;

  for(i=0; i <49;i++)
  {
    if((i%3)==0)
      GS[i]=10000;
    else
      GS[i]=10000;
  }
  */
  
  pinMode(LAT, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SIN, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  
  digitalWrite(SCLK, LOW);

  digitalWrite(SIN, HIGH);
  digitalWrite(LAT, LOW);

////////////////////////////////////////////////////////////////////////

sendd(1);

sendd(1);
sendd(0);
sendd(0);
sendd(1);
sendd(0);
sendd(1);
sendd(1);
sendd(0);

for(i=0; i<389; i++)
{
  sendd(1);
}

sendd(0);
sendd(1);
sendd(1);
sendd(1);
sendd(1);

for(i=0; i<21; i++)//bc
{
  sendd(1);
}

for(i=0; i<9; i++)//mc
{
  sendd(1);
}

for(i=0; i<336; i++)//dc
{
  sendd(1);
}

digitalWrite(LAT, HIGH);

digitalWrite(LAT, LOW);

//////////////////////////////////////////////////////////////////////////////////////

sendd(0);

for(i=0; i<720; i++)
{
  sendd(1);
}

/*
for(i=0; i<4; i++)//red
{
  sendd(0);
  sendd(0);
  sendd(1);
  sendd(1);
  
}




for(i=0; i<16; i++)//red
{
  sendd(1);
  
}

*/
//10011010 10011010 b
sendd(0);
sendd(0);
sendd(1);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(0);

sendd(0);
sendd(0);
sendd(1);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(0);



//10110111 10110111 g
sendd(1);
sendd(0);
sendd(0);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(0);

sendd(1);
sendd(0);
sendd(0);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(0);



//00111010 00111010 r

sendd(1);
sendd(0);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(1);
sendd(1);

sendd(1);
sendd(0);
sendd(1);
sendd(1);
sendd(0);
sendd(1);
sendd(1);
sendd(1);



//shiftOut(SIN, SCLK, MSBFIRST, 32769);
/*

for(i=0; i<4; i++)//red
{
  sendd(0);
  sendd(0);
  sendd(1);
  sendd(1);
  
}

*/


digitalWrite(LAT, HIGH);

digitalWrite(LAT, LOW);


}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2,HIGH);
  digitalWrite(2,LOW);

}

void sendd(int in)
{
  digitalWrite(SIN, in);
  digitalWrite(SCLK, 1);
  digitalWrite(SCLK, 0);
}


