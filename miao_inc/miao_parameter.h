/*(Ver.=0.96)
 * MIAO_parameter.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_PARAMETER_H_
#define MIAO_PARAMETER_H_

#include "OSLW_define.h"
/*(Ver.=0.96)

核心理念：
参数: 数据
操作: 动作列表（I/O）
命令: 执行相应动作

*/



//<宏组说明>数据类型定义 定义参数所用的类型</宏组说明>
//#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
//	 typedef fixpoint ParaType;
//#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT
//	 typedef float ParaType;
//#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
//	 typedef double ParaType;
//#else
//	#error "MATH TYPE IS NOT DEFINED"
//#endif


//------------------------------------------
//<结构体名>MiaoParameterData_t</结构体名>
//<成员变量>
//Parameter:实际参数
//ParameterShallow:实际参数的影子寄存器
//delt:参数的增量
//Q:格式化因子 用于格式化
//</成员变量>
//------------------------------------------
typedef struct MIAO_PARAMETER_DATA_STRUCT{
	
	ParaType Parameter;
	ParaType ParameterShallow;
	ParaUnitStatusNUM ParaStatus;
	
	ParaType delt;
	ParaType F_b,F_k;
	ParaType ParaMax,ParaMin;
	
	lw_u8 Q;
	ParaType *pExtParameter;
		
}MiaoParameterData_t;


typedef struct MIAO_PARAMETER_ORDER_STRUCT{
	union{
		ParaType real_data;
		lw_u8 para_arr[5];
		lw_8 *pstr;
		MiaoExternMemory_t ext;		
	}data;
	
	MiaoParaOrderNUM order;
	
}MiaoParameterOrder_t;

typedef struct MIAO_PARAMETER_ORDER_LIST_STRUCT{

	lw_u8 RowId;
	lw_u8 OrderDir;
	MiaoParameterOrder_t UOrder;
	MiaoParaListOrderNUM LOrder;
	MiaoExternMemory_t ExtMem;
	
}MiaoParameterListOrder_t;

#define MIAO_LIST_ORDER_GET 0

#define MIAO_LIST_ORDER_SET 1

typedef	union MIAO_PARAMETER_DATA_UNION{
		MiaoParameterData_t RealPara;
		MiaoExternMemory_t ImagPara;
}MiaoParameterDataUN;

