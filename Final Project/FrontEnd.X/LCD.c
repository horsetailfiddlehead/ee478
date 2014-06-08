
#include "globals.h"
#include "LCD.h"

const rom char font[255][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x3E, 0x5B, 0x4F, 0x5B, 0x3E},
    {0x3E, 0x6B, 0x4F, 0x6B, 0x3E},
    {0x1C, 0x3E, 0x7C, 0x3E, 0x1C},
    {0x18, 0x3C, 0x7E, 0x3C, 0x18},
    {0x1C, 0x57, 0x7D, 0x57, 0x1C},
    {0x1C, 0x5E, 0x7F, 0x5E, 0x1C},
    {0x00, 0x18, 0x3C, 0x18, 0x00},
    {0xFF, 0xE7, 0xC3, 0xE7, 0xFF},
    {0x00, 0x18, 0x24, 0x18, 0x00},
    {0xFF, 0xE7, 0xDB, 0xE7, 0xFF},
    {0x30, 0x48, 0x3A, 0x06, 0x0E},
    {0x26, 0x29, 0x79, 0x29, 0x26},
    {0x40, 0x7F, 0x05, 0x05, 0x07},
    {0x40, 0x7F, 0x05, 0x25, 0x3F},
    {0x5A, 0x3C, 0xE7, 0x3C, 0x5A},
    {0x7F, 0x3E, 0x1C, 0x1C, 0x08},
    {0x08, 0x1C, 0x1C, 0x3E, 0x7F},
    {0x14, 0x22, 0x7F, 0x22, 0x14},
    {0x5F, 0x5F, 0x00, 0x5F, 0x5F},
    {0x06, 0x09, 0x7F, 0x01, 0x7F},
    {0x00, 0x66, 0x89, 0x95, 0x6A},
    {0x60, 0x60, 0x60, 0x60, 0x60},
    {0x94, 0xA2, 0xFF, 0xA2, 0x94},
    {0x08, 0x04, 0x7E, 0x04, 0x08},
    {0x10, 0x20, 0x7E, 0x20, 0x10},
    {0x08, 0x08, 0x2A, 0x1C, 0x08},
    {0x08, 0x1C, 0x2A, 0x08, 0x08},
    {0x1E, 0x10, 0x10, 0x10, 0x10},
    {0x0C, 0x1E, 0x0C, 0x1E, 0x0C},
    {0x30, 0x38, 0x3E, 0x38, 0x30},
    {0x06, 0x0E, 0x3E, 0x0E, 0x06},
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x5F, 0x00, 0x00},
    {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14},
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},
    {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x56, 0x20, 0x50},
    {0x00, 0x08, 0x07, 0x03, 0x00},
    {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00},
    {0x2A, 0x1C, 0x7F, 0x1C, 0x2A},
    {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x80, 0x70, 0x30, 0x00},
    {0x08, 0x08, 0x08, 0x08, 0x08},
    {0x00, 0x00, 0x60, 0x60, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02},
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x72, 0x49, 0x49, 0x49, 0x46},
    {0x21, 0x41, 0x49, 0x4D, 0x33},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x31},
    {0x41, 0x21, 0x11, 0x09, 0x07},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x46, 0x49, 0x49, 0x29, 0x1E},
    {0x00, 0x00, 0x14, 0x00, 0x00},
    {0x00, 0x40, 0x34, 0x00, 0x00},
    {0x00, 0x08, 0x14, 0x22, 0x41},
    {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x00, 0x41, 0x22, 0x14, 0x08},
    {0x02, 0x01, 0x59, 0x09, 0x06},
    {0x3E, 0x41, 0x5D, 0x59, 0x4E},
    {0x7C, 0x12, 0x11, 0x12, 0x7C},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x41, 0x51, 0x73},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x1C, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x26, 0x49, 0x49, 0x49, 0x32},
    {0x03, 0x01, 0x7F, 0x01, 0x03},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x03, 0x04, 0x78, 0x04, 0x03},
    {0x61, 0x59, 0x49, 0x4D, 0x43},
    {0x00, 0x7F, 0x41, 0x41, 0x41},
    {0x02, 0x04, 0x08, 0x10, 0x20},
    {0x00, 0x41, 0x41, 0x41, 0x7F},
    {0x04, 0x02, 0x01, 0x02, 0x04},
    {0x40, 0x40, 0x40, 0x40, 0x40},
    {0x00, 0x03, 0x07, 0x08, 0x00},
    {0x20, 0x54, 0x54, 0x78, 0x40},
    {0x7F, 0x28, 0x44, 0x44, 0x38},
    {0x38, 0x44, 0x44, 0x44, 0x28},
    {0x38, 0x44, 0x44, 0x28, 0x7F},
    {0x38, 0x54, 0x54, 0x54, 0x18},
    {0x00, 0x08, 0x7E, 0x09, 0x02},
    {0x18, 0xA4, 0xA4, 0x9C, 0x78},
    {0x7F, 0x08, 0x04, 0x04, 0x78},
    {0x00, 0x44, 0x7D, 0x40, 0x00},
    {0x20, 0x40, 0x40, 0x3D, 0x00},
    {0x7F, 0x10, 0x28, 0x44, 0x00},
    {0x00, 0x41, 0x7F, 0x40, 0x00},
    {0x7C, 0x04, 0x78, 0x04, 0x78},
    {0x7C, 0x08, 0x04, 0x04, 0x78},
    {0x38, 0x44, 0x44, 0x44, 0x38},
    {0xFC, 0x18, 0x24, 0x24, 0x18},
    {0x18, 0x24, 0x24, 0x18, 0xFC},
    {0x7C, 0x08, 0x04, 0x04, 0x08},
    {0x48, 0x54, 0x54, 0x54, 0x24},
    {0x04, 0x04, 0x3F, 0x44, 0x24},
    {0x3C, 0x40, 0x40, 0x20, 0x7C},
    {0x1C, 0x20, 0x40, 0x20, 0x1C},
    {0x3C, 0x40, 0x30, 0x40, 0x3C},
    {0x44, 0x28, 0x10, 0x28, 0x44},
    {0x4C, 0x90, 0x90, 0x90, 0x7C},
    {0x44, 0x64, 0x54, 0x4C, 0x44},
    {0x00, 0x08, 0x36, 0x41, 0x00},
    {0x00, 0x00, 0x77, 0x00, 0x00},
    {0x00, 0x41, 0x36, 0x08, 0x00},
    {0x02, 0x01, 0x02, 0x04, 0x02},
    {0x3C, 0x26, 0x23, 0x26, 0x3C},
    {0x1E, 0xA1, 0xA1, 0x61, 0x12},
    {0x3A, 0x40, 0x40, 0x20, 0x7A},
    {0x38, 0x54, 0x54, 0x55, 0x59},
    {0x21, 0x55, 0x55, 0x79, 0x41},
    {0x21, 0x54, 0x54, 0x78, 0x41},
    {0x21, 0x55, 0x54, 0x78, 0x40},
    {0x20, 0x54, 0x55, 0x79, 0x40},
    {0x0C, 0x1E, 0x52, 0x72, 0x12},
    {0x39, 0x55, 0x55, 0x55, 0x59},
    {0x39, 0x54, 0x54, 0x54, 0x59},
    {0x39, 0x55, 0x54, 0x54, 0x58},
    {0x00, 0x00, 0x45, 0x7C, 0x41},
    {0x00, 0x02, 0x45, 0x7D, 0x42},
    {0x00, 0x01, 0x45, 0x7C, 0x40},
    {0xF0, 0x29, 0x24, 0x29, 0xF0},
    {0xF0, 0x28, 0x25, 0x28, 0xF0},
    {0x7C, 0x54, 0x55, 0x45, 0x00},
    {0x20, 0x54, 0x54, 0x7C, 0x54},
    {0x7C, 0x0A, 0x09, 0x7F, 0x49},
    {0x32, 0x49, 0x49, 0x49, 0x32},
    {0x32, 0x48, 0x48, 0x48, 0x32},
    {0x32, 0x4A, 0x48, 0x48, 0x30},
    {0x3A, 0x41, 0x41, 0x21, 0x7A},
    {0x3A, 0x42, 0x40, 0x20, 0x78},
    {0x00, 0x9D, 0xA0, 0xA0, 0x7D},
    {0x39, 0x44, 0x44, 0x44, 0x39},
    {0x3D, 0x40, 0x40, 0x40, 0x3D},
    {0x3C, 0x24, 0xFF, 0x24, 0x24},
    {0x48, 0x7E, 0x49, 0x43, 0x66},
    {0x2B, 0x2F, 0xFC, 0x2F, 0x2B},
    {0xFF, 0x09, 0x29, 0xF6, 0x20},
    {0xC0, 0x88, 0x7E, 0x09, 0x03},
    {0x20, 0x54, 0x54, 0x79, 0x41},
    {0x00, 0x00, 0x44, 0x7D, 0x41},
    {0x30, 0x48, 0x48, 0x4A, 0x32},
    {0x38, 0x40, 0x40, 0x22, 0x7A},
    {0x00, 0x7A, 0x0A, 0x0A, 0x72},
    {0x7D, 0x0D, 0x19, 0x31, 0x7D},
    {0x26, 0x29, 0x29, 0x2F, 0x28},
    {0x26, 0x29, 0x29, 0x29, 0x26},
    {0x30, 0x48, 0x4D, 0x40, 0x20},
    {0x38, 0x08, 0x08, 0x08, 0x08},
    {0x08, 0x08, 0x08, 0x08, 0x38},
    {0x2F, 0x10, 0xC8, 0xAC, 0xBA},
    {0x2F, 0x10, 0x28, 0x34, 0xFA},
    {0x00, 0x00, 0x7B, 0x00, 0x00},
    {0x08, 0x14, 0x2A, 0x14, 0x22},
    {0x22, 0x14, 0x2A, 0x14, 0x08},
    {0xAA, 0x00, 0x55, 0x00, 0xAA},
    {0xAA, 0x55, 0xAA, 0x55, 0xAA},
    {0x00, 0x00, 0x00, 0xFF, 0x00},
    {0x10, 0x10, 0x10, 0xFF, 0x00},
    {0x14, 0x14, 0x14, 0xFF, 0x00},
    {0x10, 0x10, 0xFF, 0x00, 0xFF},
    {0x10, 0x10, 0xF0, 0x10, 0xF0},
    {0x14, 0x14, 0x14, 0xFC, 0x00},
    {0x14, 0x14, 0xF7, 0x00, 0xFF},
    {0x00, 0x00, 0xFF, 0x00, 0xFF},
    {0x14, 0x14, 0xF4, 0x04, 0xFC},
    {0x14, 0x14, 0x17, 0x10, 0x1F},
    {0x10, 0x10, 0x1F, 0x10, 0x1F},
    {0x14, 0x14, 0x14, 0x1F, 0x00},
    {0x10, 0x10, 0x10, 0xF0, 0x00},
    {0x00, 0x00, 0x00, 0x1F, 0x10},
    {0x10, 0x10, 0x10, 0x1F, 0x10},
    {0x10, 0x10, 0x10, 0xF0, 0x10},
    {0x00, 0x00, 0x00, 0xFF, 0x10},
    {0x10, 0x10, 0x10, 0x10, 0x10},
    {0x10, 0x10, 0x10, 0xFF, 0x10},
    {0x00, 0x00, 0x00, 0xFF, 0x14},
    {0x00, 0x00, 0xFF, 0x00, 0xFF},
    {0x00, 0x00, 0x1F, 0x10, 0x17},
    {0x00, 0x00, 0xFC, 0x04, 0xF4},
    {0x14, 0x14, 0x17, 0x10, 0x17},
    {0x14, 0x14, 0xF4, 0x04, 0xF4},
    {0x00, 0x00, 0xFF, 0x00, 0xF7},
    {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x14, 0x14, 0xF7, 0x00, 0xF7},
    {0x14, 0x14, 0x14, 0x17, 0x14},
    {0x10, 0x10, 0x1F, 0x10, 0x1F},
    {0x14, 0x14, 0x14, 0xF4, 0x14},
    {0x10, 0x10, 0xF0, 0x10, 0xF0},
    {0x00, 0x00, 0x1F, 0x10, 0x1F},
    {0x00, 0x00, 0x00, 0x1F, 0x14},
    {0x00, 0x00, 0x00, 0xFC, 0x14},
    {0x00, 0x00, 0xF0, 0x10, 0xF0},
    {0x10, 0x10, 0xFF, 0x10, 0xFF},
    {0x14, 0x14, 0x14, 0xFF, 0x14},
    {0x10, 0x10, 0x10, 0x1F, 0x00},
    {0x00, 0x00, 0x00, 0xF0, 0x10},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xF0, 0xF0, 0xF0, 0xF0, 0xF0},
    {0xFF, 0xFF, 0xFF, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xFF, 0xFF},
    {0x0F, 0x0F, 0x0F, 0x0F, 0x0F},
    {0x38, 0x44, 0x44, 0x38, 0x44},
    {0x7C, 0x2A, 0x2A, 0x3E, 0x14},
    {0x7E, 0x02, 0x02, 0x06, 0x06},
    {0x02, 0x7E, 0x02, 0x7E, 0x02},
    {0x63, 0x55, 0x49, 0x41, 0x63},
    {0x38, 0x44, 0x44, 0x3C, 0x04},
    {0x40, 0x7E, 0x20, 0x1E, 0x20},
    {0x06, 0x02, 0x7E, 0x02, 0x02},
    {0x99, 0xA5, 0xE7, 0xA5, 0x99},
    {0x1C, 0x2A, 0x49, 0x2A, 0x1C},
    {0x4C, 0x72, 0x01, 0x72, 0x4C},
    {0x30, 0x4A, 0x4D, 0x4D, 0x30},
    {0x30, 0x48, 0x78, 0x48, 0x30},
    {0xBC, 0x62, 0x5A, 0x46, 0x3D},
    {0x3E, 0x49, 0x49, 0x49, 0x00},
    {0x7E, 0x01, 0x01, 0x01, 0x7E},
    {0x2A, 0x2A, 0x2A, 0x2A, 0x2A},
    {0x44, 0x44, 0x5F, 0x44, 0x44},
    {0x40, 0x51, 0x4A, 0x44, 0x40},
    {0x40, 0x44, 0x4A, 0x51, 0x40},
    {0x00, 0x00, 0xFF, 0x01, 0x03},
    {0xE0, 0x80, 0xFF, 0x00, 0x00},
    {0x08, 0x08, 0x6B, 0x6B, 0x08},
    {0x36, 0x12, 0x36, 0x24, 0x36},
    {0x06, 0x0F, 0x09, 0x0F, 0x06},
    {0x00, 0x00, 0x18, 0x18, 0x00},
    {0x00, 0x00, 0x10, 0x10, 0x00},
    {0x30, 0x40, 0xFF, 0x01, 0x01},
    {0x00, 0x1F, 0x01, 0x01, 0x1E},
    {0x00, 0x19, 0x1D, 0x17, 0x12},
    {0x00, 0x3C, 0x3C, 0x3C, 0x3C},
    {0x00, 0x00, 0x00, 0x00, 0x00}
};

