#ifndef MOTOR_H
#define MOTOR_H
#include <avr/io.h> 

class Motor{
private:
    uint8_t pinFWD;
    uint8_t pinBWD;
    uint8_t pinPWM;
    uint8_t direction; //1->FWD 0->BWD
public:
    Motor(uint8_t pinFWD,uint8_t pinBWD,uint8_t pinPWM);
    uint8_t getPinFWD();
    uint8_t getPinBWD();
    uint8_t getPinPWM();
    uint8_t getDirection();
    void setPinFWD(uint8_t pinFWD);
    void setPinBWD(uint8_t pinBWD);
    void setPinPWM(uint8_t pinPWM);
    void changeDirection();
};

#endif