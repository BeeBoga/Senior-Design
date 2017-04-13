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
boolean dlay= true;
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

  attachInterrupt(digitalPinToInterrupt(2), set_limit, FALLING);

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
  //Serial.println(analogRead(0));
  //Serial.println(THRESHOLD);
  //for(i=0;i<1000;i++)
   //Serial.println(readingadc());


  control_latch();
  control_latch();
  set_off(0);
  color();

  flag = true;
  freq();



}

int timer=0;

void loop() {
  // put your main code here, to run repeatedly:
  

 
  //digitalWrite(GSCLK,HIGH);
  //digitalWrite(GSCLK,LOW);

/*
  set_blue(0);
  set_magenta(0);
  set_red(0);
  set_purple(0);
  set_green(0);
  set_yellow(0);
  set_cyan(0);
  set_orange(0);
  set_pink(0);
  set_lightgreen(0);
  set_greencyan(0);
  set_coolblue(0);
 
*/

flag = true;
freq();

if(dlay)
  delay(250);



  

  

 
  


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


void note(float frequency)
{
  int n=0;
  int val[] = {1,2,4,8,16};

  Serial.println(frequency);

  for(n=0;n<5;n++)
  {
  
    if(((64.5*val[n])<frequency)&&(frequency<=(66.38*val[n])))                  //C
    {
      for(int a=0; a<12; a++)
        colors(3,a);
      color();
      break;
    }
    else if(((68.32*val[n])<frequency)&&(frequency<=(70.33*val[n])))            //C#
    {
      for(int a=0; a<12; a++)
        colors(9,a);
      color();
      break;
    }
    else if(((72.39*val[n])<frequency)&&(frequency<=(74.51*val[n])))             //D
    {
      for(int a=0; a<12; a++)
        colors(4,a);
      color();
      break;
    }
    else if(((76.69*val[n])<frequency)&&(frequency<=(78.94*val[n])))              //D#
    {
      for(int a=0; a<12; a++)
        colors(10,a);
      color();
      break;
    }
    else if(((81.25*val[n])<frequency)&&(frequency<=(83.64*val[n])))             //E
    {
      for(int a=0; a<12; a++)
        colors(5,a);
      color();
      break;
    }
    else if(((86.01*val[n])<frequency)&&(frequency<=(88.6*val[n])))            //F
    {
      for(int a=0; a<12; a++)
        colors(6,a);
      color();
      break;
    }
    else if(((91.2*val[n])<frequency)&&(frequency<=(93.875*val[n])))           //F#
    {
      for(int a=0; a<12; a++)
        colors(11,a);
      color();
      break;
    }
    else if(((96.63*val[n])<frequency)&&(frequency<=(99.46*val[n])))          //G
    {
      for(int a=0; a<12; a++)
        colors(7,a);
      color();
      break;
    }
    else if(((102.37*val[n])<frequency)&&(frequency<=(105.37*val[n])))          //G#
    {
      for(int a=0; a<12; a++)
        colors(12,a);
      color();
      break;
    }
    else if(((108.46*val[n])<frequency)&&(frequency<=(111.64*val[n])))          //A
    {
      for(int a=0; a<12; a++)
        colors(1,a);
      color();
      break;
    }
    else if(((114.91*val[n])<frequency)&&(frequency<=(118.27*val[n])))          //A#
    {
      for(int a=0; a<12; a++)
        colors(8,a);
      color();
      break;
    }
    else if(((121.74*val[n])<frequency)&&(frequency<=(125.31*val[n])))          //B
    {
      for(int a=0; a<12; a++)
        colors(2,a);
      color();
      break;
    }

  }
  //for(int a=0; a<12; a++)
    //set_off(a);
  //color();
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void freq()
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
          note(periodf);
          repeat=0;
          flag=false;
          count = 0;
        }
      }
        else if((A>B)&&(A<=THRESHOLD))
        {
          //Serial.println("here");
          resete=true;
        }

      
