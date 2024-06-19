#ifndef __MEASUREMENTS_INTERFACE_H
#define __MEASUREMENTS_INTERFACE_H

/**
 * @brief 	Function pointer to initialize measurement peripheral what
 * 			ever that might be. In this case it can be ADC or I2C.
 */
typedef void (*fptr_init_measurement)(void);

/**
 * @brief 	Function pointer to acquire temperature value from the
 * 			initialized peripheral. In this case it can be ADC or I2C.
 */
typedef float (*fptr_get_temp)(void);

/**
 * @brief 	Function pointer to acquire humidity value from the
 * 			initialized peripheral. It can be only used with I2C.
 */
typedef float (*fptr_get_hum)(void);

/**
 * @brief 	Interface structure for managing measurement peripheral.
 * 			It can be used with ADC or I2C peripheral. This interface
 * 			is used to hold functions that will initialize and acquire
 * 			temperature from chosen peripheral.
 */
typedef struct measurements_interface_t
{
	fptr_init_measurement 	init_measurement_handler;
	fptr_get_temp 			get_temp_handler;
	fptr_get_hum			get_hum_handler;

} measurements_interface_t;

#endif /* __MEASUREMENTS_INTERFACE_H */
