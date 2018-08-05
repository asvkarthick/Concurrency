#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Reader writer library

typedef struct
{
    pthread_mutex_t rw_mutex;
    pthread_cond_t rw_condwriter;
    pthread_cond_t rw_condreaders;
    int rw_refcount;
    int rw_nwaitwriters;
    int rw_nwaitreaders;
} pthread_rwlock_t_;

int pthread_rwlock_rdlock_(pthread_rwlock_t_ *rw)
{
    int result;

    if((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return result;

    while(rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0)
    {
        rw->rw_nwaitreaders++;
        result = pthread_cond_wait(&rw->rw_condreaders, &rw->rw_mutex);
        rw->rw_nwaitreaders--;
        if(result != 0)
            break;
    }

    if(result == 0)
        rw->rw_refcount++;

    pthread_mutex_unlock(&rw->rw_mutex);
    return result;
}

int pthread_rwlock_wrlock_(pthread_rwlock_t_ *rw)
{
    int result;

    if((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return result;

    while(rw->rw_refcount != 0)
    {
        rw->rw_nwaitwriters++;
        result = pthread_cond_wait(&rw->rw_condwriter, &rw->rw_mutex);
        rw->rw_nwaitwriters--;
        if(result != 0)
            break;
    }

    if(result == 0)
        rw->rw_refcount = -1;

    pthread_mutex_unlock(&rw->rw_mutex);
    return result;
}

int pthread_rwlock_unlock_(pthread_rwlock_t_ *rw)
{
    int result;

    if((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
        return result;

    if(rw->rw_refcount > 0)
        rw->rw_refcount--;
    else if(rw->rw_refcount == -1)
        rw->rw_refcount = 0;

    if(rw->rw_nwaitwriters > 0)
    {
        if(rw->rw_refcount == 0)
            pthread_cond_signal(&rw->rw_condwriter);
    }
    else if(rw->rw_nwaitreaders > 0)
        pthread_cond_broadcast(&rw->rw_condreaders);

    pthread_mutex_unlock(&rw->rw_mutex);
    return result;
}
