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

  int required = B10010110;//8-bits
  int FC = B01111;//5-bitsw
  int BC_B = B1000000;//7-bits
  int BC_G = B1000000;//7-bits
  int BC_R = B1000000;//7-bits
  int MC_B = B100;//3-bits
  int MC_G = B100;//3-bits
  int MC_R = B100;//3-bits
  int DC_B[16];//7-bits
  int DC_G[16];//7-bits
  int DC_R[16];//7-bits

  int GS_B [16];//actual color assigned to each LED
  int GS_G [16];
  int GS_R [16];

  int i;
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
  
  


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  for(i=0; i<16; i++)
  {
    DC_B[i] = B11111111;
    DC_G[i] = B11111111;
    DC_R[i] = B11111111;
  }

  for(i=0; i<16; i++)
  {
    GS_B[i] = B00000000;
    GS_G[i] = B00000000;
    GS_R[i] = B00000000;
  }

  
  
  pinMode(LAT, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SIN, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  
  digitalWrite(SCLK, LOW);

  digitalWrite(SIN, HIGH);
  digitalWrite(LAT, LOW);
  //analogWrite(GSCLK, 127);


  control_latch();

  set_blue(0);
  set_magenta(2);
  set_purple(3);
  set_red(4);
  color();


}

int timer=0;

void loop() {
  // put your main code here, to run repeatedly:
  

 
  digitalWrite(GSCLK,HIGH);
  digitalWrite(GSCLK,LOW);

 
  


}


/////////////////////////////////////////////////////////////////////////////////////////////
void sendd(int in)
{
  digitalWrite(SIN, in);
  digitalWrite(SCLK, 1);
  digitalWrite(SCLK, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
int shift(int digits, int binnum)//shift out data from MSB of any length
{
  int tempor = 0;
  int counter = 1;

  while(counter<(digits+1))
  {
  tempor = (1 & (binnum>>(digits-counter)));
  
  sendd(tempor);
  counter++;
  }
}

////////////////////////////////////////////////////////////////////////////////////////
int control_latch()
{
  sendd(1);

  shift(8, required);

  for(i=0; i<389; i++)
  {
    sendd(1);
  }

  shift(5, FC);

  shift(7, BC_B);
  shift(7, BC_R);
  shift(7, BC_G);


  shift(3, MC_R);
  shift(3, MC_B);
  shift(3, MC_G);


  for(i=15; i>=0; i--)//dc
  {
    shift(7, DC_B[i]);
    shift(7, DC_G[i]);
    shift(7, DC_R[i]);
    Serial.println(i);
  }
  

  digitalWrite(LAT, HIGH);

  digitalWrite(LAT, LOW);

}

//////////////////////////////////////////////////////////////////////////////////////////////
int color()
{
sendd(0);

for(i=15; i>=0; i--)
{
  shift(8,GS_B[i]);
  shift(8, referencez);
  shift(8,GS_G[i]);
  shift(8, referencez);
  shift(8,GS_R[i]);
  shift(8, referencez);
}




digitalWrite(LAT, HIGH);

digitalWrite(LAT, LOW);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void set_blue(int value)
{
    GS_B[value] = blue[0];
    GS_G[value] = blue[1];
    GS_R[value] = blue[2];
}

void set_red(int value)
{
    GS_B[value] = red[0];
    GS_G[value] = red[1];
    GS_R[value] = red[2];
}

void set_green(int value)
{
    GS_B[value] = green[0];
    GS_G[value] = green[1];
    GS_R[value] = green[2];
}

void set_yellow(int value)
{
    GS_B[value] = yellow[0];
    GS_G[value] = yellow[1];
    GS_R[value] = yellow[2];
}

void set_cyan(int value)
{
    GS_B[value] = cyan[0];
    GS_G[value] = cyan[1];
    GS_R[value] = cyan[2];
}

void set_magenta(int value)
{
    GS_B[value] = magenta[0];
    GS_G[value] = magenta[1];
    GS_R[value] = magenta[2];
}

void set_orange(int value)
{
    GS_B[value] = orange[0];
    GS_G[value] = orange[1];
    GS_R[value] = orange[2];
}

void set_purple(int value)
{
    GS_B[value] = purple[0];
    GS_G[value] = purple[1];
    GS_R[value] = purple[2];
}

void set_pink(int value)
{
    GS_B[value] = pink[0];
    GS_G[value] = pink[1];
    GS_R[value] = pink[2];
}

void set_lightgreen(int value)
{
    GS_B[value] = lightgreen[0];
    GS_G[value] = lightgreen[1];
    GS_R[value] = lightgreen[2];
}

void set_greencyan(int value)
{
    GS_B[value] = greencyan[0];
    GS_G[value] = greencyan[1];
    GS_R[value] = greencyan[2];
}

void set_coolblue(int value)
{
    GS_B[value] = coolblue[0];
    GS_G[value] = coolblue[1];
    GS_R[value] = coolblue[2];
}

