/*(Ver.=0.96)
 * MIAO_tool.c
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(MIAO_SIMPLE_LEVEL >= 2)


#if MIAO_TOOL_IMPORT_NN || MIAO_TOOL_IMPORT_ALL

#if 0



MiaoToolNNLayerActFun_t MiaoToolSigmoid= {MiaoToolNNSigmoid_Forward,MiaoToolNNSigmoid_Backward};
MiaoToolNNLayerActFun_t MiaoToolLinear= {MiaoToolNNLinear_Forward,MiaoToolNNLinear_Backward};
MiaoToolNNLayerActFun_t MiaoToolReLU= {MiaoToolNNReLU_Forward,MiaoToolNNReLU_Backward};
MiaoToolNNLayerActFun_t MiaoToolLReLU = { MiaoToolNNLeakyReLU_Forward,MiaoToolNNLeakyReLU_Backward,_ParaFrom(0.01) };
MiaoToolNNLayerActFun_t MiaoToolTanh = { MiaoToolNNTanh_Forward,MiaoToolNNTanh_Backward };
MiaoToolNNLayerActFun_t MiaoToolSwish = { MiaoToolNNSwish_Forward,MiaoToolNNSwish_Backward };


MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNSigmoid_Forward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNSigmoid_Forward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n = pIn->length;
        for(i=0; i<n; i++)  pOut->a[i]=_ParaDiv(_ParaFint(1),_ParaAdd(_ParaFint(1),_ParaExp(_ParaMul(pIn->a[i],_ParaFint(-1)))));
    }

    return pOut;


}

MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNSigmoid_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNSigmoid_Backward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n = pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=_ParaMul(pOut->a[i],_ParaSub(_ParaFint(1),pOut->a[i]));
    }

    return pOut;
}

MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNTanh_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNTanh_Forward)
{
	uint32_t n, i;
	ParaType _exp_x, _exp_nx;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			_exp_x = _ParaExp(pIn->a[i]);
			_exp_nx = _ParaExp(-(pIn->a[i]));
			pOut->a[i] = _ParaDiv(
				_ParaSub(_exp_x, _exp_nx),
				_ParaAdd(_exp_x, _exp_nx)
			);
		}
	}

	return pOut;

}

MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNTanh_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNTanh_Backward)
{
	uint32_t n, i;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) 
			pIn->a[i] = _ParaSub(
			_ParaFint(1), 
			_ParaMul(pOut->a[i], pOut->a[i])
		);
	}

	return pOut;
}


MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNLinear_Forward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNLinear_Forward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pOut->a[i]=pIn->a[i];
    }

    return pOut;


}


MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNLinear_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNLinear_Backward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=_ParaFint(1);
    }

    return pOut;
}


MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNReLU_Forward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNReLU_Forward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pOut->a[i]=pIn->a[i]>=_ParaFrom(0)?pIn->a[i]:_ParaFrom(0);
    }

    return pOut;


}

MIAO_TOOL_FUN
(MiaoMat*,MiaoToolNNReLU_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut),MiaoToolNNReLU_Backward)
{
    uint32_t n,i;
    MIAO_assert(!(pIn));
    MIAO_assert(!(pOut));
    if(MIAO_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=pOut->a[i]>=_ParaFrom(0)?_ParaFrom(1):_ParaFrom(0);
    }

    return pOut;
}


MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNLeakyReLU_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNReLU_Forward)
{
	uint32_t n, i;
	ParaType _k;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	_k = pAF->_p1;
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) pOut->a[i] = pIn->a[i] >= _ParaFrom(0) ? pIn->a[i] : _ParaMul(_k, pIn->a[i]);
	}

	return pOut;


}

MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNLeakyReLU_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNReLU_Backward)
{
	uint32_t n, i;
	ParaType _k;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	_k = pAF->_p1;
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) pIn->a[i] = pOut->a[i] >= _ParaFrom(0) ? _ParaFrom(1) : _k;
	}

	return pOut;
}


MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNSwish_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNSwish_Forward)
{
	uint32_t n, i;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			pOut->a[i] = _ParaMul(pIn->a[i], 
				_ParaDiv(
					_ParaFint(1), 
					_ParaAdd(
						_ParaFint(1), 
						_ParaExp(
							_ParaMul(
								pIn->a[i], _ParaFint(-1)
							)
						)
					)
				)
			);

		}
	}

	return pOut;


}

MIAO_TOOL_FUN
(MiaoMat*, MiaoToolNNSwish_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut), MiaoToolNNSwish_Backward)
{
	uint32_t n, i;
	ParaType _sigmoid_x;
	MIAO_assert(!(pIn));
	MIAO_assert(!(pOut));
	if (MIAO_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			_sigmoid_x= _ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMul(pIn->a[i], _ParaFint(-1)))));
			pIn->a[i] = _ParaAdd(pOut->a[i], _ParaMul(_sigmoid_x, _ParaSub(_ParaFint(1), pOut->a[i])));
		}
	}

	return pOut;
}
#endif // 0


#if MIAO_TOOL_IMPORT_NN_BPnn || MIAO_TOOL_IMPORT_ALL



#if 0



//网络层初始化函数
void MiaoToolBPnnLayerInitial(MiaoToolNNLayerFullCon_t *pNNL//网络层结构体
	, MiaoToolNNLayerActFun_t *pAF//激活函数
	, lw_u16 size_row, lw_u16 size_col//层大小
	, ParaType *pW//系数矩阵数据块
	, ParaType *pBias//偏质
	, MiaoToolRandomBasic_t *pTRB//随机数产生序列
	, ParaType rmax, ParaType rmin, ParaType nl)//范围 与 学习速率
{


	MIAO_assert(!(pNNL));
	MIAO_assert(!(pAF));

	memset(pNNL, 0, sizeof(MiaoToolNNLayerFullCon_t));

	pNNL->pActFun = pAF;
	pNNL->nl = nl;
	pNNL->pRand = pTRB;



	MiaoToolMatrixInitial(&pNNL->w, size_row, size_col, pW);

	pNNL->size_col = size_col;
	pNNL->size_row = size_row;

	MiaoToolMatrixRandomInitial(&pNNL->w, pTRB, rmax, rmin);

	if (pBias)
	{
		MiaoToolMatrixInitial(&pNNL->BiasUN.bias, 1, size_col, pBias);
		MiaoToolMatrixRandomInitial(&pNNL->BiasUN.bias, pTRB, rmax, rmin);

	}


}

//前向传播初始化
void MiaoToolBPnnLayerForwardInitial(MiaoToolNNLayerFullCon_t *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X)
{
	MIAO_assert(!(pNNL));
	MiaoToolMatrixInitial(&pNNL->in, 1, pNNL->w.row, pIN);
	MiaoToolMatrixInitial(&pNNL->out, 1, pNNL->w.col, pOUT);
	MiaoToolMatrixInitial(&pNNL->_out_x, 1, pNNL->w.col, pOUT_X);
}


//后向传播初始化
void MiaoToolBPnnLayerBackwardInitial(MiaoToolNNLayerFullCon_t *pNNL, ParaType *pINerr, ParaType *pdelt, ParaType *pOuterr)
{

	MIAO_assert(!(pNNL));
	MiaoToolMatrixInitial(&pNNL->inErr, 1, pNNL->w.row, pINerr);
	MiaoToolMatrixInitial(&pNNL->outErr, 1, pNNL->w.col, pOuterr);
	MiaoToolMatrixInitial(&pNNL->DeltaUN.delta, 1, pNNL->w.col, pdelt);

}


//BN初始化
/*
void MiaoToolBPnnLayerBatchNormInitial(MiaoToolNNLayerFullCon_t *pNNL, MiaoToolNNBatchNorm_t *pBN,ParaType epsi)
{

	MIAO_assert(!(pNNL));
	MIAO_assert(!(pBN));

	pNNL->pBN = pBN;
	memset(pBN, 0, sizeof(MiaoToolNNBatchNorm_t));
	
	pBN->BN_gamma = _ParaFint(1);
	pBN->BN_Epsi = epsi;
	
}
*/

/*
由于dropout 新版本舍弃
void MiaoToolBPnnLayerReSize(MiaoToolNNLayerFullCon_t *pNNL,lw_u16 r,lw_u16 c)
{

    MIAO_assert(!(pNNL));
    pMiaoToolMatrixReSize(&(pNNL->BiasUN.bias),1,c);
    pMiaoToolMatrixReSize(&(pNNL->delt),1,c);
    pMiaoToolMatrixReSize(&(pNNL->out),1,c);
    pMiaoToolMatrixReSize(&(pNNL->outErr),1,c);

    pMiaoToolMatrixReSize(&(pNNL->w),r,c);

    pMiaoToolMatrixReSize(&(pNNL->in),1,r);
    pMiaoToolMatrixReSize(&(pNNL->inErr),1,c);
    if(pNNL->BiasUN.bias.a)
    {
        pMiaoToolMatrixReSize(&(pNNL->BiasUN.bias),1,c);
    }

}
*/

