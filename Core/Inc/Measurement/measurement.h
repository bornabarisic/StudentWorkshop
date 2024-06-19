#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

/* ------------------------------------------------------------------------------------*/
/* Typedef ----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

typedef enum measurement_source_t
{
	MEASUREMENT_SRC_ADC,
	MEASUREMENT_SRC_I2C,
} measurement_source_t;

/* ------------------------------------------------------------------------------------*/
/* Public function declarations -------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

void InitializeMeasurements(measurement_source_t src);
float GetTemperature(void);
float GetHumidity(void);

#endif /* __MEASUREMENT_H */
