#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

/* ----------------------------------------------------------------------------------- */
/* Define ---------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define LCD_MAX_NUMBER_OF_COLUMNS	( 0x10 ) /* 16 columns per row */
#define LCD_MAX_NUMBER_OF_ROWS		( 0x02 ) /* 2 rows */

/* ----------------------------------------------------------------------------------- */
/* Typedef --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

typedef void (*fp_init_lcd)(void);
typedef int (*fp_send_data)(void);
typedef int (*fp_clear_lcd)(void);
typedef int (*fp_move_cursor)(int row, int col);

typedef struct lcd_t
{
	char 			buffer[LCD_MAX_NUMBER_OF_COLUMNS];
	fp_init_lcd		init;
	fp_clear_lcd	clear_data;
	fp_send_data 	send_data;
	fp_move_cursor 	move_cursor;
} lcd_t;

enum lcd_retval
{
	LCD_OK,
	LCD_ERROR_I2C_NOT_INITALIZED,
	LCD_ERROR_NOT_INITIALIZED,
	LCD_ERROR_TRASMIT_ERROR,
	LCD_ERROR_CLEAR_LCD_ERROR,
	LCD_ERROR_CURSOR_ERROR,
	LCD_ERROR_CURSOR_POSITION_ERROR,
};

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */
#define ASSERT(condition) 				\
	do									\
	{									\
								\
	}while(0)
#define LOG_ERR(condition1, condition2) 				\
	do									\
	{									\
								\
	}while(0)

lcd_t *GetLCDInterface(void);

#endif /* LCD_DRIVER_H_ */
