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

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerConvNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	lw_u16 move_delt,
	MiaoToolMatrixConvMethodNUM move_method,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
)
{
	MiaoToolNNLayerConv_t *node;
	uint32_t in_row, out_row;
	uint32_t out_x, out_y, out_z;

	LwMatColType kern_len1, kern_len2, kern_res_len;

	MIAO_assert(!(pmem));

	//�������
	in_row = in_x*in_y*in_z;
	out_z = kern_num;
	kern_len1 = kern_x*kern_y*in_z;
	kern_len2 = kern_x*kern_y*kern_num;

	//����ڵ��ڴ�
	node = pmem->Calloc(pmem, sizeof(MiaoToolNNLayerConv_t));

	node->databasic.basic.NN_Kind = MiaoToolNNSubLayerKind_Conv;

	//node->DataRes = (ParaType *)(((lw_u8 *)node) + sizeof(MiaoToolNNLayerConv_t));

	node->ConvMethod = move_method;

	if (move_method == MiaoToolMatrixConvMethod_Valid)
	{
		out_x = (in_x - kern_x + 1) / (move_delt);//�����ƶ�����
		out_y = (in_y - kern_y + 1) / (move_delt);//�����ƶ�����

	}
	else if (move_method == MiaoToolMatrixConvMethod_Full)
	{
		out_x = (in_x) / (move_delt)+1;//�����ƶ�����
		out_y = (in_y) / (move_delt)+1;//�����ƶ�����

	}
	else
	{
		MIAO_assert(1);
	}

	out_row = out_x*out_y*out_z;

	//��������
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	MiaoToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);

	//�������
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}


#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	MiaoToolMatrixInitial(&(node->databasic.basic.out), max_mini_batch, out_row, pout);
	MiaoToolMatrixInitial(&(node->databasic.basic.in), max_mini_batch, in_row, pin);

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	MiaoToolMatrixInitial(&(node->databasic.basic.out), out_row, max_mini_batch, pout);
	MiaoToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);
#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	//���ò���
	node->databasic.Bias.row = 1;
	node->databasic.Bias.col = out_z;
	node->databasic.Bias.length = out_z;

	node->databasic.DeltB.row = 1;
	node->databasic.DeltB.col = out_z;
	node->databasic.DeltB.length = out_z;

	node->databasic.Weight.row = kern_len1;//һ�������������ά��
	node->databasic.Weight.col = kern_num;
	node->databasic.Weight.length = node->databasic.Weight.row*node->databasic.Weight.col;

	node->databasic.DeltW.row = kern_len1;;
	node->databasic.DeltW.col = kern_num;
	node->databasic.DeltW.length = node->databasic.Weight.row*node->databasic.Weight.col;

	kern_res_len = (kern_len1 > kern_len2) ? kern_len1 : kern_len2;
	node->databasic.basic.FlowData.uData = PARA_MEM_CAL(kern_res_len);
	node->move_delt = move_delt;


	node->in_x = in_x;
	node->in_y = in_y;
	node->out_x = out_x;
	node->out_y = out_y;

	node->conv_kernal_x = kern_x;
	node->conv_kernal_y = kern_y;
	node->conv_kernal_z = in_z;
	node->conv_kernal_num = kern_num;

	//����Ҫ������ڴ��С
	node->databasic.basic.sizeofdata = PARA_MEM_CAL(node->databasic.Weight.length) + PARA_MEM_CAL(node->databasic.Bias.length);


	//��Ա����(ֻ��ǰ�򴫵��뷴�򴫵ݲ��ö�������)
	node->databasic.basic.Forward = MiaoToolBPnnLayerConvForward;
	node->databasic.basic.Backward = MiaoToolBPnnLayerConvBackward;

	node->databasic.basic.Update = MiaoToolBPnnLayerFullConUpdate;
	node->databasic.basic.NNmalloc = MiaoToolBPnnLayerFullConNNmalloc;
	node->databasic.basic.Clear = MiaoToolBPnnLayerFullConClear;
	node->databasic.basic.DataInit = MiaoToolBPnnLayerFullConDataInit;
	node->databasic.basic.Copy = MiaoToolBPnnLayerFullConCopy;
	node->databasic.basic.SoftReplace = MiaoToolBPnnLayerFullConSoftReplace;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = out_z;
		info[3] = out_x*out_y*out_z;
	}

	return (MiaoToolNNSubLayerBasic_t *)node;

}

MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerConvSetIm2Col(
	MiaoToolNNSubLayerBasic_t *pbase,
	uint32_t udata_sizeof_pdata,
	void *pdata
)
{

	MiaoToolNNLayerConv_t *pconv=(void *)pbase;

	MIAO_assert(!(pbase));

#if !(defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS)
	MIAO_assert(1);
#endif

	if (pdata==NULL && udata_sizeof_pdata ==0)
	{
		return NULL;
	}
	else if (pdata!=NULL && udata_sizeof_pdata ==0)
	{

		pconv->im2col_flag = 1;
		pbase->FlowData.pData = pdata;
	}
	else if(pdata==NULL && udata_sizeof_pdata !=0)
	{
		pconv->im2col_flag = 1;
		pbase->FlowData.uData = udata_sizeof_pdata;
	}
	else
	{
		pconv->im2col_flag = 1;
		pbase->FlowData.uData = udata_sizeof_pdata;
		pbase->FlowData.pData = pdata;
	}


	return pbase;


}


#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_F


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

)
{
	uint32_t i, j, jmax, k, kmax;
	uint32_t m, mmax, n, nmax, o, omax;
	uint32_t _minb, _minb_mpy_col, _minb_mpy_picl, _minb_mpy_outl, _d;
	ParaType *p_pic_head, *p_out_head;
	ParaType *p_pic_pos, *p_pic_pos_row, *p_out_pos;
	ParaType *p_buf;
	ParaType *p_pic_move_pix, *p_out_move_pix;
	ParaType *p_pic_move_col;
	ParaType *p_pic_move_pic;
	ParaType *p_kern;
	ParaType *p_bias;
	ParaType _sum, _mpy_b;


	MIAO_assert(!(s));
	MIAO_assert(!(in_m));
	MIAO_assert(!(kern_m));
	MIAO_assert(!(bias));
	MIAO_assert(!(buf));

	_minb = in_m->col;
	_minb_mpy_col = _minb*pic_x;
	_minb_mpy_picl = _minb_mpy_col*pic_y;
	_d = move_delt*_minb_mpy_col;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_ROW(P) (P+=_minb_mpy_col)
#define _NEXT_ROW_M(P) (P+=_d)
#define _NEXT_PIC(P) (P+=_minb_mpy_picl)
#define _NEXT_O_PIC(P) (P+=_minb_mpy_outl)


	mmax = pic_z;
	nmax = kern_y;
	omax = kern_x;

	//���㲻ͬģʽ��ѭ���Ĵ���
	jmax = out_y;
	kmax = out_x;
	move_delt *= _minb;

	//ѭ�����е�min-batch
	for (i = 0; i < now_min_batch; ++i)
	{
		p_pic_head = (in_m->a) + i;
		p_out_head = (s->a) + i;

		_minb_mpy_outl = (jmax)*(kmax);

		p_pic_pos_row = p_pic_head;
		p_out_pos = p_out_head;

		//ɨ��ͼƬ
		for (j = 0; j < jmax; ++j)
		{
			p_pic_pos = p_pic_pos_row;

			for (k = 0; k < kmax; k++)
			{
				//��ͼ �õ�С����
				p_pic_move_pic = p_pic_pos;
				p_buf = buf;
				m = mmax;
				while (m--)
				{
					p_pic_move_col = p_pic_move_pic;
					n = nmax;
					while (n--)
					{
						o = omax;
						p_pic_move_pix = p_pic_move_col;
						while (o--)
						{
							*p_buf++ = *p_pic_move_pix;
							_NEXT_PIX(p_pic_move_pix);
						}//end o
						_NEXT_ROW(p_pic_move_col);
					}//end n
					_NEXT_PIC(p_pic_move_pic);
				}//end m


				 //��ÿһ����������� ���Ҽ���ƫ��
				p_kern = kern_m->a;
				p_out_move_pix = p_out_pos;
				//ÿһ�������
				for (m = 0; m < kern_m->row; ++m)
				{

					n = kern_m->col;
					_sum = _ParaFint(0);
					p_buf = buf;

					//������
					while (n--)
					{
						_mpy_b = _ParaMul(*p_buf, *p_kern);
						_sum += _mpy_b;
						p_buf++;
						p_kern++;
					}//end while n


					*p_out_move_pix = _sum + bias->a[m];//����ƫ��
					_NEXT_O_PIC(p_out_move_pix);
				}//end for m

				_NEXT_PIX(p_out_pos);//���λ��
				p_pic_pos += move_delt;//��������λ��
			}//end k

			 //��һ��Сͼ�����
			_NEXT_ROW_M(p_pic_pos_row);

		}//end j


	}//end i 
#undef _NEXT_PIX(P) 
#undef _NEXT_ROW(P) 
#undef _NEXT_ROW_M(P) 
#undef _NEXT_PIC(P)
#undef _NEXT_O_PIC(P)

	return s;
}


