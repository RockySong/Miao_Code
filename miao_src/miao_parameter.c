/*(Ver.=0.96)
 * MIAO_parameter.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */
#include "OSLW_include.h"

#if 0

const pMiaoExeFun MiaoRealGetFunList[]= {MiaoParameter_GetNormal,MiaoParameter_GetPrecision,MiaoParameter_GetData};
const pMiaoExeFun MiaoRealSetFunList[]= {MiaoParameter_SetNormal,MiaoParameter_SetPrecision,MiaoParameter_SetData,MiaoParameter_SelfUp,MiaoParameter_SelfDown,MiaoParameter_Lock,MiaoParameter_Unlock};

/*(Ver.=0.96)
//------------------------------------------
//<函数名>MiaoParameter_GetPrecision</函数名>
//<功能说明>将parameter转换成高精度格式 采用定点数进行存储 一共5字节 实际数值=(arr[1]*2^24+arr[2]*2^16+arr[3]*2^8+arr[4])/(2^arr[0])</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标实际寄存器或者矫正寄存器</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

MiaoParameter_t *MiaoParameter_GetPrecision(MiaoParameter_t *p,lw_u8 arr[5],lw_u8 aim)
{

	ParaType data;
	fixpoint buf;
	if(p==NULL || arr== NULL)
	{
		return NULL;
	}
	MIAO_PAPA_GET_DATA(data);


	buf=_ParaInt(_ParaMul(_ParaAdd(data,p->F_b),p->F_k));


#if MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_DOUBLE
	buf=(fixpoint)(data*(double)(1<<p->Q));
#elif MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_FLOAT
	buf=(fixpoint)(data*(float)(1<<p->Q));
#elif MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_Q
	buf=FP_A_TO_FP_B(MIAO_GLOBAL_MATH_Q_FORM,(p->Q),(data));
#else
	buf=0;
#endif
	*arr++=p->Q;
	*arr++=((buf&0xff000000)>>24);
	*arr++=((buf&0x00ff0000)>>16);
	*arr++=((buf&0x0000ff00)>>8);
	*arr++=((buf&0x000000ff)>>0);
	return p;
}


//------------------------------------------
//<函数名>MiaoParameter_GetNormal</函数名>
//<功能说明>将parameter转换成普通格式 数值=(参数+F_b)*F_k 再由MIAO_PARA_NORMAL_LENGTH确定格式化之后的长度</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标实际寄存器或者影子寄存器</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

MiaoParameter_t *MiaoParameter_GetNormal(MiaoParameter_t *p,lw_u8 *arr,lw_u8 aim)
{
	ParaType data;
	lw_32 buf;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}
	MIAO_PAPA_GET_DATA(data);
	buf=_ParaInt(_ParaMul(_ParaAdd(data,p->F_b),p->F_k));

#if MIAO_PARA_NORMAL_LENGTH >= 4
	*arr++ = (buf&0xff000000)>>24;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 3
	*arr++ = (buf&0x00ff0000)>>16;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 2
	*arr++ = (buf&0x0000ff00)>>8;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 1
	*arr++ = (buf&0x000000ff);
#endif
#if  MIAO_PARA_NORMAL_LENGTH<=0
	#error "OSLW PARA NORMAL LENGTH is too short"
#endif
	return p;
}

//------------------------------------------
//<函数名>MiaoParameter_SetPrecision</函数名>
//<功能说明>MiaoParameter_GetPrecision相反</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
MiaoParameter_t *MiaoParameter_SetPrecision(MiaoParameter_t *p,lw_u8 arr[5],lw_u8 aim)
{
	ParaType *pdata;
	lw_64 buf=0;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}

	MIAO_PAPA_GET_PDATA(pdata);

	buf|=(((lw_64)arr[1]<<24)|((lw_64)arr[2]<<16)|((lw_64)arr[3]<<8)|((lw_64)arr[4]));
	*pdata=_ParaDiv(_ParaFint(buf) , (_ParaFint(((lw_32)1<<arr[0]))));

	return p;
}


//------------------------------------------
//<函数名>MiaoParameter_SetNormal</函数名>
//<功能说明>MiaoParameter_GetNormal相反</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
MiaoParameter_t *MiaoParameter_SetNormal(MiaoParameter_t *p,lw_u8 *arr,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;
	lw_64 buf=0;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}

	MIAO_PAPA_GET_PDATA(pdata);

#if MIAO_PARA_NORMAL_LENGTH >= 4
	buf|=((lw_32)(*arr++))<<24;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 3
	buf|=((lw_32)(*arr++))<<16;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 2
	buf|=((lw_32)(*arr++))<<8;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 1
	buf|=((lw_32)(*arr++));
#endif
	data=_ParaSub(_ParaDiv(_ParaFint(buf),p->F_k),p->F_b);

	MIAO_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;

	return p;
}

//------------------------------------------
//<函数名>MiaoParameter_set</函数名>
//<功能说明>将参数影子寄存器存入实际参数寄存器</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
MiaoParameter_t *MiaoParameter_SelfSL(MiaoParameter_t *p,lw_u8 cmd)
{
	ParaType *pdata;
	MIAO_assert(!p);
	pdata=(p->pExtParameter)?p->pExtParameter:&(p->Parameter);

	if(cmd)
		*pdata=p->ParameterShallow;
	else
		p->ParameterShallow=*pdata;
	return p;
}

//------------------------------------------
//<函数名>MiaoParameter_SelfUp</函数名>
//<功能说明>参数自增 parameter=parameter+delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

MiaoParameter_t *MiaoParameter_SelfUp(MiaoParameter_t *p,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;



	if(p==NULL)
	{
		return NULL;
	}

	MIAO_PAPA_GET_PDATA(pdata);

	data=_ParaAdd((*pdata),p->delt);

	MIAO_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;
	return p;
}

//------------------------------------------
//<函数名>MiaoParameter_SelfDown</函数名>
//<功能说明>参数自增 parameter=parameter-delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
MiaoParameter_t *MiaoParameter_SelfDown(MiaoParameter_t *p,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;

	if(p==NULL)
	{
		return NULL;
	}

	MIAO_PAPA_GET_PDATA(pdata);

	data=_ParaSub((*pdata),p->delt);

	MIAO_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;

	return p;
}
*/


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_GetPrecision)
{

    fixpoint buf;
    lw_u8 *arr;
    MIAO_PARA_GET_FUN_READY();
    arr=pPO->data.para_arr;
    buf=_ParaInt(_ParaMul(_ParaAdd(data,pPD->RealPara.F_b),pPD->RealPara.F_k));


#if MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_DOUBLE
    buf=(fixpoint)(data*(double)(1<<pPD->RealPara.Q));
#elif MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_FLOAT
    buf=(fixpoint)(data*(float)(1<<pPD->RealPara.Q));
#elif MIAO_GLOBAL_MATH_TYPE == MIAO_GLOBAL_MATH_Q
    buf=FP_A_TO_FP_B(MIAO_GLOBAL_MATH_Q_FORM,(p->Q),(data));
#else
    buf=0;
#endif
    *arr++=pPD->RealPara.Q;
    *arr++=((buf&0xff000000)>>24);
    *arr++=((buf&0x00ff0000)>>16);
    *arr++=((buf&0x0000ff00)>>8);
    *arr++=((buf&0x000000ff)>>0);

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_GetNormal)
{

    lw_32 buf;
    lw_u8 *arr;
    MIAO_PARA_GET_FUN_READY();
    arr=pPO->data.para_arr;
    buf=_ParaInt(_ParaMul(_ParaAdd(data,pPD->RealPara.F_b),pPD->RealPara.F_k));

#if MIAO_PARA_NORMAL_LENGTH >= 4
    *arr++ = (buf&0xff000000)>>24;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 3
    *arr++ = (buf&0x00ff0000)>>16;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 2
    *arr++ = (buf&0x0000ff00)>>8;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 1
    *arr++ = (buf&0x000000ff);
#endif
#if  MIAO_PARA_NORMAL_LENGTH<=0
#error "OSLW PARA NORMAL LENGTH is too short"
#endif
    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_SetPrecision)
{
    lw_64 buf=0;
    lw_u8 *arr;
    MIAO_PARA_SET_FUN_READY();
    arr=pPO->data.para_arr;

    buf|=(((lw_64)arr[1]<<24)|((lw_64)arr[2]<<16)|((lw_64)arr[3]<<8)|((lw_64)arr[4]));
    *pdata=_ParaDiv(_ParaFint(buf), (_ParaFint(((lw_32)1<<arr[0]))));

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_SetNormal)
{

    lw_64 buf=0;
    lw_u8 *arr;
    MIAO_PARA_SET_FUN_READY();
    arr=pPO->data.para_arr;


#if MIAO_PARA_NORMAL_LENGTH >= 4
    buf|=((lw_32)(*arr++))<<24;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 3
    buf|=((lw_32)(*arr++))<<16;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 2
    buf|=((lw_32)(*arr++))<<8;
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 1
    buf|=((lw_32)(*arr++));
#endif

    data=_ParaSub(_ParaDiv(_ParaFint(buf),pPD->RealPara.F_k),pPD->RealPara.F_b);

    MIAO_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_GetData)
{
    MIAO_PARA_GET_FUN_READY();

    pPO->data.real_data=data;

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(MiaoParameter_SetData)
{
    MIAO_PARA_SET_FUN_READY();

    data=pPO->data.real_data;

    MIAO_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(MiaoParameter_SelfUp)
{
    MIAO_PARA_SET_FUN_READY();

    data=_ParaAdd(*pdata,pPD->RealPara.delt);

    MIAO_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(MiaoParameter_SelfDown)
{
    MIAO_PARA_SET_FUN_READY();

    data=_ParaSub(*pdata,pPD->RealPara.delt);

    MIAO_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(MiaoParameter_Lock)
{
    ParaType data;
    if(pPD==NULL || pPO== NULL)
    {
        return NULL;
    }
    if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect || pPD->RealPara.ParaStatus == ParaUnitStatus_Lock)
    {
        return NULL;
    }

    data=(pPD->RealPara.pExtParameter)?(*(pPD->RealPara.pExtParameter)):pPD->RealPara.Parameter;
    pPD->RealPara.ParaStatus=ParaUnitStatus_Lock;
    pPD->RealPara.ParameterShallow=data;
    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(MiaoParameter_Unlock)
{
    ParaType *pdata;
    if(pPD==NULL || pPO== NULL)
    {
        return NULL;
    }
    if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect || pPD->RealPara.ParaStatus == ParaUnitStatus_Free)
    {
        return NULL;
    }

    pdata=(pPD->RealPara.pExtParameter)?((pPD->RealPara.pExtParameter)):(&(pPD->RealPara.Parameter));

    pPD->RealPara.ParaStatus=ParaUnitStatus_Free;
    if(pPO->order == ParaOrder_I_UnlockSave)
        *pdata=pPD->RealPara.ParameterShallow;

    return pPD;


}


void* MiaoParaListCtrlFun(MiaoParaListCtrl_t *pPLC,MiaoParameterListOrder_t *pPLO)
{
    MiaoParameter_t *pP;
    MiaoParameterDataUN *pPD;
    MiaoParameterOrder_t *pPO;
    MiaoParameterExe_t *pPE;
    lw_8 i=0,n;
    lw_u8 *arr;

    MIAO_assert(!(pPLC));
    MIAO_assert(!(pPLO));

    if(pPLO->RowId > pPLC->ParaLength)
    {
        return NULL;
    }
    else
    {
        pP=pPLC->pParaList[pPLO->RowId];
    }

    if(pPLO->OrderDir == MIAO_LIST_ORDER_GET)//判断指令方向
    {
        pPE=&pPLC->ParaGetF;
    }
    else
    {
        pPE=&pPLC->ParaSetF;
    }


    switch(pPLO->LOrder)
    {
    case ParaLOrder_Once:

        pPD=&(pP->data);
        pPO=&(pPLO->UOrder);

        if(pPLO->UOrder.order < pPE->len )
        {
            pPE->ExeFunList[pPLO->UOrder.order](pPD,pPO,NULL);
        }

        break;

    case ParaLOrder_More:
        if(pPLO->ExtMem.pData && (pPLO->UOrder.order) < pPE->len && (pPLO->RowId + pPLO->ExtMem.uData) <= pPLC->ParaLength)
        {
            arr=pPLO->ExtMem.pData;
            n=pPLO->RowId+pPLO->ExtMem.uData/MIAO_PARA_NORMAL_LENGTH;
            pPO=&(pPLO->UOrder);
            for(i=pPLO->RowId; i<n; i++)
            {
                pPE->ExeFunList[pPLO->UOrder.order](&(pPLC->pParaList[i]->data),pPO,NULL);
#if MIAO_PARA_NORMAL_LENGTH >= 4
                *arr++ = pPO->data.para_arr[3];
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 3
                *arr++ = pPO->data.para_arr[2];
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 2
                *arr++ = pPO->data.para_arr[1];
#endif
#if MIAO_PARA_NORMAL_LENGTH >= 1
                *arr++ = pPO->data.para_arr[0];
#endif
            }

        }

        break;

    default:

        break;


    }
	return pPLC;
}



lw_u8 MiaoRealParaInitial(MiaoParameter_t *pP,lw_u8 *str,lw_df data,ParaType *pExt,lw_u8 num,...)
{

    va_list ap;
    ParaType *pp;
    MIAO_assert(!(pP));

    memset(pP,0,sizeof(MiaoParameter_t));

    pP->data.RealPara.ParaStatus=ParaUnitStatus_Protect;

    pp=&pP->data.RealPara.delt;

    pP->data.RealPara.ParameterShallow = pP->data.RealPara.Parameter=_ParaFrom(data);
    pP->data.RealPara.pExtParameter=pExt;



    va_start(ap, num);

    if(num>=0 && num<=5)
    {
        while(num--)
        {
            *pp++=_ParaFrom(va_arg(ap,lw_df));
        }
    }
    pP->data.RealPara.Q=15;


    va_end(ap);

    pP->data.RealPara.ParaStatus=ParaUnitStatus_Free;

    return 1;

}



lw_u8 MiaoRealParaListInitial(MiaoParaListCtrl_t *pPLC,lw_8 *str)
{

    MIAO_assert(!pPLC);
    memset(pPLC,0,sizeof(MiaoParaListCtrl_t));
    //名字copy
    strncpy(pPLC->Name,str,sizeof(lw_8)*MIAO_PAPA_CTRL_NAME_LENGTH);

    //函数列表初始化
    pPLC->ParaGetF.ExeFunList=(void *)MiaoRealGetFunList;
    pPLC->ParaGetF.len=sizeof(MiaoRealGetFunList)/sizeof(pMiaoExeFun);
    pPLC->ParaGetF.ExeFunList=(void *)MiaoRealSetFunList;
    pPLC->ParaGetF.len=sizeof(MiaoRealSetFunList)/sizeof(pMiaoExeFun);


    return 1;
}

#endif
