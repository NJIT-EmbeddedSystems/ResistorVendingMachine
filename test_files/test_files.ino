/*
 * SKETCH FOR TESTING THE DECODERS
 * 
 * Note: Since 74LS138N outputs are active low
 * 
 * The decoder configurations:
 * MASTER:
 * G1 is the enable
 * G2A, G2B tied low
 * 
 * SLAVES:
 * G1 is tied HIGH
 * G2B is tied LOW
 * G2A is now an active low enable, connected straight to master's output
 */

#define DEC_MASTER_EN 33 // should be G1 on the decoder, tie G2A G2B low
#define DEC_MASTER_2 32
#define DEC_MASTER_1 31
#define DEC_MASTER_0 30

// slave 0 select pins
#define DEC_S0_2 38
#define DEC_S0_1 37
#define DEC_S0_0 36


void setup() {
  // open up the serial for debugging information
  Serial.begin(9600);
  
  // initialize the master decorder
  pinMode(DEC_MASTER_EN, OUTPUT);
  pinMode(DEC_MASTER_2, OUTPUT);
  pinMode(DEC_MASTER_1, OUTPUT);
  pinMode(DEC_MASTER_0, OUTPUT);

  // intialize the slave 0
  pinMode(DEC_S0_2, OUTPUT);
  pinMode(DEC_S0_1, OUTPUT);
  pinMode(DEC_S0_0, OUTPUT);

  // turn off all the decoders
  digitalWrite(DEC_MASTER_EN, LOW);
}

void loop() {
  // select slave 0 using the master's pins
  digitalWrite(DEC_MASTER_2, LOW);
  digitalWrite(DEC_MASTER_1, LOW);
  digitalWrite(DEC_MASTER_0, LOW);
  
  // loop through each of slave 0's 8 LEDs
  for (byte i=0; i<8; i++) {
    
    // let the serial know what we are doing
    Serial.print("Slave 0 | Output ");
    Serial.println(i);
    
    // turn off slave 0 (and all slaves)
    digitalWrite(DEC_MASTER_EN, LOW);

    // update the selects
    digitalWrite(DEC_S0_2, bitRead(i,2));
    digitalWrite(DEC_S0_1, bitRead(i,1));
    digitalWrite(DEC_S0_0, bitRead(i,0));

    // turn on slave 0 by enabling master decoder
    digitalWrite(DEC_MASTER_EN, HIGH);

    // wait a moment to observe the effects
    delay(1000);
  }
}
