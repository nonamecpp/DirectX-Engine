// DirectX Engine.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DirectX Engine.h"
#include "MessageEnum.h"
#include "MyDirectX.h"
using namespace std;

const int BufferSize = 1005;

MESSAGE_SURF EmptyMessageSurf;
RECT EmptyRect;

priority_queue<MESSAGE_SURF_SIMPLIFIED, vector<MESSAGE_SURF_SIMPLIFIED >, greater<MESSAGE_SURF_SIMPLIFIED > >message_queue;
priority_queue<MESSAGE_SURF_SIMPLIFIED, vector<MESSAGE_SURF_SIMPLIFIED >, greater<MESSAGE_SURF_SIMPLIFIED > >physics_message_queue;


int randint(int _Min, int _Max) {
	return _Min + (rand() % 10000 + rand() % 10000 * 10000) % (_Max - _Min + 1);
}
string FlashFileName(FLASH_TYPE type, int pos) {
	stringstream sstr;
	switch (type) {
	case TPflash_test: sstr << "FlashTest\\" << ((pos < 10) ? "00" : pos < 100 ? "0" : "") << pos << ".bmp"; break;
//	case TPflash_rain: sstr << "FlashRain\\" << ((pos < 10) ? "00" : pos < 100 ? "0" : "") << pos << ".bmp"; break;
	}
	string ans = "";
	sstr >> ans;
	return ans;
}