/*(Ver.=0.96)
 * MIAO_tool.c
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(MIAO_SIMPLE_LEVEL >= 3)


ParaType MiaoToolRandomRangeMapping(MiaoToolRandomBasic_t *pRB,ParaType r,ParaType Rmin,ParaType Rmax,ParaType delt)
{
    ParaType center1=_ParaDiv(_ParaAdd(Rmax,Rmin),_ParaFint(2)),center2=_ParaDiv(_ParaAdd(pRB->Rmax,pRB->Rmin),_ParaFint(2));//º∆À„÷––ƒµ„
    ParaType range1=_ParaSub(Rmax,Rmin),range2=_ParaSub(pRB->Rmax,pRB->Rmin);//º∆À„∑∂Œß
    //ParaType data=_ParaAdd(center1,_ParaMul(_ParaSub(r,center2),_ParaDiv(range1,range2)));//º∆À„∆´“∆  //º∆À„¥Ì¡À!!!!!!!!
	ParaType data = _ParaAdd(center1, _ParaMul(_ParaSub(r, center2), _ParaDiv(range1, range2)));//º∆À„∆´“∆

    if(delt <= _ParaFint(0))
    {
        return data;
    }
    else
    {
        lw_32 d=_ParaInt(_ParaDiv(data,delt));//º∆À„”≥…‰ ‰≥ˆ
        return _ParaMul(_ParaFint(d),delt);
    }

}


ParaType MiaoToolRandomNormal(MiaoToolRandomBasic_t *pRB, ParaType Cent, ParaType Var)
{
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM
	ParaType r1, r2;
	ParaType z;

	pRB->_normal_gen_flag = !pRB->_normal_gen_flag;
	if (!pRB->_normal_gen_flag)
	{
		return _ParaAdd(Cent, _ParaMul(pRB->_normal_last, Var));
	}

	
	do
	{
		r1 = pRB->rand(pRB, _ParaFrom(0), _ParaFrom(1));
	} while (r1 <= _ParaFrom(0));

	r2= pRB->rand(pRB, _ParaFrom(0), _ParaFrom(1));
	z = _ParaMul(
		_ParaSqrt(_ParaMul(_ParaFrom(-2), _ParaLn(r1))),
		_ParaCos(_ParaMul(_ParaFrom(2 * PI), r2))
	);

	pRB->_normal_last = _ParaMul(
		_ParaSqrt(_ParaMul(_ParaFrom(-2), _ParaLn(r1))),
		_ParaSin(_ParaMul(_ParaFrom(2 * PI), r2))
	);

	return _ParaAdd(Cent, _ParaMul(z, Var));

#elif MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_LLN
	lw_u16 i;
	ParaType sum = 0;

	for (i = 0; i < MIAO_TOOL_RAND_NORM_LLN_NUM; i++)
	{
		sum = _ParaAdd(sum, pRB->rand(pRB, _ParaFint(0), _ParaFint(1)));
	}

	sum = _ParaSub(sum, _ParaFrom(MIAO_TOOL_RAND_NORM_LLN_NUM / 2.0));
	sum = _ParaDiv(sum, _ParaFrom(MIAO_TOOL_RAND_NORM_LLN_DIV));

	return _ParaAdd(Cent, _ParaMul(sum, Var));

#elif MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
	ParaType r1, r2;
	ParaType s;
	
	pRB->_normal_gen_flag = !pRB->_normal_gen_flag;


	if (!pRB->_normal_gen_flag)
	{
		return _ParaAdd(Cent, _ParaMul(pRB->_normal_last, Var));
	}

	
	do
	{
		r1 = pRB->rand(pRB, _ParaFrom(-1), _ParaFrom(1));
		r2 = pRB->rand(pRB, _ParaFrom(-1), _ParaFrom(1));

		s = _ParaAdd(
			_ParaMul(r1, r1),
			_ParaMul(r2, r2)
		);

	} while ((s >= _ParaFrom(1)) || (s == _ParaFrom(1)));

	s = _ParaSqrt(
		_ParaMul(_ParaFrom(-2),
			_ParaDiv(_ParaLn(s), s)
		)
	);

	pRB->_normal_last = _ParaMul(r2, s);
	s = _ParaMul(s, r1);

	return _ParaAdd(Cent, _ParaMul(s, Var));
#else
	return _ParaAdd(pRB->rand(pRB, -Var, Var) , Cent);
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM


}

#if MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL

void MiaoToolMatrixRandomInitial(MiaoMat *m, void *pRand, ParaType Rmin, ParaType Rmax)
{

	uint32_t d = m->length, n = 0;
	ParaType *a;
	MiaoToolRandomBasic_t *p = (MiaoToolRandomBasic_t *)pRand;
	MIAO_assert(!(m));
	MIAO_assert(!(pRand));
	a = m->a;
	while (d--)
	{
		*a++ = p->rand((void *)p, Rmin, Rmax);
	}

}

void *MiaoToolMatrixToRandnChip(MiaoMat *s, MiaoMat *m, MiaoToolRandomBasic_t *pRand, ParaType var, ParaType Rmin, ParaType Rmax)
{
	register uint32_t i;
	register ParaType *sa, *sm;
	ParaType d;
	
	MIAO_assert(!(m));
	MIAO_assert(!(s));
	MIAO_assert(!(pRand));

	if (s->length == m->length)
	{
		i = m->length;
		sa = s->a;
		sm = m->a;
		while (i--)
		{
			d = pRand->randn(pRand, *sm, var);
			d = d > Rmax ? Rmax : d;
			d = d < Rmin ? Rmin : d;
			*sa++ = d;
			sm++;
		}
	}


	return m;

}
#endif //MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL


//-----------------------
//Logistic ”≥…‰

#if MIAO_TOOL_IMPORT_RAND_CHAOS || MIAO_TOOL_IMPORT_ALL

MiaoToolRandomChaosLogistic_t MiaoTRCLog =
{ _ParaFrom(1.0),_ParaFrom(0.0),MiaoToolRandomChaosLogisticFun,MiaoToolRandomChaosLogisticFunInt,MiaoToolRandomNormal,NULL,
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
(3.9),(0.41) };
MiaoToolRandomBasic_t *pRCLog = (MiaoToolRandomBasic_t *)&MiaoTRCLog;

ParaType MiaoToolRandomChaosLogisticFun(void *pRand,ParaType Rmin,ParaType Rmax)
{

    MiaoToolRandomChaosLogistic_t *p=(MiaoToolRandomChaosLogistic_t *) pRand;
    MIAO_assert(!pRand);

    //p->x=_ParaMul(_ParaMul(p->u,_ParaSub(_ParaFint(1),p->x)),p->x);
    p->x *= p->u*(1.0 - p->x);
    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p,_ParaFrom(p->x),Rmin,Rmax,_ParaFint(-1));
}


lw_32 MiaoToolRandomChaosLogisticFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax)
{

    MiaoToolRandomChaosLogistic_t *p=(MiaoToolRandomChaosLogistic_t *) pRand;
    MIAO_assert(!pRand);

    MiaoToolRandomChaosLogisticFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p, _ParaFrom(p->x),_ParaFint(Rmin),_ParaFint(Rmax),_ParaFint(1)));

}

//-----------------------
//’ ≈Ò”≥…‰

MiaoToolRandomChaosTent_t MiaoTRCTent=
{_ParaFrom(1.0),_ParaFrom(-1.0),MiaoToolRandomChaosTentFun,MiaoToolRandomChaosTentFunInt,MiaoToolRandomNormal,NULL,
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
(0.995),(0.01)};

MiaoToolRandomBasic_t *pRCTent = (MiaoToolRandomBasic_t *)&MiaoTRCTent;

ParaType MiaoToolRandomChaosTentFun(void *pRand,ParaType Rmin,ParaType Rmax)
{
    MiaoToolRandomChaosTent_t *p=(MiaoToolRandomChaosTent_t *) pRand;
    MIAO_assert(!pRand);

    //p->x=_ParaSub(p->a,_ParaMul(_ParaAdd(_ParaFint(1),p->a),_ParaAbs(p->x)));
    p->x = p->a - (1 + p->a)*(p->x > 0.0 ? p->x : -(p->x));
    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p,_ParaFrom(p->x),Rmin,Rmax,_ParaFint(-1));
}

lw_32 MiaoToolRandomChaosTentFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax)
{
    MiaoToolRandomChaosTent_t *p=(MiaoToolRandomChaosTent_t *) pRand;
    MIAO_assert(!pRand);

    MiaoToolRandomChaosTentFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p, _ParaFrom(p->x),_ParaFint(Rmin),_ParaFint(Rmax),_ParaFint(1)));

}

//-----------------------
//KENT”≥…‰
MiaoToolRandomChaosKent_t MiaoTRCKent=
{_ParaFrom(1.0),_ParaFrom(0.0),MiaoToolRandomChaosKentFun,MiaoToolRandomChaosKentFunInt,MiaoToolRandomNormal,NULL,
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
(0.3),(0.01)};

MiaoToolRandomBasic_t *pRCKent = (MiaoToolRandomBasic_t *)&MiaoTRCKent;

ParaType MiaoToolRandomChaosKentFun(void *pRand,ParaType Rmin,ParaType Rmax)
{
    MiaoToolRandomChaosKent_t *p=(MiaoToolRandomChaosKent_t *) pRand;
    MIAO_assert(!pRand);

    /*(Ver.=0.96)
    if(p->x <= p->a)
    {
        p->x=_ParaDiv(p->x,p->a);
    }
    else
    {
        p->x=_ParaDiv(_ParaSub(_ParaFint(1),p->x),_ParaSub(_ParaFint(1),p->a));
    }
    */
    if (p->x <= p->a)
    {
        p->x /=  p->a;
    }
    else
    {
        p->x = (1.0 - p->x) / (1.0 - p->a);
    }


    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p, _ParaFrom(p->x),Rmin,Rmax,_ParaFint(-1));

}

lw_32 MiaoToolRandomChaosKentFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax)
{
    MiaoToolRandomChaosKent_t *p=(MiaoToolRandomChaosKent_t *) pRand;
    MIAO_assert(!pRand);

    MiaoToolRandomChaosKentFun(pRand,p->basic.Rmax,p->basic.Rmin);

    return _ParaInt(MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p, _ParaFrom(p->x),_ParaFint(Rmin),_ParaFint(Rmax),_ParaFint(1)));
}
#endif // MIAO_TOOL_IMPORT_RAND_CHAOS || MIAO_TOOL_IMPORT_ALL

//-----------------------
//œﬂ–‘Õ¨”‡

MiaoToolRandomLCG_t MiaoTRLcg =
{
#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
 MIAO_GLOBAL_MATH_MAX,MIAO_GLOBAL_MATH_MIN
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
  _ParaFrom(1.0),_ParaFrom(0.0)
#endif

  ,MiaoToolRandomLCGFun,MiaoToolRandomLCGFunInt,MiaoToolRandomNormal,NULL,
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
_ParaFrom(0), 0,
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
0l,1103515245UL,12345,0xffffffffUL };

MiaoToolRandomBasic_t *pRLcg = (MiaoToolRandomBasic_t *)&MiaoTRLcg;

