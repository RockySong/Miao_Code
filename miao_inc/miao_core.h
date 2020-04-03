/*(Ver.=0.96)
 * MIAO_core.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_CORE_H_
#define MIAO_CORE_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_memory.h"
#include "OSLW_show.h"
#include "OSLW_communication.h"
#include "OSLW_timer.h"
#include "OSLW_task.h"
#include "OSLW_string_debug.h"
#include <limits.h>
#include <stdlib.h>

typedef lw_16 (*_compar_fn)(const void *,const void *);


//------------------------------------------
//<枚举>MiaoCoreErrorNUM</枚举>
//<枚举功能>枚举内核错误类型</枚举功能>
//------------------------------------------
typedef enum{
	MiaoCoreError_NoError=0,//无错误
	MiaoCoreError_PriorityRedefine,//任务优先级重定义错误
	MiaoCoreError_GiftStatusError,
	MiaoCoreError_ParaListLengthOver,
	MiaoCoreError_TimerListLengthOver,
	MiaoCoreError_FixedMalloc
}MiaoCoreErrorNUM;

//------------------------------------------
//<枚举>MiaoCoreStatusNUM</枚举>
//<枚举功能>枚举内核状态类型</枚举功能>
//------------------------------------------
typedef enum{
	MiaoCoreStatus_NAN=0,//未定义状态
	MiaoCoreStatus_Initial=1,//初始化中
	MiaoCoreStatus_Running=0x0a,//正常运行中
	MiaoCoreStatus_Error=0x05//内核错误
}MiaoCoreStatusNUM;



#if MIAO_OS_ENABLE
//------------------------------------------
//<结构体名>MiaoParameter_toForm</结构体名>
//<结构体功能>操作系统内核结构体</结构体功能>
//------------------------------------------
typedef struct MIAO_CORE_STRUCT{
	//成员变量
#if !MIAO_SIMPLE_MODE
	_BP DispatchBreakPoint;//调度器断点
#endif
	
	MiaoCoreGroupUN InitialFlagGroup;//初始化标志组
	
	MiaoCoreGroupUN ReadyFlagGroup;//准备完成标志组

	struct MIAO_TASK_STRUCT *pTaskList[MIAO_TASK_NUM_MAX];//注册过的任务的控制块

	struct MIAO_TASK_STRUCT *pCurrentTask;//当前任务控制块

#if !MIAO_SIMPLE_MODE	
	struct MIAO_TASK_STRUCT *pLastTask;//之前任务的优先级
#endif
	
	struct MIAO_TASK_STRUCT MiaoInlineTaskBoring;

	MiaoTimerCtrl_t TimerCtrl;
	
	//MiaoShow_t Show;

	lw_u8 DispatchEable:1;//调度器使能标志 0:关闭调度器 1:开启调度器
	MiaoCoreStatusNUM CoreStatus:7;//内核状态
	MiaoCoreErrorNUM CoreError:8;//内核错误
	
	lw_u8 TaskRunningNum;//当前注册任务数量
	
	lw_u8 TaskRunningList[MIAO_TASK_NUM_MAX];//以及注册的任务的优先级 用于快速查询任务
	
	lw_u8 CurrentTaskIndex;//当前任务的优先级
	
#if !MIAO_SIMPLE_MODE	
	lw_u8 LastTaskIndex;//之前任务的优先级
#endif

#if MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN

	struct MIAO_STRING_DEBUG_STRUCT StrDbg;

#endif // MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN



	
	MiaoGroupAllType _flag_BK;
	MiaoGroupAllType _mask_list[__MIAO_LOG2_TASK_NUM-2];
	MiaoGroupAllType _move_reg[__MIAO_LOG2_TASK_NUM-2];

#if MIAO_TASK_COMMUN_ENABLE==1
	//struct MIAO_COMMUN_STRCUT Comm[MIAO_CORE_COMMUN_LIST_LENGTH];
#endif

	//MiaoParaListCtrl_t ParaBook[MIAO_CORE_PARA_PAGE_NUM];
	//lw_u16 ParaIdCount;




	//成员函数
#if MIAO_STEP_RUNNING
	void(*StepFun)(struct MIAO_CORE_STRUCT *pOS);//系统开始函数
#else
	void(*StartFun)(struct MIAO_CORE_STRUCT *pOS);//系统开始函数
#endif
	struct MIAO_CORE_STRUCT *(*TaskAppendFun)(struct MIAO_CORE_STRUCT *pOS, struct MIAO_TASK_STRUCT *pta);//任务注册函数
	struct MIAO_CORE_STRUCT *(*TimerAppendFun)(struct MIAO_CORE_STRUCT *pOS, MiaoTimer_t *pT);
	/*(Ver.=0.96)
#if !(MIAO_SIMPLE_MODE)
	void (*DispatchFun)(struct MIAO_CORE_STRUCT *pOS);//调度器函数
	void (*ClockCBFun)(struct MIAO_CORE_STRUCT *pOS);//系统时钟回掉函数 !!!!!!!!!不建议以成员函数调用!!!!!!!!!
	void (*StartFun)(struct MIAO_CORE_STRUCT *pOS);//系统开始函数
	struct MIAO_CORE_STRUCT *(*TaskAppendFun)(struct MIAO_CORE_STRUCT *pOS,struct MIAO_TASK_STRUCT *pta);//任务注册函数
	struct MIAO_CORE_STRUCT *(*ParaAppendFun)(struct MIAO_CORE_STRUCT *pOS,lw_u8 num,struct MIAO_PARAMETER_STRUCT *p);
	struct MIAO_CORE_STRUCT *(*TimerAppendFun)(struct MIAO_CORE_STRUCT *pOS,MiaoTimer_t *pT);
#endif
*/

