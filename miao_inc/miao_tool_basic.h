/*(Ver.=0.96)
 * MIAO_tool.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef MIAO_TOOL_BASIC_H_
#define MIAO_TOOL_BASIC_H_


#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_memory.h"
#include "OSLW_tool_define.h"

#if MIAO_TOOL_IMPORT_SLIP_WIN|| MIAO_TOOL_IMPORT_ALL
//--------------------
//<����>
//pMasterData �����ݳ�
//pBufferData �������ݳ�
//MDataLen_forSize_t �����ݶ���size_t�Ĵ�С
//BufferLength �������Ѵ����ݴ�С!!!!!!!!!�û����뱣֤�������㹻!!!!!!!!!!!!!
//MDataLockFlag �����������־ ���Ϊ1 ��ʾ���������� �������뻺����
//AppendFun ��Ա���� psw:this pd:�����׵�ַ num:���ݳ���
//</����>
//--------------------
typedef struct MIAO_TOOL_SLIPWIN_STRUCT{

	void *pMasterData;
	void *pBufferData;

	uint32_t MDataLen_forSize_t;
	
	
	lw_u16 BufferLength;	
	lw_u8 MDataLockFlag;
	
	struct MIAO_TOOL_SLIPWIN_STRUCT* (*AppendFun)(struct MIAO_TOOL_SLIPWIN_STRUCT *psw,void *pd,lw_u8 num);

}  MiaoToolSlipwin_t;

MiaoToolSlipwin_t* MiaoToolSlipwinAppend(MiaoToolSlipwin_t *psw,void *pd,lw_u8 num);


#define MIAO_TOOL_SLIPWIN_DEFAULT(MD,BUF,DTYPE,LEN) {(MD),(BUF),(sizeof(DTYPE)*LEN),0,0,MiaoToolSlipwinAppend};

#define MIAO_TOOL_MEMCPY(P,Q,N) do {while((N)--)*(P)++=*(Q)++;}while(0)

#endif //MIAO_TOOL_IMPORT_SLIP_WIN


#if MIAO_TOOL_IMPORT_DLIST|| MIAO_TOOL_IMPORT_ALL
//-------------------
//<����ڵ�>
//</����ڵ�>
//--------------------
typedef struct MIAO_TOOL_DLIST_NODE_STRUCT{

	MiaoToolDListNodeConnect_t con;
	MiaoExternMemory_t Key;
	MiaoExternMemory_t Data;
	
}MiaoToolDListNode_t;


typedef enum{
	MiaoToolDListIndexMode_byNum=0,
	MiaoToolDListIndexMode_byId=1

}MiaoToolDListIndexModeNUM;

typedef lw_8(*pMiaoDlistCmpFun)(MiaoToolDListNode_t *pDLN, lw_u16 data, lw_u16 count, void *pdata);
#define MIAO_TOOL_DLIST_FUN_PARA_DEFAULT pMiaoDlistCmpFun pCmpFun,lw_u16 data,void *pdata


//--------------------
//<˫������>
//</˫������>
//--------------------
typedef struct MIAO_TOOL_DLIST_STRUCT{
	MiaoToolDListNode_t Head,*pTail;
	lw_u16 MaxLen,NowLen;
	MiaoToolDListNode_t* (*IndexOfFUN)(struct MIAO_TOOL_DLIST_STRUCT *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
	MiaoToolDListNode_t* (*AppendFUN)(struct MIAO_TOOL_DLIST_STRUCT *pDL,MiaoToolDListNode_t *pDLN);
	MiaoToolDListNode_t* (*InsertFUN)(struct MIAO_TOOL_DLIST_STRUCT *pDL,MiaoToolDListNode_t *pDLN, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
	MiaoToolDListNode_t* (*RemoveFUN)(struct MIAO_TOOL_DLIST_STRUCT *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
	
}MiaoToolDList_t;

#define MIAO_TOOL_DLIST_FUN_PARA_DEFAULT pMiaoDlistCmpFun pCmpFun,lw_u16 data,void *pdata


#if !(MIAO_SIMPLE_LEVEL >= 4)
MiaoToolDListNode_t* MiaoToolDListIndexOf(MiaoToolDList_t *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
MiaoToolDListNode_t* MiaoToolDListAppend(struct MIAO_TOOL_DLIST_STRUCT *pDL, MiaoToolDListNode_t *pDLN);
MiaoToolDListNode_t* MiaoToolDListInsert(struct MIAO_TOOL_DLIST_STRUCT *pDL,MiaoToolDListNode_t *pDLN, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
MiaoToolDListNode_t* MiaoToolDListRemove(struct MIAO_TOOL_DLIST_STRUCT *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT);
MiaoToolDListNode_t* _MiaoToolDListRemoveNode (struct MIAO_TOOL_DLIST_STRUCT *pDL, MiaoToolDListNode_t *pDLNbuf);
#endif // !(MIAO_SIMPLE_LEVEL >= 4)


#define MIAO_TOOL_DLIST_NODE_CONNECT(P1,P2) 	(P1)->con.pNext=(MiaoToolDListNodeConnect_t *)(P2);/*(Ver.=0.96)β������һ��Ϊ�µ�*/\
(P2)->con.pLast = (MiaoToolDListNodeConnect_t *)(P1);/*(Ver.=0.96)�½ڵ����һ��Ϊԭ��β��*/

