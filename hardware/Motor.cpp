#include "Motor.h"
#define FWD 1
#define BWD 0


Motor::Motor(uint8_t pinFWD,uint8_t pinBWD,uint8_t pinPWM){
    this->direction=FWD;
    this->pinFWD=pinFWD;
    this->pinBWD=pinBWD;
    this->pinPWM=pinPWM;
}
uint8_t Motor::getPinFWD(){
    return this->pinFWD;
}
uint8_t Motor::getPinBWD(){
    return this->pinBWD;
}
uint8_t Motor::getPinPWM(){
    return this->pinPWM;
}
uint8_t Motor::getDirection(){
    return this->direction;
}
void Motor::setPinFWD(uint8_t pinFWD){
    this->pinFWD=pinFWD;
}
void Motor::setPinBWD(uint8_t pinBWD){
    this->pinBWD=pinBWD;
}
void Motor::setPinPWM(uint8_t pinPWM){
    this->pinPWM=pinPWM;
}
void Motor::changeDirection(){
    this->direction=!this->direction;
}