#if MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN
	lw_u8 *(*StrDbgFun)(struct MIAO_CORE_STRUCT *pOS, lw_u8 *instr);
#endif // MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN

	//部分临时中间变量
	lw_u8 n1_Dispatch,n2_Dispatch;
	lw_u8 Temp;
	lw_u8 temp2;
	lw_u8 _ReadyTaskIndex;

	//稳定运行时间计数
	lw_u64 NormalRunningTimerCount;

#if MIAO_SIMPLE_LEVEL >= 3
	lw_u8 CPU_Usage;
	lw_u8 _CPU_Usage_cal_reg;
	lw_u8 _CPU_Usage_count;
#else
	lw_u16 CPU_Usage;
	lw_u16 _CPU_Usage_cal_reg;
	lw_u16 _CPU_Usage_count;
#endif // MIAO_SIMPLE_LEVEL >= 3



}MiaoCore_t;



//函数声明
void MiaoTaskDispatch(MiaoCore_t *_pOS);
void MiaoClockCallBack(MiaoCore_t *pOS);
void MiaoCoreInitial(MiaoCore_t *pOS);
MiaoCore_t* MiaoTaskAppend(MiaoCore_t *pOS,struct MIAO_TASK_STRUCT *pta);
MiaoCore_t* MiaoParaAppend(MiaoCore_t *pOS,lw_u8 num,struct MIAO_PARAMETER_STRUCT *p);
MiaoCore_t* MiaoTimerAppend(MiaoCore_t *pOS,MiaoTimer_t *pT);
#if MIAO_STEP_RUNNING
void MiaoCoreStep(MiaoCore_t *pOS);
#else
void MiaoCoreStart(MiaoCore_t *pOS);
#endif

void _quicksort (void *const pbase, size_t total_elems, size_t size,_compar_fn cmp);
lw_16 comp(const void*a,const void*b);


#endif //MIAO_OS_ENABLE


//<排序算法宏>
#define SWAP(a, b, size)						      \
  do									      \
    {									      \
      size_t __size = (size);						      \
      char *__a = (a), *__b = (b);					      \
      do								      \
	{								      \
	  char __tmp = *__a;						      \
	  *__a++ = *__b;						      \
	  *__b++ = __tmp;						      \
	} while (--__size > 0);						      \
    } while (0)

#define MAX_THRESH 4


typedef struct
  {
    char *lo;
    char *hi;
} stack_node;

