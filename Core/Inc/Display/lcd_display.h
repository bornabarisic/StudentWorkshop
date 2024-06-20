#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H

/* ------------------------------------------------------------------------------------*/
/* Public function declarations -------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

void InitializeLCD(void);
void LCDWriteData(int row, int col, char *pdata);
void LCDClearScreen(void);

#endif /* __LCD_DISPLAY_H */