ParaType MiaoToolRandomLCGFun(void *pRand, ParaType Rmin, ParaType Rmax)
{
    MiaoToolRandomLCG_t *p = (MiaoToolRandomLCG_t *)pRand;
    ParaType res;
	uint32_t _r1;
    MIAO_assert(!pRand);
    p->rand_x = ((uint32_t)(p->rand_x) *(uint32_t)(p->rand_a) + (uint32_t)(p->rand_c)) & p->rand_mod;
	_r1 = ((p->rand_x) >> 16) & 0x7fff;
	p->rand_x = ((uint32_t)(p->rand_x) *(uint32_t)(p->rand_a) + (uint32_t)(p->rand_c)) & p->rand_mod;
	_r1 = _r1 | (((p->rand_x) & 0x7fff0000) >> 1);

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
    res = _r1;
    return (res) % ((Rmax) - (Rmin)) + (Rmin);
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
    res = _ParaMul(_ParaFint(_r1), _ParaFrom(1.0/0x3fffffff));
    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)p, res, Rmin, Rmax, _ParaFint(-1));
#endif



}
lw_32 MiaoToolRandomLCGFunInt(void *pRand, lw_32 Rmin, lw_32 Rmax)
{
    MiaoToolRandomLCG_t *p = (MiaoToolRandomLCG_t *)pRand;
	uint32_t _r1;
    MIAO_assert(!pRand);
	p->rand_x = ((uint32_t)(p->rand_x) *(uint32_t)(p->rand_a) + (uint32_t)(p->rand_c)) & p->rand_mod;
	_r1 = ((p->rand_x) >> 16) & 0x7fff;
	p->rand_x = ((uint32_t)(p->rand_x) *(uint32_t)(p->rand_a) + (uint32_t)(p->rand_c)) & p->rand_mod;
	_r1 = _r1 | (((p->rand_x) & 0x7fff0000) >> 1);

    return (_r1) % ((Rmax) - (Rmin)) + (Rmin);


}

