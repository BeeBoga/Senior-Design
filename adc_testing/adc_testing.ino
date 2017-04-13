#define LAT 7
#define SCLK 6
#define SIN 5
#define GSCLK 4
//#define THRESHOLD 500
#define APIN 0
#define cs 8
#define data 9
#define clck 10
#define ANALOGISR 2
#define OFFSET 100

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

///////////////////////////////////////////////////


unsigned long timea=0;
unsigned long timeb=0;
float period=0;
unsigned long value[3];
unsigned long mil = 1000000.00000000;

float periodf=0.00000000000;

int i=0;
int count=0;
int repeat=0;
int A=0;
int B=0;

int THRESHOLD;

int dta[20];

char ans;

boolean flag = false;
boolean dlay= false;
boolean flag_analog=true;
boolean resete = false;



/////////////////////////////////////////////////////

  int required = B10010110;//8-bits
  int FC = B00111;//5-bitsw
  int BC_B = B1000000;//7-bits
  int BC_G = B1000000;//7-bits
  int BC_R = B1000000;//7-bits
  int MC_B = B100;//3-bits
  int MC_G = B100;//3-bits
  int MC_R = B100;//3-bits
  int DC_B[16];//7-bits
  int DC_G[16];//7-bits
  int DC_R[16];//7-bits

  int del = 300;

  int GS_B [16];//actual color assigned to each LED
  int GS_G [16];
  int GS_R [16];

  //int i;
  int ran;//used for testing

  int referencez = B00000000;
  int OFF = B00000000;
  
  int b_1 = B00000000;
  int g_1 = B10000000;
  int r_1 = B11111111;

  //colors
  int red[3] = {B00000000, B00000000, B11111111};
  int blue[3] = {B11111111, B00000000, B00000000};
  int green[3] = {B00000000, B11111111, B00000000};
  int yellow[3] = {B00000000, B11111111, B11111111};
  int cyan[3] = {B11111111, B11111111, B00000000};
  int magenta[3] = {B11111111, B00000000, B11111111};
  int orange[3] = { B00000000, B10000000, B11111111};
  int purple[3] = { B11111111, B00000000, B10000000};
  int pink[3] = {B10000000, B00000000, B11111111};
  int lightgreen[3] = {B00000000, B11111111, B10000000};
  int greencyan[3] =  {B10000000, B11111111, B00000000};
  int coolblue[3] = {B11111111, B10000000, B00000000};
  int offf[3] = {B00000000,B00000000,B00000000};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("begin");

  pinMode(cs, OUTPUT);
  pinMode(data, INPUT);
  pinMode(clck, OUTPUT);

  //pinMode(ANALOGISR, INPUT);

  
//attachInterrupt(digitalPinToInterrupt(2), set_limit, FALLING);

}

void freq()
{
  Serial.println("here");
  while(flag)
  {
    Serial.println("here");

      A=readingadc();
      B=readingadc();

      if((A<B)&&(resete==true)&&(A>=THRESHOLD))
      {
        Serial.println("here");
        if(count==0)
        {
          timea=micros();
          count++;
          resete = false;
        }
        else if(count==1)
        {
          timeb=micros();
          period=timeb-timea;
          resete = false;
          periodf = period/mil;
          periodf = 1/periodf; 
          //note(periodf);
          repeat=0;
          flag=false;
          count = 0;
        }
      }
        else if((A>B)&&(A<=THRESHOLD))
        {
          Serial.println("here");
          resete=true;
        }

  }

}







int readingadc()
{
  int number = 0;

  //a = micros();
  
  //digitalWrite(cs, HIGH);
  PORTH = PORTH | B00100000;
  //digitalWrite(cs, LOW);
  PORTH = PORTH & B11011111;

  for(int i = 0; i<15; i++)
  {
    //digitalWrite(clck, HIGH);
    PORTB = PORTB | B00010000;
    //digitalWrite(clck, LOW);
    PORTB = PORTB & B11101111;
    //dta[i] = digitalRead(data);
    if(i>2)
    {
    number = number<<1;
    number = number + ((PINH & B01000000)>>6);
    }
  }
  /*
  for(int i=3; i<15;i++)
  {
    number = number + (dta[i] <<(14-i));
  }
  */
  
  //delayMicroseconds(1);
  //b=micros();
  //a=b-a;
  //Serial.println(a);
  //Serial.println("done");
  //Serial.println(number);
  return(number);
  
  
}

