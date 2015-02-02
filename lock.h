/*
 * lock.h
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

#ifndef LOCK_H_
#define LOCK_H_

#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef WIN32
#define LOCK_HANDLE		HANDLE
#else
#define LOCK_HANDLE		pthread_mutex_t *
#endif

LOCK_HANDLE create_lock();

void destory_lock(LOCK_HANDLE);

void lock(LOCK_HANDLE handle);

void unlock(LOCK_HANDLE handle);

//bool try_lock(LOCK_HANDLE handle);


#endif /* LOCK_H_ */
