/*(Ver.=0.96)
 * MIAO_task.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_TASK_H_
#define MIAO_TASK_H_

#include <setjmp.h>
#include "OSLW_define.h"
#include "OSLW_gift.h"
#include "OSLW_memory.h"

//------------------------------------------
//<�ṹ����>MiaoTaskPrivateMemory_t</�ṹ����>
//<�ṹ�幦��>����˽�д洢�� ÿƬ����ΪTASK_PRIVATE_MEMORY_SECTION_LENGTH</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_TASK_PRIVATE_MEMORY_STRUCT{
	lw_u8 m8[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u16 m16[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//uint32_t m32[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u64 m64[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_sf msf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_df mdf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
}MiaoTaskPrivateMemory_t;


//------------------------------------------
//<�ṹ����>MiaoTaskExternMemory_t</�ṹ����>
//<�ṹ�幦��>�����ⲿ�洢�� ����ΪLength �׵�ַΪarr</�ṹ�幦��>
//------------------------------------------
//typedef struct MIAO_TASK_EXTERN_MEMORY_STRUCT{
//	lw_u16 Length;
//	void *arr;
//}MiaoTaskExternMemory_t;

typedef MiaoExternMemory_t MiaoTaskExternMemory_t;

//------------------------------------------
//<ö��>MiaoTaskStatusNUM</ö��>
//<ö�ٹ���>����״̬����</ö�ٹ���>
//------------------------------------------
typedef enum{MiaoTaskStatus_Init=0,//��ʼ��
	MiaoTaskStatus_Ready=1,//����׼����� �ȴ�ִ��
	MiaoTaskStatus_Running=2,//������������
	MiaoTaskStatus_Wait_ForSleep,//����ȴ� �ȴ�ԭ��Ϊ˯��
	MiaoTaskStatus_Wait_ForSignal,//����ȴ� �ȴ�ԭ��Ϊ�ȴ��ź���
	
	MiaoTaskStatus_Wait_ForGiftReceive,//����ȴ� �ȴ�ԭ��Ϊ�ȴ���Ϣ�������
	
	MiaoTaskStatus_Wait_ForGroupFlag,//����ȴ� �ȴ�ԭ��ȴ���־��
	
	MiaoTaskStatus_Wait_ForGiftTransmit,
	MiaoTaskStatus_Frozen//���񶳽�
}MiaoTaskStatusNUM;


typedef struct MIAO_TASK_GROUP_FLAG_STRUCT
{

	MiaoCoreGroupUN CurrentStatus;
	MiaoCoreGroupUN AimStatus;

	
}MiaoTaskGroupFlag_t;


//------------------------------------------
//<�ṹ����>MiaoTask_t</�ṹ����>
//<�ṹ�幦��>������ƿ�</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_TASK_STRUCT{
	//��Ա����

	
#if !(MIAO_SIMPLE_MODE)	
	volatile _BP BreakPoint[MIAO_TASK_BREAKPOINT_DEEP];//�ϵ��б� ��ʾ֧�ֵĺ���Ƕ�״���
	volatile lw_8 Name[MIAO_TASK_NAME_LENGTH_MAX+1];//���� ������MIAO_TASK_NAME_LENGTH_MAX����	
	volatile lw_u16 BreakPointReturn[MIAO_TASK_BREAKPOINT_DEEP];//�ϵ㷵�ر� ��ʾ�����ϵ�ķ���ֵ
	
#else
	volatile _BP BreakPoint;
#endif	



	
#if MIAO_SIMPLE_LEVEL<3
	
#if MIAO_GIFT_EN	
	MiaoGiftConcierge_t Concierge;
#endif //MIAO_GIFT_EN	
	
	uint32_t RunningTimeSinceStart;
	lw_u16 RunningTimeSinceRunning;
	lw_u16 TimeSliceMax;
	MiaoTaskExternMemory_t ExternMem;//�����ⲿ�洢��	
	
	union 
	{
		struct 
		{
			lw_u8 timer_start_flag : 1;
			lw_u16 work_time_count : 15;
			lw_u16 all_time_count : 16;
		}bits;

		uint32_t all;
	}BackToSleep;	


#elif MIAO_SIMPLE_LEVEL==3 && MIAO_GIFT_EN
	MiaoToolDListNodeConnect_t Concierge;
#else

#endif
	

	
	volatile uint32_t SleepCount;//˯�߼�����
	volatile MiaoTaskGroupFlag_t TaskGroupFlag;
	//MiaoTaskPrivateMemory_t PrivateMem;//����˽�д洢��
	struct MIAO_CORE_STRUCT *pOS;//����ϵͳָ�� ָ��������������ϵͳ
	
	//��Ա����
	
	void (*TaskFun)(struct MIAO_TASK_STRUCT *pta);//������
	/*(Ver.=0.96)
#if !(MIAO_SIMPLE_MODE)
	void (*SleepFun)(struct MIAO_TASK_STRUCT *pta,uint32_t n);//˯�ߺ���
	void (*WaitGRFun)(struct MIAO_TASK_STRUCT *pta);//
	void (*WaitGTFun)(struct MIAO_TASK_STRUCT *pta);//
	void (*WaitGFFun)(struct MIAO_TASK_STRUCT *pta);//
	struct MIAO_TASK_STRUCT*(*GiftTFun)
		(struct MIAO_TASK_STRUCT *pta, struct MIAO_TASK_STRUCT *pta_aim, MiaoGiftPostman_t *pGP, MiaoGiftUnit_t *pgu, GiftPostmanStatusNUM method);
#endif	
*/
	volatile lw_u8 TaskFrozenFlag : 1;
	volatile MiaoTaskStatusNUM TaskStatus : 7;//����״̬
	
	lw_u8 Priority;//���ȼ�
}MiaoTask_t;





