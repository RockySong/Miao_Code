/*(Ver.=0.96)
 * MIAO_gift.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

#if MIAO_OS_ENABLE && MIAO_GIFT_EN

#if !(MIAO_SIMPLE_LEVEL >= 3)
//------------------------------------------
//<������>MiaoGiftConciergeInitial</������>
//<����˵��>��������ϵͳ ��ʼ��</����˵��>
//<����˵��>pgc:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------


void MiaoGiftConciergeInitial(MiaoGiftConcierge_t *pgc)
{

    MIAO_assert(!pgc);
    memset(pgc,0,sizeof(MiaoGiftConcierge_t));//��ʼ������
    MiaoToolDListInitial(&(pgc->giftR.List), 0, NULL);
#if !(MIAO_SIMPLE_MODE)
    pgc->giftR.MoveNextFun=MiaoGiftReceiveMoveNext;//˫�������ƶ���Ա����
    pgc->giftR.SlipNextFun=MiaoGiftReceiveSlipNext;//��������
    pgc->giftR.AppendFun=MiaoGiftReceiveAppend;//׷�ӳ�Ա����
#endif
    pgc->giftR.pGC=pgc;//������ ָ��

    //MiaoGiftTransmitInitial(&(pgc->giftT));

    // pgc->giftT.pGC=pgc;//������ ָ��

}


//�𽥷��� �����ڴ������ƴ���
#if 0
void MiaoGiftTransmitInitial(MiaoGiftTransmit_t *pgt)
{
    int n=0;

    MIAO_assert(!pgt);
#if !(MIAO_SIMPLE_MODE)
    pgt->IdleFun=MiaoGiftTransmitIdle;//�õ����г�Ա��������
#endif

    for(; n<MIAO_GIFT_POSTMAN_MAX; n++) //���Ա��ʼ��
    {
        pgt->PostmanList[n].PostmanID = n;
        pgt->PostmanList[n].Status = GiftPostmanStatus_Free;//����״̬Ϊ����
        pgt->PostmanList[n].pGT = pgt;//���Ա ����ָ���ʼ��
    }

}
#endif


//------------------------------------------
//<������>MiaoGiftReceiveAppend</������>
//<����˵��>���﷢�ͺ���</����˵��>
//<����˵��>pgr:this pgp:���Աָ��</����˵��>
//<���˵��>˫������ͷ��</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveAppend(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t *pgp)
{

    MIAO_assert(!pgr);
    MIAO_assert(!pgp);

#if 0

    if(!(pgr->GiftNum))//�жϵ�ǰ���������Ƿ�==0
    {   //Ϊ0

        pgr->pHead=pgr->pTail=pgp;//��β��ͬ
        pgr->pHead->pPrior=NULL;//��һ��Ϊ��
    }
    else
    {   //��Ϊ0
        pgr->pTail->pNext=pgp;//��ǰβ������һ������
        pgp->pPrior=pgr->pTail;//pgp��ǰһ��Ϊ��ǰβ��
        pgr->pTail=pgp;	//����β��
    }

    pgp->pGT->PostmanFlag.all|=((lw_u64)1<<pgp->ID);//���ͱ�־������

    pgp->Status=GiftPostmanStatus_InDelivery;//���Ա״̬Ϊ����Ͷ��
    pgr->pTail->pNext=NULL;//β����һ��Ϊ��
    pgr->GiftMax++;//�����++
    pgr->GiftNum++;//��������++

    return pgr->pHead;//����ͷ��
#endif // 0

    pgr->List.AppendFUN(&(pgr->List), &(pgp->node));

    //�𽥷��� �����ڴ������ƴ���
#if 0
    if(pgp->pGT)//˵���������ʵ�Ա
    {
        pgp->pGT->PostmanFlag.all |= ((lw_u64)1 << pgp->PostmanID);//���ͱ�־������
        if(pgp->Status!=GiftPostmanStatus_Free)//������Ա ״̬Ϊ���� �ſ��Է���ȴ�����
        {
            //�����Ϊ����
            pgr->pGC->pta->pOS->CoreError=MiaoCoreError_GiftStatusError;//���ں˴����־����
            return NULL;//����NULL
        }

    }
#endif

    pgp->Status = GiftPostmanStatus_InDelivery;//���Ա״̬Ϊ����Ͷ��
    return pgp;

}


//------------------------------------------
//<������>MiaoGiftReceiveMoveNext</������>
//<����˵��>����������ƶ� ���� do{...}while(..move..)ѭ��</����˵��>
//<����˵��>pgr:this</����˵��>
//<���˵��>��ǰͷ��</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveMoveNext(MiaoGiftReceive_t *pgr)
{

    MiaoGiftPostman_t *p,*q;
//    MiaoMemoryBasic_t *pmem;

    MIAO_assert(!pgr);

    if(pgr->List.NowLen<=1)//��������Ѿ�Ϊ0 �������ƶ�
    {
        return NULL;
    }
#if 0
    p=pgr->pHead;//ȡ��ͬ��
    if(pgr->GiftNum==1)//��������Ѿ�==1
    {
        pgr->pHead=NULL;//�ͷ�ͷ��
        pgr->pTail=NULL;//�ͷ�β��
        q=NULL;//����NULL ��Ϊ����do {}while() ѭ��
    }
    else
    {
        pgr->pHead=p->pNext;//ͷ����һ��
        q=pgr->pHead;//���ص�ǰͷ��
    }
    pgr->GiftNum--;//��������--

    if(p->Status == p->Method)//���֮ǰ����״̬ �� ģʽϵͳ
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1<<p->ID);//������ͱ�־
        p->Status=GiftPostmanStatus_Free;//����״̬Ϊ����

    }
#endif // 0


    p = (MiaoGiftPostman_t *)pgr->List.Head.con.pNext;//ȡ�ó���ͷ���ĵ�һ��

    if (!p)
    {
        return NULL;
    }
    //�𽥷��� �����ڴ������ƴ���
#if 0
    if (p->Status == p->Method && (p->pGT))//�������״̬ �� Ŀ��ģʽһ�� �� �ʵ�Ա����
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1 << p->PostmanID);//������ͱ�־
        p->Status = GiftPostmanStatus_Free;//����״̬Ϊ����

    }
#endif

    q = (MiaoGiftPostman_t *)(pgr->List.RemoveFUN(&(pgr->List), MiaoToolDListCmpCount, 1, NULL));


    return q;
}



//------------------------------------------
//<������>MiaoGiftReceiveSlipNext</������>
//<����˵��>����������ƶ� ���� do{...}while(..slip(,&pgp)..)ѭ��</����˵��>
//<����˵��>pgr:this,ppGP:ָ��ָ���ָ��</����˵��>
//<���˵��>�޸�֮���ָ��ֵ</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveSlipNext(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP)
{
    MiaoGiftPostman_t *p;

    MIAO_assert(!pgr);
    MIAO_assert(!ppGP);
    MIAO_assert(!(*ppGP));

    p=(MiaoGiftPostman_t *)((*ppGP)->node.con.pNext);

    /*(Ver.=0.96)
    if(_MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP)))
    {
    	return NULL;
    }
    */



    if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
    {
        _MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP));

        if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//����ͷ�ʧ��
        {

            (*ppGP)=p;
            return p;

        }
    }

    (*ppGP)=p;
    return *ppGP;
}


