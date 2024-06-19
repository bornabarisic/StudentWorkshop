/* ------------------------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

#include "measurement.h"
#include "measurement_interface.h"
#include "ADC.h"
#include "aht20_driver.h"

/* ------------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

static measurements_interface_t *p_measurement_interface;

/* ------------------------------------------------------------------------------------*/
/* Public function definitions -------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

/**
 * @brief 	This is a wrapper function used to initialize measurement module
 */
void InitializeMeasurements(measurement_source_t src)
{
	switch(src)
	{
		case MEASUREMENT_SRC_ADC:
			p_measurement_interface = GetADCInterface();
		break;

		case MEASUREMENT_SRC_I2C:
			p_measurement_interface = GetI2CInterface();
		break;

		default:
		break;
	}

	if (p_measurement_interface->init_measurement_handler)
	{
		p_measurement_interface->init_measurement_handler();
	}
}

/**
 * @brief	This function is used to get the value of measured temperature
 */
float GetTemperature(void)
{
	float temperature = 0.0F;

	if (p_measurement_interface->get_temp_handler)
	{
		temperature = p_measurement_interface->get_temp_handler();
	}

	return temperature;
}

/**
 * @brief 	This function is used to get the value of humidity
 */
float GetHumidity(void)
{
	float humidity = 0.0F;

	if (p_measurement_interface->get_hum_handler)
	{
		humidity = p_measurement_interface->get_hum_handler();
	}

	return humidity;
}