//��������
extern MiaoTask_t MiaoInlineTaskBoring;
extern MiaoTask_t MiaoInlineTaskParameter;

void MiaoInlineTaskBoringExe(MiaoTask_t *pta);
void MiaoInlineTaskParameterExe(MiaoTask_t *pta);
void MiaoInlineTaskShowExe(MiaoTask_t *pta);
void MiaoTaskSleep(MiaoTask_t *pta,uint32_t n);
void MiaoTaskWaitGroupFlag(MiaoTask_t *pta);
void MiaoTaskWaitGiftReceice(MiaoTask_t *pta);
void MiaoTaskWaitGiftTransmit(MiaoTask_t *pta);
MiaoGroupAllType MiaoTaskAuctionJudge(MiaoTask_t *pta,lw_8 task_price);
lw_u16 MiaoTaskToc(MiaoTask_t *pta);
#if !(MIAO_SIMPLE_LEVEL >= 3)
MiaoGiftUnit_t * MiaoTaskGetOnceGift(MiaoTask_t *pta);
void MiaoTaskReceOnceGift(MiaoTask_t *pta);
MiaoTask_t *MiaoTaskGiftTransmit(MiaoTask_t *pta, MiaoTask_t *pta_aim, MiaoGiftPostman_t *pGP, MiaoGiftUnit_t *pgu, GiftPostmanStatusNUM method);
#endif // !(MIAO_SIMPLE_LEVEL >= 3)

#if MIAO_SIMPLE_LEVEL == 3
void * MiaoTaskDelivery(MiaoTask_t *pta,MiaoMemoryBasic_t *pmem,void *src,uint32_t len);
#endif

void MiaoTaskInit(MiaoTask_t *pta,
		lw_u8 priority,
		lw_8 *name,
		lw_u16 ExMemLen,
		void *ExMemarr,
		void (*fun)(struct MIAO_TASK_STRUCT *pta)
		);



#if MIAO_SIMPLE_MODE

#define MIAO_TASK_DISPATH_EXE(PTA) do{\
(PTA)->RunningTimeSinceRunning=0;\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
return;}while(0)
	
#else
	
#define MIAO_TASK_DISPATH_EXE(PTA) do{\
(PTA)->RunningTimeSinceRunning=0;\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
MiaoTaskDispatch((PTA)->pOS);\
}while(0)	
	
	
#endif



#endif /*(Ver.=0.96) MIAO_TASK_H_*/
