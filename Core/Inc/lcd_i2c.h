#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

int InitializeLCDI2C(void);
int TransmitI2CDataToLCD(char *pdata);

#endif /* INC_LCD_I2C_H_*/
