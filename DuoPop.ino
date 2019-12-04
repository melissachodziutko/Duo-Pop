
#define IRpin_PIN      PINC
#define IRpin          PINC5 
 
#define ledPin         13 
 
 
#define MAXPULSE 1000
 
#define RESOLUTION 20 


//the reading won't be exact each time, so allow for some error room.
#define FUZZINESS 45
 
uint16_t pulses[100][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing
 
// Red Code
int DUOPOPsignalRed[] = { // ON, OFF (in 10's of microseconds) 
140, 50, 
100, 50, 
100, 100, 
50, 50, 
100, 50, 
100, 50, 
100, 100,  
50, 50,  
100, 50,  
100};

//We included blue despite our blue popper not working
int DUOPOPsignalBlue[] = { // ON, OFF (in 10's of microseconds) 
140, 50, 
100, 50, 
100, 50, 
100, 100, 
50, 50, 
100, 50, 
100, 50,  
100, 100,  
50, 50,  
100};
 
//yellow
int DUOPOPsignalYellow[] = { // ON, OFF (in 10's of microseconds) 
140, 
100, 50, 
50, 100, 
100, 50, 
50, 100, 
100, 50, 
50, 100, 
100, 50, 
50, 100, 
50, 100};

//green
int DUOPOPsignalGreen[] = { // ON, OFF (in 10's of microseconds) 
140, 
50, 100, 
100, 50, 
50, 100,
100, 50, 
50, 100, 
100, 50, 
50, 100, 
100, 50, 
50, 100};
 
 
void setup(void) {
 Serial.begin(9600);
Serial.println("Ready to decode IR!");  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(IRpin, INPUT); 
 
 
  TIMSK0&=!(1<<TOIE0);
 
  cli();
  sei();
}
 
 
void loop(void) {
   
  int numberpulses;
  numberpulses = listenForIR();
 
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  if (numberpulses == 10){  
   Serial.println("Received IR!"); 
  if (IRcompare(numberpulses, DUOPOPsignalRed)) {
  Serial.println("RED Buzzed In");
  //flash twice to indicate red
    digitalWrite(ledPin, HIGH); 
    delayMs(500);
    digitalWrite(ledPin, LOW);
    delayMs(500);
    digitalWrite(ledPin, HIGH); 
    delayMs(500);
  }
  if (IRcompare(numberpulses, DUOPOPsignalBlue)) {
   Serial.println("BLUE Buzzed In");
     digitalWrite(ledPin, HIGH); // Toggle status LED
    delayMs(500);
  }
    if (IRcompare(numberpulses, DUOPOPsignalYellow)) {
   Serial.println("Yellow Buzzed In");
     digitalWrite(ledPin, HIGH); 
    delayMs(500);
    digitalWrite(ledPin, LOW); 
    delayMs(500);
    digitalWrite(ledPin, HIGH);
    delayMs(500);
    digitalWrite(ledPin, LOW); 
    delayMs(500);
    digitalWrite(ledPin, HIGH);
    delayMs(500);
    
 
  }
   if (IRcompare(numberpulses, DUOPOPsignalGreen)) {
   Serial.println("Green Buzzed In");
     digitalWrite(ledPin, HIGH);
    delayMs(500);
    digitalWrite(ledPin, LOW); 
    delayMs(500);
    digitalWrite(ledPin, HIGH);
    delayMs(500);
    digitalWrite(ledPin, LOW); 
    delayMs(500);
    digitalWrite(ledPin, HIGH);
    delayMs(500);
    digitalWrite(ledPin, LOW); 
    delayMs(500);
    digitalWrite(ledPin, HIGH);
    delayMs(500);
 
  }
  }
digitalWrite(ledPin, LOW); 
 
}
 
int listenForIR(void) {
  currentpulse = 0;
 
  while (1) {
  
    uint16_t highpulse, lowpulse;  
    highpulse = lowpulse = 0; 
     while (IRpin_PIN & _BV(IRpin)) {
       highpulse++;
       delayMicroseconds(RESOLUTION);
 
       if (((highpulse >= MAXPULSE) && (currentpulse != 0)) || (currentpulse == 10)) {
         return currentpulse;
       }
    }
    pulses[currentpulse][0] = highpulse;
 
    
    while (! (IRpin_PIN & _BV(IRpin))) {
      
       lowpulse++;
       delayMicroseconds(RESOLUTION);
       if (((lowpulse >= MAXPULSE)  && (currentpulse != 0)) || (currentpulse == 10)) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;
 
    currentpulse++;
  }
}
 
 
boolean IRcompare(int numpulses, int Signal[]) {
 
  for (int i=0; i< numpulses-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
 

    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
 
    
      
    } else {

      return false;
    }
 
    if (abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {

    } else {

      return false;
    }
 
  }

  return true;
}

void delayMs(unsigned int ms)
{
  for (int i = 0; i < ms; i++) {
    delayMicroseconds(1000);
  }
}
