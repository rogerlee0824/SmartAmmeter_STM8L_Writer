#include "ic.h"
//#include "at88sc102.h"
#include "sle4442.h"
#include "includes.h"

ic_event_t ic_card_event;
uint8_t flag_ic_insert = 0;
uint8_t writeInfo[UART_RX_MAX_LEN - 4] = {0};
uint8_t readInfo[UART_RX_MAX_LEN - 4] = {0};

void IC_Card_Handle(void)
{


}

void SLE4442_WriteMultiBytetoMainMem(uint8_t addr, uint8_t *pSrc, uint8_t len)
{
	uint8_t i = 0;
	
	for(i = 0;i < len;i ++)
	{
		SLE4442_WriteMainMem((addr + i), (pSrc + i));
	}
}

bool CmpBuffer(uint8_t *p_buffer1, uint8_t *p_buffer2, uint8_t len)
{
	uint8_t i = 0;

	for(i = 0;i < len;i ++)
	{
		if(*(p_buffer1 + i) != *(p_buffer2 + i))
		{
			return false;
		}
	}

	return true;
}
	
/***********************************************************************
  * @brief  Handle the event from IC Card.
  * @param  None
  * @retval None
************************************************************************/
void ic_event_handler(void * p_event_data, uint16_t event_size)
{
    ic_event_t * ic_event_temp = p_event_data;
	uint8_t check_temp[3] = {0xff,0xff,0xff};
	uint8_t yemp[256] = {0},i;
	uint8_t temp = 1;
	uint8_t ic_verify_ok = 0;
    
    switch(ic_event_temp->eIC_event)
    {
        case IC_CARD_INIT:
            SLE4442_Init();
            break;
            
        case IC_CARD_INSERT:
			#ifdef IC_CARD_DEBUG
                printf("\r\nIC Card insert!!!\r\n");
            #endif

			flag_ic_insert = 1;

			LED1_OFF();
			
			SLE4442_I2C_Init();
			SLE4442_PowerON();

			if(SLE4442_Verify(check_temp))
			{
				#ifdef IC_CARD_DEBUG
                	printf("[IC] Verify OK...\r\n");
            	#endif
				ic_verify_ok = 1;
			}
			else
			{
				#ifdef IC_CARD_DEBUG
                	printf("[IC] Verify Fail...\r\n");
            	#endif
				ic_verify_ok = 0;
			}
			
			SLE4442_WriteMainMem(0x20, &temp);
			SLE4442_WriteMainMem(0x21, &temp);
			SLE4442_WriteMainMem(0x22, &temp);
			SLE4442_ReadMainMem(0x00, yemp, sizeof(yemp));
			#ifdef IC_CARD_DEBUG
				for(uint16_t i = 0;i < sizeof(yemp);i ++)
				{
					printf("0x%02x, ",yemp[i]);
					if(!((i + 1)%16))
					{
						printf("\r\n");
					}
				}
				printf("\r\n");
			#endif
            break;

		case IC_CARD_WRITE:
			#ifdef IC_CARD_DEBUG
                printf("\r\nIC_CARD_WRITE\r\n");
            #endif
			memcpy(writeInfo, &RxBuffer_Backup[2], sizeof(writeInfo));
			if(flag_ic_insert == 1)
			{
				SLE4442_I2C_Init();
				SLE4442_PowerON();

				if(SLE4442_Verify(check_temp))
				{
					#ifdef IC_CARD_DEBUG
	                	printf("[IC] Verify OK...\r\n");
	            	#endif
					ic_verify_ok = 1;
				}
				else
				{
					#ifdef IC_CARD_DEBUG
	                	printf("[IC] Verify Fail...\r\n");
	            	#endif
					ic_verify_ok = 0;
				}

				if(ic_verify_ok == 1)
				{
					if(ic_event_temp->cmd == UART_RX_CMD_WRITE_DATA)
					{
						SLE4442_WriteMultiBytetoMainMem(MAIN_MEM_BASE,writeInfo,sizeof(writeInfo));
						SLE4442_ReadMainMem(MAIN_MEM_BASE, readInfo, sizeof(readInfo));
						if(CmpBuffer(writeInfo, readInfo, sizeof(readInfo)) == true)
						{	
							LED1_OFF();
							beeper_event.eBeeper_event = BEEPER;
							beeper_event.beeper_times = 2;
							app_sched_event_put(&beeper_event,sizeof(beeper_event),beeper_event_handler);
						}
					}
					else if(ic_event_temp->cmd == UART_RX_CMD_MODIFY_PASS)
					{
						
					}
				}
			}
			else
			{
				LED1_ON();	
			}

            break;

		case IC_CARD_DEINIT:
			#ifdef IC_CARD_DEBUG
                printf("IC Card insert!!!\r\n");
            #endif
			SLE4442_I2C_DeInit();
			SLE4442_PowerOff();
			
            break;

        default:
            break;
    }
}