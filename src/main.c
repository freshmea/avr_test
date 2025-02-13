#include "lcd.h"
#include <avr/boot.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// fuse bit
#define LFUSE boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS)
#define HFUSE boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS)
#define EFUSE boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS)

int main(void)
{
  char line1[17], line2[17]; // LCD 16자릿수 + null

  lcdInit();  // LCD 초기화
  lcdClear(); // LCD 화면 클리어

  // fuse 값을 LCD에 표시
  snprintf(line1, sizeof(line1), "LF:0x%02X HF:0x%02X", LFUSE, HFUSE);
  snprintf(line2, sizeof(line2), "EF:0x%02X", EFUSE);

  lcdGotoXY(0, 0); // 첫 번째 줄, 첫 번째 칸
  lcdPrint(line1);
  lcdGotoXY(0, 1); // 두 번째 줄, 첫 번째 칸
  lcdPrint(line2);

  while (1)
  {
    // ...주기적 작업 또는 무한 루프...
    _delay_ms(1000);
  }

  return 0;
}
