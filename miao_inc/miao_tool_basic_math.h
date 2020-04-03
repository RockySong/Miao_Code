/*(Ver.=0.96)
* MIAO_tool.h
*
*  Created on: 2017-7-25
*      Author: slw
*/

#ifndef MIAO_TOOL_BASIC_MATH_H_
#define MIAO_TOOL_BASIC_MATH_H_


#include "OSLW_define.h"
#include "OSLW_tool_define.h"
#include "OSLW_parameter.h"

#if !(MIAO_SIMPLE_LEVEL >= 4)

#if MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL

typedef uint32_t LwMatRowType;
typedef uint32_t LwMatColType;
typedef uint32_t LwMatLenType;

typedef struct MIAO_TOOL_MATRIX_STRUCT {
	ParaType *a;
	LwMatLenType length;
	LwMatRowType row;
	LwMatColType col;

}MiaoMat;

typedef MiaoMat MiaoMat;

typedef enum
{
	MiaoMartixRes_Find = 0,
	MiaoMartixRes_NoFind,
	MiaoMartixRes_Over,
	MiaoMartixRes_Error

}MiaoMartixResNum;


void MiaoToolMatrixInitial(MiaoMat *m, LwMatRowType row, LwMatColType col, ParaType *a);

//MiaoMat* MiaoToolMatrixAdd(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);
//typedef MiaoMat* (*pMiaoToolMatrixAddFunType)(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixAdd,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2))

//MiaoMat* MiaoToolMatrixSub(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);
//typedef MiaoMat* (*pMiaoToolMatrixSubFunType)(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);
MIAO_DECLARE(MiaoMat*, MiaoToolMatrixSub,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2))


MIAO_DECLARE(MiaoMat*, MiaoToolMatrixDot,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2))

//MiaoMat* MiaoToolMatrixMul(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);
//typedef MiaoMat* (*pMiaoToolMatrixMulFunType)(MiaoMat *s,MiaoMat *m1,MiaoMat *m2);
MIAO_DECLARE(MiaoMat*, MiaoToolMatrixMul,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixTurn,
(MiaoMat *s, MiaoMat *buf))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixReSize,
(MiaoMat *s, lw_u16 row, lw_u16 col))


MIAO_DECLARE(MiaoMat*, MiaoToolMatrixMaxMin,
(MiaoMat *s, ParaType *pMax, lw_u16 *pMaxIndex, ParaType *pMin, lw_u16 *pMinIndex))

MIAO_DECLARE(MiaoMartixResNum, MiaoToolMatrixIndex,
(MiaoMat *s, ParaType data, lw_u16 *pindex, lw_u16 *pmaxlen))

MIAO_DECLARE(MiaoMartixResNum, MiaoToolMatrixFind,
(MiaoMat *s, MiaoMat *m, lw_u16 SpacingLen, lw_u16 First, lw_u16 *pres))


MIAO_DECLARE(MiaoMartixResNum, MiaoToolMatrixCmp,
(MiaoMat *s, MiaoMat *m, lw_u8 *pbuf))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixSet,
(MiaoMat *s, ParaType data, MiaoMat *a))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixJoin,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2))

MIAO_DECLARE(ParaType, MiaoToolMatrixLossSquare,
(MiaoMat *s, MiaoMat *ref, MiaoMat *pre))

MIAO_DECLARE(ParaType, MiaoToolMatrixLossCrossEntropyForSoftMax,
(MiaoMat *s, MiaoMat *ref, MiaoMat *pre))

/*

MIAO_DECLARE(ParaType, MiaoToolMatrixSum,
(MiaoMat *s));

MIAO_DECLARE(ParaType, MiaoToolMatrixVar,
(MiaoMat *s, ParaType *pMean, MiaoMat *_normal_res, ParaType epsi));
*/

MIAO_DECLARE(ParaType, MiaoToolMatrixSum,
(MiaoMat *DST, MiaoMat *SRC, lw_u8 dim))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixMPYA,
(MiaoMat *s, MiaoMat *x, MiaoMat *we, MiaoMat *bi))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixWeXBi,
(MiaoMat *s, MiaoMat *we, MiaoMat *x, MiaoMat *bi))

MIAO_DECLARE(MiaoMat*, MiaoToolMatrixXWeBi,
(MiaoMat *s, MiaoMat *we, MiaoMat *x, MiaoMat *bi))

MIAO_DECLARE(MiaoMat*,MiaoToolMatrixTurnMul,
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2, lw_u8 flag))