/*
void  MiaoToolBPnnDropOutInitial(MiaoToolNNLayerFullCon_t *pNNL,ParaType P,ParaType *pWBuf,ParaType *pBiasUN.biasBuf,lw_u16 *pList1,lw_u16 *pList2)
{

    MIAO_assert(!(pNNL));
    MIAO_assert(!(pwbuf));
    MIAO_assert(!(pList1));
    MIAO_assert(!(pList2));


    pNNL->PDropOut=P;
    pNNL->pDropOutRow=pList1;
    pNNL->pDropOutCol=pList2;
    pNNL->pDropOutBiasUN.bias=pBiasUN.biasBuf;
    pNNL->pDropOutW=pWBuf;

}
*/

void  MiaoToolBPnnDropOutInitial(MiaoToolNNLayerFullCon_t *pNNL,MiaoToolNNDropOut_t *pdrop, ParaType P, ParaType *pDropList)
{

	MIAO_assert(!(pNNL));
	MIAO_assert(!(pdrop));

	pNNL->pDropOut = pdrop;
	pdrop->PossDropOut = _ParaSub(_ParaFint(1), P);
	MiaoToolMatrixInitial(&(pdrop->DropOutMat), pNNL->in.row, pNNL->in.col, pDropList);

}

//神经网络层追加 使用的是tool中的双向链表
MiaoToolBPnn_t *MiaoToolBPnnAppend(MiaoToolBPnn_t *pBPnn, MiaoToolDListNode_t *pDLN, MiaoToolNNLayerFullCon_t *pNNL)
{
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pDLN));
	MIAO_assert(!(pNNL));

	pBPnn->Net.AppendFUN(&pBPnn->Net, pDLN);
	pDLN->Data.uData = sizeof(MiaoToolNNLayerFullCon_t);
	pDLN->Data.pData = (void *)pNNL;
	if (pBPnn->ParaGroupNum == 0)
	{

		MiaoToolMatrixInitial(&pBPnn->x, pNNL->in.row, pNNL->in.col, pNNL->in.a);

	}

	MiaoToolMatrixInitial(&pBPnn->y, pNNL->out.row, pNNL->out.col, pNNL->out.a);

	pBPnn->ParaGroupNum += 2;//同时加上偏置与权重

	pBPnn->WeightParaNum += pNNL->w.length;
	pBPnn->BiasParaNum += pNNL->BiasUN.bias.length;


	return pBPnn;
}


void*
MiaoToolBPnnLayerAppend_Dynamic
(
	MiaoToolBPnn_t *pBPnn,
	MiaoToolNNLayerActFun_t *pAF,
	lw_u16 row,
	lw_u16 col,
	MiaoToolRandomBasic_t *pTRB,
	ParaType Rmax, ParaType Rmin,
	ParaType nl,
	ParaType *pin, ParaType *pout,
	MiaoMemoryBasic_t *pMem,
	ParaType DropOutEps
)
{
	void *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL, *p5 = NULL, *p6 = NULL, *p7 = NULL, *p8 = NULL;
    MiaoToolNNLayerFullCon_t *pTNNL;

    MIAO_assert(!(pBPnn));
    MIAO_assert(!(pAF));
    MIAO_assert(!(pTRB));
    MIAO_assert(!(pMem));

    p1 = pMem->Malloc(pMem, sizeof(MiaoToolNNLayerFullCon_t));
    p2 = pMem->Malloc(pMem, sizeof(ParaType)*row*col);
    p3 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);
    if (!(p1 && p2 && p3))
    {
        MIAO_assert(1);
        return NULL;
    }

    MiaoToolBPnnLayerInitial(p1, pAF, row, col, p2, p3, pTRB, Rmax, Rmin, nl);
    if (!(pout))//如果没有自定义输出
    {
        p3 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个输出
    }
    else
    {
        p3 = pout;
    }

	p8 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个X输出

    if(pBPnn->ParaGroupNum == 0)//只有输入层
    {

        if (!pin)//如果没有定义输入
        {
            p2 = pMem->Malloc(pMem, sizeof(ParaType)*row);//分配一个输入
        }
        else
        {
            p2 = pin;
        }

        p4 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * row);//in error
        p5 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//delt
        p6 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//out error

    }
    else
    {
        pTNNL = pBPnn->Net.pTail->Data.pData;//得到上一层内存地址
        p2 = pTNNL->out.a;
        p4 = pTNNL->outErr.a;//输入的误差是上一层输出误差
        p5 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//delt
        p6 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//out error

    }



    p7 = pMem->Malloc(pMem, sizeof(MiaoToolDListNode_t));
	
	if (!(p1 && p2 && p3 && p4 && p5 && p6 && p7 && p8))
	{
		MIAO_assert(1);
		return NULL;
	}

    memset(p7, 0, sizeof(MiaoToolDListNode_t));


    MiaoToolBPnnLayerForwardInitial(p1, p2, p3, p8);
    MiaoToolBPnnLayerBackwardInitial(p1, p4, p5, p6);
    MiaoToolBPnnAppend(pBPnn, p7, p1);

	
	if (DropOutEps > _ParaFint(0))
	{
	
		MiaoToolBPnnDropOutInitial(p1, pMem->Malloc(pMem, sizeof(MiaoToolNNDropOut_t)), DropOutEps, pMem->Malloc(pMem, sizeof(ParaType) * 1 * col));

	}
	else
	{
		((MiaoToolNNLayerFullCon_t *)p1)->pDropOut = NULL;
	}
	

    return p1;

}



//神经网络初始化
void MiaoToolBPnnInitial(MiaoToolBPnn_t *pBPnn)
{
    MIAO_assert(!(pBPnn));
    memset(pBPnn, 0, sizeof(MiaoToolBPnn_t));
    MiaoToolDListInitial(&pBPnn->Net,sizeof(MiaoToolNNLayerFullCon_t),NULL);
    pBPnn->_nl_factor = _ParaFrom(1.0);

}