void delay(int x) {
    int j = 0;
    for (j = 0; j < x; j++);
}

// Sends a "command" to the LCD

void sendcomand(char input) {
    int j = 0;
    PORTC &= ~A0;
    PORTB &= ~CS;
    SSP1BUF = input;
    for (j = 0; j < 1; j++);
    PORTB |= CS;
}

// Sends "data" to the LCD

void senddata(char input) {
    int j = 0;
    PORTC |= A0;
    PORTB &= ~CS;
    SSP1BUF = input;
    for (j = 0; j < 1; j++);
    PORTB |= CS;
}

// Sets individual pixels at coords x and y  to the given color

void SetPix(char x, char y, int color) {
    char Hig = 0;
    char Low = color & 0x00ff;
    color >>= 8;
    Hig = color;

    sendcomand(ST7735_CASET); // Column addr set
    senddata(0x00);
    senddata(x); // XSTART
    senddata(0x00);
    senddata(x + 1); // XEND

    sendcomand(ST7735_RASET); // Row addr set
    senddata(0x00);
    senddata(y); // YSTART
    senddata(0x00);
    senddata(y + 1); // YEND

    sendcomand(ST7735_RAMWR);

    senddata(Low);
    senddata(Hig);
}



// Returns a custom color in RGB values

int customColor(int r, int g, int b) {
    return RGB565(r, g, b);
}

