/*
 * File:   oscilador.c
 * Author: Andres Lemus
 *
 * Created on October 23, 2022, 8:07 AM
 */


#include <xc.h>
#include "oscilador.h"

void osc(void){
    OSCCONbits.IRCF = 0b111; //8 MHz
    OSCCONbits.SCS = 1;
}