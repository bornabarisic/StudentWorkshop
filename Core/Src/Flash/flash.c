/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
#include "flash.h"
#include "log.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
#define NUMBER_OF_ITEMS_TO_STORE_IN_FLASH	( 10U )
#define SIZE_OF_DATA_WRITTEN_TO_FLASH		( sizeof(int) )

/* Start @ of user Flash area */
#define FLASH_USER_START_ADDR   ( ADDR_FLASH_SECTOR_4 )

/* End @ of user Flash area */
#define FLASH_USER_END_ADDR     ( ADDR_FLASH_SECTOR_5 - 1 )

/* End @ of Flash area used for temperature logs */
#define FLASH_VALUES_END_ADDR	( ADDR_FLASH_SECTOR_4  +  NUMBER_OF_ITEMS_TO_STORE_IN_FLASH * sizeof(uint32_t) )

#define FIRST_SECTOR	( FLASH_SECTOR_4 )
#define NBOFSECTORS		( 1U )

/**
 * @brief 	This factor is used for storing decimal spaces from float variable
 * 			to integer so wen can save it into flash.
 *
 * @note	We want to save up to two decimal spaces and to achieve this float
 * 			variable needs to be multiplied by PRESERVATION_FACTOR and then
 * 			typecasted to uint32_t.
 *
 * @example
 * 		float 	input_val 			= 45.896843
 * 		int 	casted_input_value 	= (int)(input_val * PRESERVATION_FACTOR)
 * 									= 4589
 * 		Value 4589 is written to flash.
 * 		When reading from flash:
 * 		int 	read_input_val 	= 4589
 * 		float 	input_value 	= (float)read_input_val / PRESERVATION_FACTOR
 * 								= 45.89
 */
#define PRESERVATION_FACTOR 	( 100.0F )

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static uint32_t temperature_logs[NUMBER_OF_ITEMS_TO_STORE_IN_FLASH] = {0};

/* Variable used for Erase procedure */
static FLASH_EraseInitTypeDef EraseInitStruct;

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static int FlashReadAndAdd(uint32_t input_val);
static int FlashErase(void);
static int FlashWrite(void);
static int FlashVerify(void);

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief  Function for writing a value to flash
  * @param  val - Value to be written in flash
  * @retval 0 if successful
  */
void FlashWriteLog(float input_val)
{
	int flash_status = 0;

	int input_val_int = (int)(input_val * PRESERVATION_FACTOR);

	__disable_irq();

	/* Read previous logs from flash and append the new value */
	flash_status = FlashReadAndAdd(input_val_int);
	ASSERT(flash_status != 0);

	/* Unlock the Flash to enable the flash control register access */
	HAL_FLASH_Unlock();

	/* Erase the flash sector that will be used */
	flash_status = FlashErase();
	ASSERT(flash_status != 0);

	/* Write the logs to flash */
	flash_status = FlashWrite();
	ASSERT(flash_status != 0);

	/* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
	HAL_FLASH_Lock();

	__enable_irq();

	/* Verify that the data was written successfully */
	flash_status = FlashVerify();
	ASSERT(flash_status != 0);
}

/**
  * @brief  Reads the last 10 logs from flash
  * @param  None
  * @retval none
  */
void FlashReadLogs(void)
{
	uint32_t Address = FLASH_USER_START_ADDR;
	uint32_t data32 = 0;
	float data32_float = 0.0F;

	LOG_INFO("Prethodna mjerenje iznose:\n");

	while (Address < FLASH_VALUES_END_ADDR)
	{
		data32 = *(uint32_t *)Address;

		if ((data32 != 0) && (data32 != 0xFFFFFFFF))
		{
		    Address = Address + 4;
		    data32_float = (float)data32 / PRESERVATION_FACTOR;
		    LOG_INFO("%.2f\n", data32_float);
		}
		else
		{
			break;
		}
	}
}

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief  Reads previous logs from flash and stores them in the array along with the new value
  * @param  None
  * @retval 0 - successful
  */
