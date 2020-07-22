#ifndef CONFIG_H
#define CONFIG_H



//PA8-12 UART1

#define SPI1_NSS_PIN PA4    //SPI_1 Chip Select pin is PA4.

#define VH_PIN PB12 //ENABLE_VH required
#define LATCH_PIN  PB13 //18
#define PEM_PIN  PB14 // 1
#define PEM_CTL_PIN PB8 //3 
#define ENABLE_PEM digitalWrite(PEM_CTL_PIN,HIGH)
#define DISABLE_PEM digitalWrite(PEM_CTL_PIN,LOW) 

#define CLK_PIN PA5
#define MOSI_PIN PA7

/**
  *@brief STB_NUMBER stand for STROBE NUMBER of lines, which means how many lines
  *        are going to be activated
**/
#define STB_NUMBER        1

#define STB1_PIN PB15 //13
#define STB2_PIN PB8
#define STB3_PIN PA15
#define STB4_PIN PB3
#define STB5_PIN PB4
#define STB6_PIN PB5


#define PH1_PIN PA0
#define PH2_PIN PA1
#define PH3_PIN PA2
#define PH4_PIN PA3

///0 1 3 2 mine
#define PA_PIN  PH1_PIN //
#define PNA_PIN PH2_PIN //
#define PB_PIN  PH3_PIN //
#define PNB_PIN PH4_PIN //



#define ENABLE1_PIN  PA13
#define ENABLE2_PIN  PA14

#define THERMISTORPIN PA6  //ADC,14


#define MOTOR_ENABLE1
#define MOTOR_ENABLE2 

#define MOTOR_DISABLE1  
#define MOTOR_DISABLE2 
 
#define ENABLE_VH digitalWrite(VH_PIN, HIGH)
#define DISABLE_VH digitalWrite(VH_PIN, LOW)

#define LATCH_ENABLE digitalWrite(LATCH_PIN, LOW)
#define LATCH_DISABLE digitalWrite(LATCH_PIN, HIGH)

#define ASK4PAPER  digitalRead(PEM_PIN)


#define ERROR_FEED_PITCH  ((uint8_t) 0x01)
#define IS_PAPER          0x00
#define NO_PAPER          0x01
#define HOT_PRINTER       0x02

#define FORWARD           0x00
#define BACKWARD          0x01

#define HOT 64

#define BCoefficent       3950
#define RthNominal        30000
#define TempNominal       25 
#define ADCResolution     4096
#define SeriesResistor    30000

#define NumSamples        10

#define KELVIN            1
#define CELSIUS           0

#define HEAT_TIME 300 // heat time,better not greater than 1000,300-1000 0-f

#define int16               uint16_t
#define int8                uint8_t

#define asciistart          ((uint8_t)'A')
#define netxcharacter       ((uint8_t)24)
#define Fontrows            ((uint8_t)24)
#define FontColums          ((uint8_t)16)

#define Maxdotsperline      ((uint16_t)384)
#define nextcharactercolum  ((uint8_t)Fontrows/8) // = 3
#define Maxcharacterperline ((uint16_t)Maxdotsperline/FontColums) // 384/16=24



#define ASCII_TAB '\t' // Horizontal tab
#define ASCII_LF  '\n' // Line feed
#define ASCII_FF  '\f' // Form feed
#define ASCII_CR  '\r' // Carriage return
#define ASCII_EOT   4  // End of Transmission
#define ASCII_DLE  16  // Data Link Escape
#define ASCII_DC2  18  // Device control 2 //0x12
#define ASCII_ESC  27  // Escape //0x1b
#define ASCII_FS   28  // Field separator//0x1c
#define ASCII_GS   29  // Group separator //0x1d

#define PRINT_STATE 0
#define ESC_STATE 1
#define GET_IMAGE 2


#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

#define MAX_DOTS 384
#define IMAGE_MAX 9224

#define BITS8 8
#define PRINTER_BITS 384
#define MAXPIXELS 48

//extract bits 
#define LAST(k,n) ((k) & ((1<<(n))-1))
#define MID(k,m,n) LAST((k)>>(m),((n)-(m)))


typedef struct Margin{
  uint16_t width;
  uint8_t esgs;
};


typedef struct FONT {

  uint8_t width;//in bits
  uint8_t height;
  const uint8_t *data;
};

typedef struct ImageCache{
  
  uint16_t idx;
  uint16_t num;
  uint16_t width;
  uint16_t height;
  uint8_t need_print:1;
  uint8_t revert_bits:1;//MSB OR LSB
  uint8_t cache[IMAGE_MAX]; // 48x192bytes(384x192 pixels) ,max
  
};

typedef struct CONFIG
{  
  uint8_t state;
  uint8_t line_space;
  uint8_t align;
  uint8_t reverse; //reverse print   
  uint8_t orient;
  uint8_t under_line;
  uint8_t feed_pitch;
  uint8_t density:4; //0-f,300+density*46 HEAT_TIME

  uint16_t wordgap:10;//1023 max
  
  Margin margin;
  FONT*font;
  ImageCache *img;
};


typedef struct SerialCache{
  uint8_t idx;
  uint8_t data[77];//384/5
};

void PrintDots8bit(uint8_t *Array, uint8_t characters,uint8_t feed_num);

uint8_t invert_bit(uint8_t a);

#endif
