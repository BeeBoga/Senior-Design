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
#define INTRODELAY 50

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

int mode = 0;
int last_color=-1;
int last_interrupt=0;
int led_order[12] = {1,5,0,4,8,9,10,11,7,3,6,2};
int led_colors[12] ={1,8,2,3,9,4,10,5,6,11,7,12};

unsigned long timea=0;
unsigned long timeb=0;
float period=0;
unsigned long value[3];
unsigned long mil = 1000000.00000000;

float periodf=0.00000000000;
float freq1 = 0.00000000000;
float freq2 = 0.00000000000;

int i=0;
int count=0;
int repeat=0;
int A=0;
int B=0;

int THRESHOLD;

int dta[20];

char ans;

boolean flag = false;
boolean dlay= true;
boolean flag_analog=true;
boolean resete = false;
boolean flag_switch = true;
boolean flag_intro = true;



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

  attachInterrupt(digitalPinToInterrupt(2), to_tuner, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), set_limit, FALLING);
  attachInterrupt(digitalPinToInterrupt(18), switch_mode, FALLING);

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

  pinMode(ANALOGISR, INPUT);
  
  pinMode(LAT, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SIN, OUTPUT);
  pinMode(GSCLK, OUTPUT);
  
  digitalWrite(SCLK, LOW);

  digitalWrite(SIN, HIGH);
  digitalWrite(LAT, LOW);
  //analogWrite(GSCLK, 127);

  pinMode(cs,OUTPUT);
  pinMode(data,INPUT);
  pinMode(clck,OUTPUT);

  THRESHOLD = map(analogRead(0),0,1023,0,4095);

  control_latch();
  control_latch();
  //set_off(0);
  color();

  flag = true;

  
  //freq();
  //intro();



}

int timer=0;