// Draw a border in a box shape at upper left x, y coords
// Box will be height tall, width wide, have a border of the given size and color
// of the given color

void drawBox(char x, char y, char height, char width, int border, int color) {
    border -= 1;
    drawBoxFill(x, y, border, width, color);
    drawBoxFill(x + width - border, y, height, border, color);
    drawBoxFill(x, y + height - border, border, width, color);
    drawBoxFill(x, y, height, border, color);
}

// Draws a box and fills it in with the given color

void drawBoxFill(char x, char y, char height, char width, int color) {
    char Hig = 0;
    char Low = color & 0x00ff;
    int i = 0;
    int j = 0;
    Hig = color >> 8;

    sendcomand(ST7735_CASET); // Column addr set
    senddata(0x00);
    senddata(x); // XSTART
    senddata(0x00);
    senddata(x + width); // XEND

    sendcomand(ST7735_RASET); // Row addr set
    senddata(0x00);
    senddata(y); // YSTART
    senddata(0x00);
    senddata(y + height); // YEND

    sendcomand(ST7735_RAMWR);

    for (j = 0; j <= width; j++) {
        for (i = 0; i <= height; i++) {
            senddata(Low);
            senddata(Hig);
        }
    }
}

// Cleans the entire screen as a certain color

void clean(int color) {

    char Hig = 0;
    int x = 0;
    int y = 0;
    char Low = color & 0x00ff;
    color >>= 8;
    Hig = color;

    sendcomand(ST7735_CASET); // Column addr set
    senddata(0x00);
    senddata(0); // XSTART
    senddata(0x00);
    senddata(V); // XEND

    sendcomand(ST7735_RASET); // Row addr set
    senddata(0x00);
    senddata(0); // YSTART
    senddata(0x00);
    senddata(H); // YEND

    sendcomand(ST7735_RAMWR);

    for (x = 0; x < V; x++) {
        for (y = 0; y < H; y++) {
            senddata(Low);
            senddata(Hig);
        }
    }
}

