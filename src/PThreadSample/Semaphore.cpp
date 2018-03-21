#include "Thread.h"
#include <pthread.h>
#include<time.h>
#include<sys/time.h>
#include<semaphore.h>

namespace ratel
{
    namespace native
    {
		constexpr long  MICRO_SEC_PER_SEC = 1000 * 1000;			//΢��
		constexpr long  NANO_SEC_PER_SEC = 1000 * 1000 * 1000;	//����

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
        * @param max_count ������
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
        * �ͷ�һ���ź�
        * @param n �ͷ��ź�����
        * @return �Ƿ��ͷųɹ�
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
            //	return !sem_post_multiple((sem_t *)ptr,n);	//ע�����������������os��֧��
        }

        /**
        * ���Ի�ȡһ���ź�
        * @return �Ƿ���ȡ���ź�
        */
        bool Semaphore::TryAcquire()
        {
            if(!ptr)return(false);

            return !sem_trywait((sem_t *)ptr);
        }

        /**
        * �ȴ�����ȡһ���ź�
        * @param t �ȴ����ʱ��,ʹ��0��ʾ���޵ȴ�.(��λ��)
        * @return �Ƿ�ȴ�����,��������ʱ��,��δ�ȵ���Ϊ��ʱ,����false
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
