/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "stm32f4xx_hal.h"

#include "lcd_driver.h"
#include "lcd_i2c.h"
#include "log.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define LCD_ROW_ONE						( 0x00 )
#define LCD_ROW_TWO						( 0x01 )
#define LCD_FIRST_ROW_BASE_ADDRESS		( 0x00 ) /* Addr first row:	 0x00 - 0x27 */
#define LCD_SECOND_ROW_BASE_ADDRESS		( 0x40 ) /* Addr second row: 0x40 - 0x67 */

/* ----------------------------------------------------------------------------------- */
/* Typedef --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

typedef enum lcd_msg_type_t
{
	LCD_MSG_TYPE_COMMAND = 0x00,
	LCD_MSG_TYPE_DATA    = 0x40
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

/**
 * @brief	This function is used initialize I2C peripheral for LCD display and
 * 			send sequence of commands to initialize LCD display HW.
 */
static void InitializeLCD(void)
{
	int lcd_state = LCD_OK;

	lcd_state = InitializeLCDI2C();

	/* Start of initialization commands --- */
	/* This LCD initializes the basic configuration by it's self */

	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x01); /* Screen clear */
	HAL_Delay(2);

	ASSERT(lcd_state != LCD_OK);

	/* 8-bit bus, 2-line display, 5x8 dot format */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x38);
	HAL_Delay(50);

	ASSERT(lcd_state != LCD_OK);

	/* Display switch: Display on, cursor off, blink off */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x0C);
	HAL_Delay(50);

	ASSERT(lcd_state != LCD_OK);

	/* Entry mode : increment by 1, no display shift */
	lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x06);
	HAL_Delay(50);

	ASSERT(lcd_state != LCD_OK);

	HAL_Delay(1000);

	lcd_state = ClearLCD();

	ASSERT(lcd_state != LCD_OK);
}

/**
 * @brief 	This function is used to send data/command to the LCD display using I2C
 * 			peripheral.
 *
 * @note 	This function can send data or command to LCD display. Data is written to
 * 			display and command is used to configure display.
 *
 * @param 	msg_type type of the transmitted data, it can be command or data
 * @param 	pdata data to be sent to LCD display
 * @return 	int LCD display error code
 */
static int SendToLCD(lcd_msg_type_t msg_type, char pdata)
{
	int lcd_state = LCD_OK;
	char buffer[I2C_MSG_SIZE] = {0};

	buffer[0] = msg_type;
	buffer[1] = pdata;

	if (TransmitI2CDataToLCD(buffer) != 0)
	{
		lcd_state = LCD_ERROR_TRASMIT_ERROR;
	}

	return lcd_state;
}

/**
 * @brief 	This function is wrapper function that is used to send data to
 * 			LCD display.
 *
 * @return 	int LCD display error code
 */
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

/**
 * @brief 	This function is used to set cursor position on the LCD display
 *
 * @param 	row row index
 * @param 	col column index
 * @return 	int LCD display error code
 */
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

/**
 * @brief 	This function is used to send "clear display" command to display.
 *
 * @return 	int LCD display error code
 */
static int ClearLCD(void)
{
	int lcd_state = SendToLCD(LCD_MSG_TYPE_COMMAND, 0x01);

	HAL_Delay(2);

	if (lcd_state != LCD_OK)
	{
		lcd_state = LCD_ERROR_CLEAR_LCD_ERROR;
	}

	return lcd_state;
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief	This function is used to get LCD interface
 */
lcd_t *GetLCDInterface(void)
{
	return &lcd_interface;
}