MIAO_DECLARE(void*, MiaoToolMatrixConv2,
(
	MiaoMat *s, //目标的
	MiaoMat *m_kernal, //卷积核
	MiaoMat *m2,//被卷积 
	lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
	lw_u8 EqualModel, //赋值模式 1:直接复制 0:相加
	MiaoToolMatrixConvMethodNUM MoveModel, //移动模式 'v'/'f'
	lw_u8 KernalModel, //核模式 0/180 180+‘f’=数学二维卷积
	ParaType *fast_buf//快速卷积内存区
	)
)

//只计算一个batch
MIAO_DECLARE(void*, MiaoToolMatrixConvFastMultCh,
(
	MiaoMat *m_out, //输出 row-col 代表一个通道 length代表真正大小
	MiaoMat *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
	MiaoMat *m_in,//被卷积 row-col 代表一个通道
	MiaoMat *bias,//偏置 row-col-length 无所谓
	lw_u16 in_high,//输入高度 
	lw_u16 out_high,//输出高度
	lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
	MiaoToolMatrixConvMethodNUM conv_method,
	lw_u8 FD_1_or_BK_0,//前向传递或者反向传递
	ParaType *fast_buf//核 区域
	)
)


MIAO_DECLARE(
	void*, MiaoToolMatrixConvFastMultChIm2ColFd,
	(
		MiaoMat *m_out, //输出 row-col 代表一个通道 length代表真正大小
		MiaoMat *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
		MiaoMat *m_in,//被卷积 row-col 代表一个通道
		MiaoMat *bias,//偏置 row-col-length 无所谓
		lw_u16 in_high,//输入高度 
		lw_u16 out_high,//输出高度
		lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
		MiaoToolMatrixConvMethodNUM conv_method,
		uint32_t now_flow_len,//现实缓冲区的大小
		ParaType *fast_buf//缓冲区
		)
)

MIAO_DECLARE(void*, MiaoToolMatrixMoments,
(
	MiaoMat *src, 
	ParaType *mean,
	ParaType *var,
	lw_u8 dim	
))

MIAO_DECLARE(void*, MiaoToolMatrixVectShift,
(
	MiaoMat *y,
	MiaoMat *we,
	MiaoMat *x,
	MiaoMat *bi
	)
)

MIAO_DECLARE(void*, MiaoToolMatrixDotSum,
(
	MiaoMat *y,
MiaoMat *x1,
MiaoMat *x2,
lw_u8 dim
)
)

MIAO_DECLARE(
	MiaoMat*,
MiaoToolMatrix_RATIO_ADD,
(MiaoMat *s, ParaType a, MiaoMat *m1, ParaType b, MiaoMat *m2)
)


#define MIAO_TOOL_M_SIZE_EQU(A,B) ((A)->length == (B)->length)

#define MIAO_TOOL_M_GET(M,R,C) (*((M).a+(R)*(M).col+(C)))

#define MIAO_TOOL_M_GET_R(M,R) ((M).a+(R)*(M).col)

#define MiaoMatGet(M,R,C) ((M).a+(R)*(M).col+(C))

#define MiaoMatAt(M,R,C) (*((M).a+(R)*(M).col+(C)))

#define MiaoMatGetR(M,R) ((M).a+(R)*(M).col)

#define MIAO_TOOL_M_DY_INIT(M,R,C,PMEM) do{MiaoToolMatrixInitial(&(M),(R),(C),(PMEM)->Malloc((PMEM) , PARA_MEM_CAL((R)*(C))));}while(0)

#define MiaoMatInit MiaoToolMatrixInitial

#define MiaoMatNew MIAO_TOOL_M_DY_INIT

#define MiaoMatDef(R,C,A) {((ParaType *)(A)),(R)*(C),(R),(C)}

#define MiaoMatStaticNew(M,R,C) static ParaType M##R##C [(R)*(C)];static MiaoMat M={((ParaType *)( M##R##C )),(R)*(C),(R),(C)};




#if PARA_LEN==8
#define LW_MAT_CLR(A) do{\
	register uint32_t *_pCLR_D=(void *)((A)->a);\
	register uint32_t _CLR_len=((A)->length<<1);\
	while (_CLR_len--) *_pCLR_D++=0;\
}while(0)

#define LW_MAT_CPY(A,B) do{\
	register uint32_t *_pCLR_D1=(void *)((A)->a);\
	register uint32_t *_pCLR_D2=(void *)((B)->a);\
	register uint32_t _CLR_len=(((A)->length)<<1);\
	while (_CLR_len--) *_pCLR_D1++=*_pCLR_D2++;\
}while(0)

#define LW_PARA_JOIN(PD,PS1,L1,PS2,L2) do{\
	register uint32_t *_p_D=(void *)(PD);\
	register uint32_t *_p_S1=(void *)(PS1);\
	register uint32_t *_p_S2=(void *)(PS2);\
	register uint32_t _L_1=(L1)<<1;\
	register uint32_t _L_2=(L2)<<1;\
	if(_p_S1){while(_L_1--) *_p_D++=*_p_S1++;}\
	else {while (_L_1--) *_p_D++=0;}\
	if(_p_S2){while (_L_2--) *_p_D++=*_p_S2++;}\
	else {while (_L_2--) *_p_D++=0;}\
}while(0)

