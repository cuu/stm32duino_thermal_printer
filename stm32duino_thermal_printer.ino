// for ltp02-245
// 203 dpi, 384dots in 48mm(1.88976inch) every dots == 0.125mm, 1byte==8dots==1mm
#include <SPI.h>

#include "logo.h"

#include "pcf_5x7-ISO8859-1_5x7.h"
#include "pcf_6x12-ISO8859-1_6x12.h"
#include "pcf_7x14-ISO8859-1_7x14.h"
//#include "ttf_Px437_IBM_ISO8_8x16.h"
//#include "ttf_Px437_IBM_ISO9_8x16.h"

#include "ttf_Px437_IBM_PS2thin1_8x16.h"
#include "ttf_Px437_IBM_PS2thin2_8x16.h"

//#include "ttf_Px437_IBM_PS2thin3_8x16.h"

#include "config.h"
#include "utils.h"
#include "printer.h"

SerialCache ser_cache;

uint8_t cmd[10];
uint8_t cmd_idx;

uint8_t newline;
char buf[2];

unsigned long time;

ImageCache img_cache;

FONT current_font;

CONFIG g_config;


void printer_set_font(CONFIG*cfg,uint8_t fnbits);
void parse_serial_stream(CONFIG*cfg,uint8_t input_ch);

void reset_cmd(){
  cmd_idx = 0;
  ser_cache.idx=0;
  g_config.state = PRINT_STATE;
}

void init_printer(){
  
  memset(cmd,0,10);

  newline = 0;
  cmd_idx = 0;

  img_cache.idx=0;
  img_cache.num=0;
  img_cache.width=0;

  img_cache.need_print=0;
  img_cache.revert_bits = 0;

/*
  current_font.width=5; current_font.height=7; current_font.data = font_pcf_5x7_ISO8859_1_5x7; 
  current_font.width=6;current_font.height=12; current_font.data = font_pcf_6x12_ISO8859_1_6x12;
  current_font.width=7; current_font.height=14; current_font.data = font_pcf_7x14_ISO8859_1_7x14;

  //current_font.width=8;current_font.height=16; current_font.data= font_ttf_Px437_IBM_ISO8_8x16;
  //current_font.width=8;current_font.height=16; current_font.data= font_ttf_Px437_IBM_ISO9_8x16;
  
  current_font.width=8;current_font.height=16; current_font.data= font_ttf_Px437_IBM_PS2thin2_8x16;
*/
  current_font.width=8;current_font.height=16; current_font.data= font_ttf_Px437_IBM_PS2thin1_8x16;


  ser_cache.idx=0;

  g_config.line_space=0;
  g_config.align = ALIGN_LEFT;
  g_config.reverse = 0;
  
  g_config.margin.width = 0;
  g_config.margin.esgs = 0; 
  
  g_config.orient = BACKWARD;

  g_config.wordgap = 0;

  g_config.font = &current_font;
  
  g_config.under_line = 0;

  g_config.state = PRINT_STATE;

  g_config.img = &img_cache;
  g_config.density = 0;

  g_config.feed_pitch = 2;
  
}

const  char url[]  ={"https://www.clockworkpi.com"};

