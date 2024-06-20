/* ------------------------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "lcd_display.h"
#include "lcd_driver.h"
#include "lcd_interface.h"

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
		return;
	}

	sprintf(plcd->buffer, pdata);

	plcd->move_cursor(row, col);
	plcd->send_data();
}

/**
 * @brief 	This function is a wrapper function used to clear entire LCD display.
 */
void LCDClearScreen(void)
{
	lcd_t *plcd = GetLCDInterface();

	if (plcd == NULL)
	{
		return;
	}

	plcd->clear_data();
}