#if MIAO_TOOL_IMPORT_RAND_WELL || MIAO_TOOL_IMPORT_ALL

static lw_u64 _MiaoToolRandWELL512Rand(MiaoToolRandomWELL512_t *_pr)
{
	register uint32_t a, b, c, d;
	register uint32_t *state;
	register uint32_t index;

	index = _pr->_index;
	state = _pr->_state;

	a = state[index];
	c = state[(index + 13) & 15];
	b = a^c ^ (a << 16) ^ (c << 15);
	c = state[(index + 9) & 15];
	c ^= (c >> 11);
	a = state[index] = b^c;
	d = a ^ ((a << 5) & 0xDA442D20);
	index = (index + 15) & 15;
	a = state[index];
	state[index] = a^b^d ^ (a << 2) ^ (b << 18) ^ (c << 28);

	_pr->_index = index;

	return state[index];

}

void MiaoToolRandomWELL512Seed(MiaoToolRandomWELL512_t *pRand, uint32_t seed)
{
	uint32_t i = 16;
	uint32_t *p;
	MIAO_assert(!(pRand));

	memset(pRand, 0, sizeof(MiaoToolRandomWELL512_t));

	seed = seed ? seed : 1;

	p = pRand->_state;
	while (i--)
	{
		*p++ = seed;
	}

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	pRand->basic.Rmax = MIAO_GLOBAL_MATH_MAX;
	pRand->basic.Rmin = MIAO_GLOBAL_MATH_MIN;
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
    pRand->basic.Rmax = _ParaFint(1);
    pRand->basic.Rmin = _ParaFint(0);
#endif


	
	pRand->basic.rand = MiaoToolRandomWELL512Fun;
	pRand->basic.randint = MiaoToolRandomWELL512FunInt;
	pRand->basic.randn = MiaoToolRandomNormal;
	pRand->basic.Seed = MiaoToolRandomWELL512Seed;


}

ParaType MiaoToolRandomWELL512Fun(void *pRand, ParaType Rmin, ParaType Rmax)
{
	uint32_t _r;

	MIAO_assert(!(pRand));

	_r = (uint32_t)_MiaoToolRandWELL512Rand(pRand);

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	return (_r) % ((Rmax) - (Rmin)) + (Rmin);
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)pRand, _ParaMul(_ParaFrom(_r), _ParaFrom(1.0/0xFFFFFFFF)), Rmin, Rmax, _ParaFint(-1));
#endif

}


lw_32 MiaoToolRandomWELL512FunInt(void *pRand, lw_32 Rmin, lw_32 Rmax)
{
	uint32_t _r;

	MIAO_assert(!(pRand));

	_r = (uint32_t)_MiaoToolRandWELL512Rand(pRand);

	return (_r) % ((Rmax) - (Rmin)) + (Rmin);
}
#endif // MIAO_TOOL_IMPORT_RAND_WELL || MIAO_TOOL_IMPORT_ALL