#elif PARA_LEN==4
#define LW_MAT_CLR(A) do{\
	register uint32_t *_pCLR_D=(void *)((A)->a);\
	register uint32_t _CLR_len=(A)->length;\
	while (_CLR_len--) *_pCLR_D++=0;\
}while(0)

#define LW_MAT_CPY(A,B) do{\
	register uint32_t *_pCLR_D1=(void *)((A)->a);\
	register uint32_t *_pCLR_D2=(void *)((B)->a);\
	register uint32_t _CLR_len=(A)->length;\
	while (_CLR_len--) *_pCLR_D1++=*_pCLR_D2++;\
}while(0)

#define LW_PARA_JOIN(PD,PS1,L1,PS2,L2) do{\
	register uint32_t *_p_D=(void *)(PD);\
	register uint32_t *_p_S1=(void *)(PS1);\
	register uint32_t *_p_S2=(void *)(PS2);\
	register uint32_t _L_1=(L1);\
	register uint32_t _L_2=(L2);\
	if(_p_S1){while(_L_1--) *_p_D++=*_p_S1++;}\
	else {while (_L_1--) *_p_D++=0;}\
	if(_p_S2){while (_L_2--) *_p_D++=*_p_S2++;}\
	else {while (_L_2--) *_p_D++=0;}\
}while(0)

#define LW_WHILE_MEMSET(P,I,DATA) \
while ((I)--)\
	*(P)++ = DATA;

#define LW_WHILE_MEMCPY(P,Q,I) \
while ((I)--)\
	*(P)++ = *(Q)++;



#define LW_VEXTOR_DOT(SUM,P,Q,DIV,MOD) do{ \
uint32_t i=(DIV)\
(SUM)=_ParaFint(0);\
while (i--){\
	(SUM)=_ParaAdd((SUM),_ParaMul((P)[0],(Q)[0]));\
	(SUM)=_ParaAdd((SUM),_ParaMul((P)[1],(Q)[1]));\
	(P)+=2;(Q)+=2;\
}\
i=(MOD);\
while (i--)\
(SUM)=_ParaAdd((SUM),_ParaMul(*(P)++,*(Q)++));\
while(0);


#define LW_V_ADD_V_MUL_V(DST,S1,S2,DIV,MOD) do{ \
uint32_t i=(DIV)\
while (i--){\
	(DST)[0]=_ParaAdd((DST)[0],_ParaMul((S1)[0],(S2)[0]));\
	(DST)[1]=_ParaAdd((DST)[1],_ParaMul((S1)[1],(S2)[1]));\
	(S1)+=2;(S2)+=2;(DST)+=2;\
}\
i=(MOD);\
while (i--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMul(*(S1)++,*(S2)++));\
while(0);

#define LW_V_EQU_V_MUL_V(DST,S1,S2,DIV,MOD) do{ \
uint32_t i=(DIV)\
while (i--){\
	(DST)[0]=(_ParaMul((S1)[0],(S2)[0]);\
	(DST)[1]=(_ParaMul((S1)[1],(S2)[1]);\
	(S1)+=2;(S2)+=2;(DST)+=2;\
}\
i=(MOD);\
while (i--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMul(*(S1)++,*(S2)++));\
while(0);




#endif





static inline ParaType _MiaoToolMathExp256(ParaType _x)
{

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	register ParaType x = _ParaFrom(1) + (_x >> 8);
#else
	register ParaType x = _ParaFrom(1) + _ParaDiv(_x, _ParaFint(256));
#endif

	x = _ParaMul(x, x);x = _ParaMul(x, x);x = _ParaMul(x, x);
	x = _ParaMul(x, x);x = _ParaMul(x, x);x = _ParaMul(x, x);
	x = _ParaMul(x, x);x = _ParaMul(x, x);

	return x;
}


static	inline ParaType _MiaoToolMathExp1024(ParaType _x)
{

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	register ParaType x = _ParaFrom(1) + (_x >> 10);
#else
	register ParaType x = _ParaFrom(1) + _ParaDiv(_x, _ParaFint(1024));
#endif

	x = _ParaMul(x, x);x = _ParaMul(x, x);x = _ParaMul(x, x);
	x = _ParaMul(x, x);x = _ParaMul(x, x);x = _ParaMul(x, x);
	x = _ParaMul(x, x);x = _ParaMul(x, x);x = _ParaMul(x, x);
	x = _ParaMul(x, x);

	return x;
}


#endif //MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 4)
#endif
