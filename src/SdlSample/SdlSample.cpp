#include <iostream>
#include <map>
#include <string>

using namespace std;

using ushort = unsigned short;
using String = std::string;

class Keys
{
public:
	static constexpr int Key1 = 10001;
	static constexpr int Key2 = 10002;

	static String GetKeyName(int key);

private:
	static std::map<ushort, String> _keyNames;

};

std::map<ushort, String> Keys::_keyNames;

String Keys::GetKeyName(int key)
{
	
	if (_keyNames.size() == 0)
	{
		_keyNames[Keys::Key1] = "Key1";
		_keyNames[Keys::Key2] = "Key2";

		std::cout << "_keyNamesSize: " << _keyNames.size() << std::endl;
		auto end = _keyNames.end();
		for (auto it = _keyNames.begin(); it != end; ++it)
		{
			std::cout << it->first << "," << it->second << std::endl;
		}
	}


	if (_keyNames.find(key) != _keyNames.end())
		return _keyNames[key];
	return "unknown message " + std::to_string(key);
}

int main()
{
	Keys::GetKeyName(10001);
	cout << "key 10001 name: " << Keys::GetKeyName(10001) << endl;
	cout << "key 10002 name: " << Keys::GetKeyName(10002) << endl;

#ifndef EMSCRIPTEN
	cin.get();
	cin.get();
#endif
	return 0;
}