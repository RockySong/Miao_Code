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
//<ö��>MiaoCoreErrorNUM</ö��>
//<ö�ٹ���>ö���ں˴�������</ö�ٹ���>
//------------------------------------------
typedef enum{
	MiaoCoreError_NoError=0,//�޴���
	MiaoCoreError_PriorityRedefine,//�������ȼ��ض������
	MiaoCoreError_GiftStatusError,
	MiaoCoreError_ParaListLengthOver,
	MiaoCoreError_TimerListLengthOver,
	MiaoCoreError_FixedMalloc
}MiaoCoreErrorNUM;

//------------------------------------------
//<ö��>MiaoCoreStatusNUM</ö��>
//<ö�ٹ���>ö���ں�״̬����</ö�ٹ���>
//------------------------------------------
typedef enum{
	MiaoCoreStatus_NAN=0,//δ����״̬
	MiaoCoreStatus_Initial=1,//��ʼ����
	MiaoCoreStatus_Running=0x0a,//����������
	MiaoCoreStatus_Error=0x05//�ں˴���
}MiaoCoreStatusNUM;



#if MIAO_OS_ENABLE
//------------------------------------------
//<�ṹ����>MiaoParameter_toForm</�ṹ����>
//<�ṹ�幦��>����ϵͳ�ں˽ṹ��</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_CORE_STRUCT{
	//��Ա����
#if !MIAO_SIMPLE_MODE
	_BP DispatchBreakPoint;//�������ϵ�
#endif
	
	MiaoCoreGroupUN InitialFlagGroup;//��ʼ����־��
	
	MiaoCoreGroupUN ReadyFlagGroup;//׼����ɱ�־��

	struct MIAO_TASK_STRUCT *pTaskList[MIAO_TASK_NUM_MAX];//ע���������Ŀ��ƿ�

	struct MIAO_TASK_STRUCT *pCurrentTask;//��ǰ������ƿ�

#if !MIAO_SIMPLE_MODE	
	struct MIAO_TASK_STRUCT *pLastTask;//֮ǰ��������ȼ�
#endif
	
	struct MIAO_TASK_STRUCT MiaoInlineTaskBoring;

	MiaoTimerCtrl_t TimerCtrl;
	
	//MiaoShow_t Show;

	lw_u8 DispatchEable:1;//������ʹ�ܱ�־ 0:�رյ����� 1:����������
	MiaoCoreStatusNUM CoreStatus:7;//�ں�״̬
	MiaoCoreErrorNUM CoreError:8;//�ں˴���
	
	lw_u8 TaskRunningNum;//��ǰע����������
	
	lw_u8 TaskRunningList[MIAO_TASK_NUM_MAX];//�Լ�ע�����������ȼ� ���ڿ��ٲ�ѯ����
	
	lw_u8 CurrentTaskIndex;//��ǰ��������ȼ�
	
#if !MIAO_SIMPLE_MODE	
	lw_u8 LastTaskIndex;//֮ǰ��������ȼ�
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




	//��Ա����
#if MIAO_STEP_RUNNING
	void(*StepFun)(struct MIAO_CORE_STRUCT *pOS);//ϵͳ��ʼ����
#else
	void(*StartFun)(struct MIAO_CORE_STRUCT *pOS);//ϵͳ��ʼ����
#endif
	struct MIAO_CORE_STRUCT *(*TaskAppendFun)(struct MIAO_CORE_STRUCT *pOS, struct MIAO_TASK_STRUCT *pta);//����ע�ắ��
	struct MIAO_CORE_STRUCT *(*TimerAppendFun)(struct MIAO_CORE_STRUCT *pOS, MiaoTimer_t *pT);
	/*(Ver.=0.96)
#if !(MIAO_SIMPLE_MODE)
	void (*DispatchFun)(struct MIAO_CORE_STRUCT *pOS);//����������
	void (*ClockCBFun)(struct MIAO_CORE_STRUCT *pOS);//ϵͳʱ�ӻص����� !!!!!!!!!�������Գ�Ա��������!!!!!!!!!
	void (*StartFun)(struct MIAO_CORE_STRUCT *pOS);//ϵͳ��ʼ����
	struct MIAO_CORE_STRUCT *(*TaskAppendFun)(struct MIAO_CORE_STRUCT *pOS,struct MIAO_TASK_STRUCT *pta);//����ע�ắ��
	struct MIAO_CORE_STRUCT *(*ParaAppendFun)(struct MIAO_CORE_STRUCT *pOS,lw_u8 num,struct MIAO_PARAMETER_STRUCT *p);
	struct MIAO_CORE_STRUCT *(*TimerAppendFun)(struct MIAO_CORE_STRUCT *pOS,MiaoTimer_t *pT);
#endif
*/