// Initializes the LCD screen by sending a bajillion commands

void initLCD() {
    PORTB &= ~CS;
    delay(0);
    //    PORTC |= RE;
    //    delay(100);
    //    PORTC &= ~RE;
    //    delay(100);
    //    PORTC |= RE;
    delay(10000);
    sendcomand(ST7735_SWRESET); //  1: Software reset, 0 args, w/delay 150ms 0x01
    delay(1000);
    sendcomand(ST7735_SLPOUT); //  2: Out of sleep mode, 0 args, w/delay 500ms 0x11
    delay(10000);
    sendcomand(ST7735_FRMCTR1); //  3: Frame rate ctrl - normal mode) 3 args: 0xb1
    senddata(0x01);
    senddata(0x2C);
    senddata(0x2D); //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    sendcomand(ST7735_FRMCTR2); //  4: Frame rate control - idle mode) 3 args: 0xb2
    senddata(0x01);
    senddata(0x2C);
    senddata(0x2D); //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    sendcomand(ST7735_FRMCTR3); //  5: Frame rate ctrl - partial mode) 6 args: 0xb3
    senddata(0x01);
    senddata(0x2C);
    senddata(0x2D); //     Dot inversion mode
    senddata(0x01);
    senddata(0x2C);
    senddata(0x2D); //     Line inversion mode
    sendcomand(ST7735_INVCTR); //  6: Display inversion ctrl) 1 arg) no delay: 0xb4
    senddata(0x07); //     No inversion
    sendcomand(ST7735_PWCTR1); //  7: Power control) 3 args) no delay: 0xc0
    senddata(0xA2);
    senddata(0x02); //     -4.6V
    senddata(0x84); //     AUTO mode
    sendcomand(ST7735_PWCTR2); //  8: Power control) 1 arg) no delay: 0xc1
    senddata(0xC5); //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    sendcomand(ST7735_PWCTR3); //  9: Power control) 2 args) no delay: 0xc2
    senddata(0x0A); //     Opamp current small
    senddata(0x00); //     Boost frequency
    sendcomand(ST7735_PWCTR4); // 10: Power control) 2 args) no delay:
    senddata(0x8A); //     BCLK/2) Opamp current small & Medium low
    senddata(0x2A);
    sendcomand(ST7735_PWCTR5); // 11: Power control) 2 args) no delay:
    senddata(0x8A);
    senddata(0xEE);
    sendcomand(ST7735_VMCTR1); // 12: Power control) 1 arg) no delay:
    senddata(0x0E);
    sendcomand(ST7735_INVOFF); // 13: Don't invert display) no args) no delay 0x20
    sendcomand(ST7735_MADCTL); // 14: Memory access control (directions)) 1 arg:
    senddata(0xC8); //     row addr/col addr); bottom to top refresh
    sendcomand(ST7735_COLMOD); // 15: set color mode); 1 arg); no delay:
    senddata(0x05); //     16-bit color
    sendcomand(ST7735_GMCTRP1); // Gamma correction
    senddata(0x0f);
    senddata(0x1a);
    senddata(0x0f);
    senddata(0x18);
    senddata(0x2f);
    senddata(0x28);
    senddata(0x20);
    senddata(0x22);
    senddata(0x1f);
    senddata(0x1b);
    senddata(0x23);
    senddata(0x37);
    senddata(0x00);
    senddata(0x07);
    senddata(0x02);
    senddata(0x10);
    sendcomand(ST7735_GMCTRN1);
    senddata(0x0f);
    senddata(0x1b);
    senddata(0x0f);
    senddata(0x17);
    senddata(0x33);
    senddata(0x2c);
    senddata(0x29);
    senddata(0x2e);
    senddata(0x30);
    senddata(0x30);
    senddata(0x39);
    senddata(0x3f);
    senddata(0x00);
    senddata(0x07);
    senddata(0x03);
    senddata(0x10);

    sendcomand(ST7735_NORON); //  3: Normal display on, no args, w/delay 10ms 0x13
    delay(100);
    sendcomand(ST7735_DISPON); //  4: Main screen turn on, no args w/delay 100ms 0x29
    delay(1000);
}

