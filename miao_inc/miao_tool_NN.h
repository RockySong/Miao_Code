/*(Ver.=0.96)
 * MIAO_tool.h
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#ifndef MIAO_TOOL_NN_H_
#define MIAO_TOOL_NN_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_basic_math.h"

#if !(MIAO_SIMPLE_LEVEL >= 2)

#if 0
typedef struct MIAO_TOOL_NN_LAYER_ACT_FUN_STRUCT{
	
	MiaoMat *(*Forward)(struct MIAO_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, MiaoMat *pOUTX, MiaoMat *pOut);
	MiaoMat *(*Backward)(struct MIAO_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, MiaoMat *pOUTX,MiaoMat *pOut);
	ParaType _p1;
}MiaoToolNNLayerActFun_t;

MIAO_DECLARE
(MiaoMat*,MiaoToolNNSigmoid_Forward,(MiaoToolNNLayerActFun_t *pAF,MiaoMat *pIn,MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*,MiaoToolNNSigmoid_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolSigmoid;


MIAO_DECLARE
(MiaoMat*, MiaoToolNNTanh_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*, MiaoToolNNTanh_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolTanh;


MIAO_DECLARE
(MiaoMat*,MiaoToolNNLinear_Forward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*,MiaoToolNNLinear_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolLinear;


MIAO_DECLARE
(MiaoMat*,MiaoToolNNReLU_Forward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*,MiaoToolNNReLU_Backward,(MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn,MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolReLU;

MIAO_DECLARE
(MiaoMat*, MiaoToolNNLeakyReLU_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*, MiaoToolNNLeakyReLU_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolLReLU;

MIAO_DECLARE
(MiaoMat*, MiaoToolNNSwish_Forward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
MIAO_DECLARE
(MiaoMat*, MiaoToolNNSwish_Backward, (MiaoToolNNLayerActFun_t *pAF, MiaoMat *pIn, MiaoMat *pOut));
extern  MiaoToolNNLayerActFun_t MiaoToolSwish;

/*
typedef struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT {
	struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT *pLayer;
	uint32_t sizeofdata;
	lw_u16 TrainTableColId;
	
}MiaoToolNNSubLayerBasic_t;


typedef struct MIAO_TOOL_NN_SUB_FULL_CONNECTION_LAYER_STRUCT
{
	MiaoToolNNSubLayerBasic_t basic;
	MiaoMat w;
	MiaoMat in, out;
	MiaoMat inErr, outErr;

}MiaoToolNNSubLayerFullCon_t;


typedef struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT {
	MiaoToolRandomBasic_t *pRand;
	MiaoExternMemory_t SubLayer;

	struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT(*ForwardFun)(struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB);
	struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT(*BackWardFun)(struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB, MiaoToolTable_t *pTable, lw_u16 row);
	struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT(*UpdateFun)(struct MIAO_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB, MiaoToolTable_t *pTable, lw_u16 row);


}MiaoToolNNLayerBasic_t;


typedef struct MIAO_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	MiaoToolNNLayerBasic_t basic;
	ParaType nl;
	lw_u16 xout_dim_z;
	MiaoMat out_x, out_y;
	MiaoMat delt, Bias;
	MiaoMat in, inErr, outErr;
	MiaoToolNNLayerActFun_t *pActFun;


}MiaoToolNNLayerFullCon_t;
*/
//typedef MiaoToolNNSubLayerFullCon_t MiaoToolNNLayerFullCon_t;




/*
//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
typedef struct MIAO_TOOL_NN_BATCH_NORMAL_STRUCT
{
	ParaType BN_gamma;
	ParaType BN_beta;
	ParaType BN_Epsi;

	ParaType RealMean;
	ParaType RealVar;

	ParaType NowMean;
	ParaType NowVar;

	ParaType *_p_org_x;

}MiaoToolNNBatchNorm_t;
*/

typedef struct MIAO_TOOL_NN_DROP_OUT_STRUCT
{
	MiaoMat DropOutMat;
	ParaType PossDropOut;

}MiaoToolNNDropOut_t;

//旧版本
typedef struct MIAO_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT{
	MiaoToolNNLayerActFun_t *pActFun;
	lw_u16 size_row,size_col;
	ParaType nl;
	MiaoToolRandomBasic_t *pRand;
	MiaoMat in, w, out, _out_x;
	MiaoMat inErr,outErr;
	union 
	{
		MiaoMat bias;
		MiaoMat _x_out_N;
	}BiasUN;

	union 
	{
		MiaoMat delta;
		MiaoMat _d_x_N;
	}DeltaUN;
	
	//MiaoToolNNBatchNorm_t *pBN;
	//ParaType PDropOut;
	MiaoToolNNDropOut_t *pDropOut;
}MiaoToolNNLayerFullCon_t;

void MiaoToolBPnnLayerInitial(
	MiaoToolNNLayerFullCon_t *pNNL,
	MiaoToolNNLayerActFun_t *pAF,
	lw_u16 size_row,lw_u16 size_col,
	ParaType *pW,
	ParaType *pBias,
	MiaoToolRandomBasic_t *pTRB,
	ParaType rmax,ParaType rmin,
	ParaType nl);

void MiaoToolBPnnLayerForwardInitial(MiaoToolNNLayerFullCon_t *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X);
void MiaoToolBPnnLayerBackwardInitial(MiaoToolNNLayerFullCon_t *pNNL,ParaType *pINerr,ParaType *pdelt,ParaType *pOuterr);
//void MiaoToolBPnnLayerReSize(MiaoToolNNLayerFullCon_t *pNNL,lw_u16 r,lw_u16 c);
//void  MiaoToolBPnnDropOutInitial(MiaoToolNNLayerFullCon_t *pNNL,ParaType P ,ParaType *pWBuf,ParaType *pBiasBuf,lw_u16 *pList1,lw_u16 *pList2);



