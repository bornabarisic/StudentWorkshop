#ifndef __LCD_INTERFACE_H
#define __LCD_INTERFACE_H

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

void LCDInit(void);
void LCDWriteData(int row, int col, char *pdata);
void LCDClearScreen(void);

#endif /* __LCD_INTERFACE_H */
