/*
File:   IOs.h (submit)
Author: Sheharyar, Aryan Mogera, Gerardo Garcia de Leon
*/

#ifndef IOS_H
#define IOS_H

#include <xc.h>

#define LED LATBbits.LATB8
#define PB1 PORTAbits.RA2
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA4

void IOinit(void);
void IOcheck(void);

#endif // IOS_H