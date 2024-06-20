#ifndef __LCD_INTERFACE_H
#define __LCD_INTERFACE_H

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

#endif /* __LCD_INTERFACE_H */