typedef ParaType (*LossFunTYPE)(MiaoMat *s,MiaoMat *ref,MiaoMat *pre);


typedef enum
{
	MiaoToolNNTrainSaveMethod_OnceSave = 0,//单条存储 会将新计算出的delt加上原来的（一般使用 占用内存较小）
	MiaoToolNNTrainSaveMethod_AllSave = 1//完全存储 每一条delt是独立存储（用于算法优化使用）
}MiaoToolNNTrainSaveMethodNum;

typedef enum
{
	MiaoToolNNTrainUpdateMethod_Auto = 0,//自动更新 当计数器==样本总长度时 自动执行复习程序
	MiaoToolNNTrainUpdateMethod_Manu = 1//手动更新 当计数器==样本总长度时 不在存储 等待手动更新
}MiaoToolNNTrainUpdateMethodNum;


typedef enum
{
	MiaoToolNNTrainStatus_Wait = 0,
	MiaoToolNNTrainStatus_Complete = 1,
	MiaoToolNNTrainStatus_Over = 2,
	MiaoToolNNTrainStatus_Using = 3
}MiaoToolNNTrainStatusNum;

typedef struct MIAO_TOOL_NN_TRAIN_STRUCT {
	MiaoToolTable_t DeltTable;
	MiaoToolRandomBasic_t *pRand;
	LossFunTYPE LossFun;
	
	uint32_t BatchSetLength;
	uint32_t SampleRandNum;
	uint32_t SampleSetLength;
	uint32_t count;

	lw_u16 BatchSampleDiv;
	lw_u16 SampleCount;

	uint32_t _Count_for_NN;

	struct 
	{
		MiaoToolNNTrainSaveMethodNum SaveMethod : 4;
		MiaoToolNNTrainUpdateMethodNum UpdateMethod : 4;
		MiaoToolNNTrainStatusNum Status : 4;
		lw_u16 rst : 4;
	}Flag;


}MiaoToolNNTrain_t;

typedef enum
{
	MiaoToolNNReguType_NAN = 0,
	MiaoToolNNReguType_L1 = 1,
	MiaoToolNNReguType_L2 = 2

}MiaoToolNNReguTypeNUM;


typedef struct MIAO_TOOL_NN_REGU_STRUCT {
	ParaType Lambda;
	ParaType WeSum;
	
	MiaoToolNNReguTypeNUM ReguType:8;
	lw_u16 ReguSumFlag : 8;

}MiaoToolNNRegu_t;

typedef struct MIAO_TOOL_BP_NN_STRUCT{
	MiaoToolDList_t Net;

	MiaoToolNNTrain_t Train;
	MiaoToolNNRegu_t Regu;

	//LossFunTYPE loss;
	
	MiaoMat x, y, ref;
	ParaType Error;
	ParaType _nl_factor;

	
	lw_u16 ParaGroupNum;
	uint32_t WeightParaNum;
	uint32_t BiasParaNum;

}MiaoToolBPnn_t;




void MiaoToolBPnnInitial(MiaoToolBPnn_t *pBPnn);
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
	ParaType BN_epsi
);

//损失层
void MiaoToolBPnnLossInitial(
	MiaoToolBPnn_t *pBPnn,//this
	ParaType *pRef,//ref 地址（可为NULL）
	MiaoToolRandomBasic_t *pRand,//随机数指针
	MiaoMemoryBasic_t *pMem,//内存分配指针
	uint32_t set_len,//样本总长度
	uint32_t sample_len,//采样长度 
	LossFunTYPE loss,//损失计算函数
	MiaoToolNNTrainSaveMethodNum savem,//存储模式
	MiaoToolNNTrainUpdateMethodNum updatem//更新模式
);
void MiaoToolBPnnReguInitial(
	MiaoToolBPnn_t *pBPnn, 
	MiaoToolNNReguTypeNUM ReguType, 
	ParaType lambda
);

MiaoToolBPnn_t *MiaoToolBPnnAppend(MiaoToolBPnn_t *pBPnn,MiaoToolDListNode_t *pDLN,MiaoToolNNLayerFullCon_t *pNNL);


MiaoToolBPnn_t *MiaoToolBPnnRunning(MiaoToolBPnn_t *_pBPnn);
MiaoToolBPnn_t *MiaoToolBPnnErrCalu(MiaoToolBPnn_t *pBPnn);//误差计算
MiaoToolBPnn_t *MiaoToolBPnnDeltCalu(MiaoToolBPnn_t *_pBPnn, lw_u16 _batch_index);//增量计算
ParaType *MiaoToolBPnnGradForInput(MiaoToolBPnn_t *_pBPnn);//对输入求导
MiaoToolBPnn_t *_MiaoToolBPnnReviewOnce(MiaoToolBPnn_t *_pBPnn, lw_u16 batch_num, MiaoMat *pmat_k);
MiaoToolBPnn_t *MiaoToolBPnnReview(MiaoToolBPnn_t *_pBPnn);//复习
MiaoToolBPnn_t *MiaoToolBPnnTrain(MiaoToolBPnn_t *_pBPnn);//训练

MiaoToolBPnn_t *MiaoToolBPnnCopy(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2);
MiaoToolBPnn_t *MiaoToolBPnnSoftReplace(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2, ParaType tu);

#endif