//------------------------------------------
//<������>MiaoGiftReceiveForeah</������>
//<����˵��>��������/����˵��>
//<����˵��>pgr:this,ppGP:ָ��ָ���ָ��</����˵��>
//<���˵��>��һ������</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveForeah(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP)
{
    MiaoGiftPostman_t *p;

    MIAO_assert(!pgr);
    MIAO_assert(!ppGP);
    MIAO_assert(!(*ppGP));

    p=(MiaoGiftPostman_t *)((*ppGP)->node.con.pNext);
	
		if((*ppGP) != (MiaoGiftPostman_t *)(&(pgr->List.Head)))
		{
			if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
			{
					_MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP));

					if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//����ͷ�ʧ��
					{

							(*ppGP)=p;
							return p;

					}
			}
			 
		}
		(*ppGP)=p;
    return *ppGP;
}

//�𽥷��� �����ڴ������ƴ���
#if 0
//------------------------------------------
//<������>MiaoGiftTransmitIdleIndex</������>
//<����˵��>�õ������ʵ�Ա����</����˵��>
//<����˵��>pgt:this</����˵��>
//<���˵��>�����ʵ�Ա��ID �������-1��ʾû�п���</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftTransmitIdle(MiaoGiftTransmit_t *pgt)
{

    lw_u64 sta=pgt->PostmanFlag.all;

    lw_8 n=0;

    while(((1<<n)&sta) && n<MIAO_GIFT_POSTMAN_MAX) n++;//ѭ���ж��ʵ�Ա ����ж��������� �����ҵ��� ��ֹͣѭ��

    return n<MIAO_GIFT_POSTMAN_MAX? (pgt->PostmanList+n):NULL;

}
#endif


//------------------------------------------
//<������>MiaoGiftAllocPostman</������>
//<����˵��>��������ʵ�Ա</����˵��>
//<����˵��>pmem:�ڴ���亯��</����˵��>
//<���˵��>NUL��ʾû��</���˵��>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftAllocPostman(MiaoMemoryBasic_t *pmem)
{

    MiaoGiftPostman_t *pgp;

    MIAO_assert(!pmem);

    pgp=(MiaoGiftPostman_t *)pmem->Malloc(pmem,sizeof(MiaoGiftPostman_t));

    if(!pgp)
    {
        return pgp;
    }

    pgp->pmem=pmem;
    pgp->Method=GiftPostmanStatus_Received;


    return pgp;

}

#endif // !(MIAO_SIMPLE_LEVEL >= 3)

#if MIAO_SIMPLE_LEVEL ==3

void * MiaoGiftReceiveMoveNext(MiaoToolDListNodeConnect_t *pgr)
{
	MiaoToolDListNodeConnect_t *p;
	MiaoMemoryBasic_t *pmem;
	if(pgr->pLast == NULL || pgr->pNext == NULL)
	{
		return NULL;
	}
	
	p=pgr->pNext;
	pmem=(MiaoMemoryBasic_t *)(p->pLast);
	
	if(pgr->pLast == pgr->pNext)//ֻҪһ����Ϣ
	{
		
		pgr->pLast=NULL;
		pgr->pNext=NULL;
		pmem->Free(pmem,p);
		
		return NULL;
	}
	else
	{
		pgr->pNext=p->pNext;
		pmem->Free(pmem,p);
		return 	pgr->pNext;	
	}
	
	
//	return NULL;
}

#endif


#endif
