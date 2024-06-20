#ifndef __LCD_I2C_H
#define __LCD_I2C_H

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define I2C_MSG_SIZE	( 2U )

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

int InitializeLCDI2C(void);
int TransmitI2CDataToLCD(char *pdata);

#endif /* __LCD_I2C_H */
