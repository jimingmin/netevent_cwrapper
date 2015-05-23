/*
 * net_timer.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: jimm
 */

#include "net_timer.h"
#include "list.h"
#include "net_ctype.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

EXPORT struct list_head *g_pNetTimerList;

int32_t net_timer_init()
{
	g_pNetTimerList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetTimerList);
	return 0;
}

int32_t net_timer_uninit()
{
	struct list_head *pos, *backup;
	NetTimer *net_timer;

	list_for_each_safe(pos, backup, g_pNetTimerList)
	{
		net_timer = list_entry(pos, NetTimer, list);
		list_del(&net_timer->list);
		free(net_timer);
	}


	return 0;
}

NetTimer *net_create_timer(TimerProc Proc, void *pTimerData, int32_t nCycleTime, int32_t bIsLoop)
{
	NetTimer *net_timer;
	time_t now;
	time(&now);

	net_timer = (NetTimer *)malloc(sizeof(NetTimer));
	net_timer->Proc = Proc;
	net_timer->pTimerData = pTimerData;
	net_timer->nCycleTime = nCycleTime;
	net_timer->bIsLoop = bIsLoop;
	net_timer->nEndTime = (int32_t)(now + nCycleTime);
	net_timer->bCanUse = 1;

	net_insert_timer(net_timer);

	return net_timer;
}

int32_t net_deprecate_timer(NetTimer *net_timer)
{
	net_timer->bCanUse = 0;
	//list_del(&net_timer->list);

	//free(net_timer->pTimerData);
	//free(net_timer);

	return 0;
}

int32_t net_destroy_timer(NetTimer *net_timer)
{
	list_del(&net_timer->list);

	free(net_timer->pTimerData);
	free(net_timer);
	return 0;
}

int32_t net_destroy_timer_by_sessionid(SessionID nSessionID)
{
	NetTimer *find_net_timer = net_find_timer(nSessionID);
	if(find_net_timer != NULL)
	{
		net_deprecate_timer(find_net_timer);
	}
	return 0;
}

int32_t net_insert_timer(NetTimer *net_timer)
{
	struct list_head *pos, *backup;
	NetTimer *find_net_timer;

    int32_t bIsFind = 0;
	list_for_each_safe(pos, backup, g_pNetTimerList)
	{
		find_net_timer = list_entry(pos, NetTimer, list);
		if(find_net_timer->nEndTime >= net_timer->nEndTime)
		{
            bIsFind = 1;
			list_add_tail(&net_timer->list, &find_net_timer->list);
			break;
		}
	}
    
    if(!bIsFind)
    {
        list_add_tail(&net_timer->list, g_pNetTimerList);
    }

	return 0;
}

NetTimer *net_find_timer(SessionID nSessionID)
{
	struct list_head *pos, *backup;
	NetTimer *find_net_timer;
	struct HeartbeatTimerData *pTimerData;

	find_net_timer = NULL;
	list_for_each_safe(pos, backup, g_pNetTimerList)
	{
		find_net_timer = list_entry(pos, NetTimer, list);
		pTimerData = (struct HeartbeatTimerData *)(find_net_timer->pTimerData);
		if(pTimerData->nSessionID == nSessionID)
		{
			break;
		}
	}

	return find_net_timer;
}

int32_t net_loop_timer()
{
	struct list_head *pos, *backup;
	NetTimer *net_timer;
	time_t now;

	if(list_empty(g_pNetTimerList))
	{
		return 0;
	}

	time(&now);

	list_for_each_safe(pos, backup, g_pNetTimerList)
	{
		net_timer = list_entry(pos, NetTimer, list);
        if(now < net_timer->nEndTime)
        {
            continue;
        }
		if(net_timer->bCanUse == 0)
		{
			net_destroy_timer(net_timer);
			continue;
		}
        
        (*net_timer->Proc)(net_timer->pTimerData);

		if(net_timer->bIsLoop != 0)
		{
			net_timer->nEndTime = (int32_t)(net_timer->nCycleTime + now);
		}
		else
		{
			net_destroy_timer(net_timer);
		}
	}

	return 0;
}