typedef enum {
	MiaoToolNNSubLayerKind_FullCon='F',
	MiaoToolNNSubLayerKind_ActFun='A',
	MiaoToolNNSubLayerKind_Conv='C',
	MiaoToolNNSubLayerKind_Pool='P',
	MiaoToolNNSubLayerKind_BasicRnn = 'R',
	MiaoToolNNSubLayerKind_GruRnn = 'G',
	MiaoToolNNSubLayerKind_LNorm = 'L',
	MiaoToolNNSubLayerKind_INorm = 'I',
	MiaoToolNNSubLayerKind_Shift = 's',
	MiaoToolNNSubLayerKind_Split='S',
	MiaoToolNNSubLayerKind_Mix = 'M',
	MiaoToolNNSubLayerKind_Extend = 'E',
	MiaoToolNNSubLayerKind_Pad = 'p'
}MiaoToolNNSubLayerKind;

//神经网络层的基类
typedef struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT {
	MiaoMat in, out;//输入矩阵与输出矩阵
	lw_ptr(*Forward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//前向传递函数 输入:this 当前min_batch
	lw_ptr(*Backward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//反向传递函数 输入:this 当前min_batch
	lw_ptr(*Update)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//参数更新函数 输入:this
	lw_ptr(*NNmalloc)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);//内存分配函数 输入:this 前向传递参数指针 反向传递参数指针
	lw_ptr(*Clear)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//清空神经网络输出与delta 输入:this
	lw_ptr(*DataInit)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//数据初始化函数 用于初始化参数 输入:this
	lw_ptr(*Copy)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);//复制函数 输入:this1 this2
	lw_ptr(*SoftReplace)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);//软复制函数 输入:this1 this2
	
	MiaoToolNNSubLayerKind NN_Kind : 8;
	lw_u16 RunningStopFlag : 8;
	
	uint32_t sizeofdata;//sizeof(参数)
	
	MiaoExternMemory_t FlowData;

	
	ParaType nl;//学习速率
	ParaType LamdaL2;//L2正则化系数

	struct MIAO_TOOL_BP_NN_STRUCT *pNN;//指向本网络指针

}MiaoToolNNSubLayerBasic_t;

#define _MIAO_TOOL_NN_SUB_BASIC_DEFAULT MiaoToolBPnnLayerForwardDefault,MiaoToolBPnnLayerBackwardDefault,MiaoToolBPnnLayerUpdateDefault,MiaoToolBPnnLayerNNmallocDefault,MiaoToolBPnnLayerClearDefault,\
0,\
0,0,0,NULL,\
0,0,0,NULL,\
_ParaFint(0),\
NULL

//类型:初始化函数类型 可以适用于rand randi randn
typedef ParaType (*MiaoNNinitFunType)(void *p, ParaType d1, ParaType d2);


//神经网络全连接层
typedef struct MIAO_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	MiaoToolNNSubLayerBasic_t basic;//基类

	MiaoMat Weight, Bias;//权重与偏置
	MiaoMat DeltW, DeltB;//权重与偏置的修正量

	//权重初始化
	MiaoNNinitFunType pfun;//初始化函数 下面三个参数会原封不动输入内部
	void *pr;//
	ParaType initd1, initd2;//
	
	//偏置初始化（如果以下指针不为NULL 就调用权重初始化函数）
	//优先级为 _BiasInitFun>_BiasInit>pfun
	ParaType(*_BiasInitFun)();//偏置初始化函数 可以为NULL
	ParaType *_BiasInit;//偏置初始化数值 可以为NULL

	//数据恢复区指针
	ParaType *pRecover;

	
}MiaoToolNNLayerFullCon_t;

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerFullConNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	LwMatColType InCol,//权重的行 = 输入的行
	LwMatColType OutCol,//权重的列 = 输出的行
	lw_u16 max_mini_batch,//最大minibatch
	MiaoMemoryBasic_t *pmem//内存分配结构体
);

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerShiftNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType Col,
	LwMatColType weight_len,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem
);

typedef struct MIAO_TOOL_NN_LAYER_CONVOLUTION_STRUCT{
	MiaoToolNNLayerFullCon_t databasic;//借用全连接类 作为基类
	lw_u16 in_x, in_y;//输入图像的大小
	lw_u16 out_x, out_y;//输出图像的大小
	lw_u16 conv_kernal_x, conv_kernal_y;//卷积核的大小
	lw_u16 conv_kernal_z;//卷积核的高度 == 输入图像的高度
	lw_u16 conv_kernal_num;//卷积核的数量 == 输出图像的高度
	lw_u16 move_delt;
	MiaoToolMatrixConvMethodNUM ConvMethod : 16;
	lw_u16 im2col_flag;
}MiaoToolNNLayerConv_t;


MiaoMat * _MiaoToolNNKernalConvFd
(
	MiaoMat *s, MiaoMat *in_m, MiaoMat *kern_m, MiaoMat *bias,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y, lw_u16 out_z,
	lw_u16 move_delt,
	lw_u16 now_min_batch,
	lw_u8 move_method,
	ParaType *buf

);

MiaoMat* _MiaoToolNNKernalConvBK
(
	MiaoMat *inerr, MiaoMat *outerr, MiaoMat *kern,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 now_min_batch,
	lw_u16 move_delt,
	ParaType *buf
);

ParaType _MiaoToolNNKernalConvCalDeltOnce
(
	ParaType *delt, ParaType *inerr, ParaType *outerr1,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 min_batch,
	lw_u16 move_delt
);


//新建卷积层
MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerConvNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//输入图像维度 长 宽 高
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,//卷积核维度 长 宽 高
	lw_u16 move_delt,//移动间隔 默认1
	MiaoToolMatrixConvMethodNUM move_method,//移动模式
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoMemoryBasic_t *pmem,//内存分配指针
	uint32_t info[4]//下一层信息 分别为 输出图像 长 宽 高 总长度(用于与全连接连接）
);

//设置卷积层为im2col模式
MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerConvSetIm2Col(
	MiaoToolNNSubLayerBasic_t *pbase,
	uint32_t udata_sizeof_pdata,
	void *pdata
);



