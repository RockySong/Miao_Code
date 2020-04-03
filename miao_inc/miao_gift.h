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
//<共用体名>MiaoGiftUnitUN</共用体名>
//<共用体功能>礼物模块私有单元 就有多重访问功能</共用体功能>
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
//<结构体名>MiaoGiftExternUnit_t</结构体名>
//<结构体功能>礼物外部存储空间</结构体功能>
//------------------------------------------
//typedef struct MIAO_GIFT_EXTERN_UNIT_STRUCT{
//	lw_16 Length;
//	void *arr;
//}MiaoGiftExternUnit_t;

typedef MiaoExternMemory_t MiaoGiftExternUnit_t;

//------------------------------------------
//<枚举>GiftUnitStatusNUM</枚举>
//<枚举功能>礼物状态类型</枚举功能>
//------------------------------------------
typedef enum{
	GiftUnitStatus_NAN=0,
	GiftUnitStatus_Free=1,
	GiftUnitStatus_InDelivery,
	GiftUnitStatus_Received
}GiftUnitStatusNUM;

//------------------------------------------
//<枚举>GiftPostmanStatusNUM</枚举>
//<枚举功能>快递员状态类型</枚举功能>
//------------------------------------------
typedef enum{

	GiftPostmanStatus_NAN=0,//为初始化
	GiftPostmanStatus_Free=1,//自由状态
	GiftPostmanStatus_InDelivery,//正在派送
	GiftPostmanStatus_Received,//已签收 表示目标任务已经接受
	GiftPostmanStatus_Return//返回 表示目标任务已经返回(用于数据具有双向性的模式)
}GiftPostmanStatusNUM;


//------------------------------------------
//<结构体名>MiaoGiftUnit_t</结构体名>
//<结构体功能>礼物单元空间 包含一片私有与外部单元</结构体功能>
//------------------------------------------
typedef struct MIAO_GIFT_UNIT_STRUCT{
	
	MiaoGiftExternUnit_t ExternUnit;//礼物外部单元
	MiaoGiftUnitUN PrivateUnit;//礼物私有单元
	
	
}MiaoGiftUnit_t;


//------------------------------------------
//<结构体名>MiaoGiftPostman_t</结构体名>
//<结构体功能>快递员 结构体 本质为双向链表的节点</结构体功能>
//------------------------------------------
typedef struct MIAO_GIFT_POSTMAN_STRUCT{

	MiaoToolDListNode_t node;
	
	lw_u8 GroupFlag;
	
	lw_u8 PostmanID;

	struct MIAO_TASK_STRUCT *pSource,*pAim;//指向源任务与目标任务的指针 暂时无用
	
	struct MIAO_MEMORY_BASIC_STRUCT *pmem;
	
	GiftPostmanStatusNUM Status,Method;//快递员当前状态与目标 当目标与状态相同时 表示快递员使命结束
		
	struct MIAO_GIFT_TRANSMIT_STRUCT *pGT;//指向所属发送单位的指针


}MiaoGiftPostman_t;


//------------------------------------------
//<结构体名>MiaoGiftReceive_t</结构体名>
//<结构体功能>礼物接收器结构体 本质为双向链表的控制单元</结构体功能>
//------------------------------------------
typedef struct MIAO_GIFT_RECEIVE_STRUCT{
	//成员变量

	MiaoToolDList_t List;
	
	struct MIAO_GIFT_CONCIERGE_STRUCT *pGC;//指向自己所属门卫机构的指针
	
	//成员函数
#if !(MIAO_SIMPLE_MODE)	
	struct MIAO_GIFT_POSTMAN_STRUCT *(*AppendFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr,struct MIAO_GIFT_POSTMAN_STRUCT *pgp);//追加函数
	struct MIAO_GIFT_POSTMAN_STRUCT *(*MoveNextFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr);//移动函数 
	struct MIAO_GIFT_POSTMAN_STRUCT *(*SlipNextFun)(struct MIAO_GIFT_RECEIVE_STRUCT *pgr,struct MIAO_GIFT_POSTMAN_STRUCT **ppGP);//滑动函数 	
#endif
}MiaoGiftReceive_t;




//------------------------------------------
//<结构体名>MiaoGiftTransmit_t</结构体名>
//<结构体功能>礼物发送器结构体</结构体功能>
//逐渐放弃 采用内存分配机制代替
//------------------------------------------
#if 0
typedef struct MIAO_GIFT_TRANSMIT_STRUCT{
	
	MiaoGiftPostman_t PostmanList[MIAO_GIFT_POSTMAN_MAX];//快递员列表 存储一部分快递员
	
	union MIAO_CORE_GROUP_UNION PostmanFlag;//状态标志组 为0表示发送完成 一位代表一个快递员 
	
	struct MIAO_GIFT_CONCIERGE_STRUCT *pGC;	//指向自己所属门卫机构的指针
#if !(MIAO_SIMPLE_MODE)		
	MiaoGiftPostman_t* (*IdleFun)(struct MIAO_GIFT_TRANSMIT_STRUCT *pgt);
#endif	
}MiaoGiftTransmit_t;
#endif // 0

//------------------------------------------
//<结构体名>MiaoGiftConcierge_t</结构体名>
//<结构体功能>门卫机构结构体</结构体功能>
//------------------------------------------
typedef struct MIAO_GIFT_CONCIERGE_STRUCT{

	
	MiaoGiftReceive_t giftR;//接收
	//MiaoGiftTransmit_t giftT;//发送

	struct MIAO_TASK_STRUCT *pta;//指向自己所属任务的指针
	
}MiaoGiftConcierge_t;


//函数声明
void MiaoGiftConciergeInitial(MiaoGiftConcierge_t *pgc);

//逐渐放弃 采用内存分配机制代替
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


//取消部分宏定义
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
