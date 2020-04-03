/*(Ver.=0.96)
 * MIAO_memory.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

lw_32 MiaoMemorySizeInitial(MiaoMemSize_t *ps, lw_u16 len)
{
#if MIAO_MEM_ALLOC_FAST
	int i = 15;
	len = len < sizeof(int) ? sizeof(int) : len;
	do
	{
		if (len & ((lw_u16)1 << i ))
		{
			break;
		}

	} while (--i);

	
	if (!_MIAO_IS_2_POW(len))//����2^n Ҫ��+1
	{
		i++;
	}
	i = i > 15 ? 15 : i;//�޷�

	ps->MovNum = i;
	ps->ModNum = (((uint32_t)1) << (i)) - 1;
	ps->SectionSize = MIAO_MEM_SIZE_MUL(1, *ps);
#else
    ps->SectionSize = len;
		return 0;
#endif // MIAO_MEM_ALLOC_FAST

}

void * MiaoMemoryGlobalCalloc(struct MIAO_MEMORY_BASIC_STRUCT *pMU, uint32_t dsize)
{

	void *p;
	MIAO_assert(!(pMU));
	p = pMU->Malloc(pMU, dsize);

	MIAO_assert(!(p));

	memset(p, 0, dsize);

	return p;
}

#if MIAO_MEMORY_IMPORT_MAP

lw_32 MiaoMemoryMapInital
(
    MiaoMemoryMap_t *pMU,//�洢���ṹ��
    void *pMem,//�ڴ�ص�ַ �ܳ���=lenMem*(MemSize+sizeof(void *))
    lw_u16 lenMem,//�ڴ�Ƭ����
    lw_u16 MemSize,//�ڴ�Ƭ��С(sizeof)
    uint32_t sizeofmem
)
{
	void *pMemInfo = pMem;
    uint32_t reallen;
    lw_u16 lenMemInfo;

    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));
    MIAO_assert(!(MemSize));
    MIAO_assert(!(sizeofmem));


    lenMemInfo=lenMem;
	MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	if (reallen > sizeofmem)//���»���Ƭ��С
	{
		//MIAO_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = sizeofmem / (pMU->basic.MemSectionSize.SectionSize + sizeof(pMU->Mem.pData));
		reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	}


	pMU->Mem.pData = (void *)((size_t)pMem + (lenMem * sizeof(pMU->Mem.pData)));
    pMU->Mem.uData=lenMem;//Ϊ�˱�̷���û��ʹ���泤��
    pMU->MemInfo.pData=pMemInfo;
    pMU->MemInfo.uData=lenMemInfo;//Ϊ�˱�̷���û��ʹ���泤��
	pMU->pMemInfoButtom = (void *)((size_t)pMU->MemInfo.pData + pMU->MemInfo.uData * sizeof(pMU->Mem.pData));


    pMU->basic.Malloc=(void *)MiaoMemoryMapAlloc;
    pMU->basic.Free=(void *)MiaoMemoryMapFree;
    pMU->basic.ReAlloc = (void *)MiaoMemoryMapReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;
    pMU->basic.memMax = lenMem;//�ڴ�Ƭ�ܴ�С
    pMU->basic.memSurplus = pMU->basic.memMax;//ʣ���С

	pMU->pMemButtom = (void *)((size_t)pMem + reallen);

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;

}

void* MiaoMemoryMapAlloc(MiaoMemoryMap_t *pMU,uint32_t dsize)
{

    size_t parr,phead= (size_t)NULL;
    void **pinfoarr;
    uint32_t len;
    lw_u16 count=0;


    //lw_u16 *pMemButtom=(lw_u16 *)pMU->Mem.pData+pMU->Mem.Data*pMU->MemMapSize,*pMemInfoButtom=(lw_u16 *)pMU->MemInfo.pData+pMU->MemInfo.Data*sizeof(pMU->Mem.pData);

    MIAO_assert(!(pMU));


    parr = (size_t)(pMU->Mem.pData);
    pinfoarr = pMU->MemInfo.pData;
    if (dsize == 0)
    {
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);

    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.96)
    if(len * pMU->MemSectionSize < dsize)
    	len++;
    	*/

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    if (pMU->basic.memSurplus < len)//û���㹻���ڴ� ��������
    {
        return NULL;
    }

    do {
        //������ڴ�����ʹ��
        if((*pinfoarr)==NULL)
        {
            count++;//������++
            if(phead == (size_t)(NULL))//���֮ǰû���ҵ���
                phead=parr;//�������ڴ�Ƭͷ��
        }
        else//����Ѿ�����ʹ��
        {
            count=0;//����������
            phead= (size_t)NULL;//ͷ���ͷ�
        }
        if(count==len)//����ҵ����㹻�������ڴ�Ƭ
        {
            while (count--) *pinfoarr-- = (void *)(phead);//���ڴ�Ƭ����
            count=len;//�������ָ�����
            break;
        }


    } while((parr+=pMU->basic.MemSectionSize.SectionSize) < (size_t)(pMU->pMemButtom) && (size_t)(pinfoarr++) < (size_t)(pMU->pMemInfoButtom));

    if(count == len)//���������� == Ҫ�󳤶�
    {
        pMU->basic.memSurplus -= len;
        return (void *)phead;
    }
    else
    {
        return NULL;
    }

    //return NULL;

}

