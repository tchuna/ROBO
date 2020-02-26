#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "Motor.h"

//IF PWM SIGNAL IS A DIMMER
#define PWM_CHANGE_RATE 20

//LED PIN
#define LED PB5

//UART PARAMETERS
#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#define BAUD 9600 //9600 has 0.2% error, its GOOD
//#define MY_UBRR F_CPU/16/BAUD-1
#define MY_UBRR 103

Motor motorLeft = Motor(PD4,PD7,PD5);
Motor motorRight = Motor(PB0,PB4,PD6);

void setup_PWM_timer0(){
//clear
TCCR0B = 0;
TIFR0 |= (1<<OCF0B) | (1<<OCF0A) | (1<<TOV0); 

//define mode Fast PWM
TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0B1) | (1<<COM0A1);

//load BOTTOM and TOP values
TCNT0 = 0;
OCR0A = 50;
OCR0B = 50;

//disable interrupts
TIMSK0 = 0;

//start TC2 with a prescaler of 1024
TCCR0B |= (1<<CS02) | (1<<CS00);
  
}

void init_UART_communication(uint16_t baud_rate){
//setting the baud rate
UBRR0H = (uint8_t)(MY_UBRR>>8);
UBRR0L = (uint8_t)(MY_UBRR); 

//setting frame format: 8 data bits (UCSZn) + 1 stop bits (USBSn). by default its asynchronous and 1 stop bit
UCSR0C =  (1<<UCSZ01) | (1<<UCSZ00);

//disabling interrupts and enabling the Tx and/or Rx
UCSR0B = 0;
UCSR0B = (1<<RXEN0) | (1<<TXEN0);
UCSR0B |= (1<<RXCIE0); //enable receive interrupt
}

ISR(USART_RX_vect){
  uint8_t data;
  //double toggle to signal data received
  PORTB=PORTB^(1<<LED);
  PORTB=PORTB^(1<<LED);
  data = UDR0;

  if(data=='h'){
    OCR0A=250;
    OCR0B=250;
    PORTB=PORTB^(1<<LED);
  }
  if(data=='l'){
    OCR0A=50;
    OCR0B=50;
    PORTB=PORTB^(1<<LED);
  }

  if(data=='n'){
    OCR0A=0;
    OCR0B=0;
    PORTB=PORTB^(1<<LED);
  }
}

ISR(TIMER0_COMPA_vect){
  PORTB=PORTB^(1<<LED);
}

int main(void){
  DDRD |= (1<<motorLeft.getPinFWD());
  DDRD |= (1<<motorLeft.getPinBWD());
  DDRD |= (1<<motorLeft.getPinPWM());

  DDRB |= (1<<motorRight.getPinFWD());
  DDRB |= (1<<motorRight.getPinBWD());
  DDRD |= (1<<motorRight.getPinPWM());
  //DDRB |= (1<<motorRight.getPinPWM());

  DDRB |= (1<<LED);

  setup_PWM_timer0();
  init_UART_communication(9600);

  sei();
  while(1){
    //OCR0B++;
    //OCR0A++;
    //_delay_ms(PWM_CHANGE_RATE);
  }
  return 0;
}