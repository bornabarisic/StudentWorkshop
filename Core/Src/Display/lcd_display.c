/* ------------------------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "lcd_display.h"
#include "lcd_driver.h"
#include "lcd_interface.h"
#include "log.h"

/* ------------------------------------------------------------------------------------*/
/* Public function definitions  -------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

/**
 * @brief 	This function is a wrapper function used to initialize LCD display driver.
 */
void InitializeLCD(void)
{
	lcd_t *plcd = GetLCDInterface();

	if (plcd == NULL)
	{
		return;
	}

	memset(plcd->buffer, 0x00, LCD_MAX_NUMBER_OF_COLUMNS);
	plcd->init();
}

/**
 * @brief 	This function is a wrapper function used to write data to the LCD display.
 *
 * @param 	row row index
 * @param 	col column index
 * @param 	pdata data sent to display
 */
void LCDWriteData(int row, int col, char *pdata)
{
	int lcd_state = LCD_OK;

	lcd_t *plcd = GetLCDInterface();

	if (plcd == NULL)
	{
		return;
	}

	if (pdata == NULL)
	{
		return;
	}

	if ( (row >= LCD_MAX_NUMBER_OF_ROWS) || (col >= LCD_MAX_NUMBER_OF_COLUMNS) )
	{
		LOG_ERR("Krivo postavljeni red ili stupac"
				"\n\tRedak: %d"
				"\n\rStupac: %d\n",
				row, col);
		return;
	}

	sprintf(plcd->buffer, pdata);

	lcd_state = plcd->move_cursor(row, col);

	if (lcd_state != LCD_OK)
	{
		LOG_ERR("Nije uspjesno pomaknut kursor, Error code: %d\n", lcd_state);
	}

	lcd_state = plcd->send_data();

	if (lcd_state != LCD_OK)
	{
		LOG_ERR("Nije uspjesno poslana poruka prema LCDu, Error code: %d\n", lcd_state);
	}
}

/**
 * @brief 	This function is a wrapper function used to clear entire LCD display.
 */
void LCDClearScreen(void)
{
	int lcd_state = LCD_OK;

	lcd_t *plcd = GetLCDInterface();

	if (plcd == NULL)
	{
		return;
	}

	lcd_state = plcd->clear_data();

	if (lcd_state != LCD_OK)
	{
		LOG_ERR("Nije uspjesno ociscen display, Error code: %d", lcd_state);
	}
}
