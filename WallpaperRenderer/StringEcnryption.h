#pragma once
#include <string>
#include <memory>
#include <Windows.h>

/*string encryption
CRYPT->crypt the string
ECRYPT->crypt and decrypt the string
(this ecrypt allows to skip the string in debuggers(for example - IDA pro))
string encryption*/
class StringEcnryption
{
public:
	StringEcnryption() {
		srand(GetTickCount());
		_hashkey = rand() % (1 << 10);
	}
	__forceinline std::string encrypt(std::string str) noexcept { // получаем исключительно копию строки!
		for (char& c : str) {
			c = c ^ _hashkey;
		}
		return str;
	}
	__forceinline std::string decrypt(const std::string& str) noexcept { // получаем исключительно копию строки!
		std::string s = std::string();
		for (char c : str) {
			s += c ^ _hashkey;			
		}
		return s;
	}
private:
	int _hashkey{ 0 };
};
inline std::unique_ptr<StringEcnryption>encryption(new StringEcnryption());
#define CRYPT(str)encryption->encrypt(str).c_str()
#define DERYPT(str)encryption->decrypt(str).c_str()
#define ECRYPT(str)encryption->decrypt(encryption->encrypt(str)).c_str()