void MiaoToolBPnnLossInitial(
    MiaoToolBPnn_t *pBPnn,
    ParaType *pRef,
    MiaoToolRandomBasic_t *pRand,
    MiaoMemoryBasic_t *pMem,
    uint32_t set_len,
    uint32_t sample_len,
    LossFunTYPE loss,
    MiaoToolNNTrainSaveMethodNum savem,
    MiaoToolNNTrainUpdateMethodNum updatem
)
{

    MiaoToolNNLayerFullCon_t *pTNL;
    uint32_t *pKind;
    lw_u16 i = 0;
    MiaoToolDListNode_t  *pDLN;
    MiaoToolNNLayerFullCon_t *pNNL;

    MIAO_assert(!(pBPnn));
	MIAO_assert(!(pRef));
	//MIAO_assert(!(loss));
    MIAO_assert(!(pRand));
    MIAO_assert(!(pMem));

    pTNL=pBPnn->Net.pTail->Data.pData;




    MiaoToolMatrixInitial(&(pBPnn->ref),1,pTNL->out.col,pRef);
    pBPnn->Train.BatchSetLength = set_len - 1;
	pBPnn->Train.SampleSetLength = sample_len;
    pBPnn->Train.BatchSampleDiv = set_len / sample_len;
    pBPnn->Train.BatchSampleDiv = pBPnn->Train.BatchSampleDiv ? pBPnn->Train.BatchSampleDiv : 1;

    pBPnn->Train.LossFun = loss;
    pBPnn->Train.pRand = pRand;
    pBPnn->Train.Flag.SaveMethod = savem;
    pBPnn->Train.Flag.UpdateMethod = updatem;

	pKind = pMem->Malloc(pMem, sizeof(uint32_t)*(pBPnn->ParaGroupNum + 1));
    pKind[i++] = PARA_MEM_CAL(1);
    //得到每一列的大小
    pDLN = pBPnn->Net.pTail;
    do {
        pNNL = (MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
        pKind[i++] = PARA_MEM_CAL(pNNL->w.length);
		pKind[i++] = PARA_MEM_CAL(pNNL->BiasUN.bias.length);

        pDLN = (MiaoToolDListNode_t *)pDLN->con.pLast;
    } while (pDLN->Key.uData);


    if (pBPnn->Train.Flag.SaveMethod == MiaoToolNNTrainSaveMethod_AllSave)
    {
        MiaoToolTableInitial(&(pBPnn->Train.DeltTable), sample_len, NULL, pBPnn->ParaGroupNum + 1, pMem, NULL, pKind);//完全存储就会全部存下
    }
    else
    {
        MiaoToolTableInitial(&(pBPnn->Train.DeltTable), 1, NULL,  pBPnn->ParaGroupNum + 1, pMem, NULL, pKind);//节约存储只会存一条delt
    }

    //pMem->Free(pMem, pKind);

    //pBPnn->loss=loss;
}

void MiaoToolBPnnReguInitial(MiaoToolBPnn_t *pBPnn, MiaoToolNNReguTypeNUM ReguType, ParaType lambda)
{

	MIAO_assert(!pBPnn);

	pBPnn->Regu.ReguType = ReguType;
	pBPnn->Regu.Lambda = lambda;

}

/*(Ver.=0.96)
//对网络减肥
//dropout 效果不好
MiaoToolBPnn_t* MiaoToolBPnnDropOutStart(MiaoToolBPnn_t *_pBPnn)
{
    static MiaoToolBPnn_t *pBPnn;
    static MiaoToolDListNode_t  *pDLN;
    static MiaoToolNNLayerFullCon_t *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    MIAO_assert(!(pBPnn));

    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return NULL;

    pDLN=(MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;

    do
    {
        pNNL=(MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
        if(!(pNNL->pDropOutW && pNNL->pDropOutCol && pNNL->pDropOutRow))
        {
            return NULL;
        }

        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        count_row=count_col;
        count_col=0;
        //准备过程 得到具体的行和列
        if(pDLN == (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext)
        {

            count_row=pNNL->size_row;
            for(i=0; i<pNNL->size_row; i++) //行不要dropout
                *prow++=i;
            for(j=0; j<pNNL->size_col; j++) //列要
            {
                if(pNNL->pRand->rand(pNNL->pRand,_ParaFint(1),_ParaFint(0)) > pNNL->PDropOut)
                {
                    *pcol++ = j;
                    count_col++;
                }//end if
            }//end for
        }//end if
        else if(pDLN == pBPnn->Net.pTail)
        {
            count_col=pNNL->size_col;
            for(j=0; j<pNNL->size_col; j++) //行不要dropout
                *pcol++=j;

        }
        else
        {

            for(j=0; j<pNNL->size_col; j++) //列dropout
            {
                if(pNNL->pRand->rand(pNNL->pRand,_ParaFint(1),_ParaFint(0)) > pNNL->PDropOut)
                {
                    *pcol++ = j;
                    count_col++;
                }// end if
            }//end for
        }//end else

        //开始具体dropout W
        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        pbuf=pNNL->pDropOutW;
        for(i=0; i<count_row; i++)
            for(j=0; j<count_col; j++)
                *pbuf++=_ParaDiv(MIAO_TOOL_M_GET(pNNL->w,prow[i],pcol[j]),_ParaFrom(1));

        //交换doupoutW与w矩阵
        pbuf=pNNL->w.a;
        pNNL->w.a=pNNL->pDropOutW;
        pNNL->pDropOutW=pbuf;


        //Dropout BiasUN.bias 与 BiasUN.bias
        pbuf=pNNL->pDropOutBiasUN.bias;
        if(pNNL->BiasUN.bias.a)
        {
            for(j=0; j<count_col; j++)
                *pbuf++=_ParaDiv(pNNL->w.a[pcol[j]],_ParaFrom(1));
        }

        pbuf=pNNL->BiasUN.bias.a;
        pNNL->BiasUN.bias.a=pNNL->pDropOutBiasUN.bias;
        pNNL->pDropOutBiasUN.bias=pbuf;


        //调整层大小
        MiaoToolBPnnLayerReSize(pNNL,count_row,count_col);

        pDLN=(MiaoToolDListNode_t *)pDLN->con.pNext;

    } while(pDLN);

    return pBPnn;

}

//恢复网络
MiaoToolBPnn_t* MiaoToolBPnnDropOutStop(MiaoToolBPnn_t *_pBPnn)
{
    static MiaoToolBPnn_t *pBPnn;
    static MiaoToolDListNode_t  *pDLN;
    static MiaoToolNNLayerFullCon_t *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    MIAO_assert(!(pBPnn));

    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return NULL;

    pDLN=(MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;

    do
    {
        pNNL=(MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
        if(!(pNNL->pDropOutW && pNNL->pDropOutCol && pNNL->pDropOutRow))
        {
            return NULL;
        }
        count_row=pNNL->w.row;
        count_col=pNNL->w.col;

        //交换doupoutW与w矩阵
        pbuf=pNNL->w.a;
        pNNL->w.a=pNNL->pDropOutW;
        pNNL->pDropOutW=pbuf;

        //dropoutW恢复
        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        pbuf=pNNL->pDropOutW;
        for(i=0; i<count_row; i++)
            for(j=0; j<count_col; j++)
            {
                MIAO_TOOL_M_GET(pNNL->w,prow[i],pcol[j])=_ParaMul(*pbuf,_ParaFrom(1));
                pbuf++;
            }


        //BiasUN.bias 恢复
        pbuf=pNNL->BiasUN.bias.a;
        pNNL->BiasUN.bias.a=pNNL->pDropOutBiasUN.bias;
        pNNL->pDropOutBiasUN.bias=pbuf;

        for(j=0; j<count_col; j++)
        {
            *(pNNL->BiasUN.bias.a+pcol[j])=_ParaMul(*pbuf,_ParaFrom(1));
            pbuf++;
        }


        //调整层大小
        MiaoToolBPnnLayerReSize(pNNL,pNNL->size_row,pNNL->size_col);

        pDLN=(MiaoToolDListNode_t *)pDLN->con.pNext;
    } while(pDLN);

    return pBPnn;


}
*/

//前向传播
MiaoToolBPnn_t *MiaoToolBPnnRunning(MiaoToolBPnn_t *_pBPnn)
{

    MiaoToolBPnn_t *pBPnn;
    MiaoToolDListNode_t  *pDLN;
    MiaoToolNNLayerFullCon_t *pNNL;
    MIAO_assert(!(_pBPnn));
    pBPnn=_pBPnn;
    if(!(pBPnn->ParaGroupNum))
        return _pBPnn;
    pDLN=(MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
    do {
        pNNL=(MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)//如果有定义BN
		{
			MiaoMat stas;//用于统计矩阵
			ParaType stas_res;
			stas.row = stas.col = stas.length = 2;
			stas.a = &stas_res;

			if (pBPnn->Train.Flag.Status != MiaoToolNNTrainStatus_Using)//在训练状态
			{

				MiaoMat m1;
				MiaoMat m2;

				pMiaoToolMatrixMPYA(&(pNNL->BiasUN._x_out_N), &(pNNL->in), &(pNNL->w), &(stas));
				pNNL->pBN->NowVar = pMiaoToolMatrixVar(&(pNNL->BiasUN._x_out_N), stas.a, &(pNNL->BiasUN._x_out_N), pNNL->pBN->BN_Epsi);

				m1.length = 1;
				m1.a = &(pNNL->pBN->BN_gamma);


				m2.length = 1;
				m2.a = &(pNNL->pBN->BN_beta);

				pMiaoToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->BiasUN._x_out_N), &(m1), &(m2));

				pNNL->pBN->NowMean = stas_res;
				
				//位移法记录总的平均值与方差
				pNNL->pBN->RealMean = _ParaAdd(
					_ParaMul(pNNL->pBN->RealMean, _ParaFrom(0.99)),
					_ParaMul(pNNL->pBN->NowMean, _ParaFrom(0.01))
				);

				pNNL->pBN->RealVar = _ParaAdd(
					_ParaMul(pNNL->pBN->RealVar, _ParaFrom(0.99)),
					_ParaMul(pNNL->pBN->NowVar, _ParaFrom(0.01))
				);


			}
			else
			{
				MiaoMat m1;
				MiaoMat m2;
				//k=gamma/sqrt(var+e)
				ParaType k = _ParaDiv(
					pNNL->pBN->BN_gamma, 
					_ParaSqrt(
						_ParaAdd(pNNL->pBN->RealVar, pNNL->pBN->BN_Epsi)
					)
				);
				//b=beta-mean/sqrt(var+e)
				ParaType b = _ParaSub(
					pNNL->pBN->BN_beta,
					_ParaDiv(
						pNNL->pBN->RealMean,
						_ParaSqrt(
							_ParaAdd(pNNL->pBN->RealVar, pNNL->pBN->BN_Epsi)
						)
					)
				);

				m1.length = m2.length = 1;
				m1.a = &k;
				m2.a = &b;

				pMiaoToolMatrixMPYA(&(pNNL->BiasUN._x_out_N), &(pNNL->in), &(pNNL->w), &(stas));

				pMiaoToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->BiasUN._x_out_N), &(m1), &(m2));
		
			}
			

		}
		else
		{
			pMiaoToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->in), &(pNNL->w), &(pNNL->BiasUN.bias));
		}
		*/

		if (pBPnn->Train.Flag.Status != MiaoToolNNTrainStatus_Using && pNNL->pDropOut)
		{
			uint32_t i = pNNL->size_col;
			ParaType *pd = pNNL->pDropOut->DropOutMat.a;
			ParaType *pin = pNNL->in.a;
			ParaType poss = pNNL->pDropOut->PossDropOut;
			ParaType _poss_data;
			while (i--)
			{

				_poss_data = pNNL->pRand->rand(pNNL->pRand, _ParaFrom(1), _ParaFrom(0));
				if (_poss_data <= poss)//小于保留概率 保留
				{
					*pd = _ParaDiv(_ParaFint(1), poss);
				}
				else
				{
					*pd = _ParaFint(0);
				}

				*pin = _ParaMul(*pin, *pd);

				pin++;
				pd++;
			}


		}

		pMiaoToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->in), &(pNNL->w), &(pNNL->BiasUN.bias));
        pNNL->pActFun->Forward(pNNL->pActFun,&(pNNL->_out_x),&(pNNL->out));
        pDLN=(MiaoToolDListNode_t *)pDLN->con.pNext;

    } while(pDLN);


    return pBPnn;
}



