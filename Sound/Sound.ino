#include <SoftwareSerial.h>

const int soundPin = A5; //sound sensor attach to A0 

void setup() 
{ 
  Serial.begin(9600); //initialize serial 
} 
void loop() 
{
  MeasuredSound();
  readVcc();
}
void MeasuredSound()
{
  int value = analogRead(soundPin);//read the value of A0 
  //read ref voltage
  float ref_volt = float(readVcc())/1000.0;
  
  //preallocate
  float dbValue;
  char db_str[4];
  
  
  // read and convert analog data to dB
    dbValue = (analogRead(soundPin)/1024.0)*ref_volt*50.0;    
    dtostrf(dbValue,1,2,db_str);
    Serial.println(db_str);
    delay(80);
}
long readVcc() 
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
  



