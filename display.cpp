#include "display.hpp"

#include "Arduino.h"

/*Variometer seven segment led display driver by Jere Koivisto.
 * 
 * 
 */

//Display pin defines.
#define DIGIT_0 4
#define DIGIT_1 5
#define DIGIT_2 6
#define DIGIT_3 7

#define BIT_0 15
#define BIT_1 16
#define BIT_2 17
#define BIT_3 18

//Display constructor.
Display::Display(){
  //Initialize pins.
  digits[0] = DIGIT_3;
  digits[1] = DIGIT_2;
  digits[2] = DIGIT_1;
  digits[3] = DIGIT_0;

  bits[0] = BIT_0;
  bits[1] = BIT_1;
  bits[2] = BIT_2;
  bits[3] = BIT_3;

  for(uint8_t i=0;i<4;i++){
    pinMode(digits[i], OUTPUT);
    pinMode(bits[i], OUTPUT);
  }
  reset();

  //Initialize digit counter.
  counter = 0;
}

//Display destructor.
Display::~Display(){
  
}

//Sets all pinouts to LOW.
void Display::reset(){
  for(uint8_t i=0;i<4;i++){
    digitalWrite(digits[i], LOW);
    digitalWrite(bits[i], LOW);
  }
}

//Writes a value to the display from between -999 to 999.
void Display::write(int32_t value){
  //Reset writes.
  reset();

  if(counter == 3){
    //This digit displays sign.
    if(value < 0){
      digitalWrite(digits[counter], HIGH);
      //digitalWrite(bits[1], HIGH);
      //digitalWrite(bits[3], HIGH);    
    }
  }else{
    value = abs(value);
    if(value > 999)value=999;
    
    //Determine value of current digit.
    for(uint8_t i=0;i<counter;i++)value/=10;
    value = value % 10;

    //Write digit with value.
    digitalWrite(digits[counter], HIGH);
    if(value & 0b0001)digitalWrite(bits[0], HIGH);
    if(value & 0b0010)digitalWrite(bits[1], HIGH);
    if(value & 0b0100)digitalWrite(bits[2], HIGH);
    if(value & 0b1000)digitalWrite(bits[3], HIGH);
  
  }
  
  //Advance counter.
  counter = (counter + 1) % 4;
}
