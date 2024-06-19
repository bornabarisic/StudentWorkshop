#ifndef INC_AHT20_I2C_H_
#define INC_AHT20_I2C_H_

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

int InitializeAHT20I2C(void);
int TransmitI2CDataToAHT20(uint8_t *pdata, uint8_t length);
int ReceiveI2CDataFromAHT20(uint8_t *pdata, uint8_t length);

#endif /* INC_AHT20_I2C_H_*/
