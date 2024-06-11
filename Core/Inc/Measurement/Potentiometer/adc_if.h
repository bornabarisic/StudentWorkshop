/*
 ******************************************************************************
 * @file    ADC.h
 * @brief   This file contains the headers of the ADC_IF.
 ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_IF_H
#define __ADC_IF_H

#ifdef __cplusplus
  extern "C" {
#endif

// Defines
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

// Function prototypes
void ADC1_Init(void);
void LED_control_with_ADC(void);
void LCD_control_with_ADC(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_IF_H */
