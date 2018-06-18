#ifndef __TASK_PROTOCOL_H__
#define __TASK_PROTOCOL_H__
#include <stdint.h>


#define PROTO_BUFF_LEN				128
#define PROTO_DATA_LEN				16

#define PROTO_SOH_FF    			0xFF
#define PROTO_SOH_55    			0x55
#define PROTO_SOH_AA    			0xAA
#define REC_BYTE_MAX_TC 1	//相邻字节的最大接收时间


typedef enum
{
    PROTO_WAIT_FF = 0,
    PROTO_WAIT_55 = 1,
    PROTO_WAIT_AA = 2,
    PROTO_WAIT_LEN = 3,
    PROTO_WAIT_TYPE = 4,
    PROTO_WAIT_DATA = 5,
    PROTO_WAIT_CRC = 6,
}PROTO_STAT;

typedef struct _PROTO_BUFF
{
	PROTO_STAT proto_stat;
	uint8_t data_cnt;
	uint8_t data_len;
	uint8_t proto_type;
	uint8_t data_crc;
	uint8_t data[PROTO_DATA_LEN];
	uint32_t head_pos;
	uint32_t tail_pos;
	uint32_t buff_len;
	uint8_t buffer[PROTO_BUFF_LEN];
	uint8_t RecTimer;
}PROTO_BUFF;


void AppTaskProtoParse(void);


#endif