void loop() {
  // put your main code here, to run repeatedly:
  
if(!flag_switch)
{
  delay(250);
  flag_switch = true;
}
//Serial.println("hello");

if(flag_intro)
  {
    intro();
    flag_intro = false;
    mode = 0;
  }

while(mode==1)
{
  flag = true;
  all_flash(freq());
  if(dlay)
    delay(250);
}

while(mode==0)
{
  flag = true;
  freq2 = freq();
  if(freq2<(freq1*1.05)&&freq2>(freq1*.95))
  {
    tune(freq2);
  }
  freq1=freq2;
  //Serial.println(mode);
}

//The following is a stability code for stabalizing outputs
/*
freq2 = freq();
  if(freq2<(freq1*1.1)&&freq2>(freq1*.9))
  {
    all_flash(freq2);
  }
  freq1=freq2;

  
*/
  

 
  


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
    //Serial.println(i);
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

void set_off(int value)
{
    GS_B[value] = offf[0];
    GS_G[value] = offf[1];
    GS_R[value] = offf[2];
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void colors(int note, int number)
{
  if(note == 1)
    set_blue(number);
  else if(note == 8)
    set_red(number);
  else if(note == 2)
    set_yellow(number);
  else if(note == 3)
    set_green(number);
  else if(note == 9)
    set_cyan(number);
  else if(note == 4)
    set_magenta(number);
  else if(note == 10)
    set_orange(number);
  else if(note == 5)
    set_purple(number);
  else if(note == 6)
    set_pink(number);
  else if(note == 11)
    set_lightgreen(number);
  else if(note == 7)
    set_greencyan(number);
  else if(note == 12)
    set_coolblue(number);
}


///////////////////////////////////////////////////////////////////////////////////////////////////


void all_flash(float frequency)
{
  int n=0;
  int val[] = {1,2,4,8,16};

  Serial.println(frequency);


  for(n=0;n<5;n++)
  {
  
    if(((64.5*val[n])<frequency)&&(frequency<=(66.38*val[n])))                  //C
    {
      if(last_color==3)
        break;
      for(int a=0; a<12; a++)
        colors(3,a);
      color();
      last_color = 3;
      break;
    }
    else if(((68.32*val[n])<frequency)&&(frequency<=(70.33*val[n])))            //C#
    {
      if(last_color==9)
        break;
      for(int a=0; a<12; a++)
        colors(9,a);
      color();
      last_color = 9;
      break;
    }
    else if(((72.39*val[n])<frequency)&&(frequency<=(74.51*val[n])))             //D
    {
      if(last_color==4)
        break;
      for(int a=0; a<12; a++)
        colors(4,a);
      color();
      last_color = 4;
      break;
    }
    else if(((76.69*val[n])<frequency)&&(frequency<=(78.94*val[n])))              //D#
    {
      if(last_color==10)
        break;
      for(int a=0; a<12; a++)
        colors(10,a);
      color();
      last_color = 10;
      break;
    }
    else if(((81.25*val[n])<frequency)&&(frequency<=(83.64*val[n])))             //E
    {
      if(last_color==5)
        break;
      for(int a=0; a<12; a++)
        colors(5,a);
      color();
      last_color = 5;
      break;
    }
    else if(((86.01*val[n])<frequency)&&(frequency<=(88.6*val[n])))            //F
    {
      if(last_color==6)
        break;
      for(int a=0; a<12; a++)
        colors(6,a);
      color();
      last_color = 6;
      break;
    }
    else if(((91.2*val[n])<frequency)&&(frequency<=(93.875*val[n])))           //F#
    {
      if(last_color==11)
        break;
      for(int a=0; a<12; a++)
        colors(11,a);
      color();
      last_color = 11;
      break;
    }
    else if(((96.63*val[n])<frequency)&&(frequency<=(99.46*val[n])))          //G
    {
      if(last_color==7)
        break;
      for(int a=0; a<12; a++)
        colors(7,a);
      color();
      last_color = 7;
      break;
    }
    else if(((102.37*val[n])<frequency)&&(frequency<=(105.37*val[n])))          //G#
    {
      if(last_color==12)
        break;
      for(int a=0; a<12; a++)
        colors(12,a);
      color();
      last_color = 12;
      break;
    }
    else if(((108.46*val[n])<frequency)&&(frequency<=(111.64*val[n])))          //A
    {
      if(last_color==1)
        break;
      for(int a=0; a<12; a++)
        colors(1,a);
      color();
      last_color = 1;
      break;
    }
    else if(((114.91*val[n])<frequency)&&(frequency<=(118.27*val[n])))          //A#
    {
      if(last_color==8)
        break;
      for(int a=0; a<12; a++)
        colors(8,a);
      color();
      last_color = 8;
      break;
    }
    else if(((121.74*val[n])<frequency)&&(frequency<=(125.31*val[n])))          //B
    {
      if(last_color==2)
        break;
      for(int a=0; a<12; a++)
        colors(2,a);
      color();
      last_color = 2;
      break;
    }

  }
  //for(int a=0; a<12; a++)
    //set_off(a);
  //color();
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void tune(float frequency)
{
  int n=0;
  int val[] = {1,2,4,8,16};

  Serial.println(frequency);


  for(n=0;n<5;n++)
  {
  
    if(((64.5*val[n])<frequency)&&(frequency<=(66.38*val[n])))                  //C
    {
      if(last_color==3)
        break;
      for(int a=0; a<12; a++)
        set_off(a);
      colors(3,4);
      color();
      last_color = 3;
      break;
    }
    else if(((68.32*val[n])<frequency)&&(frequency<=(70.33*val[n])))            //C#
    {
      if(last_color==9)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(9,8);
      color();
      last_color = 9;
      break;
    }
    else if(((72.39*val[n])<frequency)&&(frequency<=(74.51*val[n])))             //D
    {
      if(last_color==4)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(4,9);
      color();
      last_color = 4;
      break;
    }
    else if(((76.69*val[n])<frequency)&&(frequency<=(78.94*val[n])))              //D#
    {
      if(last_color==10)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(10,10);
      color();
      last_color = 10;
      break;
    }
    else if(((81.25*val[n])<frequency)&&(frequency<=(83.64*val[n])))             //E
    {
      if(last_color==5)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(5,11);
      color();
      last_color = 5;
      break;
    }
    else if(((86.01*val[n])<frequency)&&(frequency<=(88.6*val[n])))            //F
    {
      if(last_color==6)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(6,7);
      color();
      last_color = 6;
      break;
    }
    else if(((91.2*val[n])<frequency)&&(frequency<=(93.875*val[n])))           //F#
    {
      if(last_color==11)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(11,3);
      color();
      last_color = 11;
      break;
    }
    else if(((96.63*val[n])<frequency)&&(frequency<=(99.46*val[n])))          //G
    {
      if(last_color==7)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(7,6);
      color();
      last_color = 7;
      break;
    }
    else if(((102.37*val[n])<frequency)&&(frequency<=(105.37*val[n])))          //G#
    {
      if(last_color==12)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(12,2);
      color();
      last_color = 12;
      break;
    }
    else if(((108.46*val[n])<frequency)&&(frequency<=(111.64*val[n])))          //A
    {
      if(last_color==1)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(1,1);
      color();
      last_color = 1;
      break;
    }
    else if(((114.91*val[n])<frequency)&&(frequency<=(118.27*val[n])))          //A#
    {
      if(last_color==8)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(8,5);
      color();
      last_color = 8;
      break;
    }
    else if(((121.74*val[n])<frequency)&&(frequency<=(125.31*val[n])))          //B
    {
      if(last_color==2)
        break;
      for(int a=0; a<12; a++)
      set_off(a);
        colors(2,0);
      color();
      last_color = 2;
      break;
    }

  }
  //for(int a=0; a<12; a++)
    //set_off(a);
  //color();
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

float freq()
{
  //Serial.println("here");
  while(flag)
  {
    //Serial.println("here");

      A=readingadc();
      B=readingadc();

      

      if((A<B)&&(resete==true)&&(A>=THRESHOLD))
      {
        //Serial.println("here");
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
          //repeat=0;
          flag=false;
          count = 0;
          return periodf;
        }
      }
        else if((A>B)&&(A<=THRESHOLD))
        {
          //Serial.println("here");
          resete=true;
        }

  }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

  return(number);
  
  
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void set_limit()
{
  THRESHOLD = map(analogRead(0),0,1023,0,4095);
  flag_analog = true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void switch_mode()
{
  //if(flag_switch)
  if((millis()-last_interrupt)>350)
  {
    if(mode<1)
      mode++;
     else
      mode=1;
    last_interrupt = millis();
     //flag_switch = false;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void to_tuner()
{
  if((millis()-last_interrupt)>350)
    {
      if(mode==0)
      {
        flag_intro = true;
        mode = -1;
        last_interrupt = millis();
      }
      else
      {
        mode = 0;
        last_interrupt = millis();
      }
    }
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void intro()
{
  for(int b=0; b<12;b++)
    set_off(b);
  color();
  
  for(int b=0; b<12;b++)
  {
    if(b==0)
    {
      colors(led_colors[b],led_order[b]);
      color();
      delay(INTRODELAY);
    }
    else
    {
      set_off(led_order[b-1]);
      colors(led_colors[b], led_order[b]);
      color();
      delay(INTRODELAY);
    }
      
  }

  
  for(int b=0; b<12;b++)
  {
    if(b==0)
    {
      set_off(led_order[11]);
      colors(led_colors[b],led_order[b]);
      color();
      delay(INTRODELAY);
    }
    else
    {
      set_off(led_order[b-1]);
      colors(led_colors[b], led_order[b]);
      color();
      delay(INTRODELAY);
    }
      
  }
  
  
  for(int b=0; b<12;b++)
  {
    if(b==0)
    {
      set_off(led_order[11]);
      colors(led_colors[b],led_order[b]);
      color();
      delay(INTRODELAY);
    }
    else
    {
      set_off(led_order[b-1]);
      colors(led_colors[b], led_order[b]);
      color();
      delay(INTRODELAY);
    }
      
  }
  
  for(int b=0; b<12;b++)
    set_off(b);
  color();

  for(int a=0;a<3;a++)
  {
    for(int b=0;b<12;b++)
    {
      colors(led_colors[b], led_order[b]);
      color();
      delay(INTRODELAY);
    }

    for(int b=0; b<12;b++)
      set_off(b);
    color();
  }

  for(int a=0;a<3;a++)
  {
    for(int b=0;b<12;b++)
    {
      colors(led_colors[b], led_order[b]);
      
    }
    color();
    delay(INTRODELAY*4);

    for(int b=0; b<12;b++)
      set_off(b);
    color();
    delay(INTRODELAY*4);
  }
  
  
}