//------------------------------------------
//<������>_MiaoMemoryMapFind</������>
//<����˵��>ӳ�����ڴ���� ���Һ���</����˵��>
//<����˵��>pmu �ڴ������ p:������ڴ� pLen(��ΪNULL):��ǰָ����ڴ� pNum(��ΪNULL):��ǰָ����ڴ�+֮��Ŀ����ڴ�Ƭ���� </����˵��>
//<���˵��>lw_32 �ڴ�Ƭ��Ӧ������ -1��ʾû�и��ڴ�</���˵��>
//------------------------------------------
static lw_32 _MiaoMemoryMapFind(MiaoMemoryMap_t *pMU, void *p,lw_u16 *pLen,lw_u16 *pNum)
{
    lw_u16 count = 0, len = 0;
    lw_u16 headernum;
    void **pinfoarr;

    MIAO_assert(!(pMU));
    MIAO_assert(!(p));

    //���ڷ�Χ֮�� �Ҳ���
    if
    (
        p < pMU->Mem.pData
        ||
        p > pMU->pMemButtom
    )
    {
        return (lw_32)-1;
    }

    //headernum = (size_t)((size_t)p - (size_t)(pMU->Mem.pData))/((size_t)(pMU->MemSectionSize));//����ƫ��
	headernum = (lw_u16)(MIAO_MEM_SIZE_DIV((size_t)((size_t)p - (size_t)(pMU->Mem.pData)), (pMU->basic.MemSectionSize))); //����ƫ��
    if (pNum && pLen)
    {
        pinfoarr = pMU->MemInfo.pData;
        pinfoarr += headernum;
        while ((*pinfoarr == p) || (*pinfoarr==NULL) && (pinfoarr != pMU->pMemInfoButtom))
        {
            if (*pinfoarr == p)
            {
                len++;
            }
            count++;
            pinfoarr++;
        }

        *pNum = count;
        *pLen = len;
    }
    return (lw_32)headernum;

}

void * MiaoMemoryMapFree(MiaoMemoryMap_t *pMU,void *p)
{
    /*(Ver.=0.96)
    	lw_u8 **pinfoarr=pMU->MemInfo.pData;
    	lw_u8 **pMemInfoButtom;
    	lw_u8 FindFlag = 0;
    	uint32_t count = 0;

    	//���ڷ�Χ֮�� �������ͷ�
    	if
    	(
    		p < pMU->Mem.pData
    		||
    		p > pMU->pMemButtom
    	)
    	{
    		return NULL;
    	}

    	do{
    		//������ڴ��ʹ������p
    		if(*pinfoarr == p)
    		{
    			*pinfoarr = NULL;//�ָ�Ϊ��
    			count++;
    			FindFlag = 1;//��Ѱ����־λΪ1
    		}
    		else if(FindFlag)//���û���ҵ��ұ�־λΪ1 ˵���������
    		{
    			break;
    		}

    	}while((pinfoarr++) < (lw_u8 **)(pMU->pMemInfoButtom));

    	pMU->basic.memSurplus += count;
    	*/

    lw_32 findres;
    uint32_t count = 0;
    void **pinfoarr;

    MIAO_assert(!(pMU));


    findres = _MiaoMemoryMapFind(pMU, p, NULL,NULL);
    if (findres < 0)//û���ҵ�
    {
        return NULL;
    }
    pinfoarr = ((void **)pMU->MemInfo.pData)+ findres;

    while (*pinfoarr == p)
    {
        *pinfoarr = NULL;//�ָ�Ϊ��
        count++;
        pinfoarr++;
    }
    pMU->basic.memSurplus += count;


    return p;

}

