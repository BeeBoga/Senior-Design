#define DEBUG false
#define on true
#define iterate 50
#define limit 50



int a =         0;
int b =         0;
int count =     0;
int temp =      0;
int increment = 0;
int val =       0;

unsigned int timer[2] = {0,0};
//unsigned int halfperiod = 0;
unsigned int timediff[iterate];

float halfperiod = 0.0;
float freq = 0.0;



void setup() {
  
  
  Serial.begin(9600);
  Serial.println("Begin communication");

  for(count = 0; count< iterate; count++)
    timediff[count] = 0;
}

void loop() {

  while((increment<limit) or (on))
  {
    a = analogRead(0);
    b = analogRead(0);

    if((a<=0 && b>0) || (b<=0 && a>0))
     {
        if(DEBUG)
          Serial.println("Crossed zero");
        timer[count] = micros();
        count++;

        if(count > 1)
        {
          if(DEBUG)
            Serial.println("Found a pair of crossing");
          timediff[temp] = timer[1] - timer[0];
          count = 0;
          temp++;
          if(temp >= iterate)
          {
            if(DEBUG)
              Serial.println("Found a frequency");
            for(val = 0; val<iterate; val++)
            {
              halfperiod = halfperiod + timediff[val];
            }
            halfperiod /= iterate;
            if(DEBUG)
              Serial.println(halfperiod);
            halfperiod = (double)halfperiod / (1000 );
            freq = 1/halfperiod;
            freq *= 1000;
            Serial.println(freq);
            if(DEBUG)
              Serial.println();
            increment++;
            halfperiod = 0;
            temp = 0;
          }
          
          
        }
     }
    
  }

}
