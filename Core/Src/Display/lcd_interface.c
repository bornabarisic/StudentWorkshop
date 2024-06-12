
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include <string.h>
#include <stdio.h>

#include "lcd_interface.h"
#include "lcd_driver.h"
#include "log.h"


/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

void LCDInit(void)
{
	lcd_t *plcd = GetLCDInterface();

	ASSERT(plcd == NULL);

	memset(plcd->buffer, 0x00, LCD_MAX_NUMBER_OF_COLUMNS);

	plcd->init();
}

void LCDWriteData(int row, int col, char *pdata)
{
	int lcd_state = LCD_OK;
	lcd_t *plcd = GetLCDInterface();

	ASSERT(plcd == NULL);

	if ( (row >= LCD_MAX_NUMBER_OF_ROWS) ||
		 (col >= LCD_MAX_NUMBER_OF_COLUMNS) )
	{
		LOG_ERR("Krivo postavljeni red ili stupac"
				"\n\tRedak: %d"
				"\n\rStupac: %d",
				row, col);
		return;
	}

	sprintf(plcd->buffer, pdata);

	//lcd_state = plcd->clear_data();
	lcd_state = plcd->move_cursor(row, col);
	lcd_state = plcd->send_data();

	if (lcd_state != LCD_OK)
	{
		LOG_ERR("Nije uspjesno poslana poruka prema LCDu"
				"\n\tError: %d",
				lcd_state);
	}
}

void LCDClearScreen(void)
{
	int lcd_state = LCD_OK;
	lcd_t *plcd = GetLCDInterface();

	ASSERT(plcd == NULL);

	lcd_state = plcd->clear_data();

	if (lcd_state != LCD_OK)
	{
		LOG_ERR("Nije uspjesno poslana poruka prema LCDu"
				"\n\tError: %d",
				lcd_state);
	}
}
