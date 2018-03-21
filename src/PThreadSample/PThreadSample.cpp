#include <iostream>
#include "Thread.h"

#if defined(EMSCRIPTEN)
#include <emscripten\emscripten.h>
#endif

using namespace std;
using namespace ratel::native;

Thread *t = nullptr;
int frameCnt = 0;

void MainLoop()
{
	frameCnt++;

	if(frameCnt%100 == 0)
	{
		t->sem.Release();
		cout << GetTestFlag() << endl;
	}

#ifndef EMSCRIPTEN
	_sleep(1);
#endif
}

int main()
{
	cout << "hello world!" << endl;

	t = new Thread();
	t->Start();

#ifdef EMSCRIPTEN
	emscripten_set_main_loop(MainLoop, 0, 1);
#else
	while (true)
	{
		MainLoop();
	}
#endif


	return 0;
}