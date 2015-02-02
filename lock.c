/*
 * lock.c
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

#include "lock.h"
#include <stdlib.h>

LOCK_HANDLE create_lock()
{
#ifdef WIN32
	return CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_t *lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

	pthread_mutexattr_t mutex_attribute;
	pthread_mutexattr_init(&mutex_attribute);
	pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(lock, &mutex_attribute);
	pthread_mutexattr_destroy(&mutex_attribute);

	return lock;
#endif
}

void destory_lock(LOCK_HANDLE handle)
{
#ifdef WIN32
	CloseHandle(handle);
#else
	pthread_mutex_destroy(handle);
	free(handle);
#endif
}

void lock(LOCK_HANDLE handle)
{
#ifdef WIN32
	WaitForSingleObject(handle, INFINITE);
#else
	pthread_mutex_lock(handle);
#endif
}

void unlock(LOCK_HANDLE handle)
{
#ifdef WIN32
	ReleaseMutex(handle);
#else
	pthread_mutex_unlock(handle);
#endif
}

//bool try_lock(LOCK_HANDLE handle)
//{
//#ifdef WIN32
//	return WaitForSingleObject(handle, INFINITE) == WAIT_OBJECT_0 ? true : false;
//#else
//	return pthread_mutex_trylock(&handle) == 0 ? true : false;
//#endif
//}