MiaoMat* _MiaoToolNNKernalConvBK
(
	MiaoMat *inerr, MiaoMat *outerr, MiaoMat *kern,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 now_min_batch,
	lw_u16 move_delt,
	ParaType *buf
)
{
	uint32_t i, j, jmax;
	uint32_t _minb, kern_len, _minb_mpy_in_len, _minb_mpy_out_len;

	lw_32 ix, iy, ixmax, ixmin, iymax, iymin;
	lw_32 m, n, mmax, nmax;
	ParaType *p_inerr_head, *p_outerr_head;
	ParaType *p_outerr_pos, *p_outerr_move, *p_inerr_pos, *p_inerr_move;
	ParaType *p_buf, *p_buf_bott;
	ParaType *p_kern, *p_kern_pos;
	ParaType databuf, datasum;

#define _OUTERR_PIX(D,X,Y) (D+(Y*pic_x+X)*_minb)
#define _NEXT_PIX(P) (P+=_minb)
#define _IN_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _OUT_NEXT_PIC(P) (P+=_minb_mpy_out_len)

	if (move_delt != 1)
	{
		//û�������Ǽ��1������µ��󵼷�ʽ ����Ĭ�ϱ������ƶ�1��
		MIAO_assert(1);
	}

	//����������
	LW_MAT_CLR(inerr);


	//������ʼ��
	_minb = outerr->col;
	jmax = kern->row;
	p_buf_bott = buf + kern_x*kern_y - 1;
	kern_len = kern_x*kern_y;
	_minb_mpy_in_len = _minb*in_x*in_y;
	_minb_mpy_out_len = _minb*pic_x*pic_y;

	ixmin = 1 - (lw_32)kern_x;
	iymin = 1 - (lw_32)kern_y;

	ixmax = pic_x;
	iymax = pic_y;

	for (i = 0; i < now_min_batch; i++)
	{
		p_outerr_head = (outerr->a) + i;
		p_inerr_head = (inerr->a) + i;
		p_kern_pos = kern->a;

		p_outerr_pos = p_outerr_head;

		//�ȼ���һ��ͼ��
		for (j = 0; j < jmax; ++j)
		{
			p_inerr_pos = p_inerr_head;
			//ѭ������ͼ��
			for (iy = iymin; iy < iymax; ++iy)
			{
				for (ix = ixmin; ix < ixmax; ++ix)
				{
					p_inerr_move = p_inerr_pos;

					//ȡ��һС��
					nmax = kern_y + iy;
					p_buf = p_buf_bott;//ָ�뵹����ȡ

					for (n = iy; n < nmax; ++n)
					{
#if 0

						for (m = ix; m < ix + kern_x; ++m)
						{
							if (n<0 || n >= pic_y || m<0 || m >= pic_x)
							{
								*p_buf-- = _ParaFint(0);
							}
							else
							{
								*p_buf-- = *(_OUTERR_PIX(p_outerr_pos, m, n));
							}
						}



#else		
						mmax = kern_x;

						if (n<0 || n >= pic_y)
						{
							m = mmax;
							/*
							������Χ ��һ������ȫΪ0
							->  0  0
							0  X  X  X
							X  X  X
							*/
							while (m--)
							{
								*p_buf-- = _ParaFint(0);
							}
						}
						else
						{
							m = ix;

							if (m<0)
							{
								/*
								��ֵΪ0
								0  0
								->	0  X  X  X
								X  X  X
								*/
								while (m)
								{
									*p_buf-- = _ParaFint(0);
									++m;
								}
								mmax += (ix);
							}

							p_outerr_move = _OUTERR_PIX(p_outerr_pos, m, n);

							if (ix + kern_x > pic_x)
							{
								/*
								X  X  X
								->  X  X  X  0
								0  0
								�ȸ�ֵ�ٲ���
								*/
								m = (ix + kern_x - pic_x);//���ұ����ص�����
								mmax -= m;// �����һ������ԭ���� 000 XXXX 000 ����ʱmmaxӦ����4
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
								while (m--)
								{
									*p_buf-- = _ParaFint(0);
								}
							}
							else
							{
								/*
								0  0
								->  X  X  X
								X  X  X
								ֱ�Ӹ�ֵ ���ò���
								*/
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
							}
						}// end if
#endif
					}//end n

					 //�����Ѿ�ȡ��һС��
					p_kern = p_kern_pos;
					m = in_z;
					while (m--)
					{
						n = kern_len;
						p_buf = buf;
						datasum = _ParaFint(0);

						while (n--)
						{
							databuf = _ParaMul(*p_kern, *p_buf);
							datasum += databuf;
							++p_kern;
							++p_buf;
						}
						*p_inerr_move += datasum;
						_IN_NEXT_PIC(p_inerr_move);

					}

					_NEXT_PIX(p_inerr_pos);

				}// end iX

			}// end iY

			p_kern_pos += kern->col;
			_OUT_NEXT_PIC(p_outerr_pos);

		}//end j

	}//end i

#undef _OUTERR_PIX(D,X,Y) 
#undef _NEXT_PIX(P) 
#undef _IN_NEXT_PIC(P) 
#undef _OUT_NEXT_PIC(P) 

	return inerr;
}