void* MiaoMemoryMapReAlloc(MiaoMemoryMap_t *pMU,void *p, uint32_t dsize)
{
    lw_u16 count = 0, len, nowlen;
    lw_32 index;
    void **pinfoarr;
    void *pAlloc;

    MIAO_assert(!(pMU));


    if (p==NULL)//ֱ�ӷ����ڴ�
    {
        return MiaoMemoryMapAlloc(pMU, dsize);
    }

    if (dsize == 0)//ֱ���ͷ��ڴ�
    {
        return MiaoMemoryMapFree(pMU, p);
    }

    index = _MiaoMemoryMapFind(pMU, p, &nowlen, &count);
    if (index < 0 || count ==0)
    {
        MIAO_assert(1);
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.96)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;
    if (count >= len)//����Ҫ���·���
    {

        if (nowlen > len)//��ǰ���ȴ���Ҫ�󳤶� ��Ҫ�ͷŶ��೤��
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + nowlen - 1;//β��
            count = nowlen - len;
            pMU->basic.memSurplus += count;
            do
            {
                *pinfoarr-- = NULL;
            } while (--count);

        }
        else if (nowlen < len)//��ǰ����С��Ҫ�󳤶� ���볤��
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + len - 1;//β��
            count = len - nowlen;
            pMU->basic.memSurplus -= count;
            do
            {
                *pinfoarr-- = p;
            } while (--count);


        }
        else;//����Ҫ����
        return p;
    }
    else//���·���
    {
        pAlloc = MiaoMemoryMapAlloc(pMU, dsize);//����
        memcpy(pAlloc, p, MIAO_MEM_SIZE_MUL(nowlen, pMU->basic.MemSectionSize));//�����ڴ�����

        //�ͷ����� ���õ���free ��Լʱ��
        pinfoarr = ((void **)pMU->MemInfo.pData) + index;

        count = 0;
        while (*pinfoarr == p)
        {
            *pinfoarr = NULL;//�ָ�Ϊ��
            count++;
            pinfoarr++;
        }
        pMU->basic.memSurplus += count;

        return pAlloc;
    }

   // MIAO_assert(1);

   // return NULL;

}

#endif //MIAO_MEMORY_IMPORT_MAP



#if MIAO_MEMORY_IMPORT_SIMPLE

lw_32 MiaoMemorySimpleInital(MiaoMemorySimple_t *pMU,
                             void *pMem,
                             lw_u16 lenMem,
                             lw_u16 MemSize,
                             uint32_t sizeofmem
                            )
{
    uint32_t reallen;
    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));


    pMU->basic.Malloc = (void *)MiaoMemorySimpleAlloc;
    pMU->basic.Free = (void *)MiaoMemorySimpleFree;
    pMU->basic.ReAlloc = (void *)MiaoMemorySimpleReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;

    //pMU->MemSectionSize = MemSize;
    MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MemSize*lenMem;

	if (reallen > sizeofmem)
	{
		MIAO_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = sizeofmem / pMU->basic.MemSectionSize.SectionSize;
		reallen = MemSize*lenMem;
	}



    pMU->basic.memSurplus = pMU->basic.memMax = lenMem;

    pMU->Mem.pData = pMem;
    pMU->pMemNow=(size_t)pMem;
    pMU->Mem.uData = lenMem;


	pMU->pMemButtom = (size_t)pMem + reallen;

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;
}


void * MiaoMemorySimpleAlloc(MiaoMemorySimple_t *pMU, uint32_t dsize)
{
    uint32_t len, offsetlen = 0;
    void *pbuf = NULL;
    MIAO_assert(!(pMU));

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.96)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */
    //offsetlen = len* pMU->MemSectionSize;//�õ��ڴ�ʵ��ƫ����

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    offsetlen = MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize);//�õ��ڴ�ʵ��ƫ����

    if (pMU->pMemNow + offsetlen < pMU->pMemButtom)
    {
        pbuf = (void *)(pMU->pMemNow);
        pMU->pMemNow += offsetlen;
        pMU->basic.memSurplus -= len;
        return pbuf;

    }
    else
    {
        return NULL;
    }

}