#define STACK_SIZE	(CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
#define	STACK_NOT_EMPTY	(stack < top)
//</排序算法宏>


//宏函数 & 宏模块

//低功耗宏
//快速模式下没有
#if !MIAO_SPEED_RUNNING
#define MIAO_LOW_POW_ENABLE() ((void)0)
#define MIAO_LOW_POW_DISABLE() ((void)0)
#endif



//断点保存 
#define REMEMBER(x,r) r=_MIAO_SAVE((x));if(!(r))


#if !MIAO_SIMPLE_MODE

//调度准备 A为任务指针
#define DISPATCH_READY(A) A->BreakPointReturn[MIAO_TASK_BREAKPOINT_DEEP - 1] = _MIAO_SAVE((A->BreakPoint[MIAO_TASK_BREAKPOINT_DEEP - 1]));\
	if (!(A->BreakPointReturn[MIAO_TASK_BREAKPOINT_DEEP - 1]) && A->TaskStatus == MiaoTaskStatus_Running)

//调度返回 A为任务指针
#define DISPATCH_RETURN(A) else{A->TaskStatus = MiaoTaskStatus_Running;}

#else

#define DISPATCH_READY(A) 	case __LINE__:(A)->BreakPoint=__LINE__;\
			if((A)->TaskStatus == MiaoTaskStatus_Ready){(A)->TaskStatus=MiaoTaskStatus_Running;}else

#define DISPATCH_RETURN(A) return;

#endif


			
//任务开始			
		
#if MIAO_SIMPLE_MODE 
#define Miao_5_2(A) 	switch((A)->BreakPoint){\
		default:\
		case 0:\
		case __LINE__:(A)->BreakPoint=__LINE__;\
		if((A)->TaskStatus == MiaoTaskStatus_Init){(A)->TaskStatus=MiaoTaskStatus_Running;return;}
		

#define	Miao_2_5(A) }

#else

#define Miao_5_2(A)		do{if(!_MIAO_SAVE(A->BreakPoint[MIAO_TASK_BREAKPOINT_DEEP-1]))\
{\
	A->TaskStatus=MiaoTaskStatus_Ready;\
	_MIAO_LOAD(A->pOS->DispatchBreakPoint,A->Priority,NULL);\
}}while(0)


#define Miao_2_5(A) ;

#endif
	
	
	
#define MIAO_TASK_BENGIN(A) Miao_5_2((A))

//既济卦 万事俱备
#define MiaoJiJi(A) Miao_5_2(A)

//未济卦	 仍欠东风
#define MiaoWeiJi(A) Miao_2_5(A)			


//任务子函数调用
//<输入>LV:嵌套等级等级(在任务函数中直接使用调用则等级为0 在下一级为1 依次类推) pta:任务指针 F:被调用的函数</输入>
//<示例> TaskSubFunCall(0,pta,fun1); pta任务调用fun1嵌套等级0 </示例> 
//!!!!!!!!!!!!void fun1(MiaoTask_t *pta,void *p)!!!!!!!!!!!!!!!!!!!!!!!!!

#define MiaoTaskSubFunCall(LV,PTA,F) do {REMEMBER(PTA->BreakPoint[LV],PTA->BreakPointReturn[LV])\
																				{F(PTA,(void *)(&(PTA->BreakPoint[LV])));}}while(0)

																				
																																				
																				
//任务子函数返回
//!!!!!!与TaskSubFunCall成对调用 用于返回!!!!!!																				
#define MiaoTaskSubFunReturn(PTA,P) do { _MIAO_LOAD((*((_BP *)(P))),PTA->Priority,(PTA));}while(0)


#define MiaoCoreTaskIsReady(POS,PTA,ID) 					do{POS->ReadyFlagGroup.all|=(1<<ID);\
					PTA->TaskStatus=MiaoTaskStatus_Ready;}while(0)

					
					
					

//睡眠N个时钟
//<输入>A:任务控制块指针 N:周期</输入>
#if MIAO_SIMPLE_MODE
#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ MiaoTaskSleep(A,N-1);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ MiaoTaskSleep(A,N-1);}DISPATCH_RETURN(A)} while(0)
#endif


