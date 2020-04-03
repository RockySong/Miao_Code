/*(Ver.=0.96)
 * MIAO_driver.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"
//#include "usart.h"
//#include "OLED.h"
//#include "led.h"
/*(Ver.=0.96)
void MiaoCommunTransmit(MiaoCommun_t *pc)
{

	int i=0;
	while((pc->TxLength)--)
	{
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
		USART1->DR = (u8) (pc->TxList[i]);
		i++;
	}

	pc->pTxHead = pc->pTxTail = pc->TxList;
	pc->TxLength=0;


}
*/



#if MIAO_OS_ENABLE
#if MIAO_TASK_SHOW_ENABLE
void MiaoDriverShow_Clr(MiaoShow_t *pS,lw_u8 cmd,void *pbuf)
{

    OLED_CLS();

}

void MiaoDriverShowDrawParaDate(MiaoShowPvtParaCtrl_t *psppc,lw_u8 pos,lw_u8 len,MiaoParameter_t *pp,void *pbuf)
{
    /*(Ver.=0.96)
    lw_u8 str[20];
    if(pbuf==NULL)
    	if(pp->pExtParameter)
    		sprintf(str,"%f",_ParaToF(*(pp->pExtParameter)));
    	else
    	sprintf(str,"%f",_ParaToF(pp->Parameter));
    else
    	sprintf(str,"%f",_ParaToF(pp->ParameterShallow));
    OLED_P6x8Str(32,(pos+1)*6/(len),str);
    */
}


void MiaoDriverShowDrawParaId(MiaoShowPvtParaCtrl_t *psppc,lw_u8 pos,lw_u8 len,MiaoParameter_t *pp)
{

    //printf("%d_%d %d:\n",pos,len,pp->ParaId);
    lw_u8 str[20];
    sprintf(str,"%d:",pp->ParaId);
    OLED_P6x8Str(15,(pos+1)*6/(len),str);


}

void MiaoDriverShowDrawCursor(MiaoShowParaCtrl_t *pSPC,MiaoShowCursor_t *pSC)
{

//	printf("X%d\n",pSC->Position_forScreenLast);
//	pSC->Position_forScreenLast=pSC->Position_forScreen;
//	printf("V%d\n",pSC->Position_forScreen);
    OLED_CLS8x12(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength));
    pSC->Position_forScreenLast=pSC->Position_forScreen;
    if(pSC->Status == MiaoShowCursorStatus_Free)
        OLED_P6x8(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength),">");
    else
        OLED_P6x8(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength),":");
}


void MiaoDriverShowDrawPageNum(MiaoShow_t *pS)
{

//	printf("This is page %d\n",pS->Cursor.Position_forPage);

    lw_u8 str[20];
    sprintf(str,"PAGE: %d",pS->Cursor.Position_forPage);
    OLED_P6x8Str(0,0,str);

}

void OLED_CLS_win(void)
{
    int y,x;
    for(y=1; y<8; y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x=0; x<X_WIDTH; x++)
            OLED_WrDat(0,0);
    }
}

MIAO_DRIVER_SHOW_EXT_FUN_DEFAULT(MiaoDriverShowDATA1)
{
    lw_u16 i=pSEPD->mem.Data/sizeof(lw_u8);
    lw_u8 *p=pSEPD->mem.pData;
    OLED_CLS_win();
    while(i--)
    {
        OLED_PutPixel(i+20,p[i]);
    }
}

MIAO_DRIVER_SHOW_EXT_FUN_DEFAULT(MiaoDriverShowDATA2)
{
    auto lw_u16 i=pSEPD->mem.Data/sizeof(lw_u8)+20;

    lw_u8 *p=pSEPD->mem.pData;

    OLED_CLS_win();
    OLED_Rectangle(p[0],p[1],p[2],p[3],0);

}


MIAO_DRIVER_SHOW_EXT_FUN_DEFAULT(MiaoDriverShowEXTid)
{



}

void MiaoDriverShowDrawCursorExt(MiaoShowParaCtrl_t *pSPC,MiaoShowCursor_t *pSC)
{

}


void MiaoTimerShowUpdateALFun(MiaoTimer_t *pt)
{


    MiaoInlineTaskShow.TaskGroupFlag.CurrentStatus.bits.bit0=1;


    return;
}

void MiaoTimerShowIrpALFun(MiaoTimer_t *pt)
{



}
#endif
#endif