typedef enum {

	MiaoToolNNPoolingMethod_Max='M',
	MiaoToolNNPoolingMethod_Avg='A'
}
MiaoToolNNPoolingMethodNUM;


typedef struct MIAO_TOOL_NN_LAYER_POOLING_STRUCT {
	MiaoToolNNSubLayerBasic_t basic;
	ParaType **MaxPool;
	lw_u16 in_x, in_y;//输入图像的大小
	lw_u16 pic_z;//图像高度 输入输出共用
	lw_u16 out_x, out_y;//输出图像的大小
	lw_u16 move_x, move_y;//移动间隔
	MiaoToolNNPoolingMethodNUM PoolMethod : 16;
}MiaoToolNNLayerPool_t;


MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerPoolNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//输入图像维度 长 宽 高
	lw_u16 pool_x, lw_u16 pool_y,//池化大小
	MiaoToolNNPoolingMethodNUM pool_method,//池化方法
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoMemoryBasic_t *pmem,//内存分配指针
	uint32_t info[4]//下一层信息 分别为 输出图像 长 宽 高 总长度(用于与全连接连接)
);

void* _MiaoToolNNMaxPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
);

MiaoMat* _MiaoToolNNAvgPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
);

MiaoMat* _MiaoToolNNMaxPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
);

MiaoMat* _MiaoToolNNAvgPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
);

typedef struct MIAO_TOOL_NN_LAYER_ACT_FUN_STRUCT {

	MiaoToolNNSubLayerBasic_t basic;
	uint32_t _real_size;
	void(*_init)(struct MIAO_TOOL_NN_LAYER_ACT_FUN_STRUCT *p, lw_u8 TrainFlag);


}MiaoToolNNLayerActFun_t;

#define _MIAO_TOOL_NN_ACT_FUN_DEFAULT(NAME,KIND) NULL,0,0,0,NULL,0,0,0,\
MiaoToolBPnnLayer##NAME##Forward,MiaoToolBPnnLayer##NAME##Backward,MiaoToolBPnnLayerUpdateDefault,MiaoToolBPnnLayerNNmallocDefault,\
MiaoToolBPnnLayerClearDefault,MiaoToolBPnnLayerDataInitDefault,MiaoToolBPnnLayerCopyDefault,MiaoToolBPnnLayerSoftReplaceDefault,\
MiaoToolNNSubLayerKind_ActFun,0,0,\
NULL,0,\
_ParaFint(0),_ParaFint(0),\
NULL,\
sizeof(KIND),NULL

typedef struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT {
	MiaoMat cell_in_now;
	MiaoMat *cell_out_now_handle;

	MiaoExternMemory_t cell_h_mem, cell_out_mem;

	ParaType *(*cell_forward)(struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag);
	ParaType *(*cell_backward)(struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag);
	ParaType *(*cell_load)(struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, LwMatRowType index);
	ParaType *(*cell_parameter_init)(struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, ParaType *we, ParaType *bi, ParaType *dwe, ParaType *dbi, ParaType *stack);

	MiaoExternMemory_t cell_we, cell_bi;
	ParaType *p_cell_dwe, *p_cell_dbi;
	uint32_t FlowDataLen;
}MiaoToolNNLayerRnnCellBasic_t;

typedef void* (*MiaoToolNNLayerRnnCellNewFunType)(ParaType *, LwMatColType, LwMatColType,lw_u16,void *,void *);



void* MiaoToolNNLayerRnnCellNew(
	ParaType *pout,
	LwMatColType in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
	LwMatColType out_size,
	lw_u16 max_min_batch,
	MiaoToolNNLayerActFun_t *ptem,
	MiaoMemoryBasic_t *pmem
);

#define LwRnnCellType1 MiaoToolNNLayerRnnCellNew


