/*(Ver.=0.96)
 * MIAO_tool.c
 *
 *  Created on: 2019-01-22
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(MIAO_SIMPLE_LEVEL >= 2)


#if MIAO_TOOL_IMPORT_NN || MIAO_TOOL_IMPORT_ALL


#if MIAO_TOOL_IMPORT_NN_BPnn || MIAO_TOOL_IMPORT_ALL


static const ParaType _LwMathTanhTable[32][4] = { _ParaFrom(-0.323768137), _ParaFrom(0.000000000), _ParaFrom(0.999829378), _ParaFrom(0.000000012),
_ParaFrom(-0.224577786),_ParaFrom(-0.060942552), _ParaFrom(1.012310413),_ParaFrom(-0.000852027),
_ParaFrom(-0.085294811),_ParaFrom(-0.232093472), _ParaFrom(1.082413830),_ParaFrom(-0.010423480),
_ParaFrom(0.030060432),_ParaFrom(-0.444716255), _ParaFrom(1.213049268),_ParaFrom(-0.037177618),
_ParaFrom(0.093805209),_ParaFrom(-0.601375420), _ParaFrom(1.341384456),_ParaFrom(-0.072221680),
_ParaFrom(0.112109067),_ParaFrom(-0.657604872), _ParaFrom(1.398963414),_ParaFrom(-0.091875298),
_ParaFrom(0.103550306),_ParaFrom(-0.626053856), _ParaFrom(1.360193527),_ParaFrom(-0.075995152),
_ParaFrom(0.084237250),_ParaFrom(-0.542992262), _ParaFrom(1.241116425),_ParaFrom(-0.019092174),
_ParaFrom(0.063665066),_ParaFrom(-0.441875863), _ParaFrom(1.075447316), _ParaFrom(0.071385249),
_ParaFrom(0.045976471),_ParaFrom(-0.344065011), _ParaFrom(0.895162355), _ParaFrom(0.182152329),
_ParaFrom(0.032252527),_ParaFrom(-0.259745097), _ParaFrom(0.722475170), _ParaFrom(0.300040114),
_ParaFrom(0.022203363),_ParaFrom(-0.191828827), _ParaFrom(0.569473397), _ParaFrom(0.414934245),
_ParaFrom(0.015098239),_ParaFrom(-0.139444167), _ParaFrom(0.440732858), _ParaFrom(0.520398495),
_ParaFrom(0.010183947),_ParaFrom(-0.100192739), _ParaFrom(0.336229856), _ParaFrom(0.613141426),
_ParaFrom(0.006832574),_ParaFrom(-0.071365567), _ParaFrom(0.253576587), _ParaFrom(0.692135910),
_ParaFrom(0.004567905),_ParaFrom(-0.050494378), _ParaFrom(0.189460295), _ParaFrom(0.757790993),
_ParaFrom(0.003046723),_ParaFrom(-0.035540547), _ParaFrom(0.140459581), _ParaFrom(0.811312839),
_ParaFrom(0.002028966),_ParaFrom(-0.024910282), _ParaFrom(0.103449253), _ParaFrom(0.854264559),
_ParaFrom(0.001349801),_ParaFrom(-0.017399257), _ParaFrom(0.075760608), _ParaFrom(0.888288365),
_ParaFrom(0.000897363),_ParaFrom(-0.012117675), _ParaFrom(0.055208919), _ParaFrom(0.914945277),
_ParaFrom(0.000596306),_ParaFrom(-0.008418297), _ParaFrom(0.040056265), _ParaFrom(0.935633700),
_ParaFrom(0.000396132),_ParaFrom(-0.005835573), _ParaFrom(0.028948485), _ParaFrom(0.951557814),
_ParaFrom(0.000263102),_ParaFrom(-0.004037429), _ParaFrom(0.020846768), _ParaFrom(0.963725513),
_ParaFrom(0.000174724),_ParaFrom(-0.002788538), _ParaFrom(0.014963994), _ParaFrom(0.972962252),
_ParaFrom(0.000116020),_ParaFrom(-0.001922917), _ParaFrom(0.010709291), _ParaFrom(0.979933157),
_ParaFrom(0.000077044),_ParaFrom(-0.001324238), _ParaFrom(0.007644057), _ParaFrom(0.985164489),
_ParaFrom(0.000051128),_ParaFrom(-0.000910244), _ParaFrom(0.005439617), _ParaFrom(0.989077223),
_ParaFrom(0.000034046),_ParaFrom(-0.000626889), _ParaFrom(0.003872778), _ParaFrom(0.991965222),
_ParaFrom(0.000022230),_ParaFrom(-0.000423612), _ParaFrom(0.002707107), _ParaFrom(0.994193362),
_ParaFrom(0.000016157),_ParaFrom(-0.000315402), _ParaFrom(0.002064429), _ParaFrom(0.995465695),
_ParaFrom(0.000005511),_ParaFrom(-0.000119165), _ParaFrom(0.000858748), _ParaFrom(0.997934929),
_ParaFrom(0.000023128),_ParaFrom(-0.000454708), _ParaFrom(0.002989045), _ParaFrom(0.993426652) };



 ParaType MiaoToolMathTanh(ParaType _x)
{
	register ParaType y;
	register uint32_t _i;

	if (_x>0)
	{

		if (_x>_ParaFrom(6.5536))
		{
			return _ParaFrom(1);
		}

		_i = (_ParaInt(_ParaMul(_x, _ParaFrom(10000)))) >> 11;
		y = _ParaAdd(_LwMathTanhTable[_i][1], _ParaMul(_x, _LwMathTanhTable[_i][0]));
		y = _ParaAdd(_LwMathTanhTable[_i][2], _ParaMul(y, _x));
		y = _ParaAdd(_LwMathTanhTable[_i][3], _ParaMul(y, _x));

		return y;

	}
	else if (_x<0)
	{
		if (_x<_ParaFrom(-6.5536))
		{
			return _ParaFrom(-1);
		}

		_x = -_x;

		_i = (_ParaInt(_ParaMul(_x, _ParaFrom(10000)))) >> 11;
		y = _x;
		y = _ParaAdd(_LwMathTanhTable[_i][1], _ParaMul(_x, _LwMathTanhTable[_i][0]));
		y = _ParaAdd(_LwMathTanhTable[_i][2], _ParaMul(y, _x));
		y = _ParaAdd(_LwMathTanhTable[_i][3], _ParaMul(y, _x));

		return -y;

	}

	return _ParaFrom(0);

}


 ParaType MiaoToolMathSigmoid(ParaType _x)
{

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	register ParaType y;
	y = MiaoToolMathTanh(_x >> 1);
	return (y + _ParaFrom(1)) >> 1;
#else
	register ParaType y;
	y = MiaoToolMathTanh(_ParaMul(_x, _ParaFrom(0.5)));
	return _ParaMul((y + _ParaFrom(1)), _ParaFrom(0.5));
#endif

}

MiaoToolNNLayerActFun_t _MiaoToolNNSigmoid = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(Sigmoid,MiaoToolNNLayerActFun_t)};
MiaoToolNNLayerActFun_t *LwSigmoid = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNSigmoid;
lw_ptr MiaoToolBPnnLayerSigmoidForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		//*_out=_ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMul(*_in, _ParaFint(-1)))));
		*_out = MiaoToolMathSigmoid(*_in);
		*_in = *_out;
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr MiaoToolBPnnLayerSigmoidBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaMul(
			_ParaMul(*_in, _ParaSub(_ParaFint(1), *_in))
			, *_out
		);
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

MiaoToolNNLayerActFun_t _MiaoToolNNTanh = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(Tanh,MiaoToolNNLayerActFun_t) };
MiaoToolNNLayerActFun_t *LwTanh = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNTanh;

lw_ptr MiaoToolBPnnLayerTanhForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	//ParaType _exp_x,_exp_nx;
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		//_exp_x = _ParaExp(*_in);
		//_exp_nx = _ParaExp(-(*_in));
		//*_out = _ParaDiv(
		//	_ParaSub(_exp_x, _exp_nx),
		//	_ParaAdd(_exp_x, _exp_nx)
		//);
		*_out = MiaoToolMathTanh(*_in);
		*_in = *_out;
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr MiaoToolBPnnLayerTanhBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaSub(
			_ParaFint(1),
			_ParaMul(*_in, *_in)
		);
		*_in = _ParaMul(*_in, *_out);

	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


MiaoToolNNLayerActFun_t _MiaoToolNNReLU = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(ReLU,MiaoToolNNLayerActFun_t) };
MiaoToolNNLayerActFun_t *LwReLU = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNReLU;
lw_ptr MiaoToolBPnnLayerReLUForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
		*_in = *_out;
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr MiaoToolBPnnLayerReLUBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaMul(
		((*_in > _ParaInt(0)) ? _ParaInt(1) : _ParaInt(0))
		, *_out
		);

	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

MiaoToolNNLayerActFun_t _MiaoToolNNReLU6 = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(ReLU6,MiaoToolNNLayerActFun_t) };
MiaoToolNNLayerActFun_t *LwReLU6 = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNReLU6;

lw_ptr MiaoToolBPnnLayerReLU6Forward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
	*_out = (*_in < _ParaInt(6)) ? *_in : _ParaInt(6);
	*_in = *_out;
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr MiaoToolBPnnLayerReLU6Backward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register ParaType temp;
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	temp = (*_in > _ParaFrom(0)) ? _ParaFrom(1) : _ParaFrom(0);
	temp = (*_in < _ParaFrom(6)) ? _ParaFrom(1) : _ParaFrom(0);
	*_in = _ParaMul(
		temp
		, *_out
	);

	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


MiaoToolNNLayerActFun_t _MiaoToolNNSwish = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(Swish,MiaoToolNNLayerActFun_t) };
MiaoToolNNLayerActFun_t *LwSwish = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNSwish;

lw_ptr MiaoToolBPnnLayerSwishForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out= _ParaMul(*_in,
			_ParaDiv(
				_ParaFint(1),
				_ParaAdd(
					_ParaFint(1),
					_ParaExp(
						_ParaMul(
							*_in, _ParaFint(-1)
						)
					)
				)
			)
		);
	//*_in = *_out;保留输入
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr MiaoToolBPnnLayerSwishBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register ParaType temp;
	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	temp= _ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMul(*_in, _ParaFint(-1)))));
	//x*s+s(1-x*s)=x*s+s-x*s^2=s*(x+1-x*s)
	*_in = _ParaMul(
		temp, 
		_ParaAdd(
			*_in, 
			_ParaSub(
				_ParaFint(1), _ParaMul(*_in, temp)
			)
		)
	);
	*_in = _ParaMul(
		*_in
		, *_out
	);
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

static ParaType _MiaoToolNNSeLU_Default[2] = { _ParaFrom(1.0507009873554804934193349852946),_ParaFrom(1.6732632423543772848170429916717) };
MiaoToolNNLayerActFunSeLU_t _MiaoToolNNSeLU = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(SeLU,MiaoToolNNLayerActFunSeLU_t),_MiaoToolNNSeLU_Default,NULL};
MiaoToolNNLayerActFun_t *LwSeLU = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNSeLU;

lw_ptr MiaoToolBPnnLayerSeLUForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	register ParaType Lamer, alpha;
	register ParaType temp;
	register uint32_t i, _len;
	register ParaType *_in, *_out;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;
	_len = pNNSLB->out.col*(uint32_t)(mini_b_num);

	pthis = (MiaoToolNNLayerActFunSeLU_t *)pNNSLB;
	Lamer = pthis->pForward[0];
	alpha = pthis->pForward[1];

	for (i = 0; i < _len; i++)
	{
		if (*_in > _ParaFrom(0))
		{
			temp = *_in;
		}
		else
		{
			temp = _ParaMul(alpha, _ParaSub(_ParaExp(*_in), _ParaFrom(-1)));
		}

		*_out = _ParaMul(Lamer, temp);
		*_in = *_out;

		_in++;
		_out++;
	}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	register ParaType Lamer, alpha;
	register ParaType temp;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;
	pthis = (MiaoToolNNLayerActFunSeLU_t *)pNNSLB;
	Lamer = pthis->pForward[0];
	alpha = pthis->pForward[1];

	for (i = 0; i < pNNSLB->in.row; i++)
	{
		_in = _inb;
		_out = _outb;
		for (j = 0; j < (mini_b_num & 0xffff); j++)
		{
			if (*_in > _ParaFrom(0))
			{
				temp = *_in;
			}
			else
			{
				temp = _ParaMul(alpha, _ParaSub(_ParaExp(*_in), _ParaFrom(-1)));
			}

			*_out = _ParaMul(Lamer, temp);
			*_in = *_out;

			_in++;
			_out++;
		}
		_inb += mmb;
		_outb += mmb;
}
#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C



	return mini_b_num;

}


lw_ptr MiaoToolBPnnLayerSeLUBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	register ParaType LamerMulAlpha, Lamder;
	register ParaType temp;
	register uint32_t i, _len;
	register ParaType *_in, *_out;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;
	_len = pNNSLB->out.col*(uint32_t)(mini_b_num);

	pthis = (MiaoToolNNLayerActFunSeLU_t *)pNNSLB;
	Lamder = pthis->pForward[0];
	LamerMulAlpha = _ParaMul(pthis->pForward[0], pthis->pForward[1]);
	for (i = 0; i < _len; i++)
	{
		if (*_in > _ParaFrom(0))
		{
			temp = Lamder;
		}
		else
		{
			temp = _ParaAdd(*_in, LamerMulAlpha);
		}

		*_in = _ParaMul(
			temp
			, *_out
		);

		_in++;
		_out++;

	}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	
	register ParaType LamerMulAlpha, Lamder;
	register ParaType temp;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;
	pthis = (MiaoToolNNLayerActFunSeLU_t *)pNNSLB;
	Lamder = pthis->pForward[0];
	LamerMulAlpha = _ParaMul(pthis->pForward[0], pthis->pForward[1]);
	for (i = 0; i < pNNSLB->in.row; i++)
	{
		_in = _inb;
		_out = _outb;
		for (j = 0; j < (mini_b_num & 0xffff); j++)
		{
			if (*_in > _ParaFrom(0))
			{
				temp = Lamder;
			}
			else
			{
				temp = _ParaAdd(*_in, LamerMulAlpha);
			}

			*_in = _ParaMul(
				temp
				, *_out
			);

			_in++;
			_out++;
		}
		_inb += mmb;
		_outb += mmb;
	}

#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	return mini_b_num;

}


MiaoToolNNLayerActFun_t _MiaoToolNNSoftMax = { _MIAO_TOOL_NN_ACT_FUN_DEFAULT(SoftMax,MiaoToolNNLayerActFun_t) };
MiaoToolNNLayerActFun_t *LwSoftMax = (MiaoToolNNLayerActFun_t *)&_MiaoToolNNSoftMax;

lw_ptr MiaoToolBPnnLayerSoftMaxForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	
#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	register ParaType rmax, rsum;
	register lw_u16 i, j, col;
	register ParaType *_in, *_out, *_inb,*_outb;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	col = pNNSLB->out.col;
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;

	for (i = 0; i < (mini_b_num & 0xffff); i++)
	{

		//查询max
		_in = _inb;
		_out = _outb;

		rmax = *_in;
		for (j = 0; j < col; j++)
		{
			if (_in[j] > rmax)
			{
				rmax = _in[j];
			}
		}
		//计算差值 sum y与数值
		rsum = _ParaFint(0);
		for (j = 0; j < col; j++)
		{
			_out[j] = _ParaExp(_ParaSub(_in[j], rmax));
			rsum = _ParaAdd(rsum, _out[j]);
		}

		//归一化
		for (j = 0; j < col; j++)
		{
			_out[j] = _ParaDiv(_out[j], rsum);
		}

		_inb += col;
		_outb += col;

	}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	register ParaType rmax, rsum;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	MiaoToolNNLayerActFunSeLU_t *pthis;
	MIAO_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;

	for (i = 0; i < (mini_b_num & 0xffff); i++)
	{

		//查询max
		_in = _inb;
		rmax = *_in;
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			if (*_in > rmax)
			{
				rmax = *_in;
			}
			_in += mmb;
		}

		//计算差值 sum y与数值
		_in = _inb;
		_out = _outb;
		rsum = _ParaFint(0);
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			*_out = _ParaExp(_ParaSub(*_in, rmax));
			rsum = _ParaAdd(rsum, *_out);
			_in += mmb;
			_out += mmb;
		}


		//归一化
		_in = _inb;
		_out = _outb;
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			*_out = _ParaDiv(*_out, rsum);

			_in += mmb;
			_out += mmb;
		}

		_inb++;
		_outb++;

	}
#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C


	return mini_b_num;

}

lw_ptr MiaoToolBPnnLayerSoftMaxBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_MIAO_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_in = *_out;
	_MIAO_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerActFunNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType Col,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem,
	MiaoToolNNLayerActFun_t *pTemplet,
	lw_u8 TrainFlag
)
{
	uint32_t node_size;
	MiaoToolNNLayerActFun_t *node;
	MIAO_assert(!pmem);
	MIAO_assert(!pTemplet);
	//分配节点内存

	if (pTemplet->_real_size < sizeof(MiaoToolNNLayerActFun_t))
	{
		node_size = sizeof(MiaoToolNNLayerActFun_t);
	}
	else
	{
		node_size = pTemplet->_real_size;
	}

	node = pmem->Malloc(pmem, node_size);

	memcpy(node, pTemplet, node_size);



#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	MiaoToolMatrixInitial(&(node->basic.in), max_mini_batch, Col, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	MiaoToolMatrixInitial(&(node->basic.out), max_mini_batch, Col, pout);


#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	MiaoToolMatrixInitial(&(node->basic.in), Col, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	MiaoToolMatrixInitial(&(node->basic.out), Col, max_mini_batch, pout);

#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C


	if (pTemplet->_init)
	{
		pTemplet->_init(pTemplet, TrainFlag);
	}

	return (MiaoToolNNSubLayerBasic_t *)node;
}




#endif // MIAO_TOOL_IMPORT_NN_BPnn || MIAO_TOOL_IMPORT_ALL

#endif // MIAO_TOOL_IMPORT_NN || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 2)

