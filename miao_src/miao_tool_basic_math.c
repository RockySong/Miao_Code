/*(Ver.=0.96)
* MIAO_tool.c
*
*  Created on: 2017-7-14
*      Author: slw
*/

#include "OSLW_include.h"
#if !(MIAO_SIMPLE_LEVEL >= 4)

#if MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL

void MiaoToolMatrixInitial(MiaoMat *m, LwMatRowType row, LwMatColType col, ParaType *a)
{
    MIAO_assert(!(m));

    m->a = a;
    m->row = row;
    m->col = col;

    m->length = row*col;
}





MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixAdd,          
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2),         
	MiaoToolMatrixAdd             
)
{

    
    register uint32_t d;
    register ParaType *ps, *pm1, *pm2, data;
    MIAO_assert(!(s));
    MIAO_assert(!(m1));
    MIAO_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

    //if(s->row == m1->row && s->row == m2->row && s->col == m1->col && s->col == m2->col)
    if (s->length == m1->length && m1->length == m2->length)
    {
		while (d--)
		{
			*ps++ = _ParaAdd(*pm1++, *pm2++);
		}

    }
    else if(s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
        while (d--)
        {
            *ps++ = _ParaAdd(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaAdd(*pm2++, data);
        }

    }
    else
    {
        return NULL;
    }

    return s;


}

MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixSub,        
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2),         
	MiaoToolMatrixSub            
)
{
    
    register uint32_t d;
    register ParaType *ps, *pm1, *pm2, data;
    MIAO_assert(!(s));
    MIAO_assert(!(m1));
    MIAO_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

    //if(s->row == m1->row && s->row == m2->row && s->col == m1->col && s->col == m2->col)
    if (s->length == m1->length && m1->length == m2->length)
    {
        //for(i=0;i<d;i++)
        //s->a[i]=_ParaSub(m1->a[i],m2->a[i]);
        while (d--)
        {
            *ps++ = _ParaSub(*pm1++, *pm2++);
        }

    }
    else if (s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
        while (d--)
        {
            *ps++ = _ParaSub(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaSub(*pm2++, data);
        }

    }
    else
    {
        return NULL;
    }
    return s;
}

MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixDot,          
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2),          
	MiaoToolMatrixDot 
)
{
    register uint32_t d;

    register ParaType *ps, *pm1, *pm2, data;

    MIAO_assert(!(s));
    MIAO_assert(!(m1));
    MIAO_assert(!(m2));

    d = s->length;

    ps = s->a;
    pm1 = m1->a;
    pm2 = m2->a;

    if (s->length == m1->length && m1->length == m2->length)
    {
        while (d--)
        {
            *ps++ = _ParaMul(*pm1++, *pm2++);
        }

    }
    else if (s->length == m1->length && m2->length == 1)
    {
        data = *(m2->a);
        while (d--)
        {
            *ps++ = _ParaMul(*pm1++, data);
        }

    }
    else if (s->length == m2->length && m1->length == 1)
    {
        data = *(m1->a);
        while (d--)
        {
            *ps++ = _ParaMul(*pm2++, data);
        }

    }
    else
    {
		MIAO_assert(1);
        return NULL;
    }
    return s;
}

MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixMul,
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2),
	MiaoToolMatrixMul
)
{
    register LwMatColType j, k, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf;
	MIAO_assert(!(s));
	MIAO_assert(!(m1));
	MIAO_assert(!(m2));

	if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)//�����������
	{

#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS


		cblas_sgemm(
			CblasRowMajor, 
			CblasNoTrans, CblasNoTrans, 
			s->row, s->col, m1->col, 
			_ParaFint(1), m1->a, m1->col, 
			m2->a, m2->col, 
			_ParaFint(0), s->a, s->col
		);


#else
#if L1_L2_CACHE_OPTIM == 1
		uint32_t div8, mod8;

		row = s->row;
		col = s->col;
		row1 = m2->row;
		_sbuf = s->a;
		_m1buf = m1->a;
		_m2buf = m2->a;

		div8 = col / 8, mod8 = col % 8;

		LW_MAT_CLR(s);

		for (i = 0, _m1 = _m1buf; i < row; ++i)
		{
			for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
			{
				temp = *_m1;
				for (k = 0, _s = _sbuf; k < div8; ++k)
				{
					_s[0] = _ParaAdd(_s[0], _ParaMul(temp, _m2[0]));
					_s[1] = _ParaAdd(_s[1], _ParaMul(temp, _m2[1]));
					_s[2] = _ParaAdd(_s[2], _ParaMul(temp, _m2[2]));
					_s[3] = _ParaAdd(_s[3], _ParaMul(temp, _m2[3]));

					_s[4] = _ParaAdd(_s[4], _ParaMul(temp, _m2[4]));
					_s[5] = _ParaAdd(_s[5], _ParaMul(temp, _m2[5]));
					_s[6] = _ParaAdd(_s[6], _ParaMul(temp, _m2[6]));
					_s[7] = _ParaAdd(_s[7], _ParaMul(temp, _m2[7]));

					_m2 += 8;
					_s += 8;

				}

				for (k = 0; k < mod8; ++k, ++_m2, ++_s)
				{
					*_s = _ParaAdd(*_s, _ParaMul(temp, *_m2));
				}

			}
			_sbuf = _s;
		}

#else
		row = s->row;
		col = m2->col;
		row1 = m2->row;
		_s = s->a;
		_m1buf = m1->a;

		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{

				//_m1=m1->a+i*m1->col;//m1��ָ���λ m1��λ��[n,1]
				_m1 = _m1buf;
				_m2 = m2->a + j;//m2��ָ���λ m2��λ��[1,n]
				temp = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					temp = _ParaAdd(temp, _ParaMul(*_m1, *_m2));
					_m1++;
					_m2 += col;
				}//������*������
				*_s++ = temp;
			}
			_m1buf += m1->col;//m1��ָ���λ m1��λ��[n,1]
		}

#endif // L1_L2_CACHE_OPTIM == 1
#endif

	}
	else if ((m1->length == 1 || m2->length == 1) && (s->length == m1->length || s->length == m2->length) && s->length)
	{
		_s = s->a;
		if (m1->length == 1)
		{
			i = m2->length;
			_m1 = m2->a;
			temp = *(m1->a);
		}
		else
		{
			i = m1->length;
			_m1 = m1->a;
			temp = *(m2->a);
		}
		while (i--) *_s++ = _ParaMul(temp, *_m1++);
	}
	else
	{
		return NULL;
	}
	return s;
}



MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixTurn,          
	(MiaoMat *s, MiaoMat *buf),          
	MiaoToolMatrixTurn             
)
{

 
    MIAO_assert(!(s));
	MIAO_assert(!(buf));

	if (s->row == buf->col && s->col >= buf->row && s->a != buf->a)
	{
		lw_u16 i, j;
		ParaType *_ms, *_mb, *_msb, *_mbb;
		lw_u16 sc, bc;
		_msb = s->a;
		_mbb = buf->a;
		sc = s->col;
		bc = buf->col;

		for (i = 0; i < buf->row; i++)
		{
			_ms = _msb;
			_mb = _mbb;
			for (j = 0;j < buf->col;j++)
			{
				*_ms = *_mb;
				_ms += sc;
				_mb++;
			}
			_msb++;
			_mbb += bc;
		}

	}
	else
	{
		MIAO_assert(1);
	}

    return s;


}



MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixReSize,          
	(MiaoMat *s, lw_u16 row, lw_u16 col),             
	MiaoToolMatrixReSize            
)
{

    MIAO_assert(!(s));
    if (row <= s->row && col <= s->col)
    {
        s->row = row;
        s->col = col;
        s->length = row*col;
    }



    return s;


}


MIAO_TOOL_FUN(MiaoMat*, MiaoToolMatrixMaxMin,
              (MiaoMat *s, ParaType *pMax, lw_u16 *pMaxIndex, ParaType *pMin, lw_u16 *pMinIndex),
              MiaoToolMatrixMaxMin
             )
{
    register lw_u16 i;
    register ParaType dmax, dmin, *p;

    MIAO_assert(!(s));

    if ((pMax && pMaxIndex) && !(pMin && pMinIndex))
    {
        p = s->a;
        dmax = s->a[0];
        *pMaxIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] >= dmax)
            {
                dmax = p[i];
                *pMaxIndex = i;
            }
        }
        *pMax = dmax;
    }
    else if (!(pMax && pMaxIndex) && (pMin && pMinIndex))
    {

        p = s->a;
        dmin = s->a[0];
        *pMinIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] < dmin)
            {
                dmin = p[i];
                *pMinIndex = i;
            }
        }
        *pMin = dmin;

    }
    else if ((pMax && pMaxIndex) && (pMin && pMinIndex))
    {
        p = s->a;
        dmax = s->a[0];
        dmin = s->a[0];
        *pMaxIndex = 0;
        *pMinIndex = 0;
        for (i = 1; i<s->length; i++)
        {
            if (p[i] > dmax)
            {
                dmax = p[i];
                *pMaxIndex = i;
            }

            if (p[i] < dmin)
            {
                dmin = p[i];
                *pMinIndex = i;
            }

        }

        *pMax = dmax;
        *pMin = dmin;
    }
    else;


    return s;
}

MIAO_TOOL_FUN(MiaoMartixResNum, MiaoToolMatrixCmp,
              (MiaoMat *s, MiaoMat *m, lw_u8 *pbuf),
              MiaoToolMatrixCmp
             )
{
	MIAO_assert(!(s));
	MIAO_assert(!(m));

    if (pbuf == NULL)
    {
        if (s->length == m->length && (s->a) && (m->a))
        {
            return memcmp(s->a, m->a, PARA_MEM_CAL(s->length)) == 0 ? MiaoMartixRes_Find : MiaoMartixRes_NoFind;
        }
        else
        {
            return MiaoMartixRes_NoFind;
        }
    }

    return MiaoMartixRes_Error;
}

MIAO_TOOL_FUN(MiaoMartixResNum, MiaoToolMatrixFind,
              (MiaoMat *s, MiaoMat *m, lw_u16 SpacingLen, lw_u16 First, lw_u16 *pres),
              MiaoToolMatrixFind
             )
{
    register ParaType *ps, *pa, *pbottom;
    register lw_u16 len, count = 0;


    len = SpacingLen ? SpacingLen : m->length;

    if ((lw_32)(s->length - First) >= m->length)//����Ŀ��ĳ���Ҫ���ڱ������ĳ��� ��Ȼû������������
    {
        ps = s->a + First;
        pa = m->a;
        pbottom = ps + s->length;

        do
        {
            if (memcmp(ps, pa, PARA_MEM_CAL(len)) == 0)
            {
                len = 0;
                break;
            }
            count++;
            ps += len;
        } while (ps < pbottom);
        if (!len && pres)//˵���ҵ���
        {
            *pres = count;
            return MiaoMartixRes_Find;
        }
        else
        {
            return MiaoMartixRes_NoFind;
        }

    }
    else
    {
        return MiaoMartixRes_Over;
    }

    
}


MIAO_TOOL_FUN(
	MiaoMartixResNum, 
	MiaoToolMatrixIndex,          
	(MiaoMat *s, ParaType data, lw_u16 *pindex, lw_u16 *pmaxlen),          
	MiaoToolMatrixIndex 
)
{
    register lw_u16 i, maxlen;
    MIAO_assert(!(s));
    MIAO_assert(!(pindex));
    MIAO_assert(!(pmaxlen));


    maxlen = *pmaxlen;
    for (i = 0; i < s->length; i++)
    {
        if (s->a[i] == data)
        {
            *pindex++ = i;
            if (!(--maxlen))
                break;
        }
    }
    (*pmaxlen) -= maxlen;
    return MiaoMartixRes_Find;
}


MIAO_TOOL_FUN(MiaoMat*, MiaoToolMatrixSet,
              (MiaoMat *s, ParaType data, MiaoMat *a),
              MiaoToolMatrixSet
             )
{
    register LwMatLenType i = 0;
    register ParaType *ps;
    MIAO_assert(!(s));
    if (a)//�������a �൱�ڸ��ƹ��캯��
    {
        /*(Ver.=0.96)
        for (i = 0; i < s->length; i++)
        {
        s->a[i] = a->a[i];
        }
        */

        /*(Ver.=0.96)
        i = s->length;
        ps = s->a;
        pa = a->a;

        while (i--)
        {
        *ps++ = *pa++;
        }
        */
        if (s->length == a->length)
        {
            memcpy(s->a, a->a, PARA_MEM_CAL(s->length));
        }
        else
        {
            MIAO_assert(1);
        }

    }
    else//ʹ�ó�����ʼ��
    {
        /*(Ver.=0.96)
        for (i = 0; i < s->length; i++)
        {
        s->a[i] = data;
        }
        */

        i = s->length;
        ps = s->a;

        while (i--)
        {
            *ps++ = data;
        }

    }

    return s;
}




MIAO_TOOL_FUN(MiaoMat*, MiaoToolMatrixJoin,
(MiaoMat *s, MiaoMat *m1, MiaoMat *m2), MiaoToolMatrixJoin)
{
	register uint32_t i, j;
	register ParaType *sa, *m1a, *m2a;
	if (s->length >= (m1->length + m2->length ))//����ƴ������
	{
		
		sa = s->a;
		m1a = m1->a;
		m2a = m2->a;

		if (m1->row == m2->row)//������Ȳ�����ƴ��
		{
			i = m1->row;
			while (i--)//��ѭ��
			{
				//�ȸ���M1
				j = m1->col;
				while (j--)
				{
					*sa++ = *m1a++;
				}
				//�ٸ���M2
				j = m2->col;
				while (j--)
				{
					*sa++ = *m2a++;
				}
			}//end while ��ѭ��
		}
		else//������ǲ�����������ƴ��
		{
			i = m1->length;

			while (i--)
			{
				*sa++ = *m1a++;
			}

			i = m2->length;
			
			while (i--)
			{
				*sa++ = *m2a++;
			}

		}


	}
	else if (s == m1 && s->length >= m2->length)
	{
		i = m2->length;
		sa = s->a + (s->length - m2->length);
		m2a = m2->a;
		while (i--)
		{
			*sa++ = *m2a++;
		}
	}
	else
	{
		MIAO_assert(1);
		
	}
	



	return s;
}