typedef struct MIAO_TOOL_NN_LAYER_RECURRENT_CELL_STRUCT {
	MiaoToolNNLayerRnnCellBasic_t base;

	struct 
	{
		MiaoMat in, out;
		lw_ptr(*Forward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//前向传递函数 输入:this 当前min_batch
		lw_ptr(*Backward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//反向传递函数 输入:this 当前min_batch

	}ActFun;
	
}MiaoToolNNLayerRnnCell_t;



typedef struct MIAO_TOOL_NN_LAYER_RECURRENT_LNORM_CELL_STRUCT {
	MiaoToolNNLayerRnnCellBasic_t base;

	struct
	{
		MiaoMat in, out;
		lw_ptr(*Forward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//前向传递函数 输入:this 当前min_batch
		lw_ptr(*Backward)(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//反向传递函数 输入:this 当前min_batch
	}ActFun;

	ParaType *pNormWe, *pNormBi;
	ParaType *pmean, *pvar;
	ParaType *pHt, *pBuf;

}MiaoToolNNLayerRnnCellLNorm_t;

void* MiaoToolNNLayerRnnCellLNormNew(
	ParaType *pout,
	LwMatColType in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
	LwMatColType out_size,
	lw_u16 max_min_batch,
	MiaoToolNNLayerActFun_t *ptem,
	MiaoMemoryBasic_t *pmem
);

#define LwRnnCellType2 MiaoToolNNLayerRnnCellLNormNew

typedef struct MIAO_TOOL_NN_LAYER_BASIC_RECURRENT_STRUCT {
	MiaoToolNNLayerFullCon_t databasic;//借用全连接类 作为基类
	MiaoToolNNLayerRnnCellBasic_t *pRnnCell;
	MiaoMat out_t_1_mem;
	lw_u8 NeedTrainFlag;
}MiaoToolNNLayerBasicRnn_t;



MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerBasicRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pOut_t_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	MiaoToolNNLayerRnnCellNewFunType new_cell_fun,
	MiaoToolNNLayerActFun_t *ptem,
	MiaoMemoryBasic_t *pmem
);

//void MiaoToolNNLayerRnnCellForword(
//	MiaoToolNNLayerRnnCell_t *pCell
//);
//void MiaoToolNNLayerRnnCellBackword(
//	MiaoToolNNLayerRnnCell_t *pCell
//);
//void MiaoToolNNLayerRnnCellInit(
//	MiaoToolNNLayerRnnCell_t *pCell,
//	lw_u16 in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
//	lw_u16 out_size,
//	ParaType *pin,//只需要输入 因为隐藏层与输出要输入时候才确定
//	ParaType *pwe, ParaType *pbi,
//	ParaType *pdwe, ParaType *pdbi,
//	MiaoToolNNSubLayerBasic_t *pActTemplet
//);



typedef struct MIAO_TOOL_NN_LAYER_GRU_RECURRENT_STRUCT {
	MiaoToolNNLayerFullCon_t databasic;//借用全连接类 作为基类
	
	//RNN单元
	MiaoToolNNLayerRnnCellBasic_t *CoreCellHandle;
	//重置门
	MiaoToolNNLayerRnnCellBasic_t *ResetGateHandle;
	//更新门
	MiaoToolNNLayerRnnCellBasic_t *UpdateGateHandle;
	//上一轮记忆
	MiaoMat OutT_1;
	lw_u8 NeedTrainFlag;
}MiaoToolNNLayerGruRnn_t;


MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerGruRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pResetGateOut,
	ParaType *pUpdateGateOut,
	ParaType *pCoreCellOut,
	ParaType *pOutT_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	MiaoToolNNLayerRnnCellNewFunType new_cell_fun,
	MiaoToolNNLayerActFun_t *ptem,
	MiaoMemoryBasic_t *pmem
);



typedef struct MIAO_TOOL_NN_LAYER_SPILT_STRUCT {
	MiaoToolNNSubLayerBasic_t basic;
	LwMatColType ShapeList[4];
	ParaType **OutList;
	LwMatColType *SplitList;
	lw_u8 OutNum;
	lw_u8 Indim;
}MiaoToolNNLayerSplit_t;

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerSplitNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType **poutList,//输出指针列表 为NULL则分配 
	lw_u16 Dim, // 输入量的维度
	LwMatColType *ShapeList,//输入量的形状
	lw_u16 SplitNum,//要切成的块数量
	LwMatColType *SplitList,//具体的切法
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoMemoryBasic_t *pmem//内存分配指针
);

typedef struct MIAO_TOOL_NN_LAYER_MIX_STRUCT {
	MiaoToolNNSubLayerBasic_t basic;
	LwMatColType ShapeList[4];
	ParaType **InList;
	LwMatColType *MixList;
	lw_u8 InNum;
	lw_u8 Outdim;
}MiaoToolNNLayerMix_t;

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerMixNew(
	ParaType **pinList,//输入指针列表 为NULL则分配 
	ParaType *pout,//输出指针列表 为NULL则分配 
	lw_u16 Dim, // 输出量的维度
	LwMatColType *ShapeList,//输出量的形状
	lw_u16 MixNum,//块数量
	LwMatColType *MixList,//具体的切法
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoMemoryBasic_t *pmem//内存分配指针
);

typedef enum {
	MiaoToolNNPad_Constant=0,
	MiaoToolNNPad_Reflect=1
}MiaoToolNNPadNUM;

typedef struct MIAO_TOOL_NN_LAYER_PAD_STRUCT {
	MiaoToolNNSubLayerBasic_t basic;
	LwMatColType ShapeList[4];
	LwMatColType Padlist[4];
	lw_u8 PadLen;
	lw_u8 IOdim : 4;
	MiaoToolNNPadNUM PadMethod : 4;
}MiaoToolNNLayerPad_t;


MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerPadNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *PadList,//Pad的方法 1维或者2维
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoToolNNPadNUM method,
	MiaoMemoryBasic_t *pmem,//内存分配指针
	uint32_t info[4]
);


typedef enum {
	MiaoToolNNExtend_Nearest = 0,
	MiaoToolNNExtend_Linear = 1
}MiaoToolNNExtendNUM;

typedef struct MIAO_TOOL_NN_LAYER_EXTEND_STRUCT {
	MiaoToolNNSubLayerBasic_t basic;
	LwMatColType ShapeList[4];
	LwMatColType Extlist[2];
	lw_u8 ExtLen;
	lw_u8 IOdim : 4;
	MiaoToolNNExtendNUM ExtendMethod : 4;
}MiaoToolNNLayerExtend_t;

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerExtendNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *ExtList,//扩展的方法 1维或者2维
	lw_u16 max_mini_batch,//最大 minibatch
	MiaoToolNNExtendNUM method,
	MiaoMemoryBasic_t *pmem,//内存分配指针
	uint32_t info[4]
);


typedef struct MIAO_TOOL_NN_LAYER_ACT_FUN_SELU_STRUCT
{
	MiaoToolNNLayerActFun_t basic;
	ParaType *pForward, *pBackword;
}MiaoToolNNLayerActFunSeLU_t;


MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerActFunNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	LwMatColType Col,//列数
	lw_u16 max_mini_batch,//最大minibatch
	MiaoMemoryBasic_t *pmem,//内存分配结构体
	MiaoToolNNLayerActFun_t *pTemplet,//激活函数模板
	lw_u8 TrainFlag//是否训练
);



typedef struct MIAO_TOOL_NN_LAYER_NORM_STRUCT {
	MiaoToolNNLayerFullCon_t databasic;//借用全连接类 作为基类
	ParaType *pMean;
	ParaType *pVar;
}MiaoToolNNLayerLNorm_t;

ParaType _MiaoToolNNLayerLnForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType col
);


ParaType _MiaoToolNNLayerLnBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType col,
	ParaType *pBuf
);

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerLNormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType Col,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem
);

typedef struct MIAO_TOOL_NN_INSTANCE_NORM_STRUCT {
	MiaoToolNNLayerFullCon_t databasic;//借用全连接类 作为基类
	ParaType *pMean;
	ParaType *pVar;
	LwMatColType in_x, in_y, in_z;
}MiaoToolNNLayerINorm_t;


ParaType _MiaoToolNNLayerInForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType x_mpy_y
);


ParaType _MiaoToolNNLayerInBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType x_mpy_y,
	ParaType *pBuf
);

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerINormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType in_x,
	LwMatColType in_y,
	LwMatColType in_z,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem
);


//层缺省函数
lw_ptr MiaoToolBPnnLayerForwardDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerBackwardDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerUpdateDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerNNmallocDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr MiaoToolBPnnLayerClearDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerDataInitDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerCopyDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr MiaoToolBPnnLayerSoftReplaceDefault(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);


//全连接层函数
lw_ptr MiaoToolBPnnLayerFullConForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerFullConBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerFullConUpdate(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerFullConNNmalloc(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr MiaoToolBPnnLayerFullConClear(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerFullConDataInit(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerFullConCopy(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr MiaoToolBPnnLayerFullConSoftReplace(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);

lw_ptr MiaoToolBPnnLayerShiftForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerShiftBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//卷积层函数
lw_ptr MiaoToolBPnnLayerConvForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerConvBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//池化层函数
lw_ptr MiaoToolBPnnLayerPoolForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerPoolBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//BasicRnn函数
lw_ptr MiaoToolNNLayerBasicRnnBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerBasicRnnForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerBasicRnnDataInit(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerBasicRnnClear(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);


//GRURNN函数
lw_ptr MiaoToolNNLayerGruRnnBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerGruRnnForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerGruRnnDataInit(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr MiaoToolBPnnLayerGruRnnClear(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);


//layer norm函数
lw_ptr MiaoToolNNLayerLNormForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerLNormBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//Instance norm函数
lw_ptr MiaoToolNNLayerINormForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerINormBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//切分层函数
lw_ptr MiaoToolNNLayerSplitForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerSplitBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//混合层函数
lw_ptr MiaoToolNNLayerMixForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerMixBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//pad
lw_ptr MiaoToolNNLayerPadForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerPadBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//扩展层
lw_ptr MiaoToolNNLayerExtendForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolNNLayerExtendBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


extern MiaoToolNNLayerActFun_t _MiaoToolNNSigmoid;
extern MiaoToolNNLayerActFun_t *LwSigmoid;
lw_ptr MiaoToolBPnnLayerSigmoidForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerSigmoidBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


extern MiaoToolNNLayerActFun_t _MiaoToolNNTanh;
extern MiaoToolNNLayerActFun_t *LwTanh;
lw_ptr MiaoToolBPnnLayerTanhForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerTanhBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern MiaoToolNNLayerActFun_t _MiaoToolNNReLU;
extern MiaoToolNNLayerActFun_t *LwReLU;
lw_ptr MiaoToolBPnnLayerReLUForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerReLUBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern MiaoToolNNLayerActFun_t _MiaoToolNNReLU6;
extern MiaoToolNNLayerActFun_t *LwReLU6;
lw_ptr MiaoToolBPnnLayerReLU6Forward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerReLU6Backward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern MiaoToolNNLayerActFun_t _MiaoToolNNSwish;
extern MiaoToolNNLayerActFun_t *LwSwish;
lw_ptr MiaoToolBPnnLayerSwishForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerSwishBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern MiaoToolNNLayerActFunSeLU_t _MiaoToolNNSeLU;
extern MiaoToolNNLayerActFun_t *LwSeLU;
lw_ptr MiaoToolBPnnLayerSeLUForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerSeLUBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern MiaoToolNNLayerActFun_t _MiaoToolNNSoftMax;
extern MiaoToolNNLayerActFun_t *LwSoftMax;
lw_ptr MiaoToolBPnnLayerSoftMaxForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr MiaoToolBPnnLayerSoftMaxBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

#define _MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB) 	register uint32_t i, _len;\
register ParaType *_in, *_out;\
MIAO_assert(!(pNNSLB));\
_in = pNNSLB->in.a;\
_out = pNNSLB->out.a;\
_len = pNNSLB->out.col*(uint32_t)(mini_b_num);\
for (i = 0; i < _len;i++){
	


#define _MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB,mini_b_num) 			_in++;_out++;}\
return mini_b_num;\

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F

#define _MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB) 	register lw_u16 i, j, mmb;\
register ParaType *_in, *_out, *_inb, *_outb;\
MIAO_assert(!(pNNSLB));\
_inb = pNNSLB->in.a;\
_outb = pNNSLB->out.a;\
mmb = pNNSLB->out.col;\
for (i = 0; i < pNNSLB->in.row; i++)\
{\
	_in = _inb;\
	_out = _outb;\
	for (j = 0; j <(mini_b_num& 0xffff); j++)\
	{\


#define _MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB,mini_b_num) 			_in++;\
_out++;\
	}\
_inb += mmb;\
_outb += mmb;}\
return mini_b_num;\

#else

#error"NN data frame is unknown"

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C




typedef enum
{
	MiaoToolNNReguType_NAN = 0,
	MiaoToolNNReguType_L1 = 1,
	MiaoToolNNReguType_L2 = 2

}MiaoToolNNReguTypeNUM;


//typedef struct MIAO_TOOL_NN_REGU_STRUCT {
//	ParaType Lambda;
//	ParaType WeSum;
//
//	MiaoToolNNReguTypeNUM ReguType : 8;
//	lw_u16 ReguSumFlag : 8;
//
//}MiaoToolNNRegu_t;

typedef ParaType(*LossFunTYPE)(MiaoMat *s, MiaoMat *ref, MiaoMat *pre);



/*
typedef enum
{
	MiaoToolNNTrainUpdateMethod_Auto = 0,//自动更新 当计数器==样本总长度时 自动执行复习程序
	MiaoToolNNTrainUpdateMethod_Manu = 1//手动更新 当计数器==样本总长度时 不在存储 等待手动更新
}MiaoToolNNTrainUpdateMethodNum;
*/

typedef enum
{
	MiaoToolNNTrainStatus_Wait = 0,
	MiaoToolNNTrainStatus_Complete = 1,
	MiaoToolNNTrainStatus_Over = 2,
	MiaoToolNNTrainStatus_Using = 3
}MiaoToolNNTrainStatusNum;


typedef enum
{
	MiaoToolNNNeedTrain_Need = 0,
	MiaoToolNNNeedTrain_NeedNot = 1
}MiaoToolNNNeedTrainNum;

typedef enum
{
	MiaoToolNNMemoryMethod_Chip = 0,
	MiaoToolNNMemoryMethod_Block = 1

}MiaoToolNNMemoryMethodNum;


typedef enum
{
	MiaoToolNNOptim_GradDesc = 0,
	MiaoToolNNOptim_M = 1,
	MiaoToolNNOptim_RMSp = 2,
	MiaoToolNNOptim_Adam = 3,
	MiaoToolNNOptim_Nadam = 4

}MiaoToolNNOptimNum;

typedef struct MIAO_TOOL_NN_TRAIN_STRUCT {
	
	MiaoToolTable_t DataTable;
	LossFunTYPE LossFun;
	//学习速率
	ParaType nl;
	
	//L2正则化
	ParaType L2Regu;

	//梯度限幅
	ParaType DeltLimitMax;
	ParaType DeltLimitMin;

	ParaType Beta1, Beta1T;
	ParaType Beta2, Beta2T;
	ParaType Epsi;

	uint32_t AllBatchCount;
	uint32_t AllBatchMax;

	lw_u16 _batch_stream_count;

	lw_u16 mini_batch_now;
	lw_u16 mini_batch_max;

	lw_u16 _MemAllocCoff;

	lw_u16 y_offset;

	struct
	{
		MiaoToolNNNeedTrainNum NeedTrain : 2;
		lw_u16 EnableDeltLimit : 2;
		//MiaoToolNNTrainUpdateMethodNum UpdateMethod : 2;
		MiaoToolNNTrainStatusNum Status : 4;
		MiaoToolNNMemoryMethodNum MemoryMethod : 4;
		MiaoToolNNOptimNum Optim : 4;
	}Flag;

}MiaoToolNNTrain_t;



typedef struct MIAO_TOOL_BP_NN_STRUCT {
	MiaoToolDList_t Net;

	MiaoToolNNTrain_t Train;
	MiaoMat x, y, ref;

	ParaType Error;
	ParaType _nl_factor;

	MiaoMemoryBasic_t *pmem;

	lw_u16 ParaGroupNum;
}MiaoToolBPnn_t;


#define NN_LAYER_S_BAK_ARR_LEN 6


typedef struct MIAO_TOOL_NN_LAYER_SIMPLE_BACK_UP_STRUCT{

	MiaoToolNNSubLayerKind NN_Kind;
	uint32_t info[NN_LAYER_S_BAK_ARR_LEN];
	
	MiaoToolNNLayerActFun_t *pTemplet;
	
	ParaType *pDataAddr;
	
}MiaoToolNNLayerSimpleBak_t;



void MiaoToolBPnnInit(MiaoToolBPnn_t *pBPnn, lw_u16 max_mini_batch);

void MiaoToolBPnnLayerAppend
(
	MiaoToolBPnn_t *pBPnn,
	MiaoToolDListNode_t *pnode,
	uint32_t len,
	void *LayerList
);

#define _NN_GET_IN(PBPNN,PIN) do{if ((PIN) == NULL)\
{\
	if ((PBPNN)->Net.NowLen == 1)\
	{\
		(PIN) = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*(PBPNN)->Train.mini_batch_max));\
		MiaoMatInit(&((PBPNN)->x), (PBPNN)->Train.mini_batch_max, in_col, (PIN));\
	}\
	else\
	{\
		pptail = (PBPNN)->Net.pTail->Data.pData;\
		(PIN) = pptail[(PBPNN)->Net.pTail->Data.uData - 1]->out.a;\
	}\
}\
else\
{\
	if ((PBPNN)->Net.NowLen == 1)MiaoMatInit(&((PBPNN)->x), (PBPNN)->Train.mini_batch_max, in_col, (PIN));\
}\
}while (0)


#define _NN_FULL_CON_CHIP_ALLAC_1(P_BPNN,P_FC) do{\
if ((P_BPNN)->Train.Flag.MemoryMethod == MiaoToolNNMemoryMethod_Chip)\
{\
	(P_FC)->Weight.a = pWe == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL((P_FC)->Weight.length)) : pWe;\
	(P_FC)->Bias.a = pBi == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL((P_FC)->Bias.length)) : pBi;\
	if ((P_BPNN)->Train.Flag.NeedTrain == MiaoToolNNNeedTrain_Need)\
	{\
		(P_FC->basic).NNmalloc((void *)P_FC, NULL, pmem->Malloc(pmem, (P_FC->basic).sizeofdata * (uint32_t)((P_BPNN)->Train._MemAllocCoff)));\
	}\
}\
}while(0)

#define _NN_FULL_CON_CHIP_ALLAC() do{\
memcpy(&(pBPnn->y), &(pfc->basic.out), sizeof(MiaoMat));\
pfc->pfun = pfun;pfc->pr = pr;pfc->initd1 = d1;pfc->initd2 = d2;\
if (pBPnn->Train.Flag.MemoryMethod == MiaoToolNNMemoryMethod_Chip)\
{\
	pfc->Weight.a = pWe == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Weight.length)) : pWe;\
	pfc->Bias.a = pBi == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Bias.length)) : pBi;\
	if (pBPnn->Train.Flag.NeedTrain == MiaoToolNNNeedTrain_Need)\
	{\
		pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (uint32_t)(pBPnn->Train._MemAllocCoff)));\
	}\
}\
}while(0)


