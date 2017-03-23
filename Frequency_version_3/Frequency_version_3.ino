#define THRESHOLD 400
#define APIN 0

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

boolean flag = true;

void setup() {
  // put your setup code here, to run once:

  //if this was run in a function we would want to re initiate the values at the begining of the function
  Serial.begin(115200);
  Serial.println("Starting");

  while(true)
  {

  A = analogRead(APIN);
  B = analogRead(APIN);

  //Serial.println(A);
  //Serial.println(B);

  if((A<B)&&(A<THRESHOLD)&&(B>=THRESHOLD))//it will sometimes miss the threshold causing it to loop for longer than wanted, a time out feature may save this
  {
    //Serial.println("test1");
    if(count==0)
    {
      timea=micros();
      count++;
      //Serial.println("test1");
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
        if((value[0]==value[1])&&value[1]==value[2])
        {
          //then we are going to say that this is most likely the correct value
          periodf = period/mil;
          periodf = 1/periodf;
          //Serial.println(periodf);
          note(periodf);
          //return value if we ran this inside of a function.
          repeat=0;
          
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

  }
  

  

}

void loop() {
  // put your main code here, to run repeatedly:

  

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int note(float frequency)
{
  int n=0;
  int val[] = {1,2,4,8,16};

  for(n=0;n<5;n++)
  {
  
    if(((63.58*val[n])<frequency)&&(frequency<=(67.36*val[n])))
    {
      //return 'C';
      Serial.print("C");
      Serial.println(n+2);
    }
    else if(((67.36*val[n])<frequency)&&(frequency<=(71.36*val[n])))
    {
      Serial.print("C#");
      Serial.println(n+2);
    }
    else if(((71.36*val[n])<frequency)&&(frequency<=(75.6*val[n])))
    {
      Serial.print("D");
      Serial.println(n+2);
    }
    else if(((75.6*val[n])<frequency)&&(frequency<=(80.1*val[n])))
    {
      Serial.print("D#");
      Serial.println(n+2);
    }
    else if(((80.1*val[n])<frequency)&&(frequency<=(84.86*val[n])))
    {
      Serial.print("E");
      Serial.println(n+2);
    }
    else if(((84.86*val[n])<frequency)&&(frequency<=(89.91*val[n])))
    {
      Serial.print("F");
      Serial.println(n+2);
    }
    else if(((89.91*val[n])<frequency)&&(frequency<=(95.25*val[n])))
    {
      Serial.print("F#");
      Serial.println(n+2);
    }
    else if(((95.25*val[n])<frequency)&&(frequency<=(100.92*val[n])))
    {
      Serial.print("G");
      Serial.println(n+2);
    }
    else if(((100.92*val[n])<frequency)&&(frequency<=(106.92*val[n])))
    {
      Serial.print("G#");
      Serial.println(n+2);
    }
    else if(((106.92*val[n])<frequency)&&(frequency<=(113.27*val[n])))
    {
      Serial.print("A");
      Serial.println(n+2);
    }
    else if(((113.27*val[n])<frequency)&&(frequency<=(120.01*val[n])))
    {
      Serial.print("A#");
      Serial.println(n+2);
    }
    else if(((120.01*val[n])<frequency)&&(frequency<=(127.14*val[n])))
    {
      Serial.print("B");
      Serial.println(n+2);
    }

  }
    
}
