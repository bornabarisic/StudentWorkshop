#ifndef __GPIO_H
#define __GPIO_H

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

void InitializeBoardSupport(void);
void BlinkLEDWithPushButton(void);
void BlinkLEDWithConstantDelay(int delay_ms);

#endif /* __GPIO_H */