ParaType _MiaoToolNNKernalConvCalDeltOnce
(
	ParaType *delt, ParaType *inerr, ParaType *outerr1,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 min_batch,
	lw_u16 move_delt
)
{
	uint32_t i;
	uint32_t j, k;
	uint32_t m, n;
	uint32_t _minb, _minb_mpy_in_x, _minb_mpy_in_x_move_delt, _minb_mpy_move_delt, _minb_mpy_in_len;
	ParaType sum, databuf;

	ParaType *p_in_pic, *p_in_row, *p_in_pix;

	ParaType *p_in_move_row, *p_in_move_pix;

	ParaType *p_out_move;

	ParaType *p_delt;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_I_ROW(P) (P+=_minb_mpy_in_x)
#define _NEXT_I_ROW_D(P) (P+=_minb_mpy_in_x_move_delt)
#define _NEXT_I_PIC(P) (P+=_minb_mpy_in_len)
#define _NEXT_I_PIX_D(P) (P+=_minb_mpy_move_delt)
	//������ʼ��
	_minb = min_batch;

	_minb_mpy_in_x = _minb*in_x;
	_minb_mpy_in_x_move_delt = _minb_mpy_in_x*move_delt;
	_minb_mpy_move_delt = min_batch*move_delt;
	_minb_mpy_in_len = min_batch*in_x*in_y;

	p_delt = delt;
	//ѭ��ÿһ��ͼƬ

	for (i = 0, p_in_pic = inerr; i < in_z; ++i)
	{
		for (j = 0, p_in_row = p_in_pic; j < kern_y; j++)
		{
			for (k = 0, p_in_pix = p_in_row; k < kern_x; k++)
			{

				//ѭ��Сͼ������˼�
				m = out_y;
				p_in_move_row = p_in_pix;
				p_out_move = outerr1;
				sum = _ParaFint(0);

				while (m--)
				{
					n = out_x;
					p_in_move_pix = p_in_move_row;
					while (n--)
					{
						databuf = _ParaMul(*p_in_move_pix, *p_out_move);
						sum += databuf;

						_NEXT_PIX(p_out_move);
						_NEXT_PIX(p_in_move_pix);

					}// end n

					_NEXT_I_ROW(p_in_move_row);
				}// end m

				 //�õ�һ����sum
				*p_delt++ = sum;

				_NEXT_I_PIX_D(p_in_pix);
			}//end k
			_NEXT_I_ROW_D(p_in_row);
		}//end j
		_NEXT_I_PIC(p_in_pic);
	}//end i



	sum = _ParaFint(0);
	i = out_x*out_y;
	while (i--)
	{
		sum += *outerr1++;
	}

#undef _NEXT_PIX(P)
#undef _NEXT_I_ROW(P)
#undef _NEXT_I_ROW_D(P)
#undef _NEXT_I_PIC(P) 
#undef _NEXT_I_PIX_D(P)

	return sum;
}

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_F


lw_ptr MiaoToolBPnnLayerConvForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	MiaoToolNNLayerFullCon_t *pfc;
	MiaoToolNNLayerConv_t *pcv;
	lw_ptr min_b = mini_b_num;
	MiaoMat in, out, we;
	MIAO_assert(!(pNNSLB));
	pfc = (MiaoToolNNLayerFullCon_t *)pNNSLB;
	pcv = (void *)pfc;

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	MiaoMatInit(&in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	MiaoMatInit(&out, pcv->out_x, pcv->out_y, pfc->basic.out.a);
	MiaoMatInit(&we, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->Weight.a);
	we.length *= pcv->conv_kernal_z;
	out.length *= pcv->conv_kernal_z;


	if (pcv->im2col_flag)
	{
		while (min_b--)
		{

			MiaoToolMatrixConvFastMultChIm2ColFd(
				&out, &we, &in, &(pfc->Bias),
				pcv->conv_kernal_z, pcv->conv_kernal_num,
				pcv->move_delt, pcv->move_delt,
				pcv->ConvMethod,
				pfc->basic.FlowData.uData / sizeof(ParaType),
				pfc->basic.FlowData.pData
			);

			in.a += pfc->basic.in.col;
			out.a += pfc->basic.out.col;

		}
	}
	else
	{
		while (min_b--)
		{
			pMiaoToolMatrixConvFastMultCh(
				&out, &we, &in, &(pfc->Bias),
				pcv->conv_kernal_z, pcv->conv_kernal_num,
				pcv->move_delt, pcv->move_delt,
				pcv->ConvMethod,
				1,
				pfc->basic.FlowData.pData
			);

			in.a += pfc->basic.in.col;
			out.a += pfc->basic.out.col;

		}
	}






#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F

	_MiaoToolNNKernalConvFd(
		&(pfc->basic.out),
		&(pfc->basic.in),
		&(pfc->Weight),
		&(pfc->Bias),
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		1,//ǿ��Ĭ��Ϊ1
		mini_b_num,
		's',//ǿ��Ĭ��Ϊ�����
		pcv->databasic.basic.FlowData.pData
	);

#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

	return mini_b_num;

}


lw_ptr MiaoToolBPnnLayerConvBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	MiaoToolNNLayerFullCon_t *pfc;
	MiaoToolNNLayerConv_t *pcv;
	lw_u16 i = 0, j, k;
	ParaType *db, *dw, *dw2, delt_bias, *dbm;
	uint32_t move_d;
	MiaoMat m_dw, m_in, m_out;
	MIAO_assert(!(pNNSLB));
	pfc = (MiaoToolNNLayerFullCon_t *)pNNSLB;
	pcv = (void *)pfc;
	move_d = pfc->basic.in.col*pcv->out_x*pcv->out_y;

#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C
	i = mini_b_num;
	db = pfc->DeltB.a;

	MiaoMatInit(&m_dw, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->DeltW.a);
	MiaoMatInit(&m_in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	MiaoMatInit(&m_out, pcv->out_x, pcv->out_y, pfc->basic.out.a);

	////����ǵ�һ�η��򴫲� ��Ҫ����ԭ�ȵ��ݶ� update�����Զ�����ݶ�
	//if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	//{
	//	//����ÿһ�������
	//	for ( j = 0, m_out.a= pfc->basic.out.a; j < pcv->conv_kernal_num; j++,m_out.a+=m_out.length)
	//	{
	//		//����db
	//		for ( k = 0, delt_bias=_ParaFint(0), dbm=m_out.a; k < m_out.length; k++, dbm++)
	//		{
	//			delt_bias += *dbm;
	//		}
	//		*db++ = delt_bias;
	//		//����ÿһ��ά��
	//		for ( k = 0, m_in.a= pfc->basic.in.a; k < pcv->conv_kernal_z; k++,m_in.a+=m_in.length)
	//		{
	//			pMiaoToolMatrixConv2(
	//				&m_dw, &m_out, &m_in,
	//				pcv->move_delt, pcv->move_delt,
	//				1,
	//				's',
	//				0,
	//				NULL
	//			);
	//			m_dw.a += m_dw.length;
	//		}
	//	}
	//	i--;
	//}

	if (pNNSLB->pNN->Train.Flag.NeedTrain== MiaoToolNNNeedTrain_Need)
	{
		while (i--)
		{
			m_dw.a = pfc->DeltW.a;
			db = pfc->DeltB.a;

			for (j = 0, m_out.a = pfc->basic.out.a; j < pcv->conv_kernal_num; j++, m_out.a += m_out.length, db++)
			{

				//����db
				for (k = 0, delt_bias = _ParaFint(0), dbm = m_out.a; k < m_out.length; k++, dbm++)
				{
					delt_bias += *dbm;
				}
				*db += delt_bias;

				for (k = 0, m_in.a = pfc->basic.in.a; k < pcv->conv_kernal_z; k++, m_in.a += m_in.length)
				{
					pMiaoToolMatrixConv2(
						&m_dw, &m_out, &m_in,
						pcv->move_delt, pcv->move_delt,
						0,
						MiaoToolMatrixConvMethod_Valid,
						0,
						NULL
					);
					m_dw.a += m_dw.length;
				}
			}
		}
	}



	//���������
	i = mini_b_num;
	m_dw.a = pfc->Weight.a;
	m_dw.length *= pcv->conv_kernal_z;
	m_out.a = pfc->basic.out.a;
	m_in.a = pfc->basic.in.a;
	m_in.length *= pcv->conv_kernal_z;
	while (i--)
	{
		pMiaoToolMatrixConvFastMultCh(
			&m_in, &m_dw, &m_out, NULL,
			pcv->conv_kernal_num, pcv->conv_kernal_z,
			pcv->move_delt, pcv->move_delt,
			pcv->ConvMethod,
			0,
			pfc->basic.FlowData.pData
		);

		m_in.a += m_in.length;
		m_out.a += m_out.length;
	}


#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
	//����ǵ�һ�η��򴫲� ��Ҫ����ԭ�ȵ��ݶ�
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a;
		dw = pfc->DeltW.a;
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//�ȼ���delt
			delt_bias = _MiaoToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//�����ǵ�һ�η��򴫲���Ҫ���и���
			k = move_d;
			while (k--)
			{
				*dw++ = *dw2++;
			}
			*db++ = delt_bias;

		}

		i++;
	}

	for (; i < mini_b_num; i++)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a + i;
		dw = pfc->DeltW.a + i;
		//ѭ��ÿһ�������
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//�ȼ���delt
			delt_bias = _MiaoToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//���ܸ��� ���
			k = move_d;
			while (k--)
			{
				*dw += *dw2++;
				dw++;
			}
			*db += delt_bias;
			db++;
		}
	}

	//���������
	_MiaoToolNNKernalConvBK(
		&(pfc->basic.in),
		&(pfc->basic.out),
		&(pfc->Weight),
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		mini_b_num,
		1,
		pcv->databasic.basic.FlowData.pData
	);