void* MiaoToolBPnnFullConAppend
(
	MiaoToolBPnn_t *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	MiaoNNinitFunType pfun,
	MiaoToolRandomBasic_t *pr,
	ParaType d1, ParaType d2,
	MiaoToolNNLayerActFun_t *pTemplet,
	MiaoMemoryBasic_t *pmem
);

void* MiaoToolBPnnConvAppend
(
	MiaoToolBPnn_t *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	MiaoToolMatrixConvMethodNUM conv_method,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	MiaoNNinitFunType pfun,
	MiaoToolRandomBasic_t *pr,
	ParaType d1, ParaType d2,
	MiaoToolNNLayerActFun_t *pTemplet,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
);

void* MiaoToolBPnnPoolAppend
(
	MiaoToolBPnn_t *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	MiaoToolNNPoolingMethodNUM pool_method,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
);

void* MiaoToolBPnnBasicRnnAppend
(
	MiaoToolBPnn_t *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	MiaoNNinitFunType pfun,
	MiaoToolRandomBasic_t *pr,
	ParaType d1, ParaType d2,
	MiaoToolNNLayerRnnCellNewFunType new_cell_fun,
	MiaoToolNNLayerActFun_t *pTemplet,
	MiaoMemoryBasic_t *pmem
);

void* MiaoToolBPnnGruRnnAppend
(
	MiaoToolBPnn_t *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	MiaoNNinitFunType pfun,
	MiaoToolRandomBasic_t *pr,
	ParaType d1, ParaType d2,
	MiaoToolNNLayerRnnCellNewFunType new_cell_fun,
	MiaoToolNNLayerActFun_t *pTemplet,
	MiaoMemoryBasic_t *pmem
);