MiaoToolBPnn_t *MiaoToolBPnnErrCalu(MiaoToolBPnn_t *pBPnn)
{


    MiaoToolNNLayerFullCon_t *pTNL;
    MIAO_assert(!(pBPnn));

    pTNL=pBPnn->Net.pTail->Data.pData;

    if(pBPnn->Train.LossFun)
    {

        pBPnn->Error  = pBPnn->Train.LossFun(&(pTNL->outErr),&(pBPnn->ref),&(pTNL->out));

    }

    return pBPnn;

}


/*(Ver.=0.96)
//旧版本
//后向传播
MiaoToolBPnn_t *MiaoToolBPnnUpdate(MiaoToolBPnn_t *_pBPnn)
{

    MiaoToolBPnn_t *pBPnn;
    MiaoToolDListNode_t  *pDLN;
    MiaoToolNNLayerFullCon_t *pNNL;
    lw_u16 i,j;
    ParaType temp;
    MIAO_assert(!(_pBPnn));
    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return _pBPnn;
    pDLN=pBPnn->Net.pTail;
    //假设w（m*n）
    do {
        pNNL=(MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
        pNNL->pActFun->Backward(&pNNL->out,&pNNL->out);
        //delta(1*n)=outerr(1*n).*(dy)
        for(i=0; i<pNNL->out.col; i++)
            pNNL->delt.a[i]=_ParaMul(pNNL->outErr.a[i],pNNL->out.a[i]);

        //输入误差=delta(1*n)*w'(n*m)
        for(i=0; i<pNNL->w.row; i++)
        {
            temp=_ParaFint(0);
            for(j=0; j<pNNL->w.col; j++)
            {
                temp=_ParaAdd(temp,_ParaMul(pNNL->delt.a[j],MIAO_TOOL_M_GET(pNNL->w,i,j)));
            }
            pNNL->inErr.a[i]=temp;

        }
        //dw(m*n)=x'(m*1)*delta(1*n)
        for(i=0; i<pNNL->w.row; i++)
        {
            for(j=0; j<pNNL->w.col; j++)
            {
                temp=_ParaMul(pNNL->in.a[i],pNNL->delt.a[j]);
                MIAO_TOOL_M_GET(pNNL->w,i,j)=_ParaAdd(MIAO_TOOL_M_GET(pNNL->w,i,j),_ParaMul(temp,pNNL->nl));//别忘记学习速度
            }
        }
        if(pNNL->BiasUN.bias.a)
        {
            for(j=0; j<pNNL->w.col; j++)
            {
                pNNL->BiasUN.bias.a[j]=_ParaAdd(pNNL->BiasUN.bias.a[j], _ParaMul(pNNL->nl,pNNL->delt.a[j]));
            }
        }


        pDLN=(MiaoToolDListNode_t *)pDLN->con.pLast;
    } while(pDLN->Key.uData);


    return pBPnn;
}
*/





MiaoToolBPnn_t *MiaoToolBPnnDeltCalu(MiaoToolBPnn_t *_pBPnn,lw_u16 _batch_index)
{

    MiaoToolBPnn_t *pBPnn;
    MiaoToolDListNode_t  *pDLN;
    MiaoToolNNLayerFullCon_t *pNNL;
    lw_u16 i, j, count = 1, len;
	ParaType temp, nl, _regu_k_U, _regu_k_B, *_pdw, *_pw;
	MiaoMat dw, dbias;
    MiaoToolTable_t *ptable;

    MIAO_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;


    pDLN = pBPnn->Net.pTail;
    ptable = &(pBPnn->Train.DeltTable);
	
	_regu_k_B = _ParaMul(pBPnn->Regu.Lambda, _ParaFrom(pBPnn->WeightParaNum));
	

    //假设w（m*n）
    do 
	{
        pNNL = (MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
		pNNL->pActFun->Backward(pNNL->pActFun, &pNNL->_out_x, &pNNL->out);

		//取出dw
		dw.col = pNNL->w.col;
		dw.a = ptable->ReadFun(ptable, _batch_index, count++, &len);

		nl = _ParaMul(pNNL->nl, pBPnn->_nl_factor);
		
		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)//如果使能BN
		{
			
			ParaType *pdgamma, *pdbeta;
			uint32_t i = pNNL->_out_x.length;
			ParaType _sum_dg = _ParaFrom(0);
			ParaType _sum_db = _ParaFrom(0);
			ParaType *pdx_n = pNNL->DeltaUN._d_x_N.a;
			ParaType *px_n = pNNL->BiasUN._x_out_N.a;

			ParaType *pdout = pNNL->_out_x.a;
			ParaType *pdnext;

			ParaType _sum_dvar = _ParaFrom(0);
			ParaType _sum_dmean = _ParaFrom(0);
			ParaType _div_sqrt_var;
			ParaType _factor_xn;


			//第零步 计算dout也就是delta
			for (i = 0; i < pNNL->_out_x.col; i++)
				pdout[i] = _ParaMul(pNNL->outErr.a[i], pdout[i]);

			//第一步 取出 dgamma dbeta
			pdgamma = ptable->ReadFun(ptable, _batch_index, count++, &len);
			pdbeta = pdgamma + 1;



			//第二步(算法经过化简) 计算dgamma dbeta dnorm(向量) sumdvar sumdmean
			while (i--)
			{
				//dbeta = np.sum(dout, axis=0)
				_sum_db = _ParaAdd(_sum_db, *pdout);

				//dgamma = np.sum(x_normalized * dout, axis=0)
				_sum_dg = _ParaAdd(_sum_dg, _ParaMul(*pdout, *px_n));

				//dx_normalized = gamma * dout
				*pdx_n = _ParaMul(*pdout, pNNL->pBN->BN_gamma);


				//sumdvar=sum(dx_normalized[1:n] * x_normalized[1:n])
				_sum_dvar = _ParaAdd(_sum_dvar, _ParaMul(*pdx_n, *px_n));

				//sumdmean=sum(dx_normalized[1:n])
				_sum_dmean = _ParaAdd(_sum_dmean, *pdx_n);

				pdx_n++;
				px_n++;
				pdout++;
			}

			*pdgamma = _ParaAdd(*pdgamma, _ParaMul(_sum_dg, nl));
			*pdbeta = _ParaAdd(*pdbeta, _ParaMul(_sum_db, nl));

			//第三部 部分常数计算
			_div_sqrt_var = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(pNNL->pBN->NowVar, pNNL->pBN->BN_Epsi)));
			_factor_xn = _ParaMul(-_div_sqrt_var, _ParaDiv(_sum_dvar, _ParaFint(pNNL->_out_x.length)));
			_sum_dmean = _ParaDiv(_sum_dmean, _ParaFint(pNNL->_out_x.length));
			_sum_dmean = _ParaMul(_sum_dmean, _div_sqrt_var);

			//第四步 dx计算
			pdx_n = pNNL->DeltaUN._d_x_N.a;
			px_n = pNNL->BiasUN._x_out_N.a;
			pdnext = pNNL->DeltaUN.delta.a;

			i = pNNL->_out_x.length;

			while (i--)
			{
				*pdnext = _ParaMul(_div_sqrt_var, *pdx_n);
				*pdnext = _ParaAdd(*pdnext, _ParaMul(_factor_xn, *px_n));
				*pdnext = _ParaSub(*pdnext, _sum_dmean);
				//*pdnext = _ParaMul(*pdnext, 0);
				pdx_n++;
				px_n++;
				pdnext++;

			}
			

		}
		else
		{
			dbias.a = ptable->ReadFun(ptable, _batch_index, count++, &len);//取出dbias
			
			//delta(1*n)=outerr(1*n).*(dy)
			for (i = 0; i<pNNL->_out_x.col; i++)
				pNNL->DeltaUN.delta.a[i] = _ParaMul(pNNL->outErr.a[i], pNNL->_out_x.a[i]);
		}
		*/

		dbias.a = ptable->ReadFun(ptable, _batch_index, count++, &len);//取出dbias

		//delta(1*n)=outerr(1*n).*(dy)
		for (i = 0; i<pNNL->_out_x.col; i++)
			pNNL->DeltaUN.delta.a[i] = _ParaMul(pNNL->outErr.a[i], pNNL->_out_x.a[i]);



        //dw(m*n)=x'(m*1)*delta(1*n)
        for (i = 0; i<pNNL->w.row; i++)
        {
            for (j = 0; j<pNNL->w.col; j++)
            {
                temp = _ParaMul(pNNL->in.a[i], pNNL->DeltaUN.delta.a[j]);
                //MIAO_TOOL_M_GET(pNNL->w, i, j) = _ParaAdd(MIAO_TOOL_M_GET(pNNL->w, i, j), _ParaMul(temp, pNNL->nl));//别忘记学习速度
                MIAO_TOOL_M_GET(dw, i, j) = _ParaAdd(MIAO_TOOL_M_GET(dw, i, j), _ParaMul(temp, nl));//别忘记学习速度
            }
        }

        //if (pNNL->pBN == NULL)
        {
            for (j = 0; j<pNNL->w.col; j++)
            {
                dbias.a[j] = _ParaAdd(dbias.a[j], _ParaMul(nl, pNNL->DeltaUN.delta.a[j]));
            }
        }

		//输入误差=delta(1*n)*w'(n*m)
		for (i = 0; i<pNNL->w.row; i++)
		{
			temp = _ParaFint(0);
			for (j = 0; j<pNNL->w.col; j++)
			{
				temp = _ParaAdd(temp, _ParaMul(pNNL->DeltaUN.delta.a[j], MIAO_TOOL_M_GET(pNNL->w, i, j)));
			}
			pNNL->inErr.a[i] = temp;

		}

		if (pNNL->pDropOut)
		{
			pMiaoToolMatrixDot(&(pNNL->inErr), &(pNNL->inErr), &(pNNL->pDropOut->DropOutMat));
		}




		//正则化
		switch (pBPnn->Regu.ReguType)
		{
		case MiaoToolNNReguType_NAN:

			break;

		case MiaoToolNNReguType_L2:
			_pdw = dw.a;
			_pw = pNNL->w.a;
			
			_regu_k_U = _ParaMul(_regu_k_B,pNNL->nl);//别忘记学习速率

			i = pNNL->w.length;

			while (i--)
			{
				*_pdw = _ParaSub(*_pdw, _ParaMul(_regu_k_U, *_pw));
				_pdw++;
				_pw++;
			}


			break;

		case MiaoToolNNReguType_L1:
			_pdw = dw.a;
			_pw = pNNL->w.a;

			_regu_k_U = _ParaMul(_regu_k_B, pNNL->nl);//别忘记学习速率

			i = pNNL->w.length;

			while (i--)
			{
				*_pdw = _ParaSub(*_pdw, _ParaMul(_regu_k_U, (*_pw > _ParaFint(0) ? _ParaFrom(1) : _ParaFrom(-1))));
				_pdw++;
				_pw++;
			}
			break;

		default:
			break;
		}

        pDLN = (MiaoToolDListNode_t *)pDLN->con.pLast;
    } while (pDLN->Key.uData);


    return pBPnn;
}