#if MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN
	lw_u8 *(*StrDbgFun)(struct MIAO_CORE_STRUCT *pOS, lw_u8 *instr);
#endif // MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN

	//������ʱ�м����
	lw_u8 n1_Dispatch,n2_Dispatch;
	lw_u8 Temp;
	lw_u8 temp2;
	lw_u8 _ReadyTaskIndex;

	//�ȶ�����ʱ�����
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



//��������
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


//<�����㷨��>
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
//</�����㷨��>


//�꺯�� & ��ģ��

//�͹��ĺ�
//����ģʽ��û��
#if !MIAO_SPEED_RUNNING
#define MIAO_LOW_POW_ENABLE() ((void)0)
#define MIAO_LOW_POW_DISABLE() ((void)0)
#endif



//�ϵ㱣�� 
#define REMEMBER(x,r) r=_MIAO_SAVE((x));if(!(r))


#if !MIAO_SIMPLE_MODE

//����׼�� AΪ����ָ��
#define DISPATCH_READY(A) A->BreakPointReturn[MIAO_TASK_BREAKPOINT_DEEP - 1] = _MIAO_SAVE((A->BreakPoint[MIAO_TASK_BREAKPOINT_DEEP - 1]));\
	if (!(A->BreakPointReturn[MIAO_TASK_BREAKPOINT_DEEP - 1]) && A->TaskStatus == MiaoTaskStatus_Running)

//���ȷ��� AΪ����ָ��
#define DISPATCH_RETURN(A) else{A->TaskStatus = MiaoTaskStatus_Running;}

#else

#define DISPATCH_READY(A) 	case __LINE__:(A)->BreakPoint=__LINE__;\
			if((A)->TaskStatus == MiaoTaskStatus_Ready){(A)->TaskStatus=MiaoTaskStatus_Running;}else

#define DISPATCH_RETURN(A) return;

#endif


			
//����ʼ			
		
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

//�ȼ��� ���¾㱸
#define MiaoJiJi(A) Miao_5_2(A)

//δ����	 ��Ƿ����
#define MiaoWeiJi(A) Miao_2_5(A)			


//�����Ӻ�������
//<����>LV:Ƕ�׵ȼ��ȼ�(����������ֱ��ʹ�õ�����ȼ�Ϊ0 ����һ��Ϊ1 ��������) pta:����ָ�� F:�����õĺ���</����>
//<ʾ��> TaskSubFunCall(0,pta,fun1); pta�������fun1Ƕ�׵ȼ�0 </ʾ��> 
//!!!!!!!!!!!!void fun1(MiaoTask_t *pta,void *p)!!!!!!!!!!!!!!!!!!!!!!!!!

#define MiaoTaskSubFunCall(LV,PTA,F) do {REMEMBER(PTA->BreakPoint[LV],PTA->BreakPointReturn[LV])\
																				{F(PTA,(void *)(&(PTA->BreakPoint[LV])));}}while(0)

																				
																																				
																				
//�����Ӻ�������
//!!!!!!��TaskSubFunCall�ɶԵ��� ���ڷ���!!!!!!																				
#define MiaoTaskSubFunReturn(PTA,P) do { _MIAO_LOAD((*((_BP *)(P))),PTA->Priority,(PTA));}while(0)


#define MiaoCoreTaskIsReady(POS,PTA,ID) 					do{POS->ReadyFlagGroup.all|=(1<<ID);\
					PTA->TaskStatus=MiaoTaskStatus_Ready;}while(0)

					
					
					

//˯��N��ʱ��
//<����>A:������ƿ�ָ�� N:����</����>
#if MIAO_SIMPLE_MODE
#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ MiaoTaskSleep(A,N-1);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ MiaoTaskSleep(A,N-1);}DISPATCH_RETURN(A)} while(0)
#endif


