/*
 * net_timer.h
 *
 *  Created on: Mar 17, 2015
 *      Author: jimm
 */

#ifndef NET_TIMER_H_
#define NET_TIMER_H_

#include "../common/common_export.h"
#include "../netevent/net_typedef.h"
#include "list.h"

typedef int32_t (*TimerProc)(void *pTimerData);

typedef struct tagNetTimer
{
	TimerProc			Proc;
	void				*pTimerData;
	int32_t				nCycleTime;
	int32_t				bIsLoop;
	int32_t				nEndTime;
	int32_t				bCanUse;
	struct list_head	list;
}NetTimer;

int32_t net_timer_init();

int32_t net_timer_uninit();

NetTimer *net_create_timer(TimerProc Proc, void *pTimerData, int32_t nCycleTime, int32_t bIsLoop);

int32_t net_deprecate_timer(NetTimer *net_timer);

int32_t net_destroy_timer(NetTimer *net_timer);

int32_t net_destroy_timer_by_sessionid(SessionID nSessionID);

int32_t net_insert_timer(NetTimer *net_timer);

NetTimer *net_find_timer(SessionID nSessionID);

int32_t net_loop_timer();


#endif /* NET_TIMER_H_ */
