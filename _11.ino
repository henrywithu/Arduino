
const int ledPin = 14;  
int incomingByte; // a variable to read incoming serial data into
int rcv = 5; //pin of light observation

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);; 
}

void loop() {
 /* 
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin,LOW);
  delay(500);
*/
  //Serial.println();
  if (Serial.available() > 0) {
  incomingByte = Serial.read();
  Serial.write(incomingByte);
  Serial.print("  ,Incoming Byte ASCII : ");
  Serial.print(incomingByte);
  //Serial.print(" , Binary : ");
  //Serial.print(incomingByte,BIN);
  String k = String(incomingByte,BIN);
  int kl=k.length()+1;
  //Serial.print(kl);
  char a[kl];
  k.toCharArray(a,kl);
  if (kl==8){
    kl=8;
    for (int i=8; i>-1; i--){
      a[i]=a[i-1];
    }
    a[0]='0';
  }
  if (kl==7){
    kl=8;
    for (int i=8; i>-1; i--){
      a[i]=a[i-1];
      a[i+1]=a[i];
    }
    a[0]='0';
    a[1]='0';
  }
  Serial.print(" , Binary : ");
  for (int i=0; i<8; i++){
    Serial.print(a[i]);
  }
 

  // see if there's incoming serial data:
  
  for (int i=0; i<8; i++){
//  if (Serial.available() > 0)
  { 
    if (a[i] == '1') {
      digitalWrite(ledPin, LOW);
      Serial.print("   a[");
      Serial.print(i);
      Serial.print("]=");
      Serial.print(a[i]);
      delay(70);
    } 
    if (a[i] == '0') {
      digitalWrite(ledPin, HIGH);
      Serial.print("   a[");
      Serial.print(i);
      Serial.print("]=");
      Serial.print(a[i]);
      delay(70);
    }
  }  
}
digitalWrite(ledPin, LOW);
delay(25);

Serial.println();
}

/*
//Serial.print(rcv);
    if (Serial.available()==0){
    int b[7];
    int c;
    for (int i=0; i<8; i++){
      int state = digitalRead(rcv);
      Serial.print(state);
      b[i]=state;
      delay(1000);
    }
    c=b[7]*1+b[6]*2+b[5]*4+b[4]*8+b[3]*16+b[2]*32+b[1]*64+b[0]*128;
    //Serial.print(c);
    if (b[0]==0){
      Serial.print("   ASCII from transmmiter is : ");
      char s=(char)c;
      //String s = String(incomingByte,BIN);
      Serial.print(s);
    }
  }
  */
  
  int b[7];
  int c;
  String m;
  int state = digitalRead(rcv);
  if (state==0){
    Serial.println();
    Serial.print("Binany code from sender : ");
    for (int i=0; i<8; i++){
      int state = digitalRead(rcv);
      Serial.print(state);
      b[i]=state;
      delay(70);
    }
  c=b[7]*1+b[6]*2+b[5]*4+b[4]*8+b[3]*16+b[2]*32+b[1]*64+b[0]*128;
  Serial.print(" ,  ASCII from sender : ");
  char s=(char)c;
  Serial.print(s);
  Serial.println();
  Serial.print("   The word is : ");
  Serial.print(m);
  Serial.println();
  }
 
/*  if (Serial.available()==0){
    int b[7];
    int c;
    for (int i=0; i<8; i++){
      int state = digitalRead(rcv);
      Serial.print(state);
      b[i]=state;
      delay(1000);
    }
    c=b[7]*1+b[6]*2+b[5]*4+b[4]*8+b[3]*16+b[2]*32+b[1]*64+b[0]*128;
    //Serial.print(c);
    if (b[0]==0){
      Serial.print("   ASCII from transmmiter is : ");
      char s=(char)c;
      //String s = String(incomingByte,BIN);
      Serial.print(s);
    }
  }
  
*/
  delay(50);
}
