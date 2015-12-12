#include "app_trace.h"
#include "includes.h"
#include <stdio.h>
#include "stm8l15x_usart.h"
#include "stm8l15x_gpio.h"

#define USART_DR_ADDRESS       (uint16_t)0x53F1  /* USART3 Data register Address */

uint8_t RxBuffer[UART_RX_MAX_LEN];
uint8_t RxBuffer_Backup[UART_RX_MAX_LEN];


usart_event_t usart_event;


/**
  * @brief  Configure DMA peripheral  
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  	/* Deinitialize DMA channels */
  	DMA_GlobalDeInit();

  	DMA_DeInit(DMA1_Channel2);

  	/* DMA channel Rx of USART Configuration */
  	DMA_Init(DMA1_Channel2, (uint16_t)RxBuffer, (uint16_t)USART_DR_ADDRESS,
           sizeof(RxBuffer), DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);

	DMA_ITConfig(DMA1_Channel2, DMA_ITx_TC, ENABLE);

	/* Global DMA Enable */
  	DMA_GlobalCmd(ENABLE);
  
  	/* Enable the USART Rx DMA channel */
  	DMA_Cmd(DMA1_Channel2, ENABLE);      

	/* Enable the USART Rx DMA requests */
  	USART_DMACmd(APP_TRACE_USART, USART_DMAReq_RX, ENABLE);
   
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void AppTrace_Init(void)
{
	/* Enables USART3 clock. */
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_USART_RX, GPIO_PIN_USART_RX, ENABLE);
	
	USART_Init(APP_TRACE_USART, 
			   115200, 
			   USART_WordLength_8b, 
			   USART_StopBits_1,
               USART_Parity_No,  
			   (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

	DMA_Config();
	
	/* Enable USART */
  	USART_Cmd(APP_TRACE_USART, ENABLE);

	// enable interrupts 
	rim();
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void AppTrace_DeInit(void)
{
	/* Disables USART3 clock. */
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, DISABLE);
	
	USART_DeInit(APP_TRACE_USART);
	GPIO_Init(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, GPIO_Mode_Out_PP_Low_Fast);
	//GPIO_Init(GPIO_PORT_USART_RX, GPIO_PIN_USART_RX, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
int putchar(int ch)
{
	uint16_t time_out = 10000;
	
	USART_SendData8(APP_TRACE_USART, (uint8_t)ch);
	while((USART_GetFlagStatus(APP_TRACE_USART, USART_FLAG_TC) == RESET) && (time_out != 0))
	{
		time_out--;
	}
	return (ch);
}

void uart_event_handler(void * p_event_data, uint16_t event_size)
{
	usart_event_t * p_usart_event_tmp = p_event_data;
	uint16_t crc16_tmp = 0;
	
	switch(p_usart_event_tmp->eUsart_event)
	{
		case USART_INIT:
			
			break;

		case USART_RX_COMPLETE:
			#ifdef UART_RX_DEBUG
				printf("USART_RX_COMPLETE\r\n");
			#endif
			memcpy(RxBuffer_Backup, RxBuffer, sizeof(RxBuffer));
			AppTrace_Init();
			if(RxBuffer_Backup[0] == UART_FRAME_HEADER)
			{
				crc16_tmp = (RxBuffer_Backup[UART_RX_MAX_LEN - 1] << 8)| RxBuffer_Backup[UART_RX_MAX_LEN - 2];
				if(crc16_ccitt(&RxBuffer_Backup[1],sizeof(RxBuffer_Backup) - 3) == crc16_tmp)
				{
					// Build the IC card write event 
					if(RxBuffer_Backup[1] == UART_RX_CMD_WRITE_DATA)
					{
						ic_card_event.eIC_event = IC_CARD_WRITE;
						ic_card_event.cmd = UART_RX_CMD_WRITE_DATA;
						app_sched_event_put(&ic_card_event,sizeof(ic_card_event),ic_event_handler);
					}
					else if(RxBuffer_Backup[1] == UART_RX_CMD_MODIFY_PASS)
					{
						ic_card_event.eIC_event = IC_CARD_WRITE;
						ic_card_event.cmd = UART_RX_CMD_MODIFY_PASS;
						app_sched_event_put(&ic_card_event,sizeof(ic_card_event),ic_event_handler);
					}
					else
					{}
				}
			}
			break;
			
		default:
			break;
	}
}


