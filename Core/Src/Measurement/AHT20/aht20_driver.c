/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include <stdbool.h>

#include "stm32f4xx_hal.h"

#include "aht20_driver.h"
#include "aht20_i2c.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------------- */
/* Typedef --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

typedef struct aht20_measurements_t
{
	float humidity;
	float temperature;
} aht20_measurements_t;

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static aht20_measurements_t measurements = {0};

/* ----------------------------------------------------------------------------------- */
/* Private function declarations ----------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static void AHT20Initialize(void);
static int AHT20_sensorInit(void);
static bool AHT20_checkCalibration(void);
static void AHT20_triggerMeasurement(void);
static float AHT20GetTemperatureValue(void);
static float AHT20GetHumidityValue(void);

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static void AHT20Initialize(void)
{
	int aht20_state = AHT20_OK;

	HAL_Delay(20);

	aht20_state = InitializeAHT20I2C();

//	ASSERT(aht20_state != AHT20_OK);

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

	aht20_state = TransmitI2CDataToAHT20(reg, 3);

	HAL_Delay(10); // Wait 10ms for initialization

	return aht20_state;
}

static bool AHT20_checkCalibration(void)
{
    uint8_t statusReg = 0x71;
    uint8_t cal; // Result byte

    TransmitI2CDataToAHT20(&statusReg, 1);

    ReceiveI2CDataFromAHT20(&cal, 1);

    return ((cal >> 3) & 1) == 1; // Check 4th bit is equal to 1
}

static void AHT20_triggerMeasurement(void)
{
	int aht20_state = AHT20_OK;

    while(AHT20_checkCalibration() == false)
    {
    	aht20_state = AHT20_sensorInit();
    }

    uint8_t measureCommand[3] = {0xAC, 0x33, 0x00};
    uint8_t status; // result status
    uint8_t data[6]; // data array

    TransmitI2CDataToAHT20(measureCommand, 3);
    HAL_Delay(80); // Wait for measurement

    ReceiveI2CDataFromAHT20(&status, 1); // Read status byte

    // Wait for status to be ready before continuing
    while (((status >> 7) & 1) != 0)
    {
        ReceiveI2CDataFromAHT20(&status, 1);
    }

    aht20_state = ReceiveI2CDataFromAHT20(data, 6); // Read all data

//    ASSERT(aht20_state != AHT20_OK);

    // Convert data for reading and store in variable
    uint32_t humidity = data[1];
    humidity <<= 8;
    humidity |= data[2];
    humidity <<= 4;
    humidity |= data[3] >> 4;

    measurements.humidity = ((float)humidity * 100.0F) / 1048576.0F;

    uint32_t temp = data[3] & 0x0F;
    temp <<= 8;
    temp |= data[4];
    temp <<= 8;
    temp |= data[5];

    measurements.temperature = ((float)temp * 200.0F / 1048576.0F) - 50.0F;
}

static float AHT20GetTemperatureValue(void)
{
	AHT20_triggerMeasurement();

	return measurements.temperature;
}

static float AHT20GetHumidityValue(void)
{
	AHT20_triggerMeasurement();

	return measurements.humidity;
}

/* ------------------------------------------------------------------------------------*/
/* Public variables -------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

static measurements_interface_t i2c_interface =
{
	.init_measurement_handler 	= AHT20Initialize,
	.get_temp_handler			= AHT20GetTemperatureValue,
	.get_hum_handler			= AHT20GetHumidityValue,
};

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

measurements_interface_t *GetI2CInterface(void)
{
	return &i2c_interface;
}