void* MiaoToolBPnnLNormAppend
(
	MiaoToolBPnn_t *pBPnn,
	LwMatColType in_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	ParaType *pmean, ParaType *pvar,
	MiaoMemoryBasic_t *pmem
);

void* MiaoToolBPnnPadAppend
(
	MiaoToolBPnn_t *pBPnn,
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *PadList,//Pad的方法 1维或者2维
	ParaType *pin, ParaType *pout,
	MiaoToolNNPadNUM method,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
);

void* MiaoToolBPnnExtendAppend
(
	MiaoToolBPnn_t *pBPnn,
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *ExtList,//Ext的方法
	ParaType *pin, ParaType *pout,
	MiaoToolNNExtendNUM method,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
);


void MiaoToolBPnnTrainInit(
	MiaoToolBPnn_t *pBPnn,
	ParaType *pRef,
	MiaoMemoryBasic_t *pmem,
	LossFunTYPE loss,
	ParaType nl
);

void MiaoToolBPnnOptimInit(
	MiaoToolBPnn_t *pBPnn,
	MiaoToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
);

void* MiaoToolNNLayerSimpleRecover
(
	MiaoToolBPnn_t *pBPnn,
	ParaType *in,ParaType *out,
	MiaoToolNNLayerSimpleBak_t *pSimpleBak,
	MiaoMemoryBasic_t *pmem
);

void* MiaoToolBPnnSimpleRecover
(
	MiaoToolBPnn_t *pBPnn,
	ParaType *in,ParaType *out,
	MiaoToolNNLayerSimpleBak_t *pSimpleBak,
	lw_u16 simple_layer_len,
	MiaoMemoryBasic_t *pmem
);



void MiaoToolBPnnAllDataInit(MiaoToolBPnn_t *pBPnn, MiaoMemoryBasic_t *pMem);
void MiaoToolBPnnForward(MiaoToolBPnn_t *pBPnn);
void MiaoToolBPnnBackward(MiaoToolBPnn_t *pBPnn);
void MiaoToolBPnnLoadX(MiaoToolBPnn_t *pBPnn, MiaoMat *xs);
void MiaoToolBPnnLoadY(MiaoToolBPnn_t *pBPnn, MiaoMat *ys);
void MiaoToolBPnnRun(MiaoToolBPnn_t *pBPnn, MiaoMat *xs);
void MiaoToolBPnnCalErr(MiaoToolBPnn_t *pBPnn);
void MiaoToolBPnnTrain(MiaoToolBPnn_t *pBPnn, MiaoMat *ys);
void MiaoToolBPnnReview(MiaoToolBPnn_t *pBPnn);
void MiaoToolBPnnClear(MiaoToolBPnn_t *pBPnn);
void MiaoToolBPnnCopy(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2);
void MiaoToolBPnnSoftReplace(MiaoToolBPnn_t *pBPnn1, MiaoToolBPnn_t *pBPnn2, ParaType Tau);
MiaoToolNNSubLayerBasic_t *MiaoToolBPnnAt(MiaoToolBPnn_t *pBPnn, lw_32 i, lw_32 j);

#define MiaoToolBPnnL2ReguInit(A, B) do{(A)->Train.L2Regu=B;}while(0)

#endif // !(MIAO_SIMPLE_LEVEL >= 2)

#endif