MIAO_TOOL_FUN(ParaType, MiaoToolMatrixLossSquare,
              (MiaoMat *s, MiaoMat *ref, MiaoMat *pre),
              MiaoToolMatrixLossSquare
             )
{

    lw_u16 i = 0;
    ParaType sum = _ParaFint(0), temp;
    MIAO_assert(!(ref));
    MIAO_assert(!(pre));
    if (ref->length == pre->length)
    {
		if (!s)
		{
			for (i = 0; i<ref->length; i++)
			{
				temp = ref->a[i] - pre->a[i];
				sum = _ParaAdd(sum, _ParaMul(_ParaFrom(0.5), _ParaMul(temp, temp)));
			}
		}
		else
		{
			if (s->length == ref->length)
			{
				for (i = 0; i<ref->length; i++)
				{
					temp = ref->a[i] - pre->a[i];
					sum = _ParaAdd(sum, _ParaMul(_ParaFrom(0.5), _ParaMul(temp, temp)));
					s->a[i] = temp;
				}
			}
		}


    }

    return sum;
}


MIAO_TOOL_FUN(ParaType, MiaoToolMatrixLossCrossEntropyForSoftMax,
(MiaoMat *s, MiaoMat *ref, MiaoMat *pre),
MiaoToolMatrixLossCrossEntropyForSoftMax
)
{

	lw_u16 i = 0, j , col;
	ParaType sum = _ParaFint(0);
	ParaType *_pre_b, *_s_b, *_ref_b;
	MIAO_assert(!(ref));
	MIAO_assert(!(pre));
	MIAO_assert(!(s));
	if (ref->length == pre->length)
	{
			if (s->length == ref->length)
			{

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
				_s_b = s->a;
				_ref_b = ref->a;
				_pre_b = pre->a;
				col = s->col;
				for (; i < s->row; i++)
				{
					
					for ( j = 0; j < col; ++j, ++_s_b, ++_ref_b, ++_pre_b)
					{
						sum+= _ParaMul(_ParaFrom(*_ref_b), _ParaLn(*_pre_b+ MIAO_GLOBAL_MATH_DELT));
						*_s_b= *_ref_b - *_pre_b;
					}


				}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
				_s_b = s->a;
				_ref_b = ref->a;
				_pre = pre->a;
				for (i = 0; i < s->col; i++)
				{
					_s = _s_b;
					_ref = _ref_b;
					_pre = _pre_b;

					//�������ֵ
					_n_max = *_ref;
					_i_max = 0;
					for (j = 0; j < s->row; j++)
					{
						if (*_ref > _n_max)
						{
							_n_max = *_ref;
							_i_max = j;
						}

						*_s = *_pre;
						_s += col;
						_pre += col;
						_ref += col;
					}

					*(_s_b + col*_i_max) = _ParaSub(*(_s_b + col*_i_max), _ParaFint(1));

					_s_b++;
					_ref_b++;
					_pre_b++;
				}
#endif

			}
		}

	

	return -sum;
}
/*
MIAO_TOOL_FUN(ParaType, MiaoToolMatrixSum,
(MiaoMat *s),
MiaoToolMatrixSum
)
{

	ParaType *_s, sum = _ParaFrom(0);
	uint32_t i;
	MIAO_assert(s);

	i = s->length;
	_s = s->a;
	while (i--)
	{
		sum = _ParaAdd(sum, *_s++);
	}


	return sum;
}


MIAO_TOOL_FUN(ParaType, MiaoToolMatrixVar,
(MiaoMat *s, ParaType *pMean, MiaoMat *_normal_res,ParaType epsi),
MiaoToolMatrixVar
)
{
	ParaType _mean;
	ParaType _var = _ParaFint(0);
	ParaType *_s, *_nr;
	ParaType _one_div;
	uint32_t i;

	if (pMean)
	{
		_mean = *pMean;
	}
	else
	{
		_mean = _ParaDiv(pMiaoToolMatrixSum(s), _ParaFint(s->length));
	}


	i = s->length;
	_s = s->a;

	while (i--)
	{
		_var = _ParaAdd(_var, _ParaMul(_ParaSub(*_s, _mean), _ParaSub(*_s, _mean)));
		_s++;
	}

	_var = _ParaDiv(_var, _ParaFint(s->length));

	if (_normal_res)
	{
		i = _normal_res->length;
		_s = s->a;
		_nr = _normal_res->a;
		_one_div = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(_var, epsi)));

		while (i--)
		{
			*_nr++ = _ParaMul(_ParaSub(*_s, _mean), _one_div);
			_s++;
		}
	}

	return _var;


}
*/

MIAO_TOOL_FUN(ParaType, MiaoToolMatrixSum,
(MiaoMat *DST, MiaoMat *SRC, lw_u8 dim),
MiaoToolMatrixSum
)
{
	register ParaType _sum = _ParaFint(0);
	register LwMatRowType r;
	register LwMatColType c;
	register LwMatLenType l;
	register uint32_t i, j;
	register ParaType *s, *d, *si;

	MIAO_assert(!(SRC));
	l = SRC->length;
	r = SRC->row;
	c = SRC->col;
	s = SRC->a;
	switch (dim)
	{
	case 0x00:

#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
		_sum=cblas_sasum(l, s, 1);
#else

		while (l--)
		{
			_sum = _ParaAdd(_sum, *s++);
		}
#endif

		break;

	case 0x10:
		MIAO_assert(!(DST));
		d = DST->a;
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
		_sum = cblas_sasum(l, s, 1);
#else
		while (l--)
		{
			_sum = _ParaAdd(_sum, *s++);
		}
#endif

		*d = _ParaAdd(*d, _sum);
		break;

	case 0x01:
		MIAO_assert(!(DST));
		MIAO_assert(!(DST->length >= r));

		d = DST->a;
		while (r--)
		{
			i = c;

#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
			_sum = cblas_sasum(i, s, 1);
			s += i;
#else
			_sum = _ParaFrom(0);
			while (i--)
				_sum = _ParaAdd(_sum, *s++);
#endif

			*d++ = _sum;
		}
		break;

	case 0x11:
		MIAO_assert(!(DST));
		MIAO_assert(!(DST->length >= r));
		d = DST->a;
		while (r--)
		{
			i = c;

#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
			_sum = cblas_sasum(i, s, 1);
			s += i;
#else
			_sum = _ParaFrom(0);
			while (i--)
				_sum = _ParaAdd(_sum, *s++);
#endif


			*d = _ParaAdd(*d, _sum);
			++d;
		}
		break;

	case 0x02:
		MIAO_assert(!(DST));
		MIAO_assert(!(DST->length >= c));
		d = DST->a;
		j = c;
		while (j--)
		{
			i = r;
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
			_sum = cblas_sasum(i, s, c);
			++s;
#else
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
#endif

			*d++ = _sum;
		}
		break;

	case 0x12:
		MIAO_assert(!(DST));
		MIAO_assert(!(DST->length >= c));
		d = DST->a;
		j = c;
		while (j--)
		{
			i = r;
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
			_sum = cblas_sasum(i, s, c);
			++s;
#else
			_sum = _ParaFrom(0);
			si = s++;
			while (i--)
			{
				_sum = _ParaAdd(_sum, *si);
				si += c;
			}
#endif
			*d = _ParaAdd(*d, _sum);
			++d;
		}
		break;

	default:
		MIAO_assert(1);
		break;
	}


	return _sum;
}


