#ifndef PRINTER_H
#define PRINTER_H

#include "config.h"

#define PRINT_SPLIT 6 // max points printed at the same time, 384/PRINT_SPLIT==96
#define MAX_PRINT_PTS 24

void Printer_SendWorld8(uint8_t World);

void ClearPrinterBuffer();
uint8_t IsPaper();

uint8_t Header_Init();
uint8_t Header_Init1();

void Motor_Stepper_Pos1(uint8_t Position);

void Motor_Stepper_Pos2(uint8_t Position);

uint8_t feed_pitch1(uint64_t lines, uint8_t forward_backward);

uint8_t bits_number(uint8_t n);
 
void PrintDots8bit_split(CONFIG*cfg,uint8_t *Array, uint8_t characters);

void PrintDots8bit(CONFIG*cfg,uint8_t *Array, uint8_t characters,uint8_t feed_num);

uint16 temperature();

void print_lines8(CONFIG*);

uint8_t invert_bit(uint8_t a);

void print_image8(CONFIG*);
void print_cut_line(CONFIG*);

#endif
