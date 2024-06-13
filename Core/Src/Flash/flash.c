#include "flash.h"
#include "log.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_4   	   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_5 - 1    /* End @ of user Flash area */
#define FLASH_VALUES_END_ADDR	ADDR_FLASH_SECTOR_4  +  10*sizeof(uint32_t)	/* End @ of Flash area used for temperature logs */

#define FIRST_SECTOR	FLASH_SECTOR_4
#define NBOFSECTORS		1

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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t temperature_logs[10] = {0};

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
static int FlashReadAndAdd(uint32_t input_val);
static int FlashErase(void);
static int FlashWrite(void);
static int FlashVerify(void);

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  Function for writing a value to flash
  * @param  val - Value to be written in flash
  * @retval 0 if successful
  */
int FlashWriteLog(float input_val)
{
	int retval = 0;

	int input_val_int = (int)(input_val * PRESERVATION_FACTOR);

	/* Read previous logs from flash and append the new value */
	retval = FlashReadAndAdd(input_val_int);
	ASSERT(retval != 0);

	/* Unlock the Flash to enable the flash control register access */
	HAL_FLASH_Unlock();

	/* Erase the flash sector that will be used */
	retval = FlashErase();
	ASSERT(retval != 0);

	/* Write the logs to flash */
	retval = FlashWrite();
	ASSERT(retval != 0);

	/* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) */
	HAL_FLASH_Lock();

	/* Verify that the data was written successfully */
	retval = FlashVerify();
	ASSERT(retval != 0);

	return retval;
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

		else break;
	}
}

/* Private functions ---------------------------------------------------------*/

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
		    Address = Address + 4;
		}

		else break;
	}


	/* Add the new value to the array so we can write them to flash */

	if (index <= 9)
	{
		/* The array is not full yet, we can just append the value */
		temperature_logs[index] = input_val;
	}
	else
	{
		/* The array is full, we must shift all values to save the latest 10 logs */
		index = 0;

		while (index < 9)
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

	/* Erase the user Flash area
	   (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) */

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = FIRST_SECTOR;
	EraseInitStruct.NbSectors     = NBOFSECTORS;

	/* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	   you have to make sure that these data are rewritten before they are accessed during code
	   execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	   DCRST and ICRST bits in the FLASH_CR register. */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
	/*
	 Error occurred while sector erase.
	 SECTORError will contain the faulty sector and then to know the code error on this sector,
	 user can call function 'HAL_FLASH_GetError()'
	*/
	LOG_ERR("Brisanje flash sektora broj %u nije bilo uspjesno\n", (uint16_t)SECTORError);
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

	while ((Address < FLASH_VALUES_END_ADDR) && (temperature_logs[index] != 0))
	{
	    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, temperature_logs[index]) == HAL_OK)
	    {
	      Address = Address + 4;
	      index++;
	    }
	    else
	    {
	      /* Error occurred while writing data in Flash memory */
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
	    Address = Address + 4;
	    index++;
	}

	/*Check if there is an issue to program data*/
	if (MemoryProgramStatus == 0)
	{
	    /* No error detected */
		LOG_ERR("Upisivanje u flash obavljeno uspjesno\n");
	}
	else
	{
	    /* Error detected */
		LOG_ERR("Upisivanje u flash nije bilo uspjesno\n");
		retval = 1;
	}

	return retval;
}