//s=x*we+bi
//��������(����)
// xout=(Weight * xin) +Bias
MIAO_TOOL_FUN(MiaoMat*, MiaoToolMatrixMPYA,
(MiaoMat *s, MiaoMat *x, MiaoMat *we, MiaoMat *bi),
MiaoToolMatrixMPYA
)
{
	//
	//lw_u16 i, j, k, row, col, row1;
	//ParaType sum, *_s, *_x, *_we, *_xbuf, *_bi;
	//ParaType _we_a, _bi_a;
	//ParaType stas;
	//MIAO_assert(!(s));
	//MIAO_assert(!(x));
	//MIAO_assert(!(we));
	//MIAO_assert(!(bi));

	////������ǰ�򴫲�
	//if (s->row == x->row && s->col == we->col && x->col == we->row && s->length == bi->length)//�����������
	//{
	//	row = s->row;
	//	col = we->col;
	//	row1 = we->row;
	//	_s = s->a;
	//	_xbuf = x->a;
	//	_bi = bi->a;

	//	for (i = 0; i<s->row; i++)
	//	{
	//		for (j = 0; j<s->col; j++)
	//		{

	//			//_x=x->a+i*x->col;//x��ָ���λ x��λ��[n,1]
	//			_x = _xbuf;
	//			_we = we->a + j;//we��ָ���λ we��λ��[1,n]
	//			sum = _ParaFrom(0);
	//			for (k = 0; k<row1; k++)
	//			{
	//				sum = _ParaAdd(sum, _ParaMul(*_x, *_we));
	//				_x++;
	//				_we += col;
	//			}//������*������
	//			*_s++ = _ParaAdd(sum, *_bi);
	//			_bi++;
	//		}
	//		_xbuf += x->col;//x��ָ���λ x��λ��[n,1]
	//	}
	//}
	////
	//else if((bi->col == bi->row && bi->row == bi->length && bi->length >= 2) && (s->row == x->row && s->col == we->col && x->col == we->row))//����ͳ������
	//{
	//	row = s->row;
	//	col = we->col;
	//	row1 = we->row;
	//	_s = s->a;
	//	_xbuf = x->a;
	//	stas = _ParaFint(0);

	//	for (i = 0; i<s->row; i++)
	//	{
	//		for (j = 0; j<s->col; j++)
	//		{

	//			//_x=x->a+i*x->col;//x��ָ���λ x��λ��[n,1]
	//			_x = _xbuf;
	//			_we = we->a + j;//we��ָ���λ we��λ��[1,n]
	//			sum = _ParaFrom(0);
	//			for (k = 0; k<row1; k++)
	//			{
	//				sum = _ParaAdd(sum, _ParaMul(*_x, *_we));
	//				_x++;
	//				_we += col;
	//			}//������*������
	//			*_s++ = sum;

	//			stas = _ParaAdd(stas, sum);//ͳ���ۼ�

	//		}
	//		_xbuf += x->col;//x��ָ���λ x��λ��[n,1]
	//	}

	//	bi->a[0] = _ParaDiv(stas, _ParaFrom(s->length));//����ƽ��ֵ

	//}

	//else if (s->length == x->length && we->length ==1)
	//{
	//	//����s=����w*����x+����b
	//	if (bi->length == s->length)
	//	{
	//		_we_a = *(we->a);
	//		_bi = bi->a;
	//		_s = s->a;
	//		_xbuf = x->a;
	//		i = s->length;

	//		while (i--)
	//		{
	//			*_s = _ParaAdd(*_bi, _ParaMul((*_xbuf), _we_a));
	//			
	//			_s++;
	//			_xbuf++;
	//			_bi++;

	//		}


	//	}
	//	//����s=����w*����x+����b
	//	else if (bi->length == 1)
	//	{
	//		_we_a = *(we->a);
	//		_bi_a = *(bi->a);
	//		_s = s->a;
	//		_xbuf = x->a;

	//		i = s->length;
	//		while (i--)
	//		{
	//			*_s = _ParaAdd(_bi_a, _ParaMul((*_xbuf), _we_a));
	//			_s++;
	//			_xbuf++;

	//		}

	//	}
	//	else
	//	{
	//		MIAO_assert(1);
	//	}
	//}
	//else
	//{
	//	MIAO_assert(1);
	//}


	return s;
}


MIAO_TOOL_FUN(
	MiaoMat*, 
	MiaoToolMatrixWeXBi,
	(MiaoMat *s, MiaoMat *we, MiaoMat *x, MiaoMat *bi),
	MiaoToolMatrixWeXBi
)
{
	MIAO_assert(1);
	return NULL;
}

MIAO_TOOL_FUN(
	MiaoMat*,
	MiaoToolMatrixXWeBi,
	(MiaoMat *s, MiaoMat *we, MiaoMat *x, MiaoMat *bi),
	MiaoToolMatrixXWeBi
)
{

#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
	register LwMatColType j, k, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf, *_bibuf, *_bi;
	
	MIAO_assert(!(s));
	MIAO_assert(!(x));
	MIAO_assert(!(we));
	MIAO_assert(!(bi));

	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//����������ǰ������
	{
		row = s->row;
		col = s->col;
		row1 = x->col;
		_bibuf = bi->a;


		_sbuf = s->a;
		_m1buf = x->a;
		_m2buf = we->a;


		//������ƫ��

		if (_bibuf)
		{
			for (i = 0, _s = _sbuf; i < row; i++)
			{
				for (j = 0, _bi = _bibuf; j < col; j++)
				{
					*_s++ = *_bi++;
				}
			}
		}
		else
		{
			LW_MAT_CLR(s);
		}


		cblas_sgemm(
			CblasRowMajor,
			CblasNoTrans, CblasNoTrans,
			s->row, s->col, we->row,
			_ParaFint(1), x->a, x->col,
			we->a, we->col,
			_ParaFint(1), s->a, s->col
		);


	}


#else


#if L1_L2_CACHE_OPTIM == 1
	//register LwMatLenType i, j, k, row, col, row1;
	//register ParaType temp, *_s, *_sbuf, *_x, *_we, *_web, *_xb, *_bi, *_bibuf;
	
	register LwMatColType j, k, row, col, row1;
	register LwMatLenType i;
	register ParaType temp, *_s, *_m1, *_m2, *_m1buf, *_sbuf, *_m2buf, *_bibuf, *_bi;

	MIAO_assert(!(s));
	MIAO_assert(!(x));
	MIAO_assert(!(we));
	MIAO_assert(!(bi));

	//������ǰ�򴫲�
	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//����������ǰ������
	{
		row = s->row;
		col = s->col;
		row1 = x->col;
		_bibuf = bi->a;


		_sbuf = s->a;
		_m1buf = x->a;
		_m2buf = we->a;


		//������ƫ��

		if (_bibuf)
		{
			for (i = 0, _s=_sbuf; i < row; i++)
			{
				for (j = 0, _bi = _bibuf; j < col; j++)
				{
					*_s++ = *_bi++;
				}
			}
		}
		else
		{
			LW_MAT_CLR(s);
		}



		for (i = 0, _m1 = _m1buf; i < row; ++i)
		{
			for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
			{
				temp = *_m1;
				for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
				{
					*_s = _ParaAdd(*_s, _ParaMul(temp, *_m2));
				}
			}
			_sbuf = _s;
		}
	}



#else
	register lw_u16 i, j, k, row, col, row1;
	register ParaType sum, *_s, *_x, *_we, *_web, *_xb, *_bi;
	register ParaType *_we_a, *_bi_a;

	MIAO_assert(!(s));
	MIAO_assert(!(x));
	MIAO_assert(!(we));
	MIAO_assert(!(bi));

	//������ǰ�򴫲�
	if (s->col == we->col && s->row == x->row && we->row == x->col && s->col == bi->length)//����������ǰ������
	{
		row = s->row;
		col = we->col;
		row1 = we->row;
		_s = s->a;
		_xb = x->a;
		_web = we->a;
		_bi = bi->a;
		_we_a = we->a;
		_bi_a = bi->a;

		for (i = 0; i<row; i++)
		{
			for (j = 0; j<col; j++)
			{
				_we = _web;
				_x = _xb;

				sum = _ParaFrom(0);
				for (k = 0; k<row1; k++)
				{
					sum = _ParaAdd(sum, _ParaMul(*_x, *_we));
					_we += col;
					_x++;
				}//������*������

				*_s++ = _ParaAdd(sum, *_bi);
				_bi++;
				_web++;
			}
			_xb += row1;
			_bi = _bi_a;
			_web = _we_a;

		}
	}

#endif

#endif


	return s;
}