static int FlashReadAndAdd(uint32_t input_val)
{
	int retval = 0;

	uint32_t data32 = 0;
	uint32_t index = 0;

	/* Check how many values are already written and fill the array with them */

	uint32_t Address = FLASH_USER_START_ADDR;

	while (Address < FLASH_VALUES_END_ADDR)
	{
		data32 = *(uint32_t *)Address;

		if (data32 != 0xFFFFFFFF)
		{
			temperature_logs[index] = data32;
		    index++;
		    Address = Address + SIZE_OF_DATA_WRITTEN_TO_FLASH;
		}
		else
		{
			break;
		}
	}


	/* Add the new value to the array so we can write them to flash */

	if (index < NUMBER_OF_ITEMS_TO_STORE_IN_FLASH)
	{
		/* The array is not full yet, we can just append the value */
		temperature_logs[index] = input_val;
	}
	else
	{
		/* The array is full, we must shift all values to save the latest 10 logs */
		index = 0;

		while (index < (NUMBER_OF_ITEMS_TO_STORE_IN_FLASH - 1) )
		{
			temperature_logs[index] = temperature_logs[index+1];
			index++;
		}

		/* Finally, write the new value at the 10th place in the array */

		temperature_logs[index] = input_val;
	}

	return retval;
}

/**
  * @brief  Erases the flash sector
  * @param  None
  * @retval 0 - successful
  */
static int FlashErase(void)
{
	int retval = 0;
	uint32_t SECTORError = 0;

	/* Erase the user Flash area (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) */

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = FIRST_SECTOR;
	EraseInitStruct.NbSectors     = NBOFSECTORS;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/* Error occurred while sector erase. SECTORError will contain the faulty sector and
		 * then to know the code error on this sector, user can call function 'HAL_FLASH_GetError()'*/
		LOG_ERR("Brisanje flash sektora broj %u nije uspjesno\n", (uint16_t)SECTORError);
		retval = 1;
	}

	return retval;
}

/**
  * @brief  Writes the logs in the flash sector
  * @param  None
  * @retval 0 - successful
  */
static int FlashWrite(void)
{
	int retval = 0;

	/* Program the user Flash area word by word */

	uint32_t Address = FLASH_USER_START_ADDR;
	uint32_t index = 0;

	while ( (Address < FLASH_VALUES_END_ADDR) && (temperature_logs[index] != 0) )
	{
	    if ( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, temperature_logs[index]) == HAL_OK )
	    {
	    	Address = Address + SIZE_OF_DATA_WRITTEN_TO_FLASH;
	    	index++;
	    }
	    else
	    {
	    	LOG_ERR("Upisivanje u flash nije bilo uspjesno\n");
	    	return retval = 1;
	    }
	}
	return retval;
}

/**
  * @brief  Verifies that the data was written successfully
  * @param  None
  * @retval 0 - successful
  */
static int FlashVerify(void)
{
	int retval = 0;

	/* Check if the programmed data is OK
	   MemoryProgramStatus = 0: data programmed correctly
	   MemoryProgramStatus != 0: number of words not programmed correctly */

	uint32_t Address = FLASH_USER_START_ADDR;
	uint32_t index = 0;
	uint32_t data32 = 0;
	uint32_t MemoryProgramStatus = 0;

	while (Address < FLASH_VALUES_END_ADDR)
	{
		data32 = *(uint32_t *)Address;

	    if ((data32 != temperature_logs[index]) && (data32 != 0xFFFFFFFF))
	    {
	    	MemoryProgramStatus++;
	    }

	    Address = Address + SIZE_OF_DATA_WRITTEN_TO_FLASH;
	    index++;
	}

	/*Check if there is an issue to program data*/
	if (MemoryProgramStatus == 0)
	{
	    /* No error detected */
		LOG_INFO("Upisivanje u flash obavljeno uspjesno\n");
	}
	else
	{
	    /* Error detected */
		LOG_ERR("Upisivanje u flash nije bilo uspjesno\n");
		retval = 1;
	}

	return retval;
}
