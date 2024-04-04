
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "aht20_driver.h"
#include "aht20_i2c.h"

#include "stm32f4xx_hal.h"
#include <stdbool.h>

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------- */
/* Typedef --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------- */
/* Private function declarations ----------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static void InitializeAHT20(void);
static int AHT20_sensorInit(void);
static bool AHT20_checkCalibration(void);
static void AHT20_triggerMeasurement(void);
static int GetTemperatureValue(void);
static int GetHumidityValue(void);

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static AHT20_t AHT20_interface =
{
	.init			= InitializeAHT20,
	.get_temp		= GetTemperatureValue,
	.get_humidity 	= GetHumidityValue,
	.temp_val		= 0,
	.hum_val		= 0,
};

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static void InitializeAHT20(void)
{
	int aht20_state = AHT20_OK;

	HAL_Delay(20);

	aht20_state = InitializeAHT20I2C();
	//	ASSERT(ahr20_state != AHT20_OK);

	HAL_Delay(40);

	/* Initialize the sensor until calibration is successful */
	while(AHT20_checkCalibration() == false)
	{
		aht20_state = AHT20_sensorInit();
	}

	//	ASSERT(aht20_state != AHT20_OK);

}

static int AHT20_sensorInit(void)
{
	/* reg[3] holds the initialization sequence */
	int aht20_state = AHT20_OK;

	uint8_t reg[3] = {0xBE, 0x08, 0x00};
	aht20_state = TransmitI2CDataToAHT20(&reg, 3);
	HAL_Delay(10); // Wait 10ms for initialization

	return aht20_state;
}

static bool AHT20_checkCalibration(void)
{
    uint8_t statusReg = 0x71;
    uint8_t cal; // Result byte
    TransmitI2CDataToAHT20(&statusReg, 1);
    ReceiveI2CDataFromAHT20(&cal, 1);

//  return ((cal >> 3) & 1) == 1; // Check 4th bit is equal to 1 - Ovo ima smisla al nije tako u primjeru
    return ((cal >> 4) & 1) == 1; // Check 4th bit is equal to 1
}

void AHT20_triggerMeasurement(void)
{
	int aht20_state = AHT20_OK;

    while(AHT20_checkCalibration() == false) {
    	aht20_state = AHT20_sensorInit();
    }

    uint8_t measureCommand[3] = {0xAC, 0x33, 0x00};
    uint8_t status; // result status
    uint8_t data[6]; // data array

    TransmitI2CDataToAHT20(&measureCommand, 3);
    HAL_Delay(80); // Wait for measurement

    ReceiveI2CDataFromAHT20(&status, 1); // Read status byte

    // Wait for status to be ready before continuing
//  while (((status >> 7) & 1) != 0) {		Ovo ima smisla al nije tako u primjeru
    while (((status >> 8) & 1) != 0) {
        //printf("Waiting for read to complete\n");
        ReceiveI2CDataFromAHT20(&status, 1);
    }

    aht20_state = ReceiveI2CDataFromAHT20(&data, 6); // Read all data
    //	ASSERT(aht20_state != AHT20_OK);

    // Convert data for reading and store in variable

    uint32_t humidity = data[1];
    humidity <<= 8;
    humidity |= data[2];
    humidity <<= 4;
    humidity |= data[3] >> 4;
    AHT20_interface.hum_val = ((float)humidity * 100) / 1048576;

    uint32_t temp = data[3] & 0x0F;
    temp <<= 8;
    temp |= data[4];
    temp <<= 8;
    temp |= data[5];
    AHT20_interface.temp_val = ((float)temp * 200 / 1048576) - 50;
}

static int GetTemperatureValue(void)
{
	AHT20_triggerMeasurement();

	return AHT20_interface.temp_val;
}

static int GetHumidityValue(void)
{
	AHT20_triggerMeasurement();

	return AHT20_interface.hum_val;
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

AHT20_t *GetAHT20Interface(void)
{
	return &AHT20_interface;
}
