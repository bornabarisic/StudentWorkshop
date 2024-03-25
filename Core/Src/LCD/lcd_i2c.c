
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "lcd_i2c.h"
#include "lcd_driver.h"

#include "stm32F4xx_hal.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define I2C_SLAVE_ADDRESS		( 0x7C )
#define I2C_MSG__SIZE			( 2U )
#define I2C_TRANSMIT_TIMEOUT	( 100U )
#define I2C_PORT				( GPIOB )
#define I2C_SCL_PIN				( GPIO_PIN_8 )
#define I2C_SDA_PIN				( GPIO_PIN_9 )

/* ----------------------------------------------------------------------------------- */
/* Static variables ------------------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static I2C_HandleTypeDef i2chandle;

/* ----------------------------------------------------------------------------------- */
/* Static function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static void InitializeI2CGPIOPins(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef gpio;

	gpio.Pin 		= I2C_SCL_PIN | I2C_SDA_PIN;
	gpio.Pull 		= GPIO_PULLUP;
	gpio.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Mode 		= GPIO_MODE_AF_PP;
	gpio.Alternate 	= GPIO_AF4_I2C1;

	HAL_GPIO_Init(I2C_PORT, &gpio);

	__HAL_RCC_I2C1_CLK_ENABLE();
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

int InitializeLCDI2C(void)
{
	InitializeI2CGPIOPins();

	__I2C1_CLK_ENABLE();

	i2chandle.Mode 					= HAL_I2C_MODE_MASTER;
	i2chandle.Instance 				= I2C1;
	i2chandle.Init.ClockSpeed 		= 100000;
	i2chandle.Init.DutyCycle 		= I2C_DUTYCYCLE_2;
	i2chandle.Init.OwnAddress1 		= 0x00;
	i2chandle.Init.OwnAddress2		= 0x00;
	i2chandle.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	i2chandle.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
	i2chandle.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
	i2chandle.Init.NoStretchMode 	= I2C_NOSTRETCH_ENABLE;

	if (HAL_I2C_Init(&i2chandle) != HAL_OK)
	{
		return LCD_ERROR_I2C_NOT_INITALIZED;
	}

	return LCD_OK;
}

int TransmitI2CDataToLCD(char *pdata)
{
	if (HAL_I2C_Master_Transmit(&i2chandle,
								I2C_SLAVE_ADDRESS,
								(uint8_t *)pdata,
								I2C_MSG__SIZE,
								I2C_TRANSMIT_TIMEOUT) != HAL_OK)
	{
		return LCD_ERROR_TRASMIT_ERROR;
	}

	return LCD_OK;
}
