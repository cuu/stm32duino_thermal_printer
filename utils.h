#ifndef UTILS_H
#define UTILS_H

#define SEP Serial.print(" ");
#define DEBUG(a,b) Serial.print(a);SEP;Serial.println(b,DEC);

#define ALINE Serial.println();

void delayus(unsigned int _us);

uint8_t invert_bit(uint8_t a);

uint8_t bits_number(uint8_t n);


#endif