#else

#error"NN data frame is unknown"
	MIAO_assert(1);

#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C





	return mini_b_num;
}



MiaoToolNNSubLayerBasic_t * MiaoToolNNLayerPoolNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	MiaoToolNNPoolingMethodNUM pool_method,
	lw_u16 max_mini_batch,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
)
{
	MiaoToolNNLayerPool_t *node;
	uint32_t max_data_len;
	uint32_t out_x, out_y;
	LwMatRowType in_row, out_row;

	out_x = in_x / pool_x;
	out_y = in_y / pool_y;

	in_row = in_x*in_y*in_z;;
	out_row = out_x*out_y*in_z;

	if (pool_method == 'M')
	{
		node = pmem->Malloc(pmem, sizeof(MiaoToolNNLayerPool_t) + sizeof(lw_ptr)*out_row);
		node->MaxPool = (void *)(((lw_u8 *)(node)) + sizeof(MiaoToolNNLayerPool_t));
	}
	else if (pool_method == 'A')
	{
		node = pmem->Malloc(pmem, sizeof(MiaoToolNNLayerPool_t));
	}
	else
	{
		MIAO_assert(1);
	}

	node->basic.NN_Kind = MiaoToolNNSubLayerKind_Pool;

	node->in_x = in_x;
	node->in_y = in_y;
	node->move_x = pool_x;
	node->move_y = pool_y;
	node->out_x = out_x;
	node->out_y = out_y;
	node->pic_z = in_z;
	node->PoolMethod = pool_method;

	//��������
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	MiaoToolMatrixInitial(&(node->basic.in), in_row, max_mini_batch, pin);

	//�������
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}

	MiaoToolMatrixInitial(&(node->basic.out), out_row, max_mini_batch, pout);


	//��Ա���� ֻ��ǰ�򴫵ݲ��ò��ú�����������Ĭ��
	node->basic.Forward = MiaoToolBPnnLayerPoolForward;
	node->basic.Backward = MiaoToolBPnnLayerPoolBackward;

	node->basic.Update = MiaoToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = MiaoToolBPnnLayerNNmallocDefault;
	node->basic.Clear = MiaoToolBPnnLayerClearDefault;
	node->basic.DataInit = MiaoToolBPnnLayerDataInitDefault;
	node->basic.Copy = MiaoToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = MiaoToolBPnnLayerSoftReplaceDefault;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = in_z;
		info[3] = out_row;
	}

	return (MiaoToolNNSubLayerBasic_t *)node;
}





#if MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C

void* _MiaoToolNNMaxPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	ParaType *in_a = in->a, *out_a = out->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	uint32_t in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_max, *pMax = NULL;
	lw_32  jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//��ͼ��ѭ��
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < out_x; jx++, in_r += in_r_move)
			{

				//Сͼ��ѭ��
				for (k = 0, in_p_c = in_r, pMax = in_r, now_max = *in_r; k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						if (*in_p_r > now_max)
						{
							pMax = in_p_r;
							now_max = *in_p_r;
						}
					}
				}
				*data_mem++ = pMax;
				*out_a++ = now_max;
			}
		}


		in_a += in_pic_2d_move;
	}
	return (void *)data_mem;
}

MiaoMat* _MiaoToolNNAvgPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = in->a, *out_a = out->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	uint32_t in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//��ͼ��ѭ��
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < out_x; jx++, in_r += in_r_move)
			{

				//Сͼ��ѭ��
				for (k = 0, in_p_c = in_r, now_sum = _ParaFint(0); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						now_sum += *in_p_r;
					}
				}

				*out_a++ = _ParaMul(data_div, now_sum);
			}
		}


		in_a += in_pic_2d_move;
	}
	return (void *)out;
}



MiaoMat* _MiaoToolNNMaxPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	uint32_t i = now_min_batch*outerr_x*outerr_y*inerr_z;
	ParaType *out_a = outerr->a;

	LW_MAT_CLR(inerr);
	while (i--)
	{
		**data_mem = *out_a++;
		data_mem++;
	}

	return inerr;
}