// Looks up the pixels to set when writing ascii character

void ASCII(char x, char y, int color, int background, char letter, char size) {
    char data;
    char q = 0;
    char z = 0;
    char d = 0;
    char b = 0;

    for (q = 0; q < 5; q++) {
        data = font[letter][q];
        for (z = 0; z < 8 * size; z++) {
            if ((data & 1) != 0) {
                for (d = 0; d < size; d++) {
                    for (b = 0; b < size; b++) {
                        SetPix(x + (q * size) + d, y + (z * size) + b, color);
                    }
                }
            } else {
                for (d = 0; d < size; d++) {
                    for (b = 0; b < size; b++) {
                        SetPix(x + (q * size) + d, y + (z * size) + b, background);
                    }
                }
            }
            data >>= 1;
        }
    }
}

// Prints a string that is sent in " " marks
// It will start at coords x and y, with background colors and text colors
// Font size can be specified.

// Each line with size 1 should be around 8 pixels apart, font will wrap around
// the screen if it's too long

void prints(char x, char y, int color, int background, const char messageOld[], char size) {
    const far rom char* message = (const far rom char*) messageOld;
    while (*message) {
        ASCII(x, y, color, background, *message++, size);
        x += 6 * size;
        if (x > 120) {
            x = 0;
            y += 8 * size;
        }
    }
}

