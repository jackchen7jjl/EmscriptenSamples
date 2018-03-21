#include <iostream>

using namespace std;

extern "C"
{
	extern void GetTheDeviceInfo(char *info);
}

#ifndef EMSCRIPTEN
	void GetTheDeviceInfo(char *info)
	{
		char deviceInfo[] = "168.156";
		int deviceInfoLen = strlen(deviceInfo) + 1;
		memcpy(info, deviceInfo, deviceInfoLen);
	}
#endif // EMSCRIPTEN

int main()
{
	char deviceInfo[1024];
	GetTheDeviceInfo(deviceInfo);
	cout << "deviceInfo " << deviceInfo << ", " << strlen(deviceInfo)  << endl;

#ifndef EMSCRIPTEN
	cin.get();
	cin.get();
#endif

	return 0;
}