#ifndef __ENCODER_H__
#define __ENCODER_H__

void Encoder_Init(void);
int16_t GetEncoderCount1(void);
int16_t GetEncoderCount2(void);

extern volatile int16_t Encoder_Count1;
extern volatile int16_t Encoder_Count2;

#endif
