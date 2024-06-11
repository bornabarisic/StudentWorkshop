
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include <string.h>
#include <stdio.h>

#include "aht20_interface.h"
#include "aht20_driver.h"
#include "log.h"


/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

void AHT20Init(void)
{
	AHT20_t *paht20 = GetAHT20Interface();

	ASSERT(paht20 == NULL);

	paht20->init();
}

int AHT20_GetTemp(void)
{
	AHT20_t *paht20 = GetAHT20Interface();

	ASSERT(paht20 == NULL);

	return paht20->get_temp();

}

int AHT20_GetHum(void)
{
	AHT20_t *paht20 = GetAHT20Interface();

	ASSERT(paht20 == NULL);

	return paht20->get_humidity();
}

