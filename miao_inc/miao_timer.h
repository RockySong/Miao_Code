/*(Ver.=0.96)
 * MIAO_timer.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef MIAO_TIMER_H_
#define MIAO_TIMER_H_

#include "OSLW_define.h"

typedef enum{
	MiaoTimerMode_Once,
	MiaoTimerMode_Continue
}MiaoTimerModeNUM;


typedef struct MIAO_TIMER_STRUCT{
	lw_u8 ID;
	uint32_t Count,CountMax;
	lw_u8 TimerEnableFlag;
	
	MiaoTimerModeNUM Mode;
	
	void *pLeader;
	
	MiaoExternMemory_t ExtMem;
	
	void (*ClockCBFun)(struct MIAO_TIMER_STRUCT *pt);
	void (*AlarmCBFun)(struct MIAO_TIMER_STRUCT *pt);
	
	
}MiaoTimer_t;


typedef struct MIAO_TIMER_CTRL_STRUCT{
	MiaoTimer_t *pTimerList[MIAO_TIMER_CTRL_LIST_LEN];
	lw_u8 Length;
	
}MiaoTimerCtrl_t;


void MiaoTimerClockCallBack(MiaoTimer_t *pt);

#define MIAO_TIMER_DEFAULT(H,M,S,MS) {0,\
(((uint32_t)H*3600l+(uint32_t)M*60L+(uint32_t)S)*1000l+MS)/(uint32_t)MIAO_CORE_TICK_MS,\
(((uint32_t)H*3600l+(uint32_t)M*60L+(uint32_t)S)*1000l+MS)/(uint32_t)MIAO_CORE_TICK_MS,\
0,MiaoTimerMode_Once,NULL,{0,NULL},MiaoTimerClockCallBack,NULL}


#define MiaoTimerSetup(T,H,M,S,MS) do{(T).Count=(T).CountMax=(((uint32_t)H*3600l+(uint32_t)M*60L+(uint32_t)S)*1000l+MS)/(uint32_t)MIAO_CORE_TICK_MS;}while(0)

#define MiaoTimerPause(T)	do{(T).TimerEnableFlag=0;}while(0)

#define MiaoTimerRun(T) do{(T).TimerEnableFlag=1;}while(0)

#define MiaoTimerStop(T) do{(T).TimerEnableFlag=0;(T).Count=(T).CountMax;}while(0)

#define MiaoTimerStart(T,H,M,S,MS) do{(T).Count=(((uint32_t)H*3600l+(uint32_t)M*60L+(uint32_t)S)*1000l+MS)/(uint32_t)MIAO_CORE_TICK_MS;(T).TimerEnableFlag=1;}while(0)

#endif /*(Ver.=0.96) MIAO_TIMER_H_ */
