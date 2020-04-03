/*(Ver.=0.96)
 * MIAO_show.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef MIAO_SHOW_H_
#define MIAO_SHOW_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"

//V1.0
typedef enum{

	
	MiaoShowDateStatus_Display,
	MiaoShowDateStatus_Change
	
}MiaoShowDateStatusNUM ;


typedef enum{
	MiaoShowUpdateMethod_OnlyDate,
	MiaoShowUpdateMethod_OnlyCursor,
	MiaoShowUpdateMethod_All
}MiaoShowUpdateMethodNUM ;

typedef enum{
	MiaoShowCursorStatus_Free,
	MiaoShowCursorStatus_Lock
}MiaoShowCursorStatusNUM ;

typedef struct MIAO_SHOW_CURSOR_STRUCT{
		
	MiaoShowCursorStatusNUM Status;
	
	lw_16 Position_forList,Position_forScreen,Position_forScreenLast;
	
	lw_u8 Position_forPage;
	
	struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC;
	
	
}MiaoShowCursor_t;


typedef struct MIAO_SHOW_PRIVATE_PARA_CTRL_STRUCT{
	MiaoParameter_t **ppPHead;
	lw_u8 HeadIndex;		
	MiaoParaCtrl_t *pPC;
}MiaoShowPvtParaCtrl_t;


typedef struct MIAO_SHOW_EXTERN_PARA_DATA_STRUCT{
	lw_u16 ID;
	struct MIAO_SHOW_EXTERN_PARA_DATA_STRUCT *pLast,*pNext;	
	MiaoExternMemory_t mem;
	
	void (*DrawFun)(struct MIAO_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf);
	void (*ShowIdFun)(struct MIAO_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf);
}MiaoShowExtParaData_t;


typedef struct MIAO_SHOW_EXTERN_PARA_CTRL_STRUCT{
	MiaoShowExtParaData_t Head,*pTail,*pNow;
	lw_u16 Length,NowId,LengthMax;
	
	MiaoShowExtParaData_t *(*AppendFun)(struct MIAO_SHOW_EXTERN_PARA_CTRL_STRUCT *pSEPC,MiaoShowExtParaData_t *pSEPD);
	
	MiaoShowExtParaData_t *(*DeleteFun)(struct MIAO_SHOW_EXTERN_PARA_CTRL_STRUCT *pSEPC,lw_u16 id);
	
}MiaoShowExtParaCtrl_t;


typedef struct MIAO_SHOW_PARA_CTRL_STRUCT{
	lw_u8 ID;
	MiaoCoreGroupUN TypeFlag;
	union{
		MiaoShowPvtParaCtrl_t PvtCtrl;
		MiaoShowExtParaCtrl_t ExtCtrl;
	}ParaCtrl;
	
	lw_u8 ShowLength;
	struct MIAO_SHOW_STRUCT *pS;
	
	lw_u16 *pParaLength,*pHeadId;
	
	void *(*HeadMoveNextFun)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_16 len,void *pbuf);
	
	void (*DrawMoreFun)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);
	
	void (*DrawCursorFun)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,MiaoShowCursor_t *pSC);	
	
	void (*CursorLockFun)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
	void (*CursorUnlockFun)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
	void (*CursorInput)(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
}MiaoShowParaCtrl_t;


typedef struct MIAO_SHOW_STRUCT{
	
	
	MiaoShowCursor_t Cursor;
	
	MiaoShowParaCtrl_t PageList[MIAO_SHOW_PAGE_LIST_LEN];
	
	lw_u8 length;
	
	#if MIAO_SHOW_SCREEN == MIAO_SHOW_NO_KEEP
	
	lw_u8 ShowBuffer[MIAO_SHOW_BUFFER_LEN];
	
	#endif
	
	void (*UpdateFun)(struct MIAO_SHOW_STRUCT *pS);
	
	void (*InforProcFun)(struct MIAO_SHOW_STRUCT *pS,lw_u8 ch);
		
	void (*PageTrunFun)(struct MIAO_SHOW_STRUCT *pS,lw_u8 *ppageid);
	
	void (*CursorMoveFun)(struct MIAO_SHOW_STRUCT *pS,lw_8 dir);
	
	void (*DrawPageNumFun)(struct MIAO_SHOW_STRUCT *pS);
	
	struct MIAO_CORE_STRUCT *pOS;
	
}MiaoShow_t;


extern struct MIAO_TASK_STRUCT MiaoInlineTaskShow,*pta_Show;
extern struct MIAO_TIMER_STRUCT MiaoTimerShowUpdate;

#if MIAO_SHOW_SCREEN==MIAO_SHOW_NO_KEEP
extern struct MIAO_TIMER_STRUCT MiaoTimerShowIrp;
#endif


MiaoShowExtParaData_t *MiaoShowExtParaAppendFun(MiaoShowExtParaCtrl_t *pSEPC,MiaoShowExtParaData_t *pSEPD);
MiaoShowExtParaData_t *MiaoShowExtParaDeleteFun(MiaoShowExtParaCtrl_t *pSEPC,lw_u16 id);

void MiaoShowParaCtrlDrawMore_Pvt(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);
void MiaoShowParaCtrlDrawMore_Ext(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);

void *MiaoShowParaCtrlHeadMove_Pvt(MiaoShowParaCtrl_t *pSPC,lw_16 len,void *pbuf);
void *MiaoShowParaCtrlHeadMove_Ext(MiaoShowParaCtrl_t *pSPC,lw_16 len,void *pbuf);



void MiaoShowParaCtrlCursorLock_Pvt(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
void MiaoShowParaCtrlCursorUnLock_Pvt(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
void MiaoShowParaCtrlCursorInput_Pvt(struct MIAO_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);



void MiaoShowPageTrun(struct MIAO_SHOW_STRUCT *pS,lw_u8 *ppageid);
void MiaoShowCursorMoveFun(struct MIAO_SHOW_STRUCT *pS,lw_8 dir);
void MiaoShowInforProc(MiaoShow_t *pS,lw_u8 ch);
void MiaoShowUpdate(MiaoShow_t *pS);


void MiaoShowParaCtrlInitial(MiaoShowParaCtrl_t *pSPC,MiaoShow_t *pS,lw_u8 showlen,
	lw_u8 len,void *arr,
	void *lock,void *unlock,void *input,	
	void *drawdata,void *showid,void *drawcursor);

extern void MiaoDriverShow_Clr(MiaoShow_t *pS,lw_u8 cmd,void *pbuf);

extern void MiaoDriverShowDrawParaDate(MiaoShowPvtParaCtrl_t *psppc,lw_u8 pos,lw_u8 len,MiaoParameter_t *pp,void *pbuf);
extern void MiaoDriverShowDrawParaId(MiaoShowPvtParaCtrl_t *psppc,lw_u8 pos,lw_u8 len,MiaoParameter_t *pp);
extern void MiaoDriverShowDrawCursor(MiaoShowParaCtrl_t *pSPC,MiaoShowCursor_t *pSC);
extern void MiaoDriverShowDrawPageNum(MiaoShow_t *pS);

extern void MiaoTimerShowUpdateALFun(struct MIAO_TIMER_STRUCT *pt);
extern void MiaoTimerShowIrpALFun(struct MIAO_TIMER_STRUCT *pt);

	
	
#define MIAO_DRIVER_SHOW_EXT_FUN_DEFAULT(FUN_NAME) void \
	FUN_NAME(struct MIAO_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf)

#endif /*(Ver.=0.96) MIAO_SHOW_H_ */
