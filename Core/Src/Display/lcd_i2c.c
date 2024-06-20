
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "stm32F4xx_hal.h"

#include "lcd_driver.h"
#include "lcd_i2c.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define I2C_SLAVE_ADDRESS_LCD	( 0x7C )
#define I2C_TRANSMIT_TIMEOUT	( 100U ) 		/* Time set in ms */

#define I2C_PORT_LCD			( GPIOB )
#define I2C_SCL_PIN_LCD			( GPIO_PIN_8 )
#define I2C_SDA_PIN_LCD			( GPIO_PIN_9 )

/* ----------------------------------------------------------------------------------- */
/* Static variables ------------------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static I2C_HandleTypeDef i2chandle;

/* ----------------------------------------------------------------------------------- */
/* Static function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief 	This function is used to initialize GPIO pins for I2C peripheral used by
 * 			LCD display.
 */
static void InitializeLCDI2CGPIOPins(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef gpio;

	gpio.Pin 		= I2C_SCL_PIN_LCD | I2C_SDA_PIN_LCD;
	gpio.Pull 		= GPIO_NOPULL;
	gpio.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Mode 		= GPIO_MODE_AF_OD;
	gpio.Alternate 	= GPIO_AF4_I2C1;

	HAL_GPIO_Init(I2C_PORT_LCD, &gpio);

	__HAL_RCC_I2C1_CLK_ENABLE();
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief 	This function is used to initialize I2C peripheral used by LCD display.
 *
 * @return 	int LCD display error code
 */
int InitializeLCDI2C(void)
{
	InitializeLCDI2CGPIOPins();

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

/**
 * @brief 	This function is a wrapper function to send data to LCD display
 * 			using HAL function for I2C data transmit.
 *
 * @param 	pdata data send to display
 * @return 	int LCD display error code
 */
int TransmitI2CDataToLCD(char *pdata)
{
	if (HAL_I2C_Master_Transmit(&i2chandle,
								I2C_SLAVE_ADDRESS_LCD,
								(uint8_t *)pdata,
								I2C_MSG_SIZE,
								I2C_TRANSMIT_TIMEOUT) != HAL_OK)
	{
		return LCD_ERROR_TRASMIT_ERROR;
	}

	return LCD_OK;
}
