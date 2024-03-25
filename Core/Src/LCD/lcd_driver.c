
/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "lcd_driver.h"
#include "lcd_i2c.h"

#include "stm32f4xx_hal.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define I2C_MSG_BUFFER_SIZE					( 2U )

/* Display ROW / COLUMN defines */
#define LCD_ROW_ONE						( 0x00 )
#define LCD_ROW_TWO						( 0x01 )
#define LCD_FIRST_ROW_BASE_ADDRESS		( 0x00 ) /* Addr first row:	 0x00 - 0x27 */
#define LCD_SECOND_ROW_BASE_ADDRESS		( 0x40 ) /* Addr second row: 0x40 - 0x67 */

/* ----------------------------------------------------------------------------------- */
/* Typedef --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

typedef enum lcd_msg_type_t
{
	LCD_MSG_TYPE_COMMAND = 0,
	LCD_MSG_TYPE_DATA
} lcd_msg_type_t;

/* ----------------------------------------------------------------------------------- */
/* Private function declarations ----------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static void InitializeLCD(void);
static int SendToLCD(lcd_msg_type_t msg_type, char pdata);
static int SendDataToLCD(void);
static int ClearLCD(void);
static int MoveLCDCursor(int row, int col);

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static lcd_t lcd_interface =
{
	.buffer			= {0},
	.init			= InitializeLCD,
	.send_data		= SendDataToLCD,
	.clear_data 	= ClearLCD,
	.move_cursor	= MoveLCDCursor
};

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

static int SendToLCD(lcd_msg_type_t msg_type, char pdata)
{
	int lcd_state = LCD_OK;
	char buffer[I2C_MSG_BUFFER_SIZE] = {0};

	switch (msg_type)
	{
		case LCD_MSG_TYPE_COMMAND:
			buffer[0] = 0x00;
			buffer[1] = pdata;
		break;

		case LCD_MSG_TYPE_DATA:
			buffer[0] = 0x40;
			buffer[1] = pdata;
		break;

		default:
		break;
	}

	if (TransmitI2CDataToLCD(buffer) != 0)
	{
		lcd_state = LCD_ERROR_TRASMIT_ERROR;
	}

	return lcd_state;
}

static void InitializeLCD(void)
{
	int lcd_state = LCD_OK;

	lcd_state = InitializeLCDI2C();

//	/* Start of initialization commands --- */
//	This LCD initializes the basic configuration by it's self
//	/* ------------------------------------ */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x01); /* Screen clear */
	HAL_Delay(1);
//	/* ------------------------------------ */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x38); /* 8-bit bus, 2-line display, 5x8 dot format */
	HAL_Delay(50);
//	/* ------------------------------------ */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x0C); /* Display switch: Display on, cursor off, blink off */
	HAL_Delay(50);
//	/* ------------------------------------ */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x06); /* Entry mode :  increment by 1, no display shift*/
	HAL_Delay(50);

//	ASSERT(lcd_state != LCD_OK);

	HAL_Delay(1000);

	lcd_state = ClearLCD();

//	ASSERT(lcd_state != LCD_OK);
}

static int SendDataToLCD(void)
{
	int lcd_state = LCD_OK;
	int i = 0;

	while(lcd_interface.buffer[i])
	{
		lcd_state = SendToLCD(LCD_MSG_TYPE_DATA, lcd_interface.buffer[i]);
		i++;

		if (lcd_state != LCD_OK)
		{
			break;
		}

		if (i >= LCD_MAX_NUMBER_OF_COLUMNS)
		{
			break;
		}
	}

	return lcd_state;
}

static int MoveLCDCursor(int row, int col)
{
	int lcd_state = LCD_OK;
	uint8_t location = 0x00;

	switch (row)
	{
		case LCD_ROW_ONE:
			location = 	0x80 |
						LCD_FIRST_ROW_BASE_ADDRESS |
						col;
		break;

		case LCD_ROW_TWO:
			location = 	0x80 |
						LCD_SECOND_ROW_BASE_ADDRESS |
						col;
		break;

		default:
		break;
	}

	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, location);

	if (lcd_state != LCD_OK)
	{
		lcd_state = LCD_ERROR_CURSOR_ERROR;
	}

	return lcd_state;
}

static int ClearLCD(void)
{
	int lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x01);

	HAL_Delay(1);

	if (lcd_state != LCD_OK)
	{
		lcd_state = LCD_ERROR_CLEAR_LCD_ERROR;
	}

	return lcd_state;
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

lcd_t *GetLCDInterface(void)
{
	return &lcd_interface;
}