#define MiaoSleepSec(A,SEC,MS) MiaoSleepN(A,((((uint32_t)(SEC)*1000+(MS)))/(uint32_t)MIAO_CORE_TICK_MS))

//等待接收完成
//<输入>A:任务控制块指针</输入>
#if MIAO_SIMPLE_MODE
#define MiaoGiftReceiveWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftReceice(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGiftReceiveWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftReceice(A);}DISPATCH_RETURN(A)} while(0)
#endif

#define MiaoWaitGift(A) MiaoGiftReceiveWait((A))

//等待发送完成
//<输入>A:任务控制块指针</输入>
#if MIAO_SIMPLE_MODE
#define MiaoGiftTransmitWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftTransmit(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGiftTransmitWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftTransmit(A);}DISPATCH_RETURN(A)} while(0)
#endif

//等待标志组
//<输入>A:任务控制块指针</输入>
#if MIAO_SIMPLE_MODE
#define MiaoGroupFlagWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGroupFlag(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGroupFlagWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGroupFlag(A);}DISPATCH_RETURN(A)} while(0)
#endif


#define MiaoWaitFlag(A) MiaoGroupFlagWait((A))

//等待条件
//<输入>A:任务控制块指针 COND:跳出循环的条件 NUM:计数器变量 由外部定义传入 MS:每次延时所用的毫秒</输入>
#define MiaoWaitCondition(A,COND,NUM,MS)  do{MiaoSleepSec((A),0,(MS));}while(--NUM && (COND))

//放弃cpu使用权
//<输入>A:任务控制块指针</输入>
#define MiaoGiveUp(A) MiaoSleepN((A),2)

//任务停止
//<输入>A:任务控制块指针</输入>
#define MiaoTaskStop(A) do{(A)->TaskFrozenFlag=1;MiaoGiveUp(A);}while(0)

//满足条件放弃CPU使用权
//<输入>A:任务控制块指针 COND:满足条件</输入>
#define MiaoConditionGiveUp(A,COND) do{\
									if (COND)\
									{\
										MiaoGiveUp((A));\
									}\
									}while(0)


//设置闹钟
//<输入>A:任务控制块指针</输入>
#define MiaoSetAlarm(A,SEC,MS) do{(A)->TimeSliceMax=((((uint32_t)(SEC)*1000+(MS)))/(uint32_t)MIAO_CORE_TICK_MS);}while(0)


//看闹钟
//<输入>A:任务控制块指针</输入>
#define MiaoWatchAlarm(A) MiaoConditionGiveUp((A),((A)->RunningTimeSinceRunning >= (A)->TimeSliceMax))

//拍卖
//<输入>A:任务控制块指针</输入>
#define MiaoAuction(A,PRICE) MiaoConditionGiveUp((A),(MiaoTaskAuctionJudge((A),(lw_8)(PRICE))))


//测试程序运行时长 开始
//<输入>A:任务控制块指针</输入>
#define MiaoTic(A) do{(A)->BackToSleep.bits.timer_start_flag=1;}while(0)


//测试程序运行时长 结束
//<输入>A:任务控制块指针</输入>
#define MiaoToc(A) MiaoTaskToc((A))

//设置一共休息时间
//<输入>A:任务控制块指针</输入>
#define MiaoSetNap(A,S,MS) do{(A)->BackToSleep.bits.all_time_count=((S)*1000+(MS))/(MIAO_CORE_TICK_MS);MiaoTic(pta);}while(0)


//设置休息时间
//<输入>A:任务控制块指针</输入>
#define MiaoNap(A) do{lw_u16 _res_t=MiaoToc((A));if((A)->BackToSleep.bits.all_time_count >(1+ _res_t)){MiaoSleepN(A,(A)->BackToSleep.bits.all_time_count - _res_t);}}while(0)


//#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ A->SleepFun(A,N-1);}} while(0)

#endif /*(Ver.=0.96) MIAO_CORE_H_ */
