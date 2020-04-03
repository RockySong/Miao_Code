/*(Ver.=0.96)
 * MIAO_communication.h
 *
 *  Created on: 2017-7-26
 *      Author: slw
 */

#ifndef MIAO_COMMUN_H_
#define MIAO_COMMUN_H_

#include "OSLW_define.h"
#include "OSLW_tool.h"
#include "OSLW_gift.h"



typedef struct MIAO_MODBUS_STRCUT{


	
	MiaoToolSlipwin_t ComRX;
	
	lw_u8 Address;
	lw_u8 TxList[MIAO_COMMUN_TX_LENGTH];
	lw_u16 TxLength;
	lw_u8 *pTx;
	lw_u8 *pTxTail,*pTxHead;
	
	lw_u8 RxList[MIAO_COMMUN_RX_LENGTH+1];
	lw_u16 RxLength;
	lw_u8 *pRx;
	
	lw_u8 RxListBuf[MIAO_COMMUN_RX_LENGTH/2+1];

	lw_u8 ReadyLenList[MIAO_COMMUN_READY_LEN_LIST_LENGTH];
	
	lw_8 (*isReadyFun)(struct MIAO_MODBUS_STRCUT *pcom);
	void (*TxReadyFun)(struct MIAO_MODBUS_STRCUT *pcom);
	
}MiaoModBusCore_t;


extern struct MIAO_TASK_STRUCT MiaoInlineTaskCommun;

void MiaoInlineTaskCommunExe(struct MIAO_TASK_STRUCT *pta);
void MiaoModBusCallBack(struct MIAO_CORE_STRUCT *pOS,lw_u8 ComID,lw_u8 *pdata,lw_u16 len);
lw_8 MiaoModBusIsReady(void *pcom);
void MiaoModBusTxReady(void *pcom);
lw_u16 CRC16(lw_u8 *puchMsg, lw_u16 usDataLen) ;

extern void MiaoModBusTransmit(void *pc);

#define MODBUS_CONNECT_UINT8_TO_UINT16(A,B) (((uint32_t)(A))<<8)+B
#define MODBUS_CONNECT_INT8_TO_INT16(A,B) (((lw_32)(A))<<8)+B

#endif /*(Ver.=0.96) MIAO_COMMUN_H_ */
