#ifndef INC_LOG_H_
#define INC_LOG_H_

#include <stdio.h>
#include <string.h>

/* ASCII values for font colors */
#define BLACK 		"30"
#define RED 		"31"
#define GREEN 		"32"
#define YELLOW 		"33"
#define BLUE 		"34"
#define PURPLE 		"35"
#define CYAN 		"36"
#define NO_COLOR	"37"

#define __MODULE__ strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define LOG(color, format, ...) 										\
	do																	\
	{																	\
		printf("\033[0;%sm[%s]" format "\033[0;m", color, __MODULE__, ##__VA_ARGS__);	\
	}while(0)

#define LOG_INFO(format, ...)					\
	do											\
	{											\
		LOG(NO_COLOR, format, ##__VA_ARGS__);	\
	}while(0)

#define LOG_DBG(format, ...)					\
	do											\
	{											\
		LOG(GREEN, format, ##__VA_ARGS__);		\
	}while(0)

#define LOG_ERR(format, ...)					\
	do											\
	{											\
		LOG(RED, format, ##__VA_ARGS__);		\
	}while(0)

#define ASSERT(condition) 				\
	do									\
	{									\
		if ( condition )				\
		{								\
			LOG_ERR("ASSERT: "			\
					"line %d\n", 		\
					__LINE__); 			\
			while(1);					\
		}								\
	}while(0)

// Function prototypes
void log_uart_init(void);

#endif /* INC_LOG_H_ */
