/* 
 * File:   LCD.h
 * Author: castia
 *
 * Created on May 25, 2014, 7:53 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define V 128
#define H 160

#define RGB565(r,g,b) ((((r>>3)<<11) | (g>>3) | ((b>>2)<<5)))

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define BLACK RGB565(0,0,0)
#define RED RGB565(255,0,0)
#define GREEN RGB565(0,255,0)
#define BLUE RGB565(0,0,255)
#define YELLOW RGB565(255,255,0)
#define CYAN RGB565(0,255,255)
#define PINK RGB565(255,0,255)
#define GRAY RGB565(192,192,192)
#define WHITE RGB565(255,255,255)


#define CS 0b00010000 //Low = select, High = deselect.
#define RE 0b00000010 //High = normal, Low = reset.
#define A0 0b00010000 //Low = Command, High = Data.

void delay(int x);
void sendcomand(char input);
void senddata(char input);
void SetPix(char x, char y, int color);
void clean(int color);
void initLCD(void);
void ASCII(char x, char y, int color, int background, char letter, char size);

int customColor(int r, int g, int b);
void drawBoxFill(char x, char y,char height, char width, int color);
void drawBox(char x, char y, char height, char width, int border, int color);

void prints(char x, char y, int color, int background, const char message[], char size);
void integerprint(char x, char y, int color, int background,int integer, char size);

// depreciated
void box(char x, char y, char high, char breth, int color);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

