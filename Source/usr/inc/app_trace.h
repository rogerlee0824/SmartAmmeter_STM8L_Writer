#ifndef _APP_TRACE_H_
#define _APP_TRACE_H_

#include "stm8l15x.h"

#define UART_RX_MAX_LEN						(10)

#define UART_FRAME_HEADER					(0xaa)

#define UART_RX_CMD_MODIFY_PASS				(0x01)
#define UART_RX_CMD_WRITE_DATA				(0x02)


typedef enum {  
	USART_INIT,
	USART_RX_COMPLETE
} emUsart_event_t;

typedef struct{
	emUsart_event_t eUsart_event;
}usart_event_t;

extern usart_event_t usart_event;
extern uint8_t RxBuffer_Backup[];


void AppTrace_Init(void);
void AppTrace_DeInit(void);

void uart_event_handler(void * p_event_data, uint16_t event_size);


#endif
