#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include <stdint.h>
#include "main.h"

#ifndef AMP_SDZ_GPIO_Port
#define AMP_SDZ_GPIO_Port GPIOC
#endif
#ifndef AMP_SDZ_Pin
#define AMP_SDZ_Pin GPIO_PIN_4
#endif

#ifndef AMP_MUTE_GPIO_Port
#define AMP_MUTE_GPIO_Port GPIOC
#endif
#ifndef AMP_MUTE_Pin
#define AMP_MUTE_Pin GPIO_PIN_5
#endif

#ifndef AMP_FAULT_GPIO_Port
#define AMP_FAULT_GPIO_Port GPIOA
#endif
#ifndef AMP_FAULT_Pin
#define AMP_FAULT_Pin GPIO_PIN_4
#endif

#ifndef DISPLAY_DC_GPIO_Port
#define DISPLAY_DC_GPIO_Port GPIOB
#endif
#ifndef DISPLAY_DC_Pin
#define DISPLAY_DC_Pin GPIO_PIN_1
#endif

#ifndef DISPLAY_RST_GPIO_Port
#define DISPLAY_RST_GPIO_Port GPIOB
#endif
#ifndef DISPLAY_RST_Pin
#define DISPLAY_RST_Pin GPIO_PIN_2
#endif

void Board_Init(void);
void Board_DisplayInit(void);
void Board_AmpEnable(void);
void Board_AmpMute(uint8_t mute);
uint8_t Board_AmpFaultActive(void);
void Board_Log(const char *msg);

#endif /* INC_BOARD_H_ */