MIAO_TOOL_FUN(
	MiaoMat*,
	MiaoToolMatrixTurnMul,
	(MiaoMat *s, MiaoMat *m1, MiaoMat *m2, lw_u8 flag),
	MiaoToolMatrixTurnMul
)
{
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
	MIAO_assert(!(s));
	MIAO_assert(!(m1));
	MIAO_assert(!(m2));


	switch (flag)
	{
	case 0://������ת��
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{

			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasNoTrans,
				s->row, s->col, m2->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
	case 4:
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasNoTrans,
				s->row, s->col, m2->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;
	case 1://m2ת��
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
	case 5:
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasNoTrans, CblasTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;



	case 2://m1ת��
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);	

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
	case 6:
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->col,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	case 3://��ת��
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(0), s->a, s->col
			);

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
	case 7:
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			cblas_sgemm(
				CblasRowMajor,
				CblasTrans, CblasNoTrans,
				s->row, s->col, m1->row,
				_ParaFint(1), m1->a, m1->col,
				m2->a, m2->col,
				_ParaFint(1), s->a, s->col
			);
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	default:
		MIAO_assert(1);
		break;
	}

#else

#if L1_L2_CACHE_OPTIM == 1

	MIAO_assert(!(s));
	MIAO_assert(!(m1));
	MIAO_assert(!(m2));

	switch (flag)
	{
	case 0://������ת��
		LW_MAT_CLR(s);
	case 4:
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf= m1->a, *_sbuf= s->a, *_m2buf= m2->a;

			for (i = 0, _m1 = _m1buf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < row1; ++j, ++_m1)
				{
					temp = *_m1;
					for (k = 0, _s = _sbuf; k < col; ++k, ++_m2, ++_s)
					{
						*_s = _ParaAdd(*_s, _ParaMul(temp, *_m2));
					}
				}
				_sbuf = _s;
			}

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;
	case 1://m2ת��
		LW_MAT_CLR(s);		
	case 5:
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m1->col;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;

			for (i = 0, _s = _sbuf; i < row; ++i)
			{
				for (j = 0, _m2 = _m2buf; j < col; ++j,++_s)
				{
					temp = _ParaFint(0);
					for (k = 0, _m1 = _m1buf; k < row1; ++k, ++_m2,++_m1)
					{
						temp = _ParaAdd(temp, _ParaMul(*_m1, *_m2));
					}

					*_s = _ParaAdd(*_s, temp);
				}
				_m1buf = _m1;
			}
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;



	case 2://m1ת��
		LW_MAT_CLR(s);
	case 6:
		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m2->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf = s->a, *_m2buf = m2->a;

			for (i = 0, _m1 = _m1buf; i < row1; ++i)
			{
				for (j = 0, _s = _sbuf; j < row; ++j, ++_m1)
				{	
					temp = *_m1;
					for (k = 0,_m2=_m2buf; k < col; ++k, ++_s, ++_m2)
					{
						*_s = _ParaAdd(*_s, _ParaMul(temp, *_m2));
					}
				}
				_m2buf = _m2;
			}

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	case 3://��ת��
		LW_MAT_CLR(s);
	case 7:
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			register LwMatLenType i, j, k, row = s->row, col = s->col, row1 = m1->row;
			register ParaType temp, *_s, *_m1, *_m2, *_m1buf = m1->a, *_sbuf1 = s->a, *_sbuf2 = s->a, *_m2buf = m2->a;

			for (i = 0, _m2 = _m2buf; i < col; ++i, ++_sbuf2)
			{
				for (j = 0, _m1 = _m1buf; j < row1; ++j, ++_m2)
				{
					temp = *_m2;
					for (k = 0, _s = _sbuf2; k < row; ++k, ++_m1, _s += col)
					{
						*_s = _ParaAdd(*_s, _ParaMul(temp, *_m1));
					}
				}

				
			}
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	default:
		MIAO_assert(1);
		break;
	}
#else

	register LwMatLenType i, j, k, row, col, row1;
	register LwMatLenType m1d1, m1d3, m2d2, m2d3;
	register ParaType sum, *_s, *_m1, *_m2, *_m1b, *_m2b;
	register ParaType _we_a, _bi_a;

	MIAO_assert(!(s));
	MIAO_assert(!(m1));
	MIAO_assert(!(m2));


	switch (flag & 0x03)
	{
	case 0://������ת��
		if (s->row == m1->row && s->col == m2->col && m1->col == m2->row)
		{
			m1d1 = m1->col;
			m1d3 = 1;
			m2d2 = 1;
			m2d3 = m2->col;

			row1 = m1->col;

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	case 1://m2ת��
		if (s->row == m1->row && s->col == m2->row && m1->col == m2->col)
		{
			m1d1 = m1->col;
			m1d3 = 1;

			m2d2 = m2->col;
			m2d3 = 1;

			row1 = m1->col;

		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}

		break;

	case 2://m1ת��

		if (s->row == m1->col && s->col == m2->col && m1->row == m2->row)
		{
			m1d1 = 1;
			m1d3 = m1->col;

			m2d2 = 1;
			m2d3 = m2->col;

			row1 = m1->row;
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	case 3://��ת��
		if (s->row == m1->col && s->col == m2->row && m1->row == m2->col)
		{
			m1d1 = 1;
			m1d3 = m1->col;
			m2d2 = m2->col;
			m2d3 = 1;

			row1 = m1->row;
		}
		else
		{
			MIAO_assert(1);
			return NULL;
		}
		break;

	default:
		MIAO_assert(1);
		break;
	}

	_s = s->a;
	_m1 = _m1b = m1->a;
	_m2 = _m2b = m2->a;

	if (flag & 0x04)
	{
		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{
				_m1 = _m1b;
				_m2 = _m2b;
				sum = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					sum = _ParaAdd(sum, _ParaMul(*_m1, *_m2));
					_m1 += m1d3;
					_m2 += m2d3;
				}//������*������
				*_s = _ParaAdd(*_s, sum);
				_s++;
				_m2b += m2d2;
			}

			_m1b += m1d1;
			_m2b = m2->a;
		}
	}
	else
	{
		for (i = 0;i<s->row;i++)
		{
			for (j = 0;j<s->col;j++)
			{
				_m1 = _m1b;
				_m2 = _m2b;
				sum = _ParaFrom(0);
				for (k = 0;k<row1;k++)
				{
					sum = _ParaAdd(sum, _ParaMul(*_m1, *_m2));
					_m1 += m1d3;
					_m2 += m2d3;
				}//������*������
				*_s++ = sum;
				_m2b += m2d2;
			}

			_m1b += m1d1;
			_m2b = m2->a;
		}
	}
