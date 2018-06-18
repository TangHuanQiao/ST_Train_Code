#include <string.h>
#include "task_protocol.h"
#include "stm32f0xx_hal.h"

extern uint8_t ITRXBuff[1];
extern UART_HandleTypeDef huart2;

PROTO_BUFF g_proto_buff;

__weak void handle_proto_data(PROTO_BUFF *proto_buff)
{


	
}


static void insert_byte_to_buffer(PROTO_BUFF *proto_buff, uint8_t byte)
{
	if (proto_buff == NULL)
		return;

	proto_buff->buffer[proto_buff->head_pos] = byte;

	proto_buff->head_pos++;
	if (proto_buff->head_pos >= PROTO_BUFF_LEN)
		proto_buff->head_pos = 0;

	proto_buff->buff_len++;
	
	proto_buff->RecTimer=REC_BYTE_MAX_TC;
}

static void proto_reset(PROTO_BUFF *proto_buff)
{
	if (proto_buff == NULL)
		return;

	proto_buff->proto_stat = PROTO_WAIT_FF;
	proto_buff->data_cnt = 0;
	proto_buff->data_len = 0;
	proto_buff->proto_type = 0;
	proto_buff->data_crc = 0;
	memset(proto_buff->data, 0, sizeof(proto_buff->data));
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//  if(huart==&huart2)
   	insert_byte_to_buffer(&g_proto_buff,*ITRXBuff); 

  /* Start another reception: provide the buffer pointer with offset and the buffer size */
  HAL_UART_Receive_IT(huart, (uint8_t *)(ITRXBuff), 1);
}


static void _proto_parse(PROTO_BUFF *proto_buff, uint8_t data)
{
    switch (proto_buff->proto_stat)
    {
        case PROTO_WAIT_FF:
        {
            if (data == PROTO_SOH_FF)
            {
            	proto_buff->proto_stat = PROTO_WAIT_55;
            }
            break;
        }
        case PROTO_WAIT_55:
        {
            if (data == PROTO_SOH_55)
            {
            	proto_buff->proto_stat = PROTO_WAIT_AA;
            }
            else
            {
                goto proto_reset;
            }
            break;
        }
        case PROTO_WAIT_AA:
        {
            if (data == PROTO_SOH_AA)
            {
            	proto_buff->proto_stat = PROTO_WAIT_LEN;
            }
            else
            {
                goto proto_reset;
            }
            break;
        }
        case PROTO_WAIT_LEN:
        {
        	proto_buff->data_len = data;
        	proto_buff->data_crc = data;
            proto_buff->proto_stat = PROTO_WAIT_TYPE;
            break;
        }
        case PROTO_WAIT_TYPE:
        {
            if (proto_buff->data_len > proto_buff->data_cnt)
            {
            	proto_buff->data_crc += data;
                proto_buff->data_cnt++;
                proto_buff->proto_type = data;
                if (proto_buff->proto_type & 0x80)
                {
                	proto_buff->proto_stat = PROTO_WAIT_CRC;
                }
                else
                {
                	proto_buff->proto_stat = PROTO_WAIT_DATA;
                }

            }
            break;
        }
        case PROTO_WAIT_DATA:
        {
            if (proto_buff->data_len > proto_buff->data_cnt)
            {
            	proto_buff->data_crc += data;
            	proto_buff->data[proto_buff->data_cnt-1] = data;
                proto_buff->data_cnt++;
                if (proto_buff->data_len == proto_buff->data_cnt)
                {
                	proto_buff->proto_stat = PROTO_WAIT_CRC;
                }
            }
            break;
        }
        case PROTO_WAIT_CRC:
        {
            if (proto_buff->data_crc == data)
            {
            	handle_proto_data(proto_buff);
            }
            goto proto_reset;
        }
        default:
        {
            goto proto_reset;
        }
    }

    return;

proto_reset:
	proto_reset(&g_proto_buff);
}

static void proto_parse(PROTO_BUFF *proto_buff)
{
	uint32_t i;
	uint32_t buff_len = 0;;


	if (NULL == proto_buff)
		return;


	buff_len = proto_buff->buff_len;


	if (0 == buff_len)
		return;


	if(proto_buff->RecTimer)
	{
	   proto_buff->RecTimer--;
	   return;
	}


	for (i = 0; i < buff_len; i++)
	{
		_proto_parse(proto_buff, proto_buff->buffer[proto_buff->tail_pos]);
		proto_buff->tail_pos++;
		if (proto_buff->tail_pos >= PROTO_BUFF_LEN)
			proto_buff->tail_pos = 0;
	}


	proto_buff->buff_len -= buff_len;

}

 void AppTaskProtoParse(void)
{

    proto_parse(&g_proto_buff);

    HAL_UART_Receive_IT(&huart2, (uint8_t *)(ITRXBuff), 1);//定时启动接收中断，防止中断里繁忙不启动
}