MiaoMat* _MiaoToolNNAvgPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = inerr->a, *out_a = outerr->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	uint32_t in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = inerr_x*inerr_y;
	in_p_c_move = inerr_x;
	in_r_move = move_x;
	in_c_move = inerr_x*move_y;

	now_min_batch *= inerr_z;
	while (now_min_batch--)
	{

		//��ͼ��ѭ��
		for (jy = 0, in_c = in_a; jy < outerr_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < outerr_x; jx++, in_r += in_r_move, out_a++)
			{

				//Сͼ��ѭ��
				for (k = 0, in_p_c = in_r, now_sum = _ParaMul(*out_a, data_div); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						*in_p_r = now_sum;
					}
				}
			}
		}


		in_a += in_pic_2d_move;
	}



	return inerr;
}

#elif MIAO_TOOL_NN_DATA_FRAME == MIAO_TOOL_NN_D_FRAME_F
void* _MiaoToolNNMaxPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	uint32_t m_x, m_y;
	uint32_t i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType maxdata;
	ParaType *pMaxIndex;

#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	p_in_head = in->a;
	p_out_head = out->a;
	//ѭ��ÿһ��minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//ѭ��ÿһ��ͼ��
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//ѭ��Сͼ��
					p_in_move_row = p_in_pix;
					m = move_y;
					maxdata = *p_in_move_row;
					pMaxIndex = p_in_move_row;
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							if (maxdata < *p_in_move_pix)
							{
								maxdata = *p_in_move_pix;
								pMaxIndex = p_in_move_pix;
							}

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//�������
					*data_mem++ = pMaxIndex;
					*p_out_pix = maxdata;


					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return (void *)data_mem;
}

MiaoMat* _MiaoToolNNAvgPoolingFD
(
	MiaoMat *in, MiaoMat *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;

	uint32_t m_x, m_y;
	uint32_t i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType sumdata, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = in->a;
	p_out_head = out->a;
	//ѭ��ÿһ��minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//ѭ��ÿһ��ͼ��
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//ѭ��Сͼ��
					p_in_move_row = p_in_pix;
					m = move_y;
					sumdata = _ParaFint(0);
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{

							sumdata += *p_in_move_pix;

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//�������

					*p_out_pix = _ParaMul(sumdata, NumData);

					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return out;
}



MiaoMat* _MiaoToolNNMaxPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	LwMatColType _minb;
	ParaType *p_out_head, *p_out_pix;
	LwMatRowType i;

#define _NEXT_PIX(P) (P+=_minb)

	_minb = inerr->col;

	LW_MAT_CLR(inerr);

	p_out_head = outerr->a;
	while (now_min_batch--)
	{
		p_out_pix = p_out_head;
		i = outerr->row;

		while (i--)
		{
			**data_mem = *p_out_pix;
			++data_mem;
			_NEXT_PIX(p_out_pix);
		}

		p_out_head++;
	}


#undef _NEXT_PIX(P)

	return inerr;
}


MiaoMat* _MiaoToolNNAvgPoolingBK
(
	MiaoMat *inerr, MiaoMat *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	uint32_t m_x, m_y;
	uint32_t i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType data, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = inerr->col;;
	_minb_mpy_in_x = _minb * inerr_x;
	_minb_mpy_in_len = _minb_mpy_in_x * inerr_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = inerr->a;
	p_out_head = outerr->a;
	//ѭ��ÿһ��minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//ѭ��ÿһ��ͼ��
		while (inerr_z--)
		{
			i = outerr_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = outerr_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//ѭ��Сͼ��
					p_in_move_row = p_in_pix;
					m = move_y;
					data = _ParaMul(*p_out_pix, NumData);//�õ�ƽ��ֵ
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							*p_in_move_pix = data;//��ֵ
							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}
					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}
				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return inerr;
}
#else
#error"NN data frame is unknown"
#endif // MIAO_TOOL_NN_DATA_FRAME==MIAO_TOOL_NN_D_FRAME_C


lw_ptr MiaoToolBPnnLayerPoolForward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	MiaoToolNNLayerPool_t *pPL;
	MIAO_assert(!(pNNSLB));
	pPL = (MiaoToolNNLayerPool_t *)pNNSLB;


	if (pPL->PoolMethod == MiaoToolNNPoolingMethod_Max)
	{
		_MiaoToolNNMaxPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == MiaoToolNNPoolingMethod_Avg)
	{
		_MiaoToolNNAvgPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		MIAO_assert(1);
	}


	return mini_b_num;
}