#endif

#endif

	return s;
}


MIAO_TOOL_FUN(
	void *,
	MiaoToolMatrixConv2,
	(
		MiaoMat *s, //Ŀ���
		MiaoMat *m_kernal, //�����
		MiaoMat *m2,//����� 
		lw_u16 move_x, lw_u16 move_y,//���������ƶ�����
		lw_u8 EqualModel, //��ֵģʽ 1:ֱ�Ӹ��� 0:���
		MiaoToolMatrixConvMethodNUM MoveModel, //�ƶ�ģʽ 'v'/'f'
		lw_u8 KernalModel, //��ģʽ 0/180 180+��f��=��ѧ��ά���
		ParaType *fast_buf//���پ���ڴ���
	),
	MiaoToolMatrixConv2
)
{
	lw_u8 model_flag = ((MoveModel == MiaoToolMatrixConvMethod_Valid) << 1) | (KernalModel == 0);
	
	ParaType sum_buf, temp;

	ParaType *s_a;
	ParaType *k_a, *k_p;
	ParaType *m_r, *m_c, *m_p;

	lw_32 sum_jump, i, j, k, l, kern_r, kern_c, out_r, out_c, m_row, m_col;

	MIAO_assert(!(s));
	MIAO_assert(!(m_kernal));
	MIAO_assert(!(m2));
	MIAO_assert(!(move_x));
	MIAO_assert(!(move_y));
	s_a = s->a;
	
	m_r = m2->a;
	sum_jump = m2->col - m_kernal->col;
	out_r = s->row;
	out_c = s->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	m_row = m2->row;
	m_col = m2->col;
	if (fast_buf==NULL)
	{
		switch (model_flag)
		{
		//��ȫ��ѧ���
		case 0:
			k_a = m_kernal->a + m_kernal->length - 1;
			out_r = m_row + kern_r - 1;
			out_c = m_col + kern_c - 1;
			if (EqualModel)
			{
				for (i = 0; i < out_r; i+=move_x)
				{
					for (j = 0; j < out_c; j+=move_y)
					{
						//���
						sum_buf = _ParaFint(0);
						k_p = k_a;
						for (k = i- kern_c+1; k <= i; k++)
						{
							l = j - kern_r + 1;
							for (m_c= m_r + k*m_col + l; l <= j; ++l, --k_p, ++m_c)
							{
								if (!(k<0 || k>=m_col || l<0 || l>=m_row))
								{
									sum_buf += _ParaMul(*k_p, *m_c);
								}				
							}
						}
						*s_a++ = sum_buf;
					}
				}
			}
			else
			{
				for (i = 0; i < out_r; i += move_x)
				{
					for (j = 0; j < out_c; j += move_y)
					{
						//���
						sum_buf = _ParaFint(0);
						k_p = k_a;
						for (k = i - kern_c + 1; k <= i; k++)
						{
							l = j - kern_r + 1;
							for (m_c = m_r + k*m_col + l; l <= j; ++l, --k_p, ++m_c)
							{
								if (!(k<0 || k >= m_col || l<0 || l >= m_row))
								{
									sum_buf += _ParaMul(*k_p, *m_c);
								}
							}
						}
						*s_a += sum_buf;
						s_a++;
					}
				}
			}
			break;

		case 1:
			MIAO_assert(1);//�Ȳ���
			break;

		case 2:
			MIAO_assert(1);//�Ȳ���
			break;

		//CNN ��׼ģʽ
		case 3:
			k_a = m_kernal->a;
			move_y *= m2->col;
			if (EqualModel)
			{
				for (i = 0,m_r=m2->a; i < out_r; i++,m_r+= move_y)
				{
					for ( j = 0,m_c=m_r; j < out_c; j++,m_c+=move_x)
					{

						//���
						sum_buf = _ParaFint(0);
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
							for (l = 0; l < kern_r; l++)
								sum_buf += _ParaMul(*k_p++, *m_p++);

						*s_a++ = sum_buf;
					}
				}
			}
			else
			{
				for (i = 0, m_r = m2->a; i < out_r; i++, m_r += move_y)
				{
					for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
					{

						//���
						sum_buf = _ParaFint(0);
						k_p = k_a;
						m_p = m_c;
						for (k = 0; k < kern_c; k++, m_p += sum_jump)
							for (l = 0; l < kern_r; l++)
								sum_buf += _ParaMul(*k_p++, *m_p++);

						*s_a += sum_buf;
						s_a++;
					}
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		//���ǵ�Ƕ��ʽϵͳ���� ��ʱ�������Ż� 
		//����CNN�Ż��㷨�������Ϊ������� ��û�иı������� ������GPU
		MIAO_assert(1);
	}

	return s;

}

//ֻ����һ��batch
MIAO_TOOL_FUN(
	void*, MiaoToolMatrixConvFastMultCh,
	(
		MiaoMat *m_out, //��� row-col ����һ��ͨ�� length����������С
		MiaoMat *m_kernal, //����� row-col ����һ��ͨ�� length ����һ����������С [2,2,4] row:2 col:2 length:16
		MiaoMat *m_in,//����� row-col ����һ��ͨ��
		MiaoMat *bias,//ƫ�� row-col-length ����ν
		lw_u16 in_high,//����߶� 
		lw_u16 out_high,//����߶�
		lw_u16 move_x, lw_u16 move_y,//���������ƶ�����
		MiaoToolMatrixConvMethodNUM conv_method,
		lw_u8 FD_1_or_BK_0,//ǰ�򴫵ݻ��߷��򴫵�
		ParaType *fast_buf//������
		),
	MiaoToolMatrixConvFastMultCh
)
{

	lw_32 i, j, k, l, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2, out_jump, kern_len;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	ParaType temp, sum_buf;
	ParaType *o_a, *o_pic;
	ParaType *k_a, *k_p, *k_p2;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf, *pbuf2;

	lw_32 div2, mod2;

	o_a = m_out->a;
	out_r = m_out->row;
	out_c = m_out->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	in_r = m_in->row;
	in_c = m_in->col;
	k_a = m_kernal->a;
	kern_len = m_kernal->length;
	kern_one_len = kern_c*kern_r;
	
	//�ȼ���ƫ��
	if (bias)
	{
		if (bias->a)
		{
			l = m_out->col*m_out->row;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				for (j = 0; j < l; j++)
				{
					*o_a++ = temp;
				}
			}

			o_a = m_out->a;
		}
		else
		{
			LW_MAT_CLR(m_out);
		}


	}
	else
	{
		LW_MAT_CLR(m_out);
	}

	div2 = kern_len / 2, mod2 = kern_len % 2;

	if (FD_1_or_BK_0)
	{
		move_y *= m_in->col;
		sum_jump1 = m_in->col;
		sum_jump2 = in_c*in_r;
		out_jump = out_c*out_r;
		for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
		{
			for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
			{

				//��ͼ
				pbuf = fast_buf;//�õ�������
				for (k = 0, m_py = m_c; k < in_high; k++,m_py+=sum_jump2)
					for (l = 0, m_px=m_py; l < kern_r; l++, m_px+=sum_jump1)
						for (n = 0,m_p=m_px; n < kern_c; n++)
							*pbuf++ = *m_p++;

				//���
				
				for (k = 0, k_p = k_a,o_pic=o_a; k < out_high; k++, o_pic += out_jump)
				{
					//for (sum_buf = _ParaFint(0), pbuf = fast_buf, l = 0; l < kern_len; l++)
					//{
					//	sum_buf+= _ParaMul(*k_p++, *pbuf++);
					//}
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS

					sum_buf = cblas_sdot(kern_len, k_p, 1, fast_buf, 1);
					k_p += kern_len;

#else
					for (sum_buf = _ParaFint(0), pbuf = fast_buf, l = 0; l < div2; l++)
					{
						sum_buf += _ParaMul(k_p[0], pbuf[0]);
						sum_buf += _ParaMul(k_p[1], pbuf[1]);
						k_p += 2;
						pbuf += 2;
					}

					l = mod2;
					while (l--)
					{
						sum_buf += _ParaMul(*k_p++, *pbuf++);
					}
#endif
					(*o_pic) += sum_buf;
				}

				o_a++;
			}
		}
	}
	else
	{
		sum_jump1 = kern_len + kern_one_len;
		sum_jump2 = out_c*out_r;
		m_r = m_in->a;
		out_jump = in_c*in_r;
		for (i = 0; i < out_r; i += move_x)
		{
			for (j = 0; j < out_c; j += move_y)
			{
				//��ͼ
				pbuf = fast_buf;//�õ�������
				for (k = i - kern_c + 1; k <= i; k++)
				{
					l = j - kern_r + 1;
					for (m_c = m_r + k*in_c + l; l <= j; ++l, ++m_c)
					{
						pbuf2 = pbuf++;						
						if (!(k<0 || k >= in_c || l<0 || l >= in_r))
						{
							//��������ͼƬ�����������
							for ( n = 0, m_p = m_c; n < in_high; n++, pbuf2+=kern_one_len, m_p += out_jump)
							{
								*pbuf2 = *m_p;
							}
						}
						else
						{
							for (n = 0; n < in_high; n++, pbuf2 += kern_one_len)
							{
								*pbuf2 = _ParaFint(0);
							}
						}
					}
				}

				//���				
				//ѭ��ÿһ��ά��
				for ( k = 0, o_pic=o_a,k_p=k_a; k < out_high; k++,k_p+=kern_one_len)
				{
					//ѭ��ÿһ��ͨ��
					for ( l = 0, pbuf = fast_buf, k_p2 = k_p + kern_one_len - 1, sum_buf = _ParaFint(0); l < in_high; l++, k_p2 += sum_jump1)
					{
						for ( n = 0; n < kern_one_len; n++)
						{
							sum_buf += _ParaMul(*k_p2--, *pbuf++);
						}
					}

					*o_pic += sum_buf;
					o_pic += sum_jump2;

				}
				o_a++;
			}
		}
	}


	return m_out;
}


//ֻ����һ��batch
MIAO_TOOL_FUN(
	void*, MiaoToolMatrixConvFastMultChIm2ColFd,
	(
		MiaoMat *m_out, //��� row-col ����һ��ͨ�� length����������С
		MiaoMat *m_kernal, //����� row-col ����һ��ͨ�� length ����һ����������С [2,2,4] row:2 col:2 length:16
		MiaoMat *m_in,//����� row-col ����һ��ͨ��
		MiaoMat *bias,//ƫ�� row-col-length ����ν
		lw_u16 in_high,//����߶� 
		lw_u16 out_high,//����߶�
		lw_u16 move_x, lw_u16 move_y,//���������ƶ�����
		MiaoToolMatrixConvMethodNUM conv_method,
		uint32_t now_flow_len,//��ʵ�������Ĵ�С
		ParaType *fast_buf//������
		),
	MiaoToolMatrixConvFastMultChIm2ColFd
)
{

	lw_32 i, j, k, l, n;
	lw_32 out_r, out_c;
	lw_32 sum_jump1, sum_jump2, out_jump, kern_len;
	lw_32 kern_r, kern_c, kern_one_len;
	lw_32 in_r, in_c;

	uint32_t col_count = 0, col_cmax, im2col_one_len;

	ParaType temp, sum_buf;
	ParaType *o_a;
	ParaType *k_a, *k_p, *k_p2;
	ParaType *m_r, *m_c, *m_p, *m_px, *m_py;
	ParaType *pbuf, *pbuf2;

	o_a = m_out->a;
	out_r = m_out->row;
	out_c = m_out->col;
	kern_r = m_kernal->row;
	kern_c = m_kernal->col;
	in_r = m_in->row;
	in_c = m_in->col;
	k_a = m_kernal->a;
	kern_len = m_kernal->length;
	kern_one_len = kern_c*kern_r;
	im2col_one_len = (kern_one_len*in_high);
	col_cmax = now_flow_len / im2col_one_len;
	col_cmax = col_cmax >= (out_c*out_r) ? (out_c*out_r) : col_cmax;
	MIAO_assert(!(col_cmax));

	//�ȼ���ƫ��
	if (bias)
	{
		if (bias->a)
		{
			l = m_out->col*m_out->row;
			for (i = 0; i < out_high; i++)
			{
				temp = bias->a[i];
				for (j = 0; j < l; j++)
				{
					*o_a++ = temp;
				}
			}

			o_a = m_out->a;
		}
		else
		{
			LW_MAT_CLR(m_out);
		}
	}
	else
	{
		LW_MAT_CLR(m_out);
	}

	move_y *= m_in->col;
	sum_jump1 = m_in->col;
	sum_jump2 = in_c*in_r;
	out_jump = out_c*out_r;
	pbuf = fast_buf;//�õ�������

#if !(defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS)
	MIAO_assert(1);
#endif


	for (i = 0, m_r = m_in->a; i < out_r; i++, m_r += move_y)
	{
		for (j = 0, m_c = m_r; j < out_c; j++, m_c += move_x)
		{
			//��ͼ
			for (k = 0, m_py = m_c; k < in_high; k++, m_py += sum_jump2)
				for (l = 0, m_px = m_py; l < kern_r; l++, m_px += sum_jump1)
					for (n = 0, m_p = m_px; n < kern_c; n++)
						*pbuf++ = *m_p++;

			++col_count;
			if (col_count==col_cmax)
			{
				col_count = 0;
				pbuf = fast_buf;
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
				//�������˷�
				cblas_sgemm(
					CblasRowMajor,
					CblasNoTrans, CblasTrans,
					out_high, col_cmax, im2col_one_len,
					_ParaFint(1), k_a, im2col_one_len,
					pbuf, im2col_one_len,
					_ParaFint(1), o_a, out_jump
				);
#endif
				o_a += col_cmax;
			}
		}
	}

	if (col_count)
	{
		pbuf = fast_buf;
#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS
		//�������˷�
		cblas_sgemm(
			CblasRowMajor,
			CblasNoTrans, CblasTrans,
			out_high, col_count, im2col_one_len,
			_ParaFint(1), k_a, im2col_one_len,
			pbuf, im2col_one_len,
			_ParaFint(1), o_a, out_jump
		);
#endif
		o_a += col_cmax;
	}


	return m_out;
}





MIAO_TOOL_FUN(void*, MiaoToolMatrixMoments,
(
	MiaoMat *src,
	ParaType *mean,
	ParaType *var,
	lw_u8 dim
	), 
	MiaoToolMatrixMoments)
{

	register ParaType _sum1, _sum2;
	register ParaType *p, *pbase;
	register ParaType _div_m;
	register uint32_t i, j;
	register uint32_t r, c;
	MIAO_assert(!(src));
	MIAO_assert(!(mean));
	MIAO_assert(!(var));

	r = src->row;
	c = src->col;
	
	if (dim==0x01)
	{
		pbase = src->a;
		_div_m = _ParaDiv(_ParaFrom(1), _ParaFint(c));
		for ( i = 0; i < r; ++i)
		{
			p = pbase;
			
			//���

			
			for (j = 0, _sum1 = _ParaFrom(0); j < c; ++j, ++p)
			{
				_sum1 = _ParaAdd(_sum1, *p);
			}


			//��ƽ��
			_sum1 = _ParaMul(_sum1, _div_m);
			//�����ֵ
			*mean++ = _sum1;

			//����
			for (j = 0, _sum2 = _ParaFrom(0); j < c; ++j, ++pbase)
			{
				_sum2 = _ParaAdd(
					_sum2,
					_ParaMul(_ParaSub(*pbase, _sum1), _ParaSub(*pbase, _sum1))
				);
			}
			//�󷽲�
			_sum2 = _ParaMul(_sum2, _div_m);
			//���淽��
			*var++ = _sum2;
		}

	}
	else
	{
		MIAO_assert(1);
	}



	return src;
}


MIAO_TOOL_FUN(void*, MiaoToolMatrixVectShift,
(
	MiaoMat *y,
	MiaoMat *we,
	MiaoMat *x,
	MiaoMat *bi
	),
	MiaoToolMatrixVectShift)
{
	LwMatRowType i, _r;
	LwMatColType j, _c;
	ParaType *ya, *xa, *wa, *ba, *wa_b, *ba_b;
	ParaType temp_w, temp_b;
	MIAO_assert(!(y));
	MIAO_assert(!(we));
	MIAO_assert(!(x));

	if (bi)
	{
		ya = y->a;
		xa = x->a;
		wa_b = we->a;
		ba_b = bi->a;
		if (we->length==1)
		{
			temp_w = *wa_b;
			temp_b = *ba_b;
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0; j < _c; ++j, ++ya, ++xa)
				{
					*ya = _ParaAdd(_ParaMul(temp_w, *xa), temp_b);
				}
			}
		}
		else
		{
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0, wa = wa_b, ba = ba_b; j < _c; ++j, ++ya, ++wa, ++xa, ++ba)
				{
					*ya = _ParaAdd(_ParaMul(*wa, *xa), *ba);
				}
			}
		}
	}
	else
	{
		ya = y->a;
		xa = x->a;
		wa_b = we->a;
		if (we->length == 1)
		{
			temp_w = *wa_b;
			temp_b = *ba_b;
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0; j < _c; ++j, ++ya, ++xa)
				{
					*ya = _ParaMul(temp_w, *xa);
				}
			}
		}
		else
		{
			for (i = 0, _r = y->row, _c = y->col; i < _r; i++)
			{
				for (j = 0, wa = wa_b; j < _c; ++j, ++ya, ++wa, ++xa)
				{
					*ya = _ParaMul(*wa, *xa);
				}
			}
		}
	}

	return y;

}


