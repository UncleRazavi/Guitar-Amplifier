/* pcm5102.h - minimal PCM5102 DAC helper (I2S consumer) */
#ifndef DRIVERS_PCM5102_H_
#define DRIVERS_PCM5102_H_

#include "main.h"

/* Initialize PCM5102 driver GPIOs (mute pin), call after HAL init */
void pcm5102_init(GPIO_TypeDef *mute_port, uint16_t mute_pin);

/* Attach configured I2S handle and sample buffer for DMA transmit */
void pcm5102_configure_i2s(I2S_HandleTypeDef *hi2s, uint16_t *buffer, uint32_t size);

/* Start I2S streaming using the configured DMA buffer */
int pcm5102_start(void);

/* Stop I2S streaming cleanly */
int pcm5102_stop(void);

/* Set mute/unmute state (true = mute) */
void pcm5102_set_mute(uint8_t mute);

/* Query whether PCM5102 is currently muted */
uint8_t pcm5102_is_muted(void);

#endif /* DRIVERS_PCM5102_H_ */
