#include "lcd.h"
#include <avr/io.h>
int main(void)
{
    lcdInit();
    lcdGotoXY(0, 0);
    lcdDataWrite('H');
    lcdDataWrite('e');
    lcdDataWrite('l');
    lcdDataWrite('l');
    lcdDataWrite('o');
    lcdGotoXY(3, 1);
    lcdPrintData("MCU World !!", 12); // "MCU World !!" 문자열 출력
    while (1)
    {
    }
}