void * MiaoMemorySimpleFree(MiaoMemorySimple_t *pMU, void *p)
{
    MIAO_assert(1);
    //���ڴ���䲻Ӧ���ͷ� ��Ҫ�����û�

    return NULL;

}

void* MiaoMemorySimpleReAlloc(MiaoMemorySimple_t *pMU, void *p, uint32_t dsize)
{
    MIAO_assert(1);
    //���ڴ���䲻Ӧ�����·��� ��Ҫ�����û�

    return NULL;
}

#endif //MIAO_MEMORY_IMPORT_SIMPLE



#if MIAO_MEMORY_IMPORT_LIST

lw_32 MiaoMemoryListInital(MiaoMemoryList_t *pMU,
                           void *pMem,
                           lw_u16 lenMem,
                           lw_u16 MemSize,
                           uint32_t sizeofmem
                          )
{
    uint32_t reallen;
    //lw_32 data_return;
    uint32_t dsize = 0;
//    lw_u16 len;
    //MiaoMemoryListNode_t *ptail;

    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));
    MIAO_assert(!(MemSize));
    MIAO_assert(!(sizeofmem));


    pMU->basic.Malloc = (void *)MiaoMemoryListAlloc;
    pMU->basic.Free = (void *)MiaoMemoryListFree;
    pMU->basic.ReAlloc = (void *)MiaoMemoryListReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;
    //pMU->MemSectionSize = MemSize;

    MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MIAO_MEM_SIZE_MUL (lenMem, pMU->basic.MemSectionSize)+  sizeof(MiaoMemoryListNode_t);
	if (reallen > sizeofmem)
	{
		//MIAO_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = (sizeofmem - sizeof(MiaoMemoryListNode_t)) / pMU->basic.MemSectionSize.SectionSize;
		reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + sizeof(MiaoMemoryListNode_t);
	}


    pMU->Mem.uData = lenMem;
    pMU->Mem.pData = pMem;
	pMU->basic.memSurplus = pMU->basic.memMax = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize);
	pMU->pMemButtom = (void *)((size_t)pMem + reallen);
	memset(pMem, 0, reallen);
	pMU->basic.memSurplus = pMU->basic.memMax = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize);


    pMU->ptail = (MiaoMemoryListNode_t *)((size_t)pMU->pMemButtom - sizeof(MiaoMemoryListNode_t));
    pMU->phead = pMem;

    MIAO_TOOL_DLIST_NODE_CONNECT(pMU->phead, pMU->ptail);

    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//�ڴ��С�淶��

    pMU->phead->len = dsize;
    pMU->basic.memSurplus -= dsize;

	return reallen - sizeofmem;

}


void * MiaoMemoryListAlloc(MiaoMemoryList_t *pMU, uint32_t dsize)
{

    MiaoMemoryListNode_t *p, *q;
    void *preturn = NULL;
		register size_t buf1;

    MIAO_assert(!(pMU));
    MIAO_assert(!(dsize));
    /*(Ver.=0.96)
    MemSectionSize = pMU->MemSectionSize;
    dsize += sizeof(MiaoMemoryListNode_t);
    len = dsize / MemSectionSize;

    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    if ((uint32_t)len * MemSectionSize < dsize)
    	len++;

    dsize = (uint32_t)len *MemSectionSize;
    */

    if (dsize == 0)
    {
        return NULL;
    }


    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//�ڴ��С�淶��

    if (pMU->basic.memSurplus < dsize)//û���㹻���ڴ� ��������
    {
        return NULL;
    }

    p = pMU->phead;
    do
    {

        if (p < pMU->phead || p>pMU->ptail )//pԽ������쳣
        {
            MIAO_assert(1);
            return NULL;
        }
				buf1=((size_t)(p->con.pNext) - (size_t)(p) - p->len);
        if (buf1 >= dsize)//������ʣ��
        {
            q = (MiaoMemoryListNode_t *)(((size_t)p) + p->len);
            /*(Ver.=0.96)
            			q->con.pLast = (MiaoToolDListNodeConnect_t *)p;
            			q->con.pNext = (MiaoToolDListNodeConnect_t *)p->con.pNext;

            			p->con.pNext->pLast = (MiaoToolDListNodeConnect_t *)q;
            			p->con.pNext = (MiaoToolDListNodeConnect_t *)q;
            */
            MIAO_TOOL_DLIST_NODE_INSERT(p, q);

            q->len = dsize;
            preturn = (void *)((size_t)q + sizeof(MiaoMemoryListNode_t));
            pMU->basic.memSurplus -= dsize;

            break;
        }

        p = (MiaoMemoryListNode_t *)(p->con.pNext);

    } while (p->con.pNext);


    return preturn;

}