#if MIAO_TOOL_IMPORT_RAND_MT || MIAO_TOOL_IMPORT_ALL

#define N 624
#define M 397
#define R 31
#define A 0x9908B0DF
#define F 1812433253
#define U 11
#define S 7
#define B 0x9D2C5680
#define T 15
#define C 0xEFC60000
#define L 18
#define MASK_LOWER (1ull << R) - 1
#define MASK_UPPER (1ull << R)


static void _MiaoToolRandomMT_Twist(MiaoToolRandomMT_t *pR)
{
	register uint32_t  i, x, xA;
	register uint32_t *mt;

	mt = pR->_mt;

	for (i = 0; i < N; i++)
	{
		x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);

		xA = x >> 1;

		if (x & 0x1)
			xA ^= A;

		mt[i] = mt[(i + M) % N] ^ xA;
	}

	pR->_index = 0;
}

static uint32_t _MiaoToolRandomMT_Rand(MiaoToolRandomMT_t *pR)
{
	uint32_t y;
	uint32_t i = pR->_index;

	if (i >= N)
	{
		_MiaoToolRandomMT_Twist(pR);
		i = pR->_index;
	}

	y = pR->_mt[i];
	pR->_index = i + 1;

	y ^= (pR->_mt[i] >> U);
	y ^= (y << S) & B;
	y ^= (y << T) & C;
	y ^= (y >> L);

	return y;

}


void MiaoToolRandomMTSeed(MiaoToolRandomMT_t *pRand, uint32_t seed)
{
	uint32_t  i;
	pRand->_mt[0] = seed;
	for (i = 1; i < N; i++)
	{
		pRand->_mt[i] = (F * ((pRand->_mt[i - 1]) ^ ((pRand->_mt[i - 1]) >> 30)) + i);
	}
	pRand->_index = N;
	pRand->basic.rand = MiaoToolRandomMTFun;
	pRand->basic.randint = MiaoToolRandomMTFunInt;
	pRand->basic.randn = MiaoToolRandomNormal;
	pRand->basic.Seed = MiaoToolRandomMTSeed;

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
    pRand->basic.Rmax = MIAO_GLOBAL_MATH_MAX;
    pRand->basic.Rmin = MIAO_GLOBAL_MATH_MIN;
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
    pRand->basic.Rmax = _ParaFint(1);
    pRand->basic.Rmin = _ParaFint(0);
#endif

#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
	pRand->basic._normal_gen_flag = 0;
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM || MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP

}


ParaType MiaoToolRandomMTFun(void *pRand, ParaType Rmin, ParaType Rmax)
{
	uint32_t _r;

	MIAO_assert(!(pRand));

	_r = _MiaoToolRandomMT_Rand(pRand);

#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
    return (_r) % ((Rmax) - (Rmin)) + (Rmin);
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT || MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
    return MiaoToolRandomRangeMapping((MiaoToolRandomBasic_t *)pRand, _ParaMul(_ParaFrom(_r), _ParaFrom(1.0/0xFFFFFFFF)), Rmin, Rmax, _ParaFint(-1));
#endif

}

lw_32 MiaoToolRandomMTFunInt(void *pRand, lw_32 Rmin, lw_32 Rmax)
{
	uint32_t _r;

	MIAO_assert(!(pRand));
	_r = _MiaoToolRandomMT_Rand(pRand);
	return (_r) % ((Rmax) - (Rmin)) + (Rmin);
}



#undef N 
#undef M
#undef R
#undef A
#undef F 
#undef U 
#undef S 
#undef B
#undef T
#undef C
#undef L
#undef MASK_LOWER
#undef MASK_UPPER

#endif // MIAO_TOOL_IMPORT_RAND_MT || MIAO_TOOL_IMPORT_ALL



#endif // !(MIAO_SIMPLE_LEVEL >= 3)