lw_ptr MiaoToolBPnnLayerPoolBackward(struct MIAO_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	MiaoToolNNLayerPool_t *pPL;
	MIAO_assert(!(pNNSLB));
	pPL = (MiaoToolNNLayerPool_t *)pNNSLB;


	if (pPL->PoolMethod == MiaoToolNNPoolingMethod_Max)
	{
		_MiaoToolNNMaxPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == MiaoToolNNPoolingMethod_Avg)
	{
		_MiaoToolNNAvgPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		MIAO_assert(1);
	}


	return mini_b_num;
}


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
)
{

	MiaoToolNNSubLayerBasic_t *pnode1, *pnode2;
	MiaoToolNNLayerConv_t *pcv;
	MiaoToolNNLayerFullCon_t *pfc;
	MiaoToolDListNode_t *pln1, *pln2;
	ParaType *pWreal, *pBreal;
	MiaoToolNNSubLayerBasic_t **ppLIST1, **ppLIST2, **pptail;
	LwMatRowType in_col, mat_out_col;
	uint32_t out_x, out_y;
	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pmem));

	in_col = in_x*in_y*in_z;
	out_x = (in_x - kern_x) + 1;//�����ƶ�����
	out_y = (in_y - kern_y) + 1;//�����ƶ�����
	mat_out_col = out_x*out_y*kern_num;

	if (pTemplet)
	{

		_NN_GET_IN(pBPnn, pin);

		pnode1 = MiaoToolNNLayerConvNew(
			pin,
			NULL,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			conv_method,
			pBPnn->Train.mini_batch_max,
			pmem,
			info
		);

		pnode2 = MiaoToolNNLayerActFunNew(
			pnode1->out.a, pout,
			mat_out_col, pBPnn->Train.mini_batch_max,
			pmem, pTemplet, 0
		);

		pln1 = pmem->Calloc(pmem, sizeof(MiaoToolDListNode_t));
		ppLIST1 = pmem->Malloc(pmem, sizeof(MiaoToolNNSubLayerBasic_t *) * 2);


		ppLIST1[0] = pnode1;
		ppLIST1[1] = pnode2;

		MiaoToolBPnnLayerAppend(pBPnn, pln1, 2, ppLIST1);

		//ÿ�ζ�Ҫ���������
		memcpy(&(pBPnn->y), &(pnode2->out), sizeof(MiaoMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}
	else//���ò���������ķ�ʽ
	{

		_NN_GET_IN(pBPnn, pin);

		pnode1 = MiaoToolNNLayerConvNew(
			pin,
			pout,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			conv_method,
			pBPnn->Train.mini_batch_max,
			pmem,
			info
		);

		pln1 = pmem->Calloc(pmem, sizeof(MiaoToolDListNode_t));


		ppLIST1 = pmem->Malloc(pmem, sizeof(MiaoToolNNSubLayerBasic_t *) * 1);
		ppLIST1[0] = pnode1;

		MiaoToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

		//ÿ�ζ�Ҫ���������
		memcpy(&(pBPnn->y), &(pnode1->out), sizeof(MiaoMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}


	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	_NN_FULL_CON_CHIP_ALLAC_1(pBPnn, pfc);

	//pBPnn->ParaGroupNum++;

	return ppLIST1;
}


void* MiaoToolBPnnPoolAppend
(
	MiaoToolBPnn_t *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	MiaoToolNNPoolingMethodNUM pool_method,
	MiaoMemoryBasic_t *pmem,
	uint32_t info[4]
)
{

	MiaoToolNNSubLayerBasic_t *pnode1;
	MiaoToolNNLayerPool_t *pPL;
	MiaoToolDListNode_t *pln1;
	MiaoToolNNSubLayerBasic_t **ppLIST1, **pptail;
	LwMatColType in_col;


	MIAO_assert(!(pBPnn));
	MIAO_assert(!(pmem));

	in_col = in_x*in_y*in_z;

	_NN_GET_IN(pBPnn, pin);

	pnode1 = MiaoToolNNLayerPoolNew(
		pin,
		pout,
		in_x, in_y, in_z,
		pool_x, pool_x,
		pool_method,
		pBPnn->Train.mini_batch_max,
		pmem,
		info
	);

	pln1 = pmem->Calloc(pmem, sizeof(MiaoToolDListNode_t));
	ppLIST1 = pmem->Malloc(pmem, sizeof(MiaoToolNNSubLayerBasic_t *) * 1);

	ppLIST1[0] = pnode1;

	MiaoToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//ÿ�ζ�Ҫ���������
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(MiaoMat));

	return ppLIST1;
}


#endif // MIAO_TOOL_IMPORT_NN_BPnn || MIAO_TOOL_IMPORT_ALL

#endif // MIAO_TOOL_IMPORT_NN || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 2)