// Prints a string saved in ram already
// It will start at coords x and y, with background colors and text colors
// Font size can be specified.

// Each line with size 1 should be around 8 pixels apart, font will wrap around
// the screen if it's too long

void printrs(char x, char y, int color, int background, char* message, char size) {
    while (*message) {
        ASCII(x, y, color, background, *message++, size);
        x += 6 * size;
        if (x > 120) {
            x = 0;
            y += 8 * size;
        }
    }
}

// Prints an INT value to the screen

void integerprint(char x, char y, int color, int background, int integer, char size) {
    unsigned char tenthousands = 0;
    unsigned char thousands = 0;
    unsigned char hundreds = 0;
    unsigned char tens = 0;
    unsigned char ones = 0;
    if (integer >= 10000) {
        tenthousands = integer / 10000;
        ASCII(x, y, color, background, tenthousands + 48, size);
    }
    if (integer >= 1000) {
        thousands = ((integer - tenthousands * 10000)) / 1000;
        x += 6;
        ASCII(x, y, color, background, thousands + 48, size);
    }
    if (integer >= 100) {
        hundreds = (((integer - tenthousands * 10000) - thousands * 1000)) / 100;
        x += 6;
        ASCII(x, y, color, background, hundreds + 48, size);
    }
    if (integer >= 10) {
        tens = (integer % 100) / 10;
        x += 6;
        ASCII(x, y, color, background, tens + 48, size);
    }
    ones = integer % 10;
    x += 6;
    ASCII(x, y, color, background, ones + 48, size);
}

// Prints menu for operating system functions
void printMenu(char* select, int background, int box, int boxBorder, int text, int size) {
    int i;
    // Beep off
    TRISBbits.RB5 = 1;
    // Change background
    clean(background);

    // Draw Box
    drawBoxFill(0, 0, 20, V - 1, box);
    drawBox(0, 0, 20, V - 1, 2, boxBorder);

    // Write Select Options
    for (i = 0; i < size; i++) {
        // Do I have to deference this pointer?
        printrs(35, 20 * i + 40, text, background, select[i], 1);
    }
}

