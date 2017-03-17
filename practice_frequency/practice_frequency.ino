#define DEBUG FALSE

int pin =13;
int a=0;
int b=0;
unsigned int t[2]={0,0};
//int t[1]=0;
int i =0;
int c = 0;

unsigned int f = 0;
#define r 50
int p = 0;
unsigned int w[r];

int g=0;
int h=0;

float freq=0.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Begin communication");
  Serial.println(pin);
  Serial.println(analogRead(0));

  for(int count = 0; count<r; count++)
    w[count]=0;

  Serial.println(p);

  
}

void loop() {
  while(p<50)
  {
  a = analogRead(0);
  //Serial.print("a=");
  //Serial.println(a);
  //Serial.println(p);
  b = analogRead(0);
  //Serial.print("b=");
  //Serial.println(b);
  //Serial.println(b);
  if((a<=0 and b>0) or (a>0 and b<=0))
    {
    
    t[i]=micros();
    i ++;
    if(i>1)
      {
        
        w[c]=t[1]-t[0];
        //t[0]=t[1];
        i=0;
        c++;
        if(c>=r)
          {
            for(int count=0; count<r; count++)
            {
            f = f + w[count];
            //g++;
            
            //Serial.println("count");
            }
            
            //h=f/g;
            freq = (float)f/r;
            freq *= 2;
            freq= freq/(1000*1000);
            //f/= (1000* 1000);
            freq=1/freq;
            
            
            Serial.println(freq);
            //Serial.println(h);
            c=0;
            f=0;
            p++;
            //Serial.println(p);
            Serial.println("here1");
          }
          //Serial.println("here2");
      }
      }
      //Serial.println("stuck");
      
      
      
  }
 }
  

