#ifndef _AT88SC102_H_
#define _AT88SC102_H_

#include "stm8l15x.h"

#define RMM_COMM 				0x30    //������������
#define WMM_COMM 				0x38    //д����������
#define VER_COMM 				0x33    //У������
#define RSM_COMM 				0x31    //������洢��
#define WSM_COMM 				0x39    //д����洢��
#define RPM_COMM 				0x34    //�������洢��
#define WPM_COMM 				0x3c    //д�����洢��

#define MAIN_MEM_BASE			(0x20)

void SLE4442_Init(void);
void SLE4442_Reset(void);
void SLE4442_Break(void);
uint8_t SLE4442_I2C_ReadByte(void);
void SLE4442_I2C_Read(uint8 * pDstBuffer, uint8_t len);
void SLE4442_Write(uint8_t * pDstBuffer, uint8_t len);
void SLE4442_I2C_WriteByte(uint8 * pSrcBuffer);
void SLE4442_WriteMainMem(uint8_t addr, uint8_t *pt);
void SLE4442_ReadMainMem(uint8_t addr, uint8_t *pt,uint8_t count);
void SLE4442_ProtectByte(uint8_t addr,uint8_t *pt);
uint8_t SLE4442_Verify(uint8_t *pt);
void SLE4442_I2C_Init(void);
void SLE4442_I2C_DeInit(void);
void SLE4442_PowerON(void);

#endif