static lw_8 _MiaoMemoryListJudgeIllegal(MiaoMemoryList_t *pMU, MiaoMemoryListNode_t *pnode)
{

    MIAO_assert(!pMU);
    MIAO_assert(!pnode);

    if (
        ((size_t)(pnode) < (size_t)(pMU->phead) || (size_t)(pnode) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pNext) < (size_t)(pMU->phead) || (size_t)(pnode->con.pNext) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pLast) < (size_t)(pMU->phead) || (size_t)(pnode->con.pLast) > (size_t)(pMU->ptail))
    )//�ж���Χ
    {
        MIAO_assert(1);
        return 1;
    }

    //if (((size_t)pnode - (size_t)(pMU->phead)) % pMU->MemSectionSize)//�ж��Ƿ����
    if (MIAO_MEM_SIZE_MOD(((size_t)pnode - (size_t)(pMU->phead)), pMU->basic.MemSectionSize)) //�ж��Ƿ����
    {
        MIAO_assert(1);
        return 1;
    }

    return 0;

}

void * MiaoMemoryListFree(MiaoMemoryList_t *pMU, void *p)
{
    MiaoMemoryListNode_t *pnode=NULL;
    //lw_u16 len;
    //uint32_t Length;

    MIAO_assert(!(pMU));
    MIAO_assert(!(p));

    pnode = (MiaoMemoryListNode_t *)((size_t)p - sizeof(MiaoMemoryListNode_t));

    if (_MiaoMemoryListJudgeIllegal(pMU,pnode))
    {
        MIAO_assert(1);
        return NULL;
    }

    pMU->basic.memSurplus += pnode->len;

    MIAO_TOOL_DLIST_NODE_DELECT(pnode);

    pnode->con.pLast = NULL;
    pnode->con.pNext = NULL;
    pnode->len = 0;

    return pnode;

}

void* MiaoMemoryListReAlloc(MiaoMemoryList_t *pMU, void *p, uint32_t dsize)
{
    MiaoMemoryListNode_t *pnode = NULL;
    uint32_t _dsize;
    uint32_t reallen;
    void *preturn = NULL;

    MIAO_assert(!(pMU));

    _dsize = dsize;

    if (p == NULL)//ֱ�ӷ����ڴ�
    {
        return MiaoMemoryListAlloc(pMU, _dsize);
    }

    if (dsize == 0)//ֱ���ͷ��ڴ�
    {
        return MiaoMemoryListFree(pMU, p);
    }

    pnode = (MiaoMemoryListNode_t *)((size_t)p - sizeof(MiaoMemoryListNode_t));//ƫ�Ƶõ�����ڵ�

    _dsize = dsize;//����֮ǰ��С

    if (_MiaoMemoryListJudgeIllegal(pMU, pnode))//�ڵ�Ϸ����ж�
    {
        MIAO_assert(1);
        return NULL;
    }

    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//���ȹ淶��

    reallen = (size_t)(pnode->con.pNext) - (size_t)pnode;//�õ���ʵ����

    if (dsize <= reallen)//�ڴ��㹻
    {
        pMU->basic.memSurplus += pnode->len;
        pMU->basic.memSurplus -= dsize;
        pnode->len = dsize;
        preturn = p;
    }
    else
    {
        preturn = MiaoMemoryListAlloc(pMU, _dsize);//��������

        memcpy(preturn, p, pnode->len);//��������

        //�ͷŽڵ�
        pMU->basic.memSurplus += pnode->len;
        MIAO_TOOL_DLIST_NODE_DELECT(pnode);

        pnode->con.pLast = NULL;
        pnode->con.pNext = NULL;
        pnode->len = 0;

    }

    return preturn;

}

#endif //MIAO_MEMORY_IMPORT_LIST