MIAO_TOOL_FUN(void*, MiaoToolMatrixDotSum,
(
	MiaoMat *y,
	MiaoMat *x1,
	MiaoMat *x2,
	lw_u8 dim
	),
	MiaoToolMatrixDotSum)
{

	LwMatRowType i, _r;
	LwMatColType j, _c;
	LwMatLenType l, _L;
	ParaType _sum;
	ParaType *ya, *ya_b;
	ParaType *x1a, *x1a_b;
	ParaType *x2a, *x2a_b;

	MIAO_assert(!(y));
	MIAO_assert(!(x1));
	MIAO_assert(!(x2));


	switch (dim)
	{

	case 0x00:
		x1a = x1->a;
		x2a = x2->a;
		_sum = _ParaFint(0);
		for ( l = 0,_L=x1->length; l < _L; ++l,++x1a,++x2a)
		{
			_sum = _ParaAdd(_sum, (_ParaMul(*x1a, *x2a)));
		}
		y->a[0] = _sum;
		break;

	case 0x10:
		x1a = x1->a;
		x2a = x2->a;
		_sum = _ParaFint(0);
		for (l = 0, _L = x1->length; l < _L; ++l, ++x1a, ++x2a)
		{
			_sum = _ParaAdd(_sum, (_ParaMul(*x1a, *x2a)));
		}
		y->a[0] = _ParaAdd(y->a[0], _sum);
		break;

	case 0x01:
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			{
				_sum = _ParaAdd(_sum, (_ParaMul(*x1a, *x2a)));
			}
			*ya = _sum;
		}

		break;

	case 0x11:
		x1a = x1->a;
		x2a = x2->a;
		ya = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, _sum = _ParaFint(0); j < _c; ++j, ++x1a, ++x2a)
			{
				_sum = _ParaAdd(_sum, (_ParaMul(*x1a, *x2a)));
			}
			*ya = _ParaAdd(*ya, _sum);
		}
		break;

	case 0x02:
		LW_MAT_CLR(y);
	case 0x12:
		x1a = x1->a;
		x2a = x2->a;
		ya_b = y->a;
		for (i = 0, _r = x1->row, _c = x1->col; i < _r; ++i, ++ya)
		{
			for (j = 0, ya = ya_b; j < _c; ++j, ++x1a, ++x2a, ++ya)
			{
				*ya = _ParaAdd(*ya, (_ParaMul(*x1a, *x2a)));
			}
		}
		break;

	default:
		MIAO_assert(1);
		break;
	}


}



MIAO_TOOL_FUN(MiaoMat*, MiaoToolMatrix_RATIO_ADD,
(MiaoMat *s,ParaType a, MiaoMat *m1, ParaType b, MiaoMat *m2),
MiaoToolMatrix_RATIO_ADD
)
{
	register uint32_t i;
	register ParaType *_sa, *_m1a, *_m2a;
	MIAO_assert(!(s));
	MIAO_assert(!(m1));
	MIAO_assert(!(m2));

	if (s->length == m1->length && s->length == m2->length)//�����������
	{
		i = s->length;
		_sa = s->a;
		_m1a = m1->a;
		_m2a = m2->a;
		while (i--)
		{
			*_sa++ = _ParaAdd(_ParaMul(a, *_m1a), _ParaMul(b, *_m2a));
			_m1a++;
			_m2a++;
		}
		
	}
	else
	{
		return NULL;
	}


	return s;
}

#endif //MIAO_TOOL_IMPORT_MATH || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 4)

