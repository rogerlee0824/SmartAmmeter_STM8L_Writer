#include "sle4442.h"
#include "stm8l15x.h"
#include "includes.h"

void SLE4442_I2C_Init(void);

/***********************************************************************
  * @brief  Initialize AT88SC102 for IC card insert
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_Init(void)
{
	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] SLE4442_Init...\r\n");
	#endif
#if 0
    /* Disable interrupts */
	disableInterrupts();

	GPIO_Init(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST, GPIO_Mode_Out_OD_Low_Fast);
	SLE4442_I2C_Init();
	
	GPIO_Init(GPIO_PORT_IC_CARD_BIT, GPIO_PIN_IC_CARD_BIT, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_IC_CARD_BIT, EXTI_Trigger_IC_CARD_BIT);
	
	/* Enable interrupts */
	enableInterrupts();
	EXTI_ClearITPendingBit(EXTI_IT_PIN_IC_CARD_BIT);
#else
	GPIO_Init(GPIO_PORT_IC_CARD_PGM, GPIO_PIN_IC_CARD_PGM, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_IC_CARD_PW, GPIO_PIN_IC_CARD_PW, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(GPIO_PORT_IC_CARD_PRE, GPIO_PIN_IC_CARD_PRE, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);

	/* Disable interrupts */
	disableInterrupts();

	GPIO_Init(GPIO_PORT_IC_CARD_BIT, GPIO_PIN_IC_CARD_BIT, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_IC_CARD_BIT, EXTI_Trigger_IC_CARD_BIT);

	EXTI_ClearITPendingBit(EXTI_IT_PIN_IC_CARD_BIT);
	
	/* Enable interrupts */
	enableInterrupts();
	
#endif
}

