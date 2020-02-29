#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "Motor.h"

//IF PWM SIGNAL IS A DIMMER
#define PWM_CHANGE_RATE 20

//LED PIN
#define LED PB5

//INTERRUPTS
#define INT_0 PD2
#define INT_1 PD3

//UART PARAMETERS
#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#define BAUD 9600 //9600 has 0.2% error, its GOOD
//#define MY_UBRR F_CPU/16/BAUD-1
#define MY_UBRR 103

Motor motorLeft = Motor(PD4,PD7,PD5); //motorB
Motor motorRight = Motor(PB0,PB4,PD6); //motorA 

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

void setup_external_interrupt(){
//type of interrupt request (in this case its set to rising edge)
EICRA |= (1<<ISC01) | (1<<ISC00);

//enable interrupt (in this case its INT0)
EIMSK |= (1<<INT0);
}

ISR(USART_RX_vect){
  char data;
  //toggle to signal data received
  data = UDR0;

  if(data=='h'){
    OCR0A=250;
    OCR0B=250;
    PORTB=PORTB^(1<<LED);
  }
  else if(data=='l'){
    OCR0A=60;
    OCR0B=60;
    PORTB=PORTB^(1<<LED);
  }

  else if(data=='s'){
    OCR0A=0;
    OCR0B=0;
    PORTB=PORTB^(1<<LED);
  }
  else if(data=='t'){
    PORTB=PORTB^(1<<LED);
    
    PORTD=PORTD^(1<<PD4);
    PORTD=PORTD^(1<<PD7);
    PORTB=PORTB^(1<<PB0);
    PORTB=PORTB^(1<<PB4);
  }
}

ISR(INT0_vect){
  PORTB=PORTB^(1<<LED);
}

int main(void){
  /* OUTPUTS */
  //MOTOR PINS
  //LEFT MOTOR
  DDRD |= (1<<PD4);
  DDRD |= (1<<PD7);
  DDRD |= (1<<PD5);
  //RIGHT MOTOR
  DDRB |= (1<<PB0);
  DDRB |= (1<<PB4);
  DDRD |= (1<<PD6);
  //LED PB5
  DDRB |= (1<<LED);

  /* INPUTS */
  //INTERRUPT INT0
  DDRD &= ~(1<<INT_0);
  //ACTIVATE INTERNAL PULL-UP TO AVOID FLUCTUATION
  PORTD |= (1<<INT_0);
  
  //start motors in forward direction
  PORTD|=(1<<PD4);
  PORTB|=(1<<PB0);

  //activate INTERRUPT
  //PORTD|=(1<<INT0);


  setup_PWM_timer0();
  init_UART_communication(9600);
  setup_external_interrupt();

  sei();

  while(1);
  return 0;
}