#define MiaoSleepSec(A,SEC,MS) MiaoSleepN(A,((((uint32_t)(SEC)*1000+(MS)))/(uint32_t)MIAO_CORE_TICK_MS))

//�ȴ��������
//<����>A:������ƿ�ָ��</����>
#if MIAO_SIMPLE_MODE
#define MiaoGiftReceiveWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftReceice(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGiftReceiveWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftReceice(A);}DISPATCH_RETURN(A)} while(0)
#endif

#define MiaoWaitGift(A) MiaoGiftReceiveWait((A))

//�ȴ��������
//<����>A:������ƿ�ָ��</����>
#if MIAO_SIMPLE_MODE
#define MiaoGiftTransmitWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftTransmit(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGiftTransmitWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGiftTransmit(A);}DISPATCH_RETURN(A)} while(0)
#endif

//�ȴ���־��
//<����>A:������ƿ�ָ��</����>
#if MIAO_SIMPLE_MODE
#define MiaoGroupFlagWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGroupFlag(A);DISPATCH_RETURN(A)}} while(0)
#else
#define MiaoGroupFlagWait(A) do {DISPATCH_READY(A){ MiaoTaskWaitGroupFlag(A);}DISPATCH_RETURN(A)} while(0)
#endif


#define MiaoWaitFlag(A) MiaoGroupFlagWait((A))

//�ȴ�����
//<����>A:������ƿ�ָ�� COND:����ѭ�������� NUM:���������� ���ⲿ���崫�� MS:ÿ����ʱ���õĺ���</����>
#define MiaoWaitCondition(A,COND,NUM,MS)  do{MiaoSleepSec((A),0,(MS));}while(--NUM && (COND))

//����cpuʹ��Ȩ
//<����>A:������ƿ�ָ��</����>
#define MiaoGiveUp(A) MiaoSleepN((A),2)

//����ֹͣ
//<����>A:������ƿ�ָ��</����>
#define MiaoTaskStop(A) do{(A)->TaskFrozenFlag=1;MiaoGiveUp(A);}while(0)

//������������CPUʹ��Ȩ
//<����>A:������ƿ�ָ�� COND:��������</����>
#define MiaoConditionGiveUp(A,COND) do{\
									if (COND)\
									{\
										MiaoGiveUp((A));\
									}\
									}while(0)


//��������
//<����>A:������ƿ�ָ��</����>
#define MiaoSetAlarm(A,SEC,MS) do{(A)->TimeSliceMax=((((uint32_t)(SEC)*1000+(MS)))/(uint32_t)MIAO_CORE_TICK_MS);}while(0)


//������
//<����>A:������ƿ�ָ��</����>
#define MiaoWatchAlarm(A) MiaoConditionGiveUp((A),((A)->RunningTimeSinceRunning >= (A)->TimeSliceMax))

//����
//<����>A:������ƿ�ָ��</����>
#define MiaoAuction(A,PRICE) MiaoConditionGiveUp((A),(MiaoTaskAuctionJudge((A),(lw_8)(PRICE))))


//���Գ�������ʱ�� ��ʼ
//<����>A:������ƿ�ָ��</����>
#define MiaoTic(A) do{(A)->BackToSleep.bits.timer_start_flag=1;}while(0)


//���Գ�������ʱ�� ����
//<����>A:������ƿ�ָ��</����>
#define MiaoToc(A) MiaoTaskToc((A))

//����һ����Ϣʱ��
//<����>A:������ƿ�ָ��</����>
#define MiaoSetNap(A,S,MS) do{(A)->BackToSleep.bits.all_time_count=((S)*1000+(MS))/(MIAO_CORE_TICK_MS);MiaoTic(pta);}while(0)


//������Ϣʱ��
//<����>A:������ƿ�ָ��</����>
#define MiaoNap(A) do{lw_u16 _res_t=MiaoToc((A));if((A)->BackToSleep.bits.all_time_count >(1+ _res_t)){MiaoSleepN(A,(A)->BackToSleep.bits.all_time_count - _res_t);}}while(0)


//#define MiaoSleepN(A,N) do {DISPATCH_READY(A){ A->SleepFun(A,N-1);}} while(0)

#endif /*(Ver.=0.96) MIAO_CORE_H_ */
