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
#include <stdio.h>

#define _XTAL_FREQ 8000000 // Frecuencia de 8MHz

//VARIABLES
int ascii[10] = {0, 1, 2, 3, 4, 5, 7, 8, 9}; 
unsigned int palabra; //variable para guardar caracter ascii
unsigned char bandera; //bandera para manetener loop
unsigned char selector; //selector para entrar en algun modo
unsigned char flag; //bandera para mantener en loop
unsigned int centena; //variable para centenas en conversión ascii
unsigned int decena; //variable para decenas en conversión ascii
unsigned int unidad; //variable para unidades en conversión ascii
unsigned int pot; //variable para guardar valor del potenciómetro

void setup(void); 
void setupADC(void);
void setupUART(void);
void cadena(char *cursor);

//VECTOR DE INTERRUPCIONES
void __interrupt() isr(void){
    if (PIR1bits.ADIF == 1){ //chequear bandera del conversor ADC
        pot = ADRESH; //pasar valor del ADRESH a variable pot
        centena = (pot/100); //operación para obtener centenas
        decena = ((pot/10)%10); //operación para obtener decenas
        unidad = (pot%10); //operación para obtener unidades
        PIR1bits.ADIF = 0; //limpiar bandera de conversor
        }
    
}

void main(void){
    setup(); //llamar a función de setup
    setupADC(); //llamar a función de setup del ADC
    setupUART(); //llamar a función de setup del UART       
    while(1){ 
        cadena("\n-------------MENU------------------\n1) Leer Potenciometro\n2) Enviar ASCII\n"); //imprimir menu
        bandera = 1; //encender bandera
        while (bandera == 1){
                if (PIR1bits.RCIF == 1){ //revisar bandera de si el recibidor esta lleno 
                selector = RCREG; //pasar a dato a varible
                PIR1bits.RCIF = 0; //limpiar bandera
                __delay_ms(10);} //delay de 10ms
            if (selector == '1'){ //revisar si se selecciono 1
                ADCON0bits.GO = 1; //iniciar conversor ADC
                __delay_ms(5); //delay de 5ms
                TXREG = ascii[centena]+48; //mostrar valor de centena
                __delay_ms(5); //delay de 5ms
                TXREG = ascii[decena]+48; //mostrar valor de decena
                __delay_ms(5); //delay de 5ms
                TXREG = ascii[unidad]+48; //mostrar valor de unidades
                bandera = 0; //regresar al menu
                selector = 0; //limpiar selector
            }
            
            if (selector == '2'){ //revisar si se selecciono 2
                cadena("Ingrese caracter (solo un caracter)\n"); //mostrar informacion
                cadena("\n"); //enter
                flag = 1; //encender bandera para esperar que se introduzca tecla
                PIR1bits.RCIF = 0; //limpiar bandera 
                while (flag == 1){ //loop
                if (PIR1bits.RCIF == 1 && RCREG != 0){ //esperar que se presione tecla  
                PORTB = RCREG; //mostrar valor en el puerto B
                PIR1bits.RCIF = 0; //limpiar bandera
                flag = 0; //apagar bandera
                bandera = 0; //apagar bandera
                selector = 0;}}} //apagar bandera
        
        }
    }
}

void setup(void){
    ANSEL = 0b00000001; //puertos digitales menos RA0
    ANSELH = 0; //puertos digitales
    TRISB = 0; //puerto b como salida
    PORTB = 0; //limpiar puerto b
    
    OSCCONbits.IRCF = 0b111; //frecuencia de 8MHz
    OSCCONbits.SCS = 1; //utilizar oscilador interno
    
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    PIR1bits.ADIF = 0; // Limpiar bandera de interrupción del ADC
}

void setupADC(void){
    ADCON0bits.ADCS1 = 1; // Fosc/32        
    ADCON0bits.ADCS0 = 0; // =======      
    
    ADCON1bits.VCFG1 = 0; // Referencia VSS (0 Volts)
    ADCON1bits.VCFG0 = 0; // Referencia VDD (5 Volts)
    
    ADCON1bits.ADFM = 0;  // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0; // Canal AN0
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        
    
    ADCON0bits.ADON = 1; // Habilitamos el ADC
    __delay_us(100); //delay de 100 us
}

void setupUART(void){
    // Paso 1: configurar velocidad baud rate
    
    SPBRG = 12; //valor para 9600 de baud rate
    
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

//Funcion para mostrar texto
void cadena(char *cursor){
    while (*cursor != '\0'){//mientras el cursor sea diferente a nulo
        while (PIR1bits.TXIF == 0); //mientras que se este enviando no hacer nada
            TXREG = *cursor; //asignar el valor del cursor para enviar
            *cursor++;//aumentar posicion del cursor
    }
}
