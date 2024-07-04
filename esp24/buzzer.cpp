#include "buzzer.hpp"

#include "Arduino.h"

/*Variometer magnetic buzzer driver by Jere Koivisto.
 * 
 * 
 */

//Buzzer defines.
#define BUZZER_0 20

//Buzzer constructor.
Buzzer::Buzzer(){
  pinMode(BUZZER_0, OUTPUT);
  digitalWrite(BUZZER_0, LOW);
  noTone(BUZZER_0);

  counter = 0;
  toggle = 0;
}

//Buzzer destructor.
Buzzer::~Buzzer(){

}

//Tests buzzer frequencies between BUZZER_MIN_FREQ and BUZZER_MAX_FREQ in 100hz intervals.
void Buzzer::toneTest(){
  for(uint16_t freq=100;freq<=2700;freq+=100){
    tone(BUZZER_0, freq);
    delay(50); 
  }
  for(uint16_t freq=2600;freq>=100;freq-=100){
    tone(BUZZER_0, freq);
    delay(50); 
  }
  noTone(BUZZER_0);
}

//Plays an appropriate frequency and beeping based on m/s.
void Buzzer::play(int32_t value){
  value *= 10;
  if(value>999)value=999;
  if(value<-999)value=-999;
  int16_t freq = 1700+value;

  //Ascending velocity.
  if(value > 0){
    if(counter > 1000-value){
      counter = 0;
      toggle = (toggle+1)%2;
    }
    counter+=20;

    //Beeping toggle.
    if(toggle){
      tone(BUZZER_0, freq);
    }else{
      noTone(BUZZER_0);
    }
    
  //Descending velocity.
  }else{
    tone(BUZZER_0, freq);
  }
}