typedef struct MIAO_PARAMETER_EXE_STRUCT{
	lw_u8 len;
	void* (*(*ExeFunList))(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
}MiaoParameterExe_t;

typedef struct MIAO_PARAMETER_STRUCT{
	
	lw_u16 ParaId;
	lw_u8 *pParaName;	

	MiaoParameterDataUN data;
	
}MiaoParameter_t;


typedef struct MIAO_PARA_CONTROL_STRUCT{
	lw_8 Name[MIAO_PAPA_CTRL_NAME_LENGTH];
	lw_u8 PageId;
	lw_u8 ParaLength;
	MiaoParameter_t *pParaList[MIAO_PARA_LIST_LENGRH];
	struct MIAO_CORE_STRUCT *pOS;
	
	MiaoParameterExe_t ParaGetF;
	MiaoParameterExe_t ParaSetF;
	
}MiaoParaCtrl_t;

typedef MiaoParaCtrl_t MiaoParaListCtrl_t;
typedef void* (*pMiaoExeFun)(MiaoParameterDataUN*,MiaoParameterOrder_t*,void*);

extern const pMiaoExeFun MiaoRealGetFunList[];
extern const pMiaoExeFun MiaoRealSetFunList[];

//函数声明
/*(Ver.=0.96)
MiaoParameter_t *MiaoParameter_GetPrecision(MiaoParameter_t *p,lw_u8 arr[5],lw_u8 aim);
MiaoParameter_t *MiaoParameter_GetNormal(MiaoParameter_t *p,lw_u8 *arr,lw_u8 aim);

MiaoParameter_t *MiaoParameter_SetPrecision(MiaoParameter_t *p,lw_u8 arr[5],lw_u8 aim);
MiaoParameter_t *MiaoParameter_SetNormal(MiaoParameter_t *p,lw_u8 *arr,lw_u8 aim);

MiaoParameter_t *MiaoParameter_SelfSL(MiaoParameter_t *p,lw_u8 cmd);
*/
#define MIAO_PARA_SELF_CMD_SAVE 0
#define MIAO_PARA_SELF_CMD_LOAD 1
/*(Ver.=0.96)
MiaoParameter_t *MiaoParameter_SelfUp(MiaoParameter_t *p,lw_u8 aim);
MiaoParameter_t *MiaoParameter_SelfDown(MiaoParameter_t *p,lw_u8 aim);
*/

void *MiaoParameter_GetPrecision(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_GetNormal(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_GetData(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);


void *MiaoParameter_SetPrecision(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_SetNormal(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_SetData(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_SelfUp(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_SelfDown(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_Lock(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);
void *MiaoParameter_Unlock(MiaoParameterDataUN *pPD,MiaoParameterOrder_t *pPO,void *pBuf);



lw_u8 MiaoRealParaListInitial(MiaoParaListCtrl_t *pPLC,lw_8 *str);



MiaoParaCtrl_t *MiaoParaCtrlCmdExe(MiaoParaCtrl_t *pPC,void *pGU);




#define MIAO_PARA_LIMIT(D,MA,MI) 	do{if((MA) != (MI))\
{\
	(D)=(D)>(MA)?(MA):(D);\
	(D)=(D)<(MI)?(MI):(D);	\
}}while(0)

#define MIAO_PARA_DEFAULT {0,0,0,0,0,0,0,0,0,NULL,NULL\
}
#define MIAO_PARA_INIT(PARAM,DELT,B,K,M,N,EXTP) {0,_ParaFrom(PARAM),0,_ParaFrom(DELT),_ParaFrom(B),_ParaFrom(K),_ParaFrom(M),_ParaFrom(N),15,NULL,EXTP\
}

#define MIAO_PAPA_GET_DATA(DATA) 	if(p->pExtParameter)\
		DATA=aim?(*(p->pExtParameter)):(p->ParameterShallow);\
	else\
		DATA=aim?(p->Parameter):(p->ParameterShallow)

#define MIAO_PAPA_GET_PDATA(PDATA) 	if(p->pExtParameter)\
		PDATA=aim?(p->pExtParameter):(&(p->ParameterShallow));\
	else\
		PDATA=aim?(&(p->Parameter)):(&(p->ParameterShallow))


#define MIAO_PAPA_JUDGE_DATA(DATA) 		if(pPD->RealPara.pExtParameter)\
		DATA=pPD->RealPara.ParaStatus?(pPD->RealPara.ParameterShallow):(*(pPD->RealPara.pExtParameter));\
	else\
		DATA=pPD->RealPara.ParaStatus?(pPD->RealPara.ParameterShallow):(pPD->RealPara.Parameter);


#define MIAO_PAPA_JUDGE_PDATA(PDATA) 	if(pPD->RealPara.pExtParameter)\
		PDATA=pPD->RealPara.ParaStatus?(&(pPD->RealPara.ParameterShallow)):(pPD->RealPara.pExtParameter);\
	else\
		PDATA=pPD->RealPara.ParaStatus?(&(pPD->RealPara.ParameterShallow)):(&(pPD->RealPara.Parameter));


#define MIAO_PARA_GET_FUN_READY() 	ParaType data;\
	if(pPD==NULL || pPO== NULL)\
	{\
		return NULL;\
	}\
	if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect)\
	{\
		return NULL;\
	}\
	MIAO_PAPA_JUDGE_DATA(data);

	
#define MIAO_PARA_SET_FUN_READY() 		\
	ParaType *pdata;\
	ParaType data;\
	if(pPD==NULL || pPO== NULL) return NULL;\
	if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect)return NULL;\
	MIAO_PAPA_JUDGE_PDATA(pdata);

	
#define OLSW_PARA_EXE_FUN_NAME(NAME) void*NAME(MiaoParameterDataUN*pPD,MiaoParameterOrder_t*pPO,void*pBuf)	
	
#endif /*(Ver.=0.96) MIAO_PARAMETER_H*/