// Prints cursor and processes the selection based off of the index i
// Top of List [ 0 1 2 3] Bottom of List
int processPrintCursor(GlobalState* globalData, int size, int background, int text) {
    int i = 0;
    prints(25, 40, text, background, ">", 1);
    globalData->keyPress = -1;
    // Continue scanning the keypad until the user hits "D" for enter
    while (globalData->keyPress != 0x0D) {
        // Check keystroke
        keypad(globalData);
        if (globalData->keyFlag && !globalData->displayedKey) {
            globalData->keyFlag = FALSE;
            globalData->displayedKey = TRUE;
            // Change index i based off of whether user moves up or down
            switch (globalData->keyPress) {
                 // Move up = Press 2
                case 0x02:
                    // Clear original cursor
                    prints(25, 20 * i + 40, text, background, " ", 1);

                    // Find new position of cursor
                    i = (size + i - 1) % size;

                    // Print cursor in new position
                    prints(25, 20 * i + 40, text, background, ">", 1);
                    break;
                // Move down = Press 8
                case 0x08:
                    // Clear original cursor
                    prints(25, 20 * i + 40, text, background, " ", 1);

                    // Find new position of cursor
                    i = (i + 1) % size;

                    // Print cursor in new position
                    prints(25, 20 * i + 40, text, background, ">", 1);
                    break;
                // Hit back button "B"
                case 0x0B:
                    i = 0xFF;
                    // Break out of while loop - hitting B takes priority
                    globalData->keyPress = 0x0D;
                    break;
            }
        }
    }
    // Return index to indicate cursor position
    return i;
}

/* The following functions are print functions for the operating system of the
 * device.
 */

// Prints the main menu
void printMainMenu(GlobalState* globalData) {
    // LCD menu
    // Main Menu Array
    const rom char *mainMenu[3] = {"Single", "Multiplayer", "Build Card"};
    printMenu(mainMenu, BLUE, CYAN, WHITE, WHITE, 3);
    prints(35, 7, WHITE, CYAN, "Main Menu", 1);

    /*
    clean(BLUE);
    drawBoxFill(0, 0, 20, V - 1, CYAN);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    
    prints(35, globalData->mainMenuSpots[0], WHITE, BLUE, "Single Player", 1);
    prints(35, globalData->mainMenuSpots[1], WHITE, BLUE, "Multiplayer", 1);
    prints(35, globalData->mainMenuSpots[2], WHITE, BLUE, "Build Cards", 1);
     */
    prints(0, H - 8, WHITE, BLUE, "2-UP,8-DOWN,D-ENTER", 1);
}

// Print menu for selecting available games.
void printSelectGame(GlobalState *globalData) {
    char* selectGame[4] = {"Duel Game", "Clue", "Empty", "Empty"};

    printMenu(selectGame, GREEN, YELLOW, BLACK, BLACK, 4);
    prints(35, 7, BLACK, YELLOW, "Available Games:", 1);
    prints(0, H - 8, BLACK, YELLOW, "2-UP,8-DOWN,D-ENTER", 1);
    globalData->game = processPrintCursor(globalData, 4, YELLOW, BLACK);
}

// Print blue screen of death
void printBSOD() {
    // Beep off
    TRISBbits.RB5 = 1;

    drawBoxFill(30, 39, 8, 60, GRAY);
    prints(35, 40, BLUE, GRAY, " Windows ", 1);

    prints(0, 50, WHITE, BLUE, "An error has occurred,", 1);
    prints(0, 58, WHITE, BLUE, "To continue:", 1);
    prints(0, 74, WHITE, BLUE, "Remove the battery or", 1);
    prints(0, 82, WHITE, BLUE, "power supply.", 1);
    prints(0, 98, WHITE, BLUE, "Error: 0E : BFF9B3D4", 1);

    prints(10, 114, WHITE, BLUE, "Dumping memory...", 1);
    while (1);
}

// Draws the build card menu, begins an inventory read

void printBuildCard1(GlobalState *globalData) {
    // first page of build card
    clean(RED);
    prints(0, 0, BLACK, RED, "It looks like you want to build a card.", 1);
    prints(0, 16, BLACK, RED, "Available cards:", 1);

    // Tell system we need to do an inventory command
    globalData->getInventory = TRUE;
}

/*
 * Select Menus for the operating system
 * Menu goes back to previous menu after being done with a certain process
 */

void mainMenu(GlobalState* globalData) {
    // If returning from a previous menu, re-print main menu
    if(globalData->goBack) {
        printMainMenu(globalData);
        globalData->mode = processPrintCursor(globalData, 3, BLUE, WHITE);
        globalData->goBack = FALSE;
    }
    // Keep checking cursor until they stop hitting back button
    while(globalData->mode == 0xFF) {
        globalData->mode = processPrintCursor(globalData, 3, BLUE, WHITE);
    }
    // Switch menus based off of selection
    switch (globalData->mode) {
        // Single Player
        case 0:
            selectGameMenu(globalData);
            break;
        // Multiplayer
        case 1:
            selectGameMenu(globalData);
            break;
        // Build Card
        case 2:
            printBuildCard1(globalData);
            break;
    }
}

