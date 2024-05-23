/*
 * pruebaPWM3.c
 *
 * Created: 27/04/2024 11:53:15
 * Author : diego
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "Librerias/ADC.h"
#include "Librerias/PWM1.h"
#include "Librerias/TIMERS.h"
#include "Librerias/eeprom.h"
volatile uint8_t adc_activo = 0;
volatile uint8_t servo1 = 0;
volatile uint8_t servo2 = 0;
volatile uint8_t servo3 = 0;
volatile uint8_t servo4 = 0;
volatile uint8_t modo = 0;
volatile uint8_t escribir = 0;
volatile uint8_t leer = 0;
volatile uint8_t uart = 0;
volatile uint8_t hola = 0;

void setup(void);

int main(void)
{
	cli();
    salidaPWM0(2, 1024);
	salidaPWM1(2, 1024, 312);
	ADC_(128);
	ADC4();
	setup();
	sei();
	ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
    while (1) 
    {
		if (modo == 0){
			ADC4();
			ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
			PORTD &= ~((1<<PORTD2) | (1<<PORTD3));
			do
			{
				_delay_ms(5);
				ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
			}while(modo == 0);
		}else if (modo == 1){//escribir eeprom
			PORTD |= (1<<PORTD3);
			PORTD &= ~(1<<PORTD4);
			do{
				_delay_ms(5);
				ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
				if (escribir == 1){
					escribir_eeprom(0, servo1);
					escribir_eeprom(1, servo2);
					escribir_eeprom(2, servo3);
					escribir_eeprom(3, servo4);
					escribir = 0;
				}else if (escribir == 2){
					escribir_eeprom(4, servo1);
					escribir_eeprom(5, servo2);
					escribir_eeprom(6, servo3);
					escribir_eeprom(7, servo4);
					escribir = 0;
				}else if (escribir == 3){
					escribir_eeprom(8, servo1);
					escribir_eeprom(9, servo2);
					escribir_eeprom(10, servo3);
					escribir_eeprom(11, servo4);
					escribir = 0;
				}else if (escribir == 4){
					escribir_eeprom(12, servo1);
					escribir_eeprom(13, servo2);
					escribir_eeprom(14, servo3);
					escribir_eeprom(15, servo4);
					escribir = 0;
				}
			}while(modo == 1);
		}else if (modo == 2){ //leer eeprom
			ADCSRA &= ~(1<<ADEN); //deshabilitar ADC
			PORTD |= (1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			do{
				
				if (leer == 1){
					servo1 = leer_eeprom(0);
					servo2 = leer_eeprom(1);
					servo3 = leer_eeprom(2);
					servo4 = leer_eeprom(3);
					leer = 0;
				}else if (leer == 2){
					servo1 = leer_eeprom(4);
					servo2 = leer_eeprom(5);
					servo3 = leer_eeprom(6);
					servo4 = leer_eeprom(7);
					leer = 0;
				}else if (leer == 3){
					servo1 = leer_eeprom(8);
					servo2 = leer_eeprom(9);
					servo3 = leer_eeprom(10);
					servo4 = leer_eeprom(11);
					leer = 0;
				}else if (leer == 4){
					servo1 = leer_eeprom(12);
					servo2 = leer_eeprom(13);
					servo3 = leer_eeprom(14);
					servo4 = leer_eeprom(15);
					leer = 0;
				}
				update_duty_cycle0(0, servo4*15.93/255+15.93);//ceja2
				update_duty_cycle0(1, servo3*15.93/255+15.93);//ceja1
				update_duty_cycle1(0, servo1*9/255+15);//boca
				update_duty_cycle1(1, servo2*15.93/255+15.93);//ojos
			}while(modo == 2);
		}else if (modo == 3){ //adafruit
			PORTD |= (1<<PORTD3) | (1<<PORTD2);
			UCSR0B = 0b10010000;
			do{
				if (leer == 1){
					servo1 = leer_eeprom(0);
					servo2 = leer_eeprom(1);
					servo3 = leer_eeprom(2);
					servo4 = leer_eeprom(3);
					leer = 0;
					}else if (leer == 2){
					servo1 = leer_eeprom(4);
					servo2 = leer_eeprom(5);
					servo3 = leer_eeprom(6);
					servo4 = leer_eeprom(7);
					leer = 0;
					}else if (leer == 3){
					servo1 = leer_eeprom(8);
					servo2 = leer_eeprom(9);
					servo3 = leer_eeprom(10);
					servo4 = leer_eeprom(11);
					leer = 0;
					}else if (leer == 4){
					servo1 = leer_eeprom(12);
					servo2 = leer_eeprom(13);
					servo3 = leer_eeprom(14);
					servo4 = leer_eeprom(15);
					leer = 0;
				}
				if (escribir == 1){
					escribir_eeprom(0, servo1);
					escribir_eeprom(1, servo2);
					escribir_eeprom(2, servo3);
					escribir_eeprom(3, servo4);
					escribir = 0;
					}else if (escribir == 2){
					escribir_eeprom(4, servo1);
					escribir_eeprom(5, servo2);
					escribir_eeprom(6, servo3);
					escribir_eeprom(7, servo4);
					escribir = 0;
					}else if (escribir == 3){
					escribir_eeprom(8, servo1);
					escribir_eeprom(9, servo2);
					escribir_eeprom(10, servo3);
					escribir_eeprom(11, servo4);
					escribir = 0;
					}else if (escribir == 4){
					escribir_eeprom(12, servo1);
					escribir_eeprom(13, servo2);
					escribir_eeprom(14, servo3);
					escribir_eeprom(15, servo4);
					escribir = 0;
				}
				update_duty_cycle0(0, servo4*15.93/255+15.93);//ceja2
				update_duty_cycle0(1, servo3*15.93/255+15.93);//ceja1
				update_duty_cycle1(0, servo1*9/255+15);//boca
				update_duty_cycle1(1, servo2*15.93/255+15.93);//ojos
			}while(modo == 3);
			ADCSRA |= (1<<ADEN); //habilitar ADC
			UCSR0B = 0b10000000;
		}
    }
}

void setup(void){
	DDRC &= ~(1<<DDC0); //ESTABLECE PB0 COMO ENTRADA (PushBottom1)
	PORTC |= (1<<PORTC0);
	DDRC &= ~(1<<DDC1); //ESTABLECE PB0 COMO ENTRADA (PushBottom2)
	PORTC |= (1<<PORTC1);
	DDRC &= ~(1<<DDC2); //ESTABLECE PB0 COMO ENTRADA (PushBottom3)
	PORTC |= (1<<PORTC2);
	DDRC &= ~(1<<DDC3); //ESTABLECE PB0 COMO ENTRADA (PushBottom4)
	PORTC |= (1<<PORTC3);
	DDRC |= 0b11110000; // Setear demás pines como salida
	
	DDRB &= ~(1<<DDB0); //ESTABLECE PB0 COMO ENTRADA (PushBottom5)
	PORTB |= (1<<PORTB0); //boton de cambio de modo
	DDRB |= 0b11111110;
	
	DDRD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4); //leds de modo
	PORTD &= ~((1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4));
	
	PCICR = 0;
	PCMSK0 = 0;
	PCMSK1 = 0;
	PCICR |= (1<<PCIE0) | (1<<PCIE1); //habilitar interrupcion en el pcint0:7
	PCMSK0 |= (1<<PCINT0); //cambio de modo
	PCMSK1 |= (1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11); //int botones de posiciones
	
	///COMUNICACIÓN SERIAL
	//DDRD &= (1<<DDD0); //RX
	//DDRD |= (1<<DDD1); //TX
	//Configurar UCSR0A
	UCSR0A = 0;
	//Configurar UCSR0B
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0); //Habilitar interrupcion de recepcion
	//UCSR0B |= (1<<TXCIE0); //Habilitar interrupcion de transmicion
	
	//UCSR0B |= (1<<TXEN0); //Habilitar TX (transmición)
	//Configurar UCSR0C
	UCSR0C = 0;
	//UCSR0C &= ~((1<<UMSEL01) | (1<<UMSEL00)); //ASINCRONO
	//UCSR0C &= ~((1<<UPM01) | (1<<UPM00)); //NO PARIEDAD
	//UCSR0C &= ~(1<<USBS0); //UN BIT DE STOP
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); //8 BITS DE ESPACIO
	UBRR0 = 103;//Configurar velocidad del baudrate 9600
}

ISR(PCINT1_vect){
	if (!(PINC & (1<<PINC0))){ //boton de cambio de modo
		if (modo == 1){
			escribir = 1;//escribir en memoria1
			} else if (modo == 2){
			
			leer = 1;//leer memoria1
		}
		}else if (!(PINC & (1<<PINC1))){ //memoria1
		if (modo == 1){
			escribir = 2;//escribir en memoria1
			} else if (modo == 2){
			leer = 2;//leer memoria1
			
		}
		}else if (!(PINC & (1<<PINC2))){ //memoria2
		if (modo == 1){
			escribir = 3;//escribir en memoria2
			} else if (modo == 2){
			leer = 3;//leer memoria2
			
		}
		}else if (!(PINC & (1<<PINC3))){ //memoria3
		if (modo == 1){
			escribir = 4;//escribir en memoria3
			} else if (modo == 2){
			leer = 4;//leer memoria3
		}
	}
}


ISR(PCINT0_vect){
	if (PINB == 0b0){
		modo++;
	}
	if (modo > 3){
		modo = 0;
	}
}

ISR(ADC_vect){
	adc_activo = ADMUX & ((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
	ADCSRA &= ~(1<<ADEN);
	if (adc_activo == 4){
		servo1 = ADCH;
		update_duty_cycle1(0, servo1*9/255+15); //boca
		ADC5();
	} else if (adc_activo == 5){
		servo2 = ADCH;
		update_duty_cycle1(1, servo2*15.93/255+15.93); //ojos
		ADC6();
	} else if (adc_activo == 6){
		servo3 = ADCH;
		update_duty_cycle0(1, servo3*15.93/255+15.93); //ceja1
		ADC7();
	} else if (adc_activo == 7){
		servo4 = ADCH;
		update_duty_cycle0(0, servo4*15.93/255+15.93); //ceja2
		ADC4();
}
	ADCSRA |= (1<<ADIF); //apagar bandera
	ADCSRA |= (1<<ADEN);
}

ISR(USART_RX_vect){
	
	if (uart == 97){//potenciometro 1, boca
		//PORTD |= (1<<PORTD4);
		if (hola == 0){//centecimas
				servo1 = 0;
				servo1 = (UDR0 - 48)*100;
				hola = 10;
			} else if (hola == 10){//decimas
				
				servo1 = servo1 + (UDR0 - 48)*10;
				hola = 20;
			} else if (hola == 20){//unidades
				//PORTD |= (1<<PORTD4);
				servo1 = servo1 + (UDR0 - 48);
				hola = 0;
				uart = 0;
				//uart = 0;
		} 
	}
	if (uart == 98){//potenciometro 2, ojos
		//PORTD |= (1<<PORTD4);
		if (hola == 0){//centecimas
			servo2 = 0;
			servo2 = (UDR0 - 48)*100;
			hola = 10;
			} else if (hola == 10){//decimas
			
			servo2 = servo2 + (UDR0 - 48)*10;
			hola = 20;
			} else if (hola == 20){//unidades
			
			servo2 = servo2 + (UDR0 - 48);
			hola = 0;
			uart = 0;
			//uart = 0;
		}
	}
	if (uart == 99){//potenciometro 3, ceja1
		//PORTD |= (1<<PORTD4);
		if (hola == 0){//centecimas
			servo3 = 0;
			servo3 = (UDR0 - 48)*100;
			hola = 10;
			} else if (hola == 10){//decimas
			
			servo3 = servo3 + (UDR0 - 48)*10;
			hola = 20;
			} else if (hola == 20){//unidades
			
			servo3 = servo3 + (UDR0 - 48);
			hola = 0;
			uart = 0;
			//uart = 0;
		}
	}
	if (uart == 100){//potenciometro 4, ceja 2
		//PORTD |= (1<<PORTD4);
		if (hola == 0){//centecimas
			servo4 = 0;
			servo4 = (UDR0 - 48)*100;
			hola = 10;
			} else if (hola == 10){//decimas
				
			servo4 = servo4 + (UDR0 - 48)*10;
			hola = 20;
			} else if (hola == 20){//unidades
				
			servo4 = servo4 + (UDR0 - 48);
			hola = 0;
			uart = 0;
			//uart = 0;
		}
	}
	if (uart == 101){//escribir en eeprom
		if (UDR0 == 49){//1
			escribir = 1;
			uart = 0;
		}else if(UDR0 == 50){
			escribir = 2;
			uart = 0;
		}else if(UDR0 == 51){
			escribir = 3;
			uart = 0;
		}else if(UDR0 == 52){
			escribir = 4;
			uart = 0;
		}
	}
	if (uart == 102){//leer en eeprom
		if (UDR0 == 49){
			leer = 1;
			uart = 0;
		}else if(UDR0 == 50){
			leer = 2;
			uart = 0;
		}else if(UDR0 == 51){
			leer = 3;
			uart = 0;
		}else if(UDR0 == 52){
			leer = 4;
			uart = 0;
		}
	}
	if (UDR0 == 97){//a poten 1
		
		uart = 97;
		//PORTD |= (1<<PORTD4);
		}else if(UDR0 == 98){//b poten 2
		uart = 98;
		//PORTD |= (1<<PORTD4);
		}else if (UDR0 == 99){//c poten 3
		uart = 99;
		//PORTD &= ~(1<<PORTD4);
		}else if (UDR0 == 100){//d poten 4
		//PORTD |= (1<<PORTD4);
		uart = 100;
		}else if (UDR0 == 101){//e escribir eeprom
		uart = 101;		
		}else if (UDR0 == 102){//f leer eeprom
		uart = 102;
	}

}