ParaType *MiaoToolBPnnGradForInput(MiaoToolBPnn_t *_pBPnn)
{

	MiaoToolBPnn_t *pBPnn;
	MiaoToolDListNode_t  *pDLN;
	MiaoToolNNLayerFullCon_t *pNNL;
	lw_u16 i, j;
	ParaType temp, *pret_in_err;

	MIAO_assert(!(_pBPnn));
	pBPnn = _pBPnn;

	if (!(pBPnn->ParaGroupNum))
		return NULL;


	pNNL = pBPnn->Net.pTail->Data.pData;

	pMiaoToolMatrixSet(&(pNNL->outErr), _ParaFint(1), NULL);//输出误差为1 方便直接求导

	pDLN = pBPnn->Net.pTail;
	do
	{
		pNNL = (MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
		pNNL->pActFun->Backward(pNNL->pActFun, &pNNL->_out_x, &pNNL->out);
		for (i = 0; i<pNNL->w.row; i++)
		{
			temp = _ParaFint(0);
			for (j = 0; j<pNNL->w.col; j++)
			{
				temp = _ParaAdd(temp, _ParaMul(pNNL->DeltaUN.delta.a[j], MIAO_TOOL_M_GET(pNNL->w, i, j)));
			}
			pNNL->inErr.a[i] = temp;

		}

		pret_in_err = pNNL->inErr.a;
		pDLN = (MiaoToolDListNode_t *)pDLN->con.pLast;
	} while (pDLN->Key.uData);


	return pret_in_err;
}


MiaoToolBPnn_t *_MiaoToolBPnnReviewOnce(MiaoToolBPnn_t *_pBPnn,lw_u16 batch_num, MiaoMat *pmat_k)
{
	MiaoToolBPnn_t *pBPnn;
	MiaoToolDListNode_t  *pDLN;
	MiaoToolNNLayerFullCon_t *pNNL;
	MiaoToolTable_t *ptable;
	lw_u16 count = 1, len;
	MiaoMat dw, dbias;

	MIAO_assert(!(_pBPnn));
	pBPnn = _pBPnn;

	if (!(pBPnn->ParaGroupNum))
		return _pBPnn;
	ptable = &(pBPnn->Train.DeltTable);

	pDLN = pBPnn->Net.pTail;
	do {
		pNNL = (MiaoToolNNLayerFullCon_t *)pDLN->Data.pData;
		//取出dw 与 dbias
		dw.length = pNNL->w.length;
		dw.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		//更新w
		pMiaoToolMatrixMPYA(&(pNNL->w), &(dw), pmat_k, &(pNNL->w));

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)
		{

		ParaType *pdgamma, *pdbeta;

		pdgamma = ptable->ReadFun(ptable, batch_num, count++, &len);
		pdbeta = pdgamma + 1;

		pNNL->pBN->BN_beta = _ParaAdd(pNNL->pBN->BN_beta, _ParaMul(*pdbeta, temp));
		pNNL->pBN->BN_gamma = _ParaAdd(pNNL->pBN->BN_gamma, _ParaMul(*pdgamma, temp));

		}
		else
		{
		dbias.length = pNNL->BiasUN.bias.length;
		dbias.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		pMiaoToolMatrixMPYA(&(pNNL->BiasUN.bias), &(dbias), &mat_k, &(pNNL->BiasUN.bias));
		}
		*/

		dbias.length = pNNL->BiasUN.bias.length;
		dbias.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		pMiaoToolMatrixMPYA(&(pNNL->BiasUN.bias), &(dbias), pmat_k, &(pNNL->BiasUN.bias));

		pDLN = (MiaoToolDListNode_t *)pDLN->con.pLast;
	} while (pDLN->Key.uData);



	return pBPnn;
}


MiaoToolBPnn_t *MiaoToolBPnnReview(MiaoToolBPnn_t *_pBPnn)
{

    MiaoToolBPnn_t *pBPnn;
    lw_u16 batch_num;
    ParaType temp;
	MiaoMat mat_k;
    MiaoToolTable_t *ptable;

    MIAO_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;
    ptable = &(pBPnn->Train.DeltTable);
	temp = _ParaDiv(_ParaFint(1), _ParaFrom(pBPnn->Train.SampleSetLength));
	mat_k.length = 1;
	mat_k.a = &temp;

    //更新 w 与 bias
    for ( batch_num = 0; batch_num < pBPnn->Train.SampleCount+1; batch_num++)
    {
		_MiaoToolBPnnReviewOnce(pBPnn, batch_num, &mat_k);

    }

    //清空dw与dbias
    memset(ptable->Row.pData, 0, ptable->Row.uData * ptable->ColSize);


    //状态重置
    pBPnn->Train.count = 0;
    pBPnn->Train.SampleCount = 0;
    pBPnn->Train.Flag.Status = MiaoToolNNTrainStatus_Wait;

    return pBPnn;
}