#define MIAO_TOOL_DLIST_NODE_INSERT(P1,P_INSERT) (P_INSERT)->con.pLast = (MiaoToolDListNodeConnect_t *)(P1);\
(P_INSERT)->con.pNext = (MiaoToolDListNodeConnect_t *)(P1)->con.pNext;\
(P1)->con.pNext->pLast = (MiaoToolDListNodeConnect_t *)(P_INSERT);\
(P1)->con.pNext = (MiaoToolDListNodeConnect_t *)(P_INSERT);

#define MIAO_TOOL_DLIST_NODE_DELECT(P) 			(P)->con.pLast->pNext=(P)->con.pNext;/*(Ver.=0.96)��һ������һ��Ϊ��һ��*/\
(P)->con.pNext->pLast = (P)->con.pLast;/*(Ver.=0.96)��һ������һ��Ϊ��һ��*/


void MiaoToolDListInitial(MiaoToolDList_t *pDL,lw_u8 len,void *p);

lw_8 MiaoToolDListCmpId(MiaoToolDListNode_t *pDLN, lw_u16 data, lw_u16 count, void *pdata);
lw_8 MiaoToolDListCmpCount(MiaoToolDListNode_t *pDLN, lw_u16 data, lw_u16 count, void *pdata);

#endif //MIAO_TOOL_IMPORT_DLIST



#if !(MIAO_SIMPLE_LEVEL >= 4)

#if MIAO_TOOL_IMPORT_HASH_LIST|| MIAO_TOOL_IMPORT_ALL

typedef lw_u8 (*HashCalFunType)(void *pData);
lw_u8 HashCalStringBKDR(void *pData);


typedef lw_8(*HashKeyCmpFunType)(void *p1, void *p2);
lw_8 HashCmpString(void *p1, void *p2);

//--------------------
//<��ϣ��>
//</��ϣ��>
//--------------------
typedef struct MIAO_TOOL_HASH_STRUCT {
	MiaoToolDListNode_t *pHashList[MIAO_TOOL_HASH_LIST_MAX];
	lw_u8 HashListLength;

	HashCalFunType HashCalFun;
	HashKeyCmpFunType HashKeyCmp;

	lw_u8(*SelectFun)(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult, MiaoToolDListNode_t **ppDLN);
	MiaoToolDListNode_t* (*AppendFun)(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, MiaoToolDListNode_t *pDLN);
	MiaoToolDListNode_t* (*DeleteFun)(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey);

}MiaoToolHash_t;


void MiaoToolHashInitial(MiaoToolHash_t *pH, lw_u8 length, HashCalFunType pHashFun, HashKeyCmpFunType pHashKeyCmpFun);
lw_u8 MiaoToolHashSelect(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult,MiaoToolDListNode_t **ppDLN);
MiaoToolDListNode_t* MiaoToolHashAppend(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, MiaoToolDListNode_t *pDLN);
MiaoToolDListNode_t* MiaoToolHashDelete(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey);

#endif //MIAO_TOOL_IMPORT_HASH_LIST


#if MIAO_TOOL_IMPORT_TABLE|| MIAO_TOOL_IMPORT_ALL
typedef struct MIAO_TOOL_TABLE_STRUCT{

	MiaoExternMemory_t Row;

	MiaoExternMemory_t Col;

	uint32_t ColSize;

	MiaoMemoryBasic_t *pmem;

	void *(*ReadFun)(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row,lw_32 col,lw_u16 *pLen);
	lw_32 (*WriteFun)(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len);
	
	
}MiaoToolTable_t;



void MiaoToolTableInitial(MiaoToolTable_t *pT, lw_u16 RowLen, lw_u16 *pCol, lw_u16 ColLen, MiaoMemoryBasic_t *pmem, void *pTable, uint32_t *pColReal);
void * MiaoToolTableRead(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, lw_u16 *pLen);
lw_32 MiaoToolTableWrite(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len);

#endif //MIAO_TOOL_IMPORT_TABLE

#endif // !(MIAO_SIMPLE_LEVEL >= 4)

#endif