/***********************************************************************
  * @brief  Power on AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_PowerON(void)
{
	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] SLE4442_PowerON...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_PW, GPIO_PIN_IC_CARD_PW, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Power off AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_PowerOff(void)
{
	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] SLE4442_PowerOff...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_PW, GPIO_PIN_IC_CARD_PW, GPIO_Mode_Out_PP_High_Fast);
}


/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_I2C_Init(void)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] SLE4442_I2C_Init...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_OD_HiZ_Fast);
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_OD_HiZ_Fast);
}

/***********************************************************************
  * @brief  Deinitialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_I2C_DeInit(void)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] SLE4442_I2C_DeInit...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);
}


/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_I2C_Start(void)
{
	#ifdef IC_CARD_DEBUG
		//printf("[IC] SLE4442_I2C_Start...\r\n");
	#endif

	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(5);
	GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	delay1us(5);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(5);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_I2C_Stop(void)
{
	#ifdef IC_CARD_DEBUG
		//printf("[IC] SLE4442_I2C_Stop...\r\n");
	#endif
	
	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(5);
	GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	delay1us(5);
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(10);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
uint8_t SLE4442_I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t temp = 0;
	uint8_t value = 0;
	
	#ifdef IC_CARD_DEBUG
		//printf("[IC] SLE4442_I2C_ReadByte...\r\n");
	#endif

	for(i = 8;i > 0;i --) 
	{
		temp >>= 1;
		GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
		value = GPIO_ReadInputDataBit(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
		if(value)
		{
			temp |= 0x80; 
		}
		else
		{
			temp &= 0x7f;
		}
		delay1us(5);
		GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
		delay1us(5);
	}

	return (temp);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_Read(uint8_t * pDstBuffer, uint8_t len)
{
	uint8_t i;

	#ifdef IC_CARD_DEBUG
		printf("[IC] SLE4442_Read...\r\n");
	#endif
	SLE4442_I2C_Start();
	
	GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);
	for(i = 0;i < len;i ++)
	{	
		*(pDstBuffer + i) = SLE4442_I2C_ReadByte();
	}
	GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);
	
	SLE4442_I2C_Stop();
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_I2C_WriteByte(uint8 * pSrcBuffer)
{
	uint8_t i,value;
	
	#ifdef IC_CARD_DEBUG
		//printf("[IC] SLE4442_I2C_WriteByte...\r\n");
	#endif

	value = *pSrcBuffer;
	for(i = 8;i > 0;i --)
	{
		GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
		if(value & 0x01)
		{
			GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
		}
		else
		{
			GPIO_ResetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
		}
		
		delay1us(5);
		GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
		delay1us(10);
		value >>= 1;
	}
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_Write(uint8_t * pDstBuffer, uint8_t len)
{
	uint8_t i;

	#ifdef IC_CARD_DEBUG
		printf("[IC] SLE4442_Write...\r\n");
	#endif
	SLE4442_I2C_Start();
	
	for(i = 0;i < len;i ++)
	{	
		SLE4442_I2C_WriteByte(pDstBuffer + i);
	}

	SLE4442_I2C_Stop();
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_Break(void)
{
	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(5);
	GPIO_SetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
    delay1us(5);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
	delay1us(5);
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_Reset(void)
{
	uint8_t temp[4] = {0};
	
	#ifdef IC_CARD_DEBUG
		//printf("\r\n[IC] SLE4442_Reset...\r\n");
	#endif

    delay1us(5); 
	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);     
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);  
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
	delay1us(5);  
	GPIO_SetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
	delay1us(5);
	GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	delay1us(40);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
	delay1us(5);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST); 
	delay1us(10);     //��λ�͸�λӦ��ʱ��
                     //      ___
                     //RST _|   |_______________________   
                     //       _   _        __   __
                     //CLK __|0|_|1|_ ..._|31|_|32|_____
                     //   _   __  __           ___  _____
                     //     \/  \/D0\ ...    \/D31\/
                     //I/O _/\__/\__/        /\___/
    temp[0] = SLE4442_I2C_ReadByte();
    temp[1] = SLE4442_I2C_ReadByte();
    temp[2] = SLE4442_I2C_ReadByte();
    temp[3] = SLE4442_I2C_ReadByte();   									//�ն� 32Bit (4Byte)
    GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);     	//     __   __  IC sets I/O to state H
    delay1us(5);         												//CLK_|31|_|32|______________
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);          	//     __   ___  ____________
    nop();             													//     30 \/ 31\/
    GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);   		//I/O  __ /\___/
	delay1us(5);

	#ifdef IC_CARD_DEBUG
		/*for(uint8_t i = 0;i < sizeof(temp);i ++)
		{
			printf("0x%02x, ",temp[i]);
		}
		printf("\r\n");*/
	#endif
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_ReadMode(uint8_t * pt,uint8_t count)
{
	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] SLE4442_ReadMode...\r\n");
	#endif

	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
    delay1us(5);

    do{
        *pt++ = SLE4442_I2C_ReadByte();    //����һ���֣�ָ���һ
	}while(--count);             //��������һ,�ж�
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_ProcessMode(void)
{
    uint8_t i;

	#ifdef IC_CARD_DEBUG
		printf("\r\n[IC] SLE4442_ProcessMode...\r\n");
	#endif
	
	GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
    delay1us(5);    
    GPIO_ResetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
    for (i = 255; i > 0; i--)
    {
        GPIO_SetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
        delay1us(5); 
		GPIO_ResetBits(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK);
		delay1us(5);
	}
	GPIO_SetBits(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO);
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_CommWrite(uint8_t a, uint8_t b, uint8_t c)
{
    SLE4442_I2C_Start(); 
	
    SLE4442_I2C_WriteByte(&a);
    SLE4442_I2C_WriteByte(&b);         
    SLE4442_I2C_WriteByte(&c);  
	
	SLE4442_I2C_Stop();
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
uint8_t SLE4442_Verify(uint8_t *pt)
{
	uint8_t temp[4];                				//�ݴ�4�ֽڵı���������
	uint8_t i;

	#ifdef IC_CARD_DEBUG
		 printf("\r\n[IC] SLE4442_Verify...\r\n");
	#endif
	
	SLE4442_Reset();
	SLE4442_CommWrite(RSM_COMM,0xff,0xff);        	//������洢����������,��2,3�������ڴ������б�����
	SLE4442_ReadMode(temp, 4);                   	//����

	#ifdef IC_CARD_DEBUG
		for(i = 0;i < 4;i ++)
		{
			printf("0x%02x, ",temp[i]);
		}
		printf("\r\n");
	#endif
	
	if((temp[0] & 0x07) != 0)            			//��һ�ֽ��Ǵ��������,������������Ϊ0,ֱ���˳�
	{
		if((temp[0] & 0x07)==0x07)     				// 00000111
            i = 0x06;
		else if((temp[0] & 0x07)==0x06)				// 00000110 
            i = 0x04;
		else if((temp[0] & 0x07)==0x04)				// 00000100
           i = 0x00;               					//������һλΪ1�ĸ�Ϊ0
           
        SLE4442_CommWrite(WSM_COMM,0,i);            //�޸Ĵ��������
        SLE4442_ProcessMode();                  	//����
        for (i = 1; i < 4; i++, pt++)   			//У��3�ֽڵ�����
        {
			SLE4442_CommWrite(VER_COMM,i,*pt);  	//����У������,
			SLE4442_ProcessMode();             		//����
		}
        SLE4442_CommWrite(WSM_COMM,0,0xff);      	//�����������ָ����������
        SLE4442_ProcessMode();                  	//����
        SLE4442_CommWrite(RSM_COMM,0xff,0xff);   	//������洢����������,��2,3�������ڴ������б�����
        SLE4442_ReadMode(temp, 4);              	//�����������������

		if((temp[0] & 0x07)==0x07)      			//���û�б��ɹ�����,����У��ʧ��
		return 1 ;
	}

	return 0;
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_ProtectByte(uint8_t addr,uint8_t *pt)
{
    SLE4442_Reset();
    SLE4442_CommWrite(WPM_COMM, addr, *pt); //д�����洢����������,��ַ,����
    SLE4442_ProcessMode();
    SLE4442_Break();            
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_ReadMainMem(uint8_t addr, uint8_t *pt,uint8_t count)
{
    SLE4442_Reset();
    SLE4442_CommWrite(RMM_COMM, addr, 0xff);
    SLE4442_ReadMode(pt,count);
    SLE4442_Break();
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void SLE4442_WriteMainMem(uint8_t addr, uint8_t *pt)
{
    SLE4442_Reset();
    SLE4442_CommWrite(WMM_COMM, addr, *pt);     //д�����������,��ַ,����
    SLE4442_ProcessMode();
    SLE4442_Break();    
}