void printer_test(CONFIG*cfg){

  uint8_t i,j;
  uint8_t ch;
  uint16_t k;
  
  char buf[48];  
  char *font_names[]={"IBM_PS2thin2_8x16","5x7_ISO8859_1","6x12_ISO8859_1","7x14_ISO8859_1","IBM_PS2thin1_8x16"};
  
  cfg->density = 4;
  
  k = (os120_width/8)*os120_height;
  memcpy(cfg->img->cache,os120_bits,k);
  cfg->img->width = os120_width/8;
  cfg->img->num = k;
  cfg->img->revert_bits=1;
  cfg->align = ALIGN_CENTER;
  print_image8(cfg);
  
  cfg->img->revert_bits=0;
  cfg->align = ALIGN_LEFT;
  feed_pitch1(15,cfg->orient);
 
//---------------------------------------------     

  for(i=1;i<4;i++){
    printer_set_font(cfg,0);
    reset_cmd();
    for(j=0;j<strlen(font_names[i]);j++){
      
      parse_serial_stream(cfg,font_names[i][j]);      
    }
    parse_serial_stream(cfg,10);
    
    printer_set_font(cfg,i);
    reset_cmd();
    for(ch = 33;ch<127;ch++){
      parse_serial_stream(cfg,ch);      
      //Serial.print(ch,DEC);
    }
    parse_serial_stream(cfg,10);
    //Serial.println();
    feed_pitch1(48,cfg->orient);
  }

  
  printer_set_font(cfg,0);
  reset_cmd();
  for(j=0;j<strlen(font_names[0]);j++){    
    parse_serial_stream(cfg,font_names[0][j]);      
  }  
  parse_serial_stream(cfg,10);
  
  printer_set_font(cfg,0);
  reset_cmd();
  for(ch = 33;ch<127;ch++){
    parse_serial_stream(cfg,ch);      
    //Serial.print(ch,DEC);
  }
  parse_serial_stream(cfg,10);
  //Serial.println();
  feed_pitch1(28,cfg->orient);

//-------------------------------------------

  k = temperature();
  sprintf(buf,"temperature:");
  i = 48-strlen(buf);
  sprintf(buf,"temperature:%*s%d C",i-5,"",k);
  
  printer_set_font(cfg,4);
  reset_cmd();

  for(i=0;i<strlen(buf);i++){
    parse_serial_stream(cfg,buf[i]);
  }
  parse_serial_stream(cfg,10);
  reset_cmd();  

//------------------------------------------  
  sprintf(buf,"baudrate:");
  i = 48-strlen(buf);
  sprintf(buf,"baudrate:%*s%d",i-7,"",115200);
  
  printer_set_font(cfg,4);
  reset_cmd();

  for(i=0;i<strlen(buf);i++){
    parse_serial_stream(cfg,buf[i]);
  }
  parse_serial_stream(cfg,10);
  reset_cmd();  
  
//------------------------------------------  
  sprintf(buf,"version:");
  i = 48-strlen(buf);
  sprintf(buf,"version:%*s%.1f",i-4,"",0.1);
  
  printer_set_font(cfg,4);
  reset_cmd();

  for(i=0;i<strlen(buf);i++){
    parse_serial_stream(cfg,buf[i]);
  }
  parse_serial_stream(cfg,10);
  reset_cmd();  

  
  feed_pitch1(cfg->font->height,cfg->orient);
//--------------------------------------------------------------  
  printer_set_font(cfg,0);
  reset_cmd();
 
  //Serial.println(strlen(url),DEC);
  for(i=0;i<strlen(url);i++){
    parse_serial_stream(cfg,url[i]);
    
  }
  parse_serial_stream(cfg,10);
  reset_cmd();


//-----------------------------------
//grid

  for(ch = 0;ch <16;ch++){
    if(ch%2==0)
      j = 0xff;
    else
      j = 0x00; 
         
    for(k=0;k<8;k++){
      for(i=0;i<48;i++){
        if(i % 2==0) {
          buf[i]=j;
        }else{
          buf[i]=0xff-j;
        }
      }  
      PrintDots8bit_split(cfg,(uint8_t*)buf,48);
    }
  }

//--------------------------------------------------------
  feed_pitch1(cfg->font->height*2,cfg->orient);
  
}

void printer_set_font(CONFIG*cfg,uint8_t fnbits){
      uint8_t ret;
      ret =  MID(fnbits,0,3);
      
      if(ret==0) {
        cfg->font->width = 8 ;
        cfg->font->height = 16;
        cfg->font->data = font_ttf_Px437_IBM_PS2thin1_8x16;
      }
      
      if(ret==1){
        cfg->font->width = 5;
        cfg->font->height = 7;
        cfg->font->data = font_pcf_5x7_ISO8859_1_5x7;
      }
            
      if(ret==2){
        cfg->font->width = 6;
        cfg->font->height = 12;
        cfg->font->data = font_pcf_6x12_ISO8859_1_6x12;
      }
      
      if(ret==3){
        cfg->font->width = 7;
        cfg->font->height = 14;
        cfg->font->data = font_pcf_7x14_ISO8859_1_7x14;
      }
      
      if(ret == 4){
        cfg->font->width = 8 ;
        cfg->font->height = 16;
        cfg->font->data = font_ttf_Px437_IBM_PS2thin2_8x16;
      }  
}

