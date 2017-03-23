void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  note(1000);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}

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

