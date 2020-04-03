/*(Ver.=0.96)
 * MIAO_timer.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"

#if MIAO_OS_ENABLE

void MiaoTimerClockCallBack(MiaoTimer_t *pt)
{
    if(pt->TimerEnableFlag)
    {

        if(!(-- pt->Count) && pt->AlarmCBFun)
        {
            switch(pt->Mode)
            {
            case MiaoTimerMode_Once:
                pt->TimerEnableFlag=0;
                break;

            case MiaoTimerMode_Continue:
                pt->Count=pt->CountMax;
                break;

            default:

                break;
            }
            pt->AlarmCBFun(pt);
        }

    }

    return ;

}

#endif