/*
      if(((A<B)&&(A>(THRESHOLD-OFFSET))&&(B<(THRESHOLD+OFFSET)))||(count==0))
      //if((A<THRESHOLD)&&(A>(THRESHOLD-OFFSET))&&(B>=THRESHOLD)&&(B<(THRESHOLD+OFFSET))&&(A<B))
      //if((A<B)&&(A<THRESHOLD)&&(B>=THRESHOLD))//it will sometimes miss the threshold causing it to loop for longer than wanted, a time out feature may save this
      {
        if(count==0)
        {
          timea=micros();
          count++;
        }
        else if(count==1)
        {
          timeb=micros();
          period=timeb-timea;
          if(repeat==0)
          {
            value[0]=period;
            repeat++;
          }
          if(repeat==1)
          {
            value[1]=period;
            repeat++;
          }
          else if(repeat==2)
          {
            value[2]=period;
            //if((value[0]==value[1])&&value[1]==value[2])
            //if(((((value[0]-value[1])<1000)&&((value[0]-value[1])>0))||(((value[1]-value[0])<1000)&&((value[1]-value[0])>0)))&&((((value[1]-value[2])<1000)&&((value[1]-value[2])>0))||(((value[2]-value[1])<1000)&&((value[2]-value[1])>0))))
            if((((value[0]-value[1])<1000)&&((value[0]-value[1])>0))||(((value[1]-value[0])<1000)&&((value[1]-value[0])>0)))
            {
              //then we are going to say that this is most likely the correct value
              periodf = period/mil;
              periodf = 1/periodf; 
              note(periodf);
              repeat=0;
              flag=false;
              
            }
            else
            {
              value[0]=value[1];
              value[1]=value[2];
              //repeat stays at 1 thus looping through this again;
            }
          }
          count=0;
        }
      }
      
*/
  }

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void freq()

{
  while(flag)
  {

      A=readingadc();
      B=readingadc();
    
      if((A<B)&&(A<THRESHOLD)&&(B>=THRESHOLD))//it will sometimes miss the threshold causing it to loop for longer than wanted, a time out feature may save this
      {
        if(count==0)
        {
          timea=micros();
          count++;
        }
        else if(count==1)
        {
          timeb=micros();
          period=timeb-timea;
          if(repeat==0)
          {
            value[0]=period;
            repeat++;
          }
          /*if(repeat==1)
          {
            value[1]=period;
            repeat++;
          }
          */
          /*
          if(repeat==1)
          {
            //value[2]=period;
            //if((value[0]==value[1])&&value[1]==value[2])
            //if(((((value[0]-value[1])<1000)&&((value[0]-value[1])>0))||(((value[1]-value[0])<1000)&&((value[1]-value[0])>0)))&&((((value[1]-value[2])<1000)&&((value[1]-value[2])>0))||(((value[2]-value[1])<1000)&&((value[2]-value[1])>0))))
            //if((((value[0]-value[1])<1000)&&((value[0]-value[1])>0))||(((value[1]-value[0])<1000)&&((value[1]-value[0])>0)))
            //{
              //then we are going to say that this is most likely the correct value
              periodf = period/mil;
              periodf = 1/periodf; 
              note(periodf);
              repeat=0;
              flag=false;
              
            //}
           // else
            //{
              //value[0]=value[1];
              //value[1]=value[2];
              //repeat stays at 1 thus looping through this again;
            //}
          }
          count=0;
        }
      }

  }

}


*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int readingadc()
{
  int number = 0;
  
  digitalWrite(cs, HIGH);
  digitalWrite(cs, LOW);

  for(int i = 0; i<15; i++)
  {
    digitalWrite(clck, HIGH);
    digitalWrite(clck, LOW);
    dta[i] = digitalRead(data);
  }
  for(int i=3; i<15;i++)
  {
    number = number + (dta[i] <<(14-i));
  }
  //delayMicroseconds(1);
  return(number);
  
  
}

*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
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
    dta[i] = ((PINH & B01000000)>>6);
  }
  for(int i=3; i<15;i++)
  {
    number = number + (dta[i] <<(14-i));
  }
  
  //delayMicroseconds(1);
  //b=micros();
  //a=b-a;
  //Serial.println(a);
  //Serial.println("done");
  //Serial.println(number);
  return(number);
  
  
}

*/

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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void set_limit()
{
  THRESHOLD = map(analogRead(0),0,1023,0,4095);
  flag_analog = true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
