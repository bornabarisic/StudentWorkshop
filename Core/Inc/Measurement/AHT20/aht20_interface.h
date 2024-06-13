#ifndef __AHT20_INTERFACE_H
#define __AHT20_INTERFACE_H

/* ----------------------------------------------------------------------------------- */
/* Public function declarations ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

void AHT20Init(void);
float AHT20_GetTemp(void);
float AHT20_GetHum(void);

#endif /* __AHT20_INTERFACE_H */
