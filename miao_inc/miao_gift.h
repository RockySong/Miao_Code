/*(Ver.=0.96)
 * MIAO_gift.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_GIFT_H_
#define MIAO_GIFT_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"

#if !(MIAO_SIMPLE_LEVEL >= 3)


//------------------------------------------
//<��������>MiaoGiftUnitUN</��������>
//<�����幦��>����ģ��˽�е�Ԫ ���ж��ط��ʹ���</�����幦��>
//------------------------------------------
typedef struct MIAO_GIFT_UNIT_UNION{

	

	struct MIAO_PARAMETER_STRUCT *pParaData;
	struct {
		lw_u8 ComID;
		lw_u8 RxD[MIAO_GIFT_COMMUN_RXD_LENGTH];
	} ComCmd;
	
	lw_u8 str[MIAO_GIFT_UNIT_STR_LENGTH];
	lw_u8 KeyBoardInput;
}MiaoGiftUnitUN;



//------------------------------------------
//<�ṹ����>MiaoGiftExternUnit_t</�ṹ����>
//<�ṹ�幦��>�����ⲿ�洢�ռ�</�ṹ�幦��>
//------------------------------------------
//typedef struct MIAO_GIFT_EXTERN_UNIT_STRUCT{
//	lw_16 Length;
//	void *arr;
//}MiaoGiftExternUnit_t;

typedef MiaoExternMemory_t MiaoGiftExternUnit_t;

//------------------------------------------
//<ö��>GiftUnitStatusNUM</ö��>
//<ö�ٹ���>����״̬����</ö�ٹ���>
//------------------------------------------
typedef enum{
	GiftUnitStatus_NAN=0,
	GiftUnitStatus_Free=1,
	GiftUnitStatus_InDelivery,
	GiftUnitStatus_Received
}GiftUnitStatusNUM;

//------------------------------------------
//<ö��>GiftPostmanStatusNUM</ö��>
//<ö�ٹ���>���Ա״̬����</ö�ٹ���>
//------------------------------------------
typedef enum{

	GiftPostmanStatus_NAN=0,//Ϊ��ʼ��
	GiftPostmanStatus_Free=1,//����״̬
	GiftPostmanStatus_InDelivery,//��������
	GiftPostmanStatus_Received,//��ǩ�� ��ʾĿ�������Ѿ�����
	GiftPostmanStatus_Return//���� ��ʾĿ�������Ѿ�����(�������ݾ���˫���Ե�ģʽ)
}GiftPostmanStatusNUM;


//------------------------------------------
//<�ṹ����>MiaoGiftUnit_t</�ṹ����>
//<�ṹ�幦��>���ﵥԪ�ռ� ����һƬ˽�����ⲿ��Ԫ</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_GIFT_UNIT_STRUCT{
	
	MiaoGiftExternUnit_t ExternUnit;//�����ⲿ��Ԫ
	MiaoGiftUnitUN PrivateUnit;//����˽�е�Ԫ
	
	
}MiaoGiftUnit_t;


//------------------------------------------
//<�ṹ����>MiaoGiftPostman_t</�ṹ����>
//<�ṹ�幦��>���Ա �ṹ�� ����Ϊ˫������Ľڵ�</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_GIFT_POSTMAN_STRUCT{

	MiaoToolDListNode_t node;
	
	lw_u8 GroupFlag;
	
	lw_u8 PostmanID;

	struct MIAO_TASK_STRUCT *pSource,*pAim;//ָ��Դ������Ŀ�������ָ�� ��ʱ����
	
	struct MIAO_MEMORY_BASIC_STRUCT *pmem;
	
	GiftPostmanStatusNUM Status,Method;//���Ա��ǰ״̬��Ŀ�� ��Ŀ����״̬��ͬʱ ��ʾ���Աʹ������
		
	struct MIAO_GIFT_TRANSMIT_STRUCT *pGT;//ָ���������͵�λ��ָ��


}MiaoGiftPostman_t;


//------------------------------------------
//<�ṹ����>MiaoGiftReceive_t</�ṹ����>
//<�ṹ�幦��>����������ṹ�� ����Ϊ˫������Ŀ��Ƶ�Ԫ</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_GIFT_RECEIVE_STRUCT{
	//��Ա����

	MiaoToolDList_t List;
	
	struct MIAO_GIFT_CONCIERGE_STRUCT *pGC;//ָ���Լ���������������ָ��
	
	//��Ա����
#if !(MIAO_SIMPLE_MODE)	
	struct MIAO_GIFT_POSTMAN_STRUCT *(*AppendFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr,struct MIAO_GIFT_POSTMAN_STRUCT *pgp);//׷�Ӻ���
	struct MIAO_GIFT_POSTMAN_STRUCT *(*MoveNextFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr);//�ƶ����� 
	struct MIAO_GIFT_POSTMAN_STRUCT *(*SlipNextFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr,struct MIAO_GIFT_POSTMAN_STRUCT **ppGP);//�������� 	
#endif
}MiaoGiftReceive_t;




//------------------------------------------
//<�ṹ����>MiaoGiftTransmit_t</�ṹ����>
//<�ṹ�幦��>���﷢�����ṹ��</�ṹ�幦��>
//�𽥷��� �����ڴ������ƴ���
//------------------------------------------
#if 0
typedef struct MIAO_GIFT_TRANSMIT_STRUCT{
	
	MiaoGiftPostman_t PostmanList[MIAO_GIFT_POSTMAN_MAX];//���Ա�б� �洢һ���ֿ��Ա
	
	union MIAO_CORE_GROUP_UNION PostmanFlag;//״̬��־�� Ϊ0��ʾ������� һλ����һ�����Ա 
	
	struct MIAO_GIFT_CONCIERGE_STRUCT *pGC;	//ָ���Լ���������������ָ��
#if !(MIAO_SIMPLE_MODE)		
	MiaoGiftPostman_t* (*IdleFun)(struct MIAO_GIFT_TRANSMIT_STRUCT *pgt);
#endif	
}MiaoGiftTransmit_t;
#endif // 0

//------------------------------------------
//<�ṹ����>MiaoGiftConcierge_t</�ṹ����>
//<�ṹ�幦��>���������ṹ��</�ṹ�幦��>
//------------------------------------------
typedef struct MIAO_GIFT_CONCIERGE_STRUCT{

	
	MiaoGiftReceive_t giftR;//����
	//MiaoGiftTransmit_t giftT;//����

	struct MIAO_TASK_STRUCT *pta;//ָ���Լ����������ָ��
	
}MiaoGiftConcierge_t;


//��������
void MiaoGiftConciergeInitial(MiaoGiftConcierge_t *pgc);

//�𽥷��� �����ڴ������ƴ���
#if 0
void MiaoGiftTransmitInitial(MiaoGiftTransmit_t *pgt);
MiaoGiftPostman_t * MiaoGiftTransmitIdle(MiaoGiftTransmit_t *pgt);
#endif


MiaoGiftPostman_t * MiaoGiftReceiveAppend(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t *pgp);
MiaoGiftPostman_t * MiaoGiftReceiveMoveNext(MiaoGiftReceive_t *pgr);
MiaoGiftPostman_t * MiaoGiftReceiveSlipNext(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP);
MiaoGiftPostman_t * MiaoGiftReceiveForeah(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP);


MiaoGiftPostman_t * MiaoGiftAllocPostman(struct MIAO_MEMORY_BASIC_STRUCT *pmem);

#endif // !(MIAO_SIMPLE_LEVEL >= 3)


#define MIAO_GIFT_RECE_GIFT(PTA) do{MiaoTaskReceOnceGift(PTA);}while(0)

#define MIAO_GIFT_SIGNE(PGP) do{(PGP)->Method=(PGP)->Status;}while(0)

#define MIAO_GIFT_SEND_GIFT(PTA,PM) do{MiaoGiftReceiveAppend(&((PTA)->Concierge.giftR),(PM));}while(0)

#define MIAO_GIFT_GET_GIFT(PTA) (MiaoTaskGetOnceGift(PTA))

#define MIAO_GIFT_GET_MAN(PTA) ((MiaoGiftPostman_t *)(PTA->Concierge.giftR.List.Head.con.pNext))

#define MIAO_GIFT_NEXT(PTA) ((PTA)->Concierge.giftR.MoveNextFun(&(PTA->Concierge.giftR)))

#define MIAO_GIFT_NUM(PTA) (((PTA)->Concierge.giftR.List.NowLen) & 0xFFFE)

#define MIAO_GIFT_JUDGE_FLAG(PGP,NUM,COND) if((((PGP)->GroupFlag)!=NUM) && (COND)) continue;

#define MIAO_GIFT_SLIP(PTA,PGP) (MiaoGiftReceiveSlipNext(&(PTA->Concierge.giftR),&(PGP)))

#define MIAO_GIFT_SG_SF(PTA,PM,NUM) do{(PM)->GroupFlag=(NUM);MIAO_GIFT_SEND_GIFT((PTA),(PM));MIAO_FLAG_SET((PTA),(NUM));}while(0)

#define MIAO_FLAG_POP(PTA,NUM) ((PTA)->TaskGroupFlag.CurrentStatus.bits.bit##NUM)

#define MIAO_FLAG_GET(PTA,NUM) ((((PTA)->TaskGroupFlag.CurrentStatus.all)>>(NUM))&1)

#define MIAO_FLAG_CLR(PTA,NUM) do {(PTA)->TaskGroupFlag.CurrentStatus.all &= (~(1<<(NUM)));}while(0)

#define MIAO_FLAG_SET(PTA,NUM) do {(PTA)->TaskGroupFlag.CurrentStatus.all |= ((1<<(NUM)));}while(0)





#if MIAO_SIMPLE_LEVEL ==3
void * MiaoGiftReceiveMoveNext(MiaoToolDListNodeConnect_t *pgr);


//ȡ�����ֺ궨��
#undef MIAO_GIFT_SEND_GIFT
#undef MIAO_GIFT_GET_GIFT
#undef MIAO_GIFT_RECE_GIFT
#undef MIAO_GIFT_SIGNE
#undef MIAO_GIFT_SEND_GIFT
#undef MIAO_GIFT_GET_GIFT
#undef MIAO_GIFT_GET_MAN
#undef MIAO_GIFT_NEXT
#undef MIAO_GIFT_JUDGE_FLAG
#undef MIAO_GIFT_NUM




#define MIAO_GIFT_NEXT(PTA) (MiaoGiftReceiveMoveNext(&((PTA)->Concierge)))
#define MIAO_GIFT_GET_GIFT(PTA) ((void *)((lw_u8 *)((PTA)->Concierge.pNext) + (sizeof(MiaoToolDListNodeConnect_t))))
#define MIAO_GIFT_GET_MAN(PTA) ((void *)(PTA)->Concierge.pNext)


#endif

typedef MiaoGiftPostman_t LwPostman;

#define LwGetMan MIAO_GIFT_GET_MAN 
#define LwGetGift(PGP) ((PGP)->node.Data.pData) 
#define LwSignGift MIAO_GIFT_SIGNE
#define LwNextGift MIAO_GIFT_SLIP
#define LwCheckGift MIAO_GIFT_JUDGE_FLAG
#define LwSendGift MIAO_GIFT_SEND_GIFT

#define LwCallPost(PMEM) MiaoGiftAllocPostman((void *)((PMEM)))

#define LwForeachGift(PTA,PGP) for((PGP)=((MiaoGiftPostman_t *)&((PTA)->Concierge.giftR.List.Head));MiaoGiftReceiveForeah(&((PTA)->Concierge.giftR),&(PGP));)

#define LwSetFlag MIAO_FLAG_SET
#define LwClearFlag MIAO_FLAG_CLR
#define LwGetFlag MIAO_FLAG_GET
#define LwPopFlag MIAO_FLAG_POP

#define LwSendGiFg MIAO_GIFT_SG_SF

#endif /*(Ver.=0.96) MIAO_GIFT_H_ */
