

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int GS[769];
  unsigned int red0=32176;
  int x;

  Serial.println(red0);

 



  for(x=0;x<16;x++)
  {
     GS[x]= bitRead(red0,x);
     
  }

  for(x=0;x<16;x++)
  {
    Serial.println(GS[x]);
  }

  

}

void loop() {
  // put your main code here, to run repeatedly:

}
