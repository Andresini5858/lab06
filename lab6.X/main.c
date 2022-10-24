/*
 * File:   main.c
 * Author: Andrés Lemus
 * Laboratorio 5
 * Created on October 23, 2022, 08:00 AM
 */
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include "oscilador.h"

#define _XTAL_FREQ 8000000

unsigned int i;


void setup(void);
void setupUART(void);





void main(void){
    setup();
    setupUART();
    while(1){
        if(TXSTAbits.TRMT == 1){
            TXREG = 33;}   
           
        if(PIR1bits.RCIF == 1){
            PORTB = RCREG;
            PIR1bits.RCIF = 0;}
            
            __delay_ms(50); 
    }
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
    TRISD = 0;
    PORTD = 0;
}

void setupUART(void){
    // Paso 1: configurar velocidad baud rate
    
    SPBRG = 12;
    
    // Paso 2:
    
    TXSTAbits.SYNC = 0;         // Modo Asíncrono
    RCSTAbits.SPEN = 1;         // Habilitar UART
    
    // Paso 3:
    // Usar 8 bits
    
    // Paso 4:
    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    PIR1bits.TXIF = 0;
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion
}