// Select game to play based off of the mode (single/multiplayer)
void selectGameMenu(GlobalState* globalData) {
    // Print select game menu
    printSelectGame(globalData);
    globalData->game = processPrintCursor(globalData, 4, YELLOW, BLACK);

    // Run chosen game based off of the multiplayer/single-player mode
    switch (globalData->game) {
        // Game Slot 1
        case 0:
            switch(globalData->mode) {
                case 0:
                    multiPlayer(globalData);
                    break;
                case 1:
                    singlePlayer(globalData);
                    break;
            }
            break;
        // Game Slot 2
        case 1:
            clean(BLACK);
            prints(0, 35, WHITE, BLACK, "In Progress",1);
            break;
        // Game Slot 3
        case 2:
            printBSOD();
            break;
        // Game Slot 4
        case 3:
            clean(BLACK);
            prints(0, 35, WHITE, BLACK, "In Progress",1);
            break;
        // Hit Back Button
        case 0xFF:
            globalData->goBack = TRUE;
            break;
    }
    // Return to main menu after finished.
    globalData->goBack = TRUE;
}

// Depreciated

/*
// Visuals and navigation for operating system menus
void processDisplay(GlobalState* globalData) {
    // Different controls for each page being displayed
    switch (globalData->displayPage) {
        // Main menu
        case 0:
            switch (globalData->keyPress) {
                // Press 2 to move up
                case 0x02:
                    // Moves the cursor up 1 space.  Loops around
                    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, BLUE, " ", 1);
                    globalData->cursorPos = ((3 + globalData->cursorPos) - 1) % 3;
                    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, BLUE, ">", 1);
                    break;
                // Press 8 to move down
                case 0x08:
                    // Moves the cursor down 1 space.  Loops around
                    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, BLUE, " ", 1);
                    globalData->cursorPos = (globalData->cursorPos + 1) % 3;
                    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, BLUE, ">", 1);
                    break;
                // D is the enter key. Figure out the next page
                case 0x0D:
                    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, BLUE, ">>>", 1);

                    // Cursor position determines next page. Add 1 to remove main menu "case 0" from the
                    // list of options when navigating out of the main menu
                    nextPage(globalData, globalData->cursorPos + 1);
                    break;
                case 0xFF: // Debug BSOD
                    nextPage(globalData, 255);
                default:
                    break;
            }
            break;
        // Singleplayer
        case 1:
            switch (globalData->keyPress) {
                // B to go back
                case 0x0B:
                    nextPage(globalData, 0);
                    break;
                default:
                    break;
            }
            break;
        // Multiplayer
        case 2:
            switch (globalData->keyPress) {
                // B to go back
                case 0x0B:
                    nextPage(globalData, 0);
                    break;
                default:
                    break;
            }
            break;
        // build cards
        case 3:
            switch (globalData->keyPress) {
                // B to go back
                case 0x0B:
                    nextPage(globalData, 0);
                    break;
                default:
                    break;
            }
            break;
        default:
            printBSOD();
    }
}

void nextPage(GlobalState* globalData, int cursorPos) {
    // Beep off
    TRISBbits.RB5 = 1;

    switch (cursorPos) {
            // Send a 0 to go to main menu
        case 0:
            globalData->displayPage = 0;
            printMainMenu(globalData);
            break;
            // Getting a position of 1 does singleplayer
        case 1:
            globalData->displayPage = 1;
            // Print singleplayer menu
            globalData->displayPage = 1;
            singlePlayer(globalData);
            break;
            // Getting a position of 2 does multiplayer
        case 2:
            globalData->displayPage = 2;
            // Print multiplayer menu
            clean(GREEN);
            globalData->xbeeFlag = TRUE;
            prints(0, 0, BLACK, WHITE, "Nothing here. Press B to go back.", 1);
            break;
            // Getting a position of 3 does build cards
        case 3:
            globalData->displayPage = 3;
            // Print build cards menu
            printBuildCard1(globalData);
            break;
            // Error
        default:
            // BSOD
            clean(BLUE);
            globalData->keyFlag = TRUE;
            globalData->displayedKey = FALSE;
            globalData->displayPage = 255;
    }
}

*/
