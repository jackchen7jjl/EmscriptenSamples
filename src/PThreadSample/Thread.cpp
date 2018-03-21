#include"Thread.h"
#include<iostream>
#include <pthread.h>
#include <emscripten.h>
#include <emscripten/threading.h>

namespace ratel
{
    namespace native
    {
		int testFlag = 0;

		int GetTestFlag()
		{
			return testFlag;
		}

		/**
		  *  tips:	PTHREAD_CREATE_DETACHED 方式创建的线程，在退出时，自动清除线程。无法使用pthread_join函数获取运行状态,pthread_join会返回22号错误
		  *         PTHREAD_CREATE_JOINABLE 方式创建的线程，在退出时，不会清除线程，必使使用pthread_join函数获取。或是在退出时使用pthread_detach(pthread_self())。
		  */       

        bool CreateAsyncThread(THREAD_FUNC_PTR func,void *param)
        {
            pthread_t pt;
            pthread_attr_t attr;

            pthread_attr_init(&attr);

			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

            int result=pthread_create(&pt,&attr,func,param);

			pthread_attr_destroy(&attr);

            return(!result);        //返回0表示正常
        }

		THREAD_FUNC ThreadFunc(ratel::native::Thread *tc)
		{
			if(tc->ProcStartThread())
			{
				while(tc->Execute());

				tc->ProcEndThread();
			}
			
			if(tc->IsExitDelete())
			{
				delete tc;
				pthread_detach(pthread_self());
			}

			return(0);
		}

		bool Thread::Execute()
		{
			if(sem.Acquire())
			{
				testFlag++;
			}
			return true;
		}

		/**
		* (线程外部调用)执行当前线程
		* @return 是否创建线程成功
		*/
		bool Thread::Start()
		{
			if(thread_ptr)
			{
				std::cerr<<"Thread::Start() error,thread_ptr!=nullptr."<<std::endl;
			}

			pthread_attr_t attr;

			pthread_attr_init(&attr);

			pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

			if(pthread_create(&thread_ptr,&attr,(void *(*)(void *))ThreadFunc,this))						//返回0表示正常
			{
				thread_ptr=0;

				pthread_attr_destroy(&attr);
				std::cerr<<"Create Thread (pthread_create) failed.errno:"<<errno<<std::endl;
				return(false);
			}

			pthread_attr_destroy(&attr);
			return(true);
		}

		/**
		* (线程外部调用)关闭当前线程.不推荐使用此函数，正在执行的线程被强制关闭会引起无法预知的错误。
		*/
		void Thread::Close()
		{
			
		}		
    }//namespace native
}//namespace ratel
