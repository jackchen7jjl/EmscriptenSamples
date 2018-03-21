#pragma once

#ifndef _WIN32
	#include<pthread.h>
#endif//

namespace ratel
{
    namespace native
    {		

		int GetTestFlag();

#ifdef _WIN32
		#define THREAD_FUNC unsigned long __stdcall

        using THREAD_FUNC_PTR=unsigned long (__stdcall *)(void *);

		using THREAD_POINTER=void *;		//windows版是HANDLE，但HANDLE其实就是void *，这里为了减少#include<windows.h>所以直接写为void *
#else
		#define THREAD_FUNC void *

        using THREAD_FUNC_PTR=void *(*)(void *);

		using THREAD_POINTER=pthread_t;
#endif//_WIN32

        bool CreateAsyncThread(THREAD_FUNC_PTR,void *);                             ///<创建一个异步执行线程
		
		class Semaphore                                                                                 ///信号
		{
			void *ptr;

		public:

			Semaphore(int = 1024);
			virtual ~Semaphore();

			virtual bool Release(int n = 1);                                                              ///<释放信号

			virtual bool TryAcquire();                                                                  ///<尝试取得一个信号
			virtual bool Acquire(double time = 0.0);														///<等待并获取一个信号
		};//class Semaphore

		class Thread
		{
		protected:

			THREAD_POINTER thread_ptr=0;

		public:

			Semaphore sem;

			virtual ~Thread()
			{
				Close();
			}
			
			/**
			* 线程执行函数，会被反复调用
			* @return true 继续执行
			* @return false 退出执行
			*/
			virtual bool Execute();                                                                   ///<线程的运行函数

			virtual bool ProcStartThread(){return(true);}												///<线程启动运行函数,在Execute前被调用
			virtual void ProcEndThread(){}																///<结程结束运行函数,在Execute后被调用
		
			virtual bool IsExitDelete()const{return true;}												///<返回在退出线程时，是否删除本对象
			
		public:	//线程运行控制

			virtual bool Start();																		///<开始运行当前线程
			virtual void Close();                                                                       ///<强制关闭当前线程(其它线程调)
                                               			
		};//class Thread

    }//namespace native
}//namespace ratel