void parse_cmd(CONFIG*cfg,uint8_t *cmd, uint8_t cmdidx){
  
  uint8_t ret;
  
  if(cmdidx >1){
    //ESC 2
    if(cmd[0] == ASCII_ESC && cmd[1] == 0x32){
      cfg->line_space = cfg->font->height+8;
      reset_cmd();
    }

    //ESC @
    if(cmd[0] == ASCII_ESC && cmd[1] == 0x40){
      init_printer();
      reset_cmd();
      
    }
    //DC2 T  printer test page
    if(cmd[0] == ASCII_DC2 && cmd[1] == 0x54){
      printer_test(cfg);
      reset_cmd();
    }
  }
  
  if(cmdidx > 2){
     //ESC j n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x4a){
        print_lines8(cfg);
        feed_pitch1(cmd[2],BACKWARD);
        reset_cmd();
     }
     //ESC d n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x64){
        print_lines8(cfg);
        
        feed_pitch1(cmd[2]*cfg->font->height,BACKWARD);
        reset_cmd();
     }
     //ESC ! n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x21){
        printer_set_font(cfg,cmd[2]);
        reset_cmd();
     }

     //ESC 3 n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x33){
        cfg->line_space = cmd[2];
        reset_cmd();
     }
     
     // ESC v n 
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x76){
       if(temperature() > 70){
        ret |= 1 << 6;
       }
       if (IsPaper()==NO_PAPER ){
        ret |= 1 << 2;
       }

       Serial.print(ret);
       reset_cmd();
       return;
     }
     //ESC a n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x61){
        ret = cmd[2];
        if(ret == 0 || ret == 48){
          cfg->align = ALIGN_LEFT;
        }
        if(ret == 1 || ret == 49){
          cfg->align = ALIGN_CENTER;
        }
        if(ret == 2 || ret == 50){
          cfg->align = ALIGN_RIGHT;
        }
        
        reset_cmd();
     }
     //ESC - n 
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x2d){
        ret = cmd[2];
        if(ret == 0 || ret == 48){
          cfg->under_line =0;
        }
        if(ret == 1 || ret == 49){
          cfg->under_line = 1;
        }
        if(ret == 2 || ret == 50){
          cfg->under_line = 2;
        }
        
        reset_cmd();     
      
     }

     //ESC SP n
     if(cmd[0] == ASCII_ESC && cmd[1] == 0x20){
        ret = cmd[2];
        if( ret + cfg->margin.width <MAX_DOTS){
          cfg->wordgap = ret;
        }
        reset_cmd();
        return;
     }

     //DC2 # n
     if(cmd[0] == ASCII_DC2 && cmd[1] == 0x23){
        ret = cmd[2];
        cfg->density = ret;
        reset_cmd();
     }
     
     
  }

  if(cmdidx > 3){
    //GS L nL nH
    if(cmd[0] == ASCII_GS && cmd[1] == 0x4c){
      uint16_t k;
      k = cmd[2]+cmd[3]*256;
      if (k < MAX_DOTS - cfg->font->width){
        cfg->margin.width = k;
      }

      reset_cmd();
    }
    
  }

  if(cmdidx > 4){
    /*
    if(cmd[0] == ASCII_ESC && cmd[1] == 0x2a){
      if( cmd[2] == 0 || cmd[2] == 1){
        uint16_t k;
        k = cmd[3] + 256*cmd[4];
        if(k<= IMAGE_MAX){
          cfg->state = GET_IMAGE;
          cfg->img->num = k;
          cfg->img->idx = 0;
        }
      }
      
      reset_cmd();
    }
    */
  }

  if(cmdidx > 7){
    // GS v 0 p wL wH hL hH d1…dk 
    if(cmd[0] == ASCII_GS && cmd[1] == 118 && cmd[2] == 48 ) {
       uint16_t width = cmd[4] + cmd[5]*256;
       uint16_t height = cmd[6]+cmd[7]*256;
       uint16_t k;
       k = width * height;
       if(k<= IMAGE_MAX){
        cfg->state = GET_IMAGE;
        cfg->img->num = k;
        cfg->img->idx = 0;
        cfg->img->width = width;
        cfg->img->need_print=1;
        Serial.println(cfg->img->width,DEC);

       }
      reset_cmd();
    }
  }
  
}

void setup() {
  
  Header_Init();
  Header_Init1();
 
  ClearPrinterBuffer();
  
  Serial.begin(115200); 

  init_printer();
  
}


void parse_serial_stream(CONFIG*cfg,uint8_t input_ch){
  uint16_t a;
  
    if(cfg->state == GET_IMAGE){
      cfg->img->cache[cfg->img->idx] = input_ch;
      cfg->img->idx++;
      if(cfg->img->idx >= cfg->img->num){//image full 
        if(cfg->img->need_print==1){
          print_image8(cfg);
        }
        cfg->state = PRINT_STATE;
      }
    }else {//PRINT_STATE
      switch(input_ch){
        case ASCII_LF:
          print_lines8(cfg);
          reset_cmd();
          //feed_pitch1(cfg->font->height,cfg->orient);
          cfg->state = PRINT_STATE;
        break;
        case ASCII_DC2:
        case ASCII_GS: 
        case ASCII_ESC:
          cmd[cmd_idx] = input_ch;
          cfg->state = ESC_STATE;
          cmd_idx++;
          break;
        default:
          
          if(cfg->state ==  ESC_STATE) {
            cmd[cmd_idx] = input_ch;
            cmd_idx++;
            if(cmd_idx < 10){
              parse_cmd(cfg,cmd,cmd_idx);
            }else{
              reset_cmd();
              cfg->state = PRINT_STATE;
            }
            
          }
          

          ser_cache.data[ser_cache.idx]=input_ch;
          ser_cache.idx++;
         
          a = (ser_cache.idx+1)*current_font.width+(ser_cache.idx)*0+ g_config.margin.width;          
          if( a >= MAX_DOTS)
          {
            print_lines8(cfg);
            reset_cmd();
          }
      }
    } 
  
}


void loop() {
  
  if (Serial.available() > 0) {
    // read the incoming byte
    Serial.readBytes(buf,1);
    parse_serial_stream(&g_config,buf[0]);
  }
  
}