MiaoToolBPnn_t *MiaoToolBPnnTrain(MiaoToolBPnn_t *_pBPnn)
{
    MiaoToolBPnn_t *pBPnn;
    MiaoToolNNTrain_t *pTr;

    MIAO_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;

    pTr = &(pBPnn->Train);
    switch (pTr->Flag.Status)
    {
    case MiaoToolNNTrainStatus_Wait:

        if (!(pTr->count % pTr->BatchSampleDiv))//满足采样间隔要求
        {
            pTr->SampleRandNum = pTr->pRand->randint(pTr->pRand, pTr->count + pTr->BatchSampleDiv, pTr->count);
        }

        if (pTr->count ==pTr->SampleRandNum)//等到了
        {
            if (pTr->Flag.SaveMethod == MiaoToolNNTrainSaveMethod_AllSave)//如果是完全存储
            {
                pTr->SampleCount++;//向后存
            }

            MiaoToolBPnnErrCalu(pBPnn);
            MiaoToolBPnnDeltCalu(pBPnn, pTr->SampleCount);
        }

        if (pTr->count == pTr->BatchSetLength)//采集满了
        {
            pTr->Flag.Status = MiaoToolNNTrainStatus_Complete;
            if (pTr->Flag.UpdateMethod == MiaoToolNNTrainUpdateMethod_Auto)//如果自动更新
            {
                MiaoToolBPnnReview(pBPnn);
            }
        }
        else
        {
            pTr->count++;
        }


        break;

    case MiaoToolNNTrainStatus_Complete:

        break;

	case MiaoToolNNTrainStatus_Using:

		break;

    default:
        pBPnn->Train.Flag.Status = MiaoToolNNTrainStatus_Wait;
        break;
    }

    return _pBPnn;
}


//
MiaoToolBPnn_t *MiaoToolBPnnCopy(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2)
{
    MiaoToolDListNode_t  *pDLN1, *pDLN2;
    MiaoToolNNLayerFullCon_t *pNNL1, *pNNL2;

    MIAO_assert(!(pBPnn1));
    MIAO_assert(!(pBPnn2));

    if ((pBPnn1->ParaGroupNum == 0 || pBPnn2->ParaGroupNum ==0) || pBPnn1->ParaGroupNum != pBPnn2->ParaGroupNum)
        return NULL;

    pDLN1 = (MiaoToolDListNode_t *)pBPnn1->Net.Head.con.pNext;
    pDLN2 = (MiaoToolDListNode_t *)pBPnn2->Net.Head.con.pNext;
    do {
        pNNL1 = (MiaoToolNNLayerFullCon_t *)pDLN1->Data.pData;
        pNNL2 = (MiaoToolNNLayerFullCon_t *)pDLN2->Data.pData;
		
		
		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL1->pBN && pNNL2->pBN)
		{
			memcpy(pNNL1->pBN, pNNL2->pBN, sizeof(MiaoToolNNBatchNorm_t));
		}
		else
		{
			pMiaoToolMatrixSet(&(pNNL1->BiasUN.bias), 0, &(pNNL2->BiasUN.bias));//复制偏置
		}
		*/
        

		pMiaoToolMatrixSet(&(pNNL1->BiasUN.bias), 0, &(pNNL2->BiasUN.bias));//复制偏置
        pMiaoToolMatrixSet(&(pNNL1->w), 0, &(pNNL2->w));//复制权重

        pDLN1 = (MiaoToolDListNode_t *)pDLN1->con.pNext;
        pDLN2 = (MiaoToolDListNode_t *)pDLN2->con.pNext;

    } while (pDLN2 && pDLN2);

    return pBPnn1;

}


//软替换函数 用于新老网络参数更新（DDPG）
MiaoToolBPnn_t *MiaoToolBPnnSoftReplace(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2,ParaType tu)
{
	MiaoToolDListNode_t  *pDLN1, *pDLN2;
	MiaoToolNNLayerFullCon_t *pNNL1, *pNNL2;

	MIAO_assert(!(pBPnn1));
	MIAO_assert(!(pBPnn2));

	if ((pBPnn1->ParaGroupNum == 0 || pBPnn2->ParaGroupNum == 0) || pBPnn2->ParaGroupNum != pBPnn1->ParaGroupNum)
		return NULL;

	pDLN1 = (MiaoToolDListNode_t *)pBPnn1->Net.Head.con.pNext;
	pDLN2 = (MiaoToolDListNode_t *)pBPnn2->Net.Head.con.pNext;

	do {
		pNNL1 = (MiaoToolNNLayerFullCon_t *)pDLN1->Data.pData;
		pNNL2 = (MiaoToolNNLayerFullCon_t *)pDLN2->Data.pData;

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL1->pBN && pNNL2->pBN)
		{
			pNNL1->pBN->BN_gamma = _ParaAdd(
				_ParaMul(pNNL1->pBN->BN_gamma, _ParaSub(_ParaFrom(1), tu)), 
				_ParaMul(pNNL2->pBN->BN_gamma, tu)
			);

			pNNL1->pBN->BN_beta = _ParaAdd(
				_ParaMul(pNNL1->pBN->BN_beta, _ParaSub(_ParaFrom(1), tu)),
				_ParaMul(pNNL2->pBN->BN_beta, tu)
			);

		}
		else
		{
			pMiaoToolMatrix_RATIO_ADD(
				&(pNNL1->BiasUN.bias),
				_ParaSub(_ParaFrom(1), tu), &(pNNL1->BiasUN.bias),
				tu, &(pNNL2->BiasUN.bias)
			);
		}
		*/

		pMiaoToolMatrix_RATIO_ADD(
			&(pNNL1->BiasUN.bias),
			_ParaSub(_ParaFrom(1), tu), &(pNNL1->BiasUN.bias),
			tu, &(pNNL2->BiasUN.bias)
		);

		pMiaoToolMatrix_RATIO_ADD(
			&(pNNL1->w),
			_ParaSub(_ParaFrom(1), tu), &(pNNL1->w),
			tu, &(pNNL2->w)
		);

		pDLN1 = (MiaoToolDListNode_t *)pDLN1->con.pNext;
		pDLN2 = (MiaoToolDListNode_t *)pDLN2->con.pNext;

	} while (pDLN2 && pDLN2);

	return pBPnn1;

}
#endif // 0

lw_ptr MiaoToolBPnnLayerForwardDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr MiaoToolBPnnLayerBackwardDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr MiaoToolBPnnLayerUpdateDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr MiaoToolBPnnLayerNNmallocDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward) { return 0; }
lw_ptr MiaoToolBPnnLayerClearDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { LW_MAT_CLR(&(pNNSLB->out)); return 0; }
lw_ptr MiaoToolBPnnLayerDataInitDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr MiaoToolBPnnLayerCopyDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2) { return 0; }
lw_ptr MiaoToolBPnnLayerSoftReplaceDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido) { return 0; }



void MiaoToolBPnnInit(MiaoToolBPnn_t *pBPnn,lw_u16 max_mini_batch)
{
	MIAO_assert(!(pBPnn));

	memset(pBPnn, 0, sizeof(MiaoToolBPnn_t));
	MiaoToolDListInitial(&pBPnn->Net, sizeof(MiaoToolNNLayerFullCon_t), NULL);
	pBPnn->_nl_factor = _ParaFrom(1.0);
	pBPnn->Train.mini_batch_max = max_mini_batch;
	pBPnn->Train._MemAllocCoff = 1;
	pBPnn->Train.Epsi = _ParaFrom(1e-6);
}


void MiaoToolBPnnLayerAppend
(
	MiaoToolBPnn_t *pBPnn,
	MiaoToolDListNode_t *pnode,
	uint32_t len,
	void *LayerList
)
{
	lw_u16 i;
	MiaoToolNNSubLayerBasic_t **ppLIST1;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pnode));
	MIAO_assert(!(len));
	MIAO_assert(!(LayerList));

	pBPnn->Net.AppendFUN(&(pBPnn->Net), pnode);
	pnode->Data.uData = len;
	pnode->Data.pData = (void *)LayerList;

	ppLIST1 = LayerList;
	for (i = 0; i < len; i++)
	{
		ppLIST1[i]->pNN = pBPnn;
	}

	pBPnn->ParaGroupNum += len;
	return;
}




void MiaoToolBPnnTrainInit(
	MiaoToolBPnn_t *pBPnn,
	ParaType *pRef,
	MiaoMemoryBasic_t *pmem,
	LossFunTYPE loss,
	ParaType nl
)
{

	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pmem));


	memcpy(&pBPnn->ref, &pBPnn->y, sizeof(MiaoMat));

	if (pRef==NULL)
	{
		pRef = pmem->Malloc(pmem, PARA_MEM_CAL(pBPnn->ref.length));
	}

	pBPnn->ref.a = pRef;

	pBPnn->Train.LossFun = loss;
	pBPnn->Train.nl = nl;

}

void MiaoToolBPnnOptimInit(
	MiaoToolBPnn_t *pBPnn,
	MiaoToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
)
{
	MIAO_assert(!(pBPnn));

	pBPnn->Train.Epsi = _ParaFrom(1e-6);
	pBPnn->Train.Flag.Optim = optim;
	switch (optim)
	{
	case MiaoToolNNOptim_GradDesc:
		pBPnn->Train._MemAllocCoff = 1;
		break;

	case MiaoToolNNOptim_M:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train._MemAllocCoff = 2;
		break;

	case MiaoToolNNOptim_RMSp:
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 2;
		break;

	case MiaoToolNNOptim_Adam:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 3;
		break;

	case MiaoToolNNOptim_Nadam:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 3;
		break;

	default:
		MIAO_assert(1);
		break;
	}

	return;

}


void MiaoToolBPnnAllDataInit(MiaoToolBPnn_t *pBPnn,MiaoMemoryBasic_t *pMem)
{

	MiaoToolDListNode_t *node;
	MiaoToolNNSubLayerBasic_t **ppNNL;
	uint32_t *pKind;
	register uint32_t i, count = 0, j;
	uint32_t flowdatamax = 0;
	void *pFlowDataAddr = NULL;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pMem));

	if (pBPnn->ParaGroupNum==0)
	{
		return;
	}

	pBPnn->pmem = pMem;

	if (pBPnn->Train.Flag.MemoryMethod== MiaoToolNNMemoryMethod_Chip)
	{
		node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				if (ppNNL[i]->FlowData.uData >= flowdatamax && ppNNL[i]->FlowData.pData==NULL)
				{
					flowdatamax = ppNNL[i]->FlowData.uData;
				}
			}
			
			node = (MiaoToolDListNode_t *)node->con.pNext;
		}
		//分配浮动内存
		if (flowdatamax)
		{
			pFlowDataAddr = pMem->Calloc(pMem, flowdatamax);
			node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
			while (node)
			{
				ppNNL = node->Data.pData;
				for (i = 0; i < node->Data.uData; i++)
				{
					if (ppNNL[i]->FlowData.pData==NULL)
					{
						ppNNL[i]->FlowData.pData = pFlowDataAddr;
					}					
				}
				node = (MiaoToolDListNode_t *)node->con.pNext;
			}
		}

		node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				ppNNL[i]->DataInit(ppNNL[i]);
			}

			node = (MiaoToolDListNode_t *)node->con.pNext;
		}



	}
	else
	{


		//block方式统一存储内存
		pKind = pMem->Malloc(pMem, sizeof(uint32_t)*(pBPnn->ParaGroupNum));
	

		//得到table的列
		count = 0;
		node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				pKind[count++] = ppNNL[i]->sizeofdata* (uint32_t)(pBPnn->Train._MemAllocCoff);
				if (ppNNL[i]->FlowData.uData >= flowdatamax && ppNNL[i]->FlowData.pData == NULL)
				{
					flowdatamax = ppNNL[i]->FlowData.uData;
				}
			}
			node = (MiaoToolDListNode_t *)node->con.pNext;
		}
		//分配浮动内存
		if (flowdatamax)
		{
			pFlowDataAddr = pMem->Malloc(pMem, flowdatamax);
		}

		//判断是否需要训练
		if (pBPnn->Train.Flag.NeedTrain == MiaoToolNNNeedTrain_Need)
		{
			MiaoToolTableInitial(&(pBPnn->Train.DataTable), 2, NULL, pBPnn->ParaGroupNum, pMem, NULL, pKind);//存储两行 分别是原始数值与delt
		}
		else
		{
			MiaoToolTableInitial(&(pBPnn->Train.DataTable), 1, NULL, pBPnn->ParaGroupNum, pMem, NULL, pKind);//存储一行 原始数值
		}

		//分配内存并且初始化
		count = 0;
		node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				//ppNNL[j]->NNmalloc();

				ppNNL[i]->FlowData.pData = pFlowDataAddr;

				if (ppNNL[i]->sizeofdata == 0)
				{
					continue;
				}

				if (pBPnn->Train.Flag.NeedTrain == MiaoToolNNNeedTrain_Need)
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 0, count, NULL),
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 1, count, NULL)
					);

					ppNNL[i]->DataInit(ppNNL[i]);
					
				}
				else
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 0, count, NULL),
						NULL
					);

					ppNNL[i]->DataInit(ppNNL[i]);
				}
			}
		
			node = (MiaoToolDListNode_t *)node->con.pNext;
		}
	}

}

void* MiaoToolNNLayerSimpleRecover
(
	MiaoToolBPnn_t *pBPnn,
	ParaType *in,ParaType *out,
	MiaoToolNNLayerSimpleBak_t *pSimpleBak,
	MiaoMemoryBasic_t *pmem
)
{
	MiaoToolNNLayerFullCon_t *pfc;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pmem));	
	switch((*pSimpleBak).NN_Kind)
	{
		case MiaoToolNNSubLayerKind_FullCon:
			pfc=MiaoToolBPnnFullConAppend(
			pBPnn,
			(*pSimpleBak).info[0], (*pSimpleBak).info[1],
			in, out,
			NULL, NULL,
			NULL, NULL, 0, 0,
			(*pSimpleBak).pTemplet, pmem
		);
		pfc->pRecover=(*pSimpleBak).pDataAddr;
		
		break;

		case MiaoToolNNSubLayerKind_Conv:
			pfc=MiaoToolBPnnConvAppend(
			pBPnn,
			(*pSimpleBak).info[0],(*pSimpleBak).info[1],(*pSimpleBak).info[2],
			(*pSimpleBak).info[3],(*pSimpleBak).info[4],(*pSimpleBak).info[5],
			(*pSimpleBak).info[5],
			in,out,
			NULL,NULL,
			NULL, NULL, 0, 0,
			(*pSimpleBak).pTemplet,pmem,NULL
		);
		pfc->pRecover=(*pSimpleBak).pDataAddr;
		break;
		
		case MiaoToolNNSubLayerKind_Pool:
			MiaoToolBPnnPoolAppend(
			pBPnn,
			(*pSimpleBak).info[0],(*pSimpleBak).info[1],(*pSimpleBak).info[2],
			(*pSimpleBak).info[3],(*pSimpleBak).info[4],
			in,out,
			(*pSimpleBak).info[5],
			pmem,NULL
		);
		break;
		
		case	MiaoToolNNSubLayerKind_ActFun:
		
		break;
		
	}		
	
	return pfc;
	
}


void* MiaoToolBPnnSimpleRecover
(
	MiaoToolBPnn_t *pBPnn,
	ParaType *in,ParaType *out,
	MiaoToolNNLayerSimpleBak_t *pSimpleBak,
	lw_u16 simple_layer_len,
	MiaoMemoryBasic_t *pmem
)
{
	
	lw_u16 i;
	ParaType *pin=in;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pmem));	
	
		
	
	for(i=0;i<simple_layer_len-1;i++)
	{
		MiaoToolNNLayerSimpleRecover(pBPnn,pin,NULL,pSimpleBak,pmem);
		pin=NULL;
		pSimpleBak++;
	}
	
	MiaoToolNNLayerSimpleRecover(pBPnn,pin,out,pSimpleBak,pmem);
	
	return pBPnn;
	
}


void MiaoToolBPnnForward(MiaoToolBPnn_t *pBPnn)
{
	MiaoToolDListNode_t *node;
	MiaoToolNNSubLayerBasic_t **ppNNL;
	lw_ptr mmb;
	register uint32_t i;
	MIAO_assert(!(pBPnn));

	mmb = pBPnn->Train.mini_batch_now;
	node = (MiaoToolDListNode_t *)pBPnn->Net.Head.con.pNext;
	while (node)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			if (ppNNL[i]->RunningStopFlag)
			{
				return;
			}

			mmb = ppNNL[i]->Forward(ppNNL[i], mmb);
		}

		node = (MiaoToolDListNode_t *)node->con.pNext;
	}

	pBPnn->Train._batch_stream_count++;
	pBPnn->Train.AllBatchCount += pBPnn->Train.mini_batch_now;
	
	return;

}


void MiaoToolBPnnBackward(MiaoToolBPnn_t *pBPnn)
{
	MiaoToolDListNode_t *node;
	MiaoToolNNSubLayerBasic_t **ppNNL;
	lw_ptr mmb;
	register uint32_t i;
	MIAO_assert(!(pBPnn));

	mmb = pBPnn->Train.mini_batch_now;
	node = (MiaoToolDListNode_t *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		i = node->Data.uData;
		while (i--)
		{
			if (ppNNL[i]->RunningStopFlag)
			{
				return;
			}

			mmb = ppNNL[i]->Backward(ppNNL[i], mmb);
		}

		node = (MiaoToolDListNode_t *)node->con.pLast;
	}

	return;
}

void MiaoToolBPnnReview(MiaoToolBPnn_t *pBPnn)
{
	MiaoToolDListNode_t *node;
	MiaoToolNNSubLayerBasic_t **ppNNL;
	register uint32_t i;
	MIAO_assert(!(pBPnn));

	node = (MiaoToolDListNode_t *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->Update(ppNNL[i]);
		}

		node = (MiaoToolDListNode_t *)node->con.pLast;
	}

	pBPnn->Train.AllBatchCount = 0;
	pBPnn->Train._batch_stream_count = 0;
	pBPnn->Train.mini_batch_now = 0;

	pBPnn->Train.Beta1T = _ParaMul(pBPnn->Train.Beta1T, pBPnn->Train.Beta1T);
	pBPnn->Train.Beta2T = _ParaMul(pBPnn->Train.Beta2T, pBPnn->Train.Beta2T);

	return;

}


void MiaoToolBPnnLoadX(MiaoToolBPnn_t *pBPnn, MiaoMat *xs)
{
	MiaoMat _x;
	lw_32 res;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(xs));
#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	if (xs->col != pBPnn->x.col)
	{
		MIAO_assert(1);
		return;
	}

	//计算剩余的批容量
	if (pBPnn->Train.mini_batch_max < (pBPnn->Train.mini_batch_now + xs->row))
	{
		MIAO_assert(1);
		return;
	}

	res = pBPnn->Train.mini_batch_max - pBPnn->Train.mini_batch_now;

	
	
	if (res < xs->row)
	{
		//输入batch超过了最大
		//调整输入矩阵
		xs->row=res;
		xs->length=xs->col*res;
	}
	else
	{
		res=xs->row;
	}
	
	

	//初始化
	MiaoMatInit(&_x,res,xs->col,MiaoMatGetR(pBPnn->x,pBPnn->Train.mini_batch_now));
	
	//复制矩阵
	LW_MAT_CPY(&(_x),xs);
	
	
#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
		if (xs->col != pBPnn->x.row)
	{
		MIAO_assert(1);
		return;
	}

	//计算剩余的批容量
	if (pBPnn->Train.mini_batch_max < (pBPnn->Train.mini_batch_now + xs->row))
	{
		MIAO_assert(1);
		return;
	}

	res = pBPnn->Train.mini_batch_max - pBPnn->Train.mini_batch_now;

	if (res < xs->row)
	{	
		xs->row = res;
	}
	else
	{
		res=xs->row;
	}
	

	//初始化
	MiaoMatInit(&_x, pBPnn->x.row, res, pBPnn->x.a + pBPnn->Train.mini_batch_now);

	pMiaoToolMatrixTurn(&_x, xs);
	
#endif
	


	//更新批数量
	pBPnn->Train.mini_batch_now += res;
}



void MiaoToolBPnnLoadY(MiaoToolBPnn_t *pBPnn, MiaoMat *ys)
{
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(ys));
#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	
	if (ys->col != pBPnn->ref.col || pBPnn->Train.mini_batch_now != (ys->row+pBPnn->Train.y_offset) )
	{
		MIAO_assert(1);
		return;
	}

	//先拷贝y到ref 再拷贝ys到ref
	LW_PARA_JOIN(pBPnn->ref.a, pBPnn->y.a, pBPnn->Train.y_offset*ys->col, ys->a, ys->length);
	
#elif MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_F
	MIAO_assert(1);
#endif

}

void MiaoToolBPnnRun(MiaoToolBPnn_t *pBPnn, MiaoMat *xs)
{

	MIAO_assert(!(pBPnn));

	if (!(xs==NULL))
	{
		MiaoToolBPnnLoadX(pBPnn, xs);
	}
	else
	{
		pBPnn->Train.mini_batch_now = pBPnn->Train.mini_batch_max;
	}
	
	
	MiaoToolBPnnForward(pBPnn);
	
	if (pBPnn->Train.Flag.NeedTrain == MiaoToolNNNeedTrain_NeedNot)
	{
		pBPnn->Train.mini_batch_now = 0;
		pBPnn->Train._batch_stream_count = 0;
		pBPnn->Train.AllBatchCount = 0;
	}

}


void MiaoToolBPnnCalErr(MiaoToolBPnn_t *pBPnn)
{
	MiaoMat y, ref;

	MIAO_assert(!(pBPnn));

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	MiaoMatInit(&y, pBPnn->Train.mini_batch_now, pBPnn->y.col, pBPnn->y.a);
	MiaoMatInit(&ref, pBPnn->Train.mini_batch_now, pBPnn->ref.col, pBPnn->ref.a);

	if (pBPnn->Train.LossFun)
	{
		pBPnn->Error = pBPnn->Train.LossFun(&(y), &(ref), &(y));
	}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	
	MiaoMatInit(&y, pBPnn->y.row, pBPnn->Train.mini_batch_now, pBPnn->y.a);
	MiaoMatInit(&ref, pBPnn->ref.row, pBPnn->Train.mini_batch_now, pBPnn->ref.a);

	if (pBPnn->Train.LossFun)
	{
		pBPnn->Error = pBPnn->Train.LossFun(&(y), &(ref), &(y));
	}

#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C



	MiaoToolBPnnBackward(pBPnn);

	pBPnn->Train.mini_batch_now = 0;


}

void MiaoToolBPnnTrain(MiaoToolBPnn_t *pBPnn,MiaoMat *ys)
{
	MIAO_assert(!(pBPnn));

	if (ys!=NULL)
	{
		MiaoToolBPnnLoadY(pBPnn, ys);
	}

	MiaoToolBPnnCalErr(pBPnn);
	MiaoToolBPnnReview(pBPnn);
}



void MiaoToolBPnnClear(MiaoToolBPnn_t *pBPnn)
{
	MiaoToolDListNode_t *node;
	MiaoToolNNSubLayerBasic_t **ppNNL;
	uint32_t i;
	
	MIAO_assert(!(pBPnn));

	pBPnn->Train.mini_batch_now = 0;
	pBPnn->Train._batch_stream_count = 0;
	pBPnn->Train.AllBatchCount = 0;


	node = (MiaoToolDListNode_t *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->Clear(ppNNL[i]);
		}

		node = (MiaoToolDListNode_t *)node->con.pLast;
	}
}



void MiaoToolBPnnCopy(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2)
{
	MiaoToolDListNode_t *node1, *node2;
	MiaoToolNNSubLayerBasic_t **ppNNL1, **ppNNL2;
	register uint32_t i;
	MIAO_assert(!(pBPnn1));
	MIAO_assert(!(pBPnn2));


	node1 = (MiaoToolDListNode_t *)pBPnn1->Net.Head.con.pNext;
	node2 = (MiaoToolDListNode_t *)pBPnn2->Net.Head.con.pNext;
	while (node1 && node2)
	{
		ppNNL1 = node1->Data.pData;
		ppNNL2 = node2->Data.pData;
		for (i = 0; i < node1->Data.uData; i++)
		{
			ppNNL1[i]->Copy(ppNNL1[i], ppNNL2[i]);
		}

		node1 = (MiaoToolDListNode_t *)node1->con.pNext;
		node2 = (MiaoToolDListNode_t *)node2->con.pNext;
	}

	return;

}


void MiaoToolBPnnSoftReplace(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2,ParaType Tau)
{
	MiaoToolDListNode_t *node1, *node2;
	MiaoToolNNSubLayerBasic_t **ppNNL1, **ppNNL2;
	register uint32_t i;
	MIAO_assert(!(pBPnn1));
	MIAO_assert(!(pBPnn2));


	node1 = (MiaoToolDListNode_t *)pBPnn1->Net.Head.con.pNext;
	node2 = (MiaoToolDListNode_t *)pBPnn2->Net.Head.con.pNext;
	while (node1 && node2)
	{
		ppNNL1 = node1->Data.pData;
		ppNNL2 = node2->Data.pData;
		for (i = 0; i < node1->Data.uData; i++)
		{
			ppNNL1[i]->SoftReplace(ppNNL1[i], ppNNL2[i], Tau);
		}

		node1 = (MiaoToolDListNode_t *)node1->con.pNext;
		node2 = (MiaoToolDListNode_t *)node2->con.pNext;
	}

	return;
}

MiaoToolNNSubLayerBasic_t *MiaoToolBPnnAt(MiaoToolBPnn_t *pBPnn, lw_32 i, lw_32 j)
{

	MiaoToolDListNode_t *pln;
	MiaoToolNNSubLayerBasic_t **ppNNL;

	i = i < 0 ? i + pBPnn->Net.NowLen : i;

	if (i < 0 )//彻底超出维度
	{
		return NULL;
	}

	pln = pBPnn->Net.IndexOfFUN(&(pBPnn->Net), MiaoToolDListCmpCount, i, NULL);
	
	j = j < 0 ? j + pln->Data.uData : j;

	if (j < 0)//彻底超出维度
	{
		return NULL;
	}

	ppNNL = pln->Data.pData;

	return ppNNL[j];
}


#endif // MIAO_TOOL_IMPORT_NN_BPnn || MIAO_TOOL_IMPORT_ALL

#endif // MIAO_TOOL_IMPORT_NN || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 2)

