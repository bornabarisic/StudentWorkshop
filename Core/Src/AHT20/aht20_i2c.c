
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "aht20_i2c.h"
#include "aht20_driver.h"

#include "stm32F4xx_hal.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define I2C_SLAVE_ADDRESS_AHT20	( 0x38 )
#define I2C_TRANSMIT_TIMEOUT	( 100U )
#define I2C_PORT_AHT20_SCL		( GPIOA )
#define I2C_PORT_AHT20_SDA		( GPIOC )
#define I2C_SCL_PIN_AHT20		( GPIO_PIN_8 )
#define I2C_SDA_PIN_AHT20		( GPIO_PIN_9 )

/* ----------------------------------------------------------------------------------- */
/* Static variables ------------------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static I2C_HandleTypeDef i2chandle2;

/* ----------------------------------------------------------------------------------- */
/* Static function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static void InitializeAHT20I2CGPIOPins(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//   PC9     ------> I2C3_SDA
	//   PA8     ------> I2C3_SCL

	GPIO_InitTypeDef gpio;

	gpio.Pin 		= I2C_SDA_PIN_AHT20;
	gpio.Pull 		= GPIO_NOPULL;
	gpio.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Mode 		= GPIO_MODE_AF_OD;
	gpio.Alternate 	= GPIO_AF4_I2C3;

	HAL_GPIO_Init(I2C_PORT_AHT20_SDA, &gpio);

	gpio.Pin 		= I2C_SCL_PIN_AHT20;
	gpio.Pull 		= GPIO_NOPULL;
	gpio.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Mode 		= GPIO_MODE_AF_OD;
	gpio.Alternate 	= GPIO_AF4_I2C3;

	HAL_GPIO_Init(I2C_PORT_AHT20_SCL, &gpio);

	__HAL_RCC_I2C3_CLK_ENABLE();
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

int InitializeAHT20I2C(void)
{
	InitializeAHT20I2CGPIOPins();

	__I2C3_CLK_ENABLE();

	i2chandle2.Mode 				= HAL_I2C_MODE_MASTER;
	i2chandle2.Instance 			= I2C3;
	i2chandle2.Init.ClockSpeed 		= 100000;
	i2chandle2.Init.DutyCycle 		= I2C_DUTYCYCLE_2;
	i2chandle2.Init.OwnAddress1 	= 0x00;
	i2chandle2.Init.OwnAddress2		= 0x00;
	i2chandle2.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	i2chandle2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2chandle2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2chandle2.Init.NoStretchMode 	= I2C_NOSTRETCH_ENABLE;

	if (HAL_I2C_Init(&i2chandle2) != HAL_OK)
	{
		return AHT20_ERROR_I2C_NOT_INITALIZED;
	}

	return AHT20_OK;
}

int TransmitI2CDataToAHT20(uint8_t *pdata, uint8_t length)
{
	if (HAL_I2C_Master_Transmit(&i2chandle2,
								(I2C_SLAVE_ADDRESS_AHT20 << 1),
								pdata,
								length,
								I2C_TRANSMIT_TIMEOUT) != HAL_OK)
	{
		return AHT20_ERROR_TRASMIT_ERROR;
	}

	return AHT20_OK;
}

int ReceiveI2CDataFromAHT20(uint8_t *pdata, uint8_t length)
{
	if (HAL_I2C_Master_Receive(&i2chandle2,
								((I2C_SLAVE_ADDRESS_AHT20 << 1) | 0x01),
								pdata,
								length,
								I2C_TRANSMIT_TIMEOUT) != HAL_OK)
	{
		return AHT20_ERROR_TRASMIT_ERROR;
	}

	return AHT20_OK;
}
