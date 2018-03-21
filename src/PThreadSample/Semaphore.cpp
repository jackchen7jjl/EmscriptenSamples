#include "Thread.h"
#include <pthread.h>
#include<time.h>
#include<sys/time.h>
#include<semaphore.h>

namespace ratel
{
    namespace native
    {
		constexpr long  MICRO_SEC_PER_SEC = 1000 * 1000;			//微秒
		constexpr long  NANO_SEC_PER_SEC = 1000 * 1000 * 1000;	//纳秒

		void GetWaitTime(struct timespec &abstime, double t)
		{
			clock_gettime(CLOCK_REALTIME, &abstime);

			abstime.tv_sec += t;
			t -= (long)t;
			abstime.tv_nsec += t*NANO_SEC_PER_SEC;

			if (abstime.tv_nsec >= NANO_SEC_PER_SEC)
			{
				abstime.tv_nsec -= NANO_SEC_PER_SEC;
				++abstime.tv_sec;
			}
		}

		void SetTimeVal(timeval &tv, const double t_sec)
		{
			tv.tv_sec = t_sec;
			tv.tv_usec = (t_sec - (double)tv.tv_sec)*MICRO_SEC_PER_SEC;
		}

        /**
        * @param max_count 最大计数
        */
        Semaphore::Semaphore(int max_count)
        {
            ptr=new sem_t;

            if(sem_init((sem_t *)ptr,PTHREAD_PROCESS_PRIVATE,0))
            {
                delete (sem_t *)ptr;
                ptr=nullptr;
            }
        }

        Semaphore::~Semaphore()
        {
            if(!ptr)return;

            sem_destroy((sem_t *)ptr);
            delete (sem_t *)ptr;
        }

        /**
        * 释放一个信号
        * @param n 释放信号数量
        * @return 是否释放成功
        */
        bool Semaphore::Release(int n)
        {
            if(!ptr)return(false);
            if(n<=0)return(false);

            int result=0;

            for(int i=0;i<n;i++)
                result+=sem_post((sem_t *)ptr);

            return !result;

            //if(n==1)
            //	return !sem_post((sem_t *)ptr);
            //else
            //	return !sem_post_multiple((sem_t *)ptr,n);	//注：这个函数不是所有os都支持
        }

        /**
        * 尝试获取一个信号
        * @return 是否有取得信号
        */
        bool Semaphore::TryAcquire()
        {
            if(!ptr)return(false);

            return !sem_trywait((sem_t *)ptr);
        }

        /**
        * 等待并获取一个信号
        * @param t 等待的最长时间,使用0表示无限等待.(单位秒)
        * @return 是否等待到了,如果超过最长时间,仍未等到即为超时,返回false
        */
        bool Semaphore::Acquire(double t)
        {
            if(!ptr)return(false);

#ifndef EMSCRIPTEN
            if(t>0)
            {
                struct timespec abstime;

                GetWaitTime(abstime,t);

                return !sem_timedwait((sem_t *)ptr,&abstime);
            }
			else
#endif
                return !sem_wait((sem_t *)ptr);
        }
    }//namespace native
}//namespace ratel
