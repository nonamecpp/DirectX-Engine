#pragma once
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#define LL long long
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<xinput.h>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<cstdlib>
#include<iomanip>
#include<sstream>
#include<string.h>
#include<vector>
#include<stdio.h>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<deque>
#include<time.h>
#include<Windows.h>
#include"MessageEnum.h"
#include"D3DColors.h"
using namespace std;
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
//const
//program values
extern const string APPTITLE;
extern bool gameover;
const int SCREENW = 960;
const int SCREENH = 780;
const int priorbase_card_inqueue = 200;
const int priorbase_background = 100;
const int priorbase_card_onstage = 300;
const int priorbase_card_use = 400;
const int priorbase_flash = 500;


typedef int MESSAGECODE;
typedef int OBJECTCODE;

//typedef
//using MESSAGECODE = int;
//using CARDCODE = int;



//struct
struct SurfaceMsg {
	SURF_TYPE type;
	RECT rect;
};
struct SpriteMsg {
	SPRITE_TYPE type;
	RECT rect;
	int pos;
};
struct FlashMsg {
	FLASH_TYPE type;
	RECT rect;
	int pos;
};
struct TextMsg {
	TEXT_TYPE type;
	RECT rect;
	D3DCOLOR color;
	string msg;
};
struct MESSAGE_SURF {
	OBJECTCODE obj;
	MESSAGECODE code;
	MESSAGE_SURF_STATE state;
	SurfaceMsg surface;
	SpriteMsg sprite;
	FlashMsg flash;
	TextMsg text;
	int priority;
	int trans;
	bool operator <(const MESSAGE_SURF b) const { return priority < b.priority; }
	bool operator >(const MESSAGE_SURF b) const { return priority > b.priority; }
};
struct MESSAGE_SURF_SIMPLIFIED {
	MESSAGECODE code;
	int priority;
	bool operator <(const MESSAGE_SURF_SIMPLIFIED b) const { return priority < b.priority; }
	bool operator >(const MESSAGE_SURF_SIMPLIFIED b) const { return priority > b.priority; }
};
struct SPRITE_RECT {
	int framew, frameh;
	int column;
};
struct DELAY_MESSAGE {
	int fixed_time;
	MESSAGECODE code;
	DELAY_TYPE delay;
	int ContiSize;
	RECT rect;
	int pos;
	bool operator <(const DELAY_MESSAGE b) const { return fixed_time < b.fixed_time; }
	bool operator >(const DELAY_MESSAGE b) const { return fixed_time > b.fixed_time; }
};

//#define OpenLog 1
//#define CodeManagerOpenMessagebox 1

//max size -> 1e4
template<class MESSAGE>class CODE_MANAGER {

#ifndef OpenLog
#define OpenLog 0
#endif
#ifndef CodeManagerOpenMessagebox
#define CodeManagerOpenMessagebox 0
#endif
public:
	vector<MESSAGE>code_concrete;
	// ----------------------------------------------------------------
	// private here
private:
#if OpenLog
	ofstream LogOut;
#endif
	const int max_code_store_size = (int)1e4 + 5;
	struct MESSAGE_SIMPLIFIED {
		MESSAGE_SIMPLIFIED* l, * r;
		int tol;
		MESSAGE_SIMPLIFIED(int _tol) :l(NULL), r(NULL), tol(_tol) {};
	};
	int codesize;
	MESSAGE EmptyMessage;
	MESSAGE_SIMPLIFIED* first_plug;
	stack<MESSAGE_SIMPLIFIED*>iterstackx;
	stack<int>iterstackl, iterstackr, iterstackstate;
	void __cdmg_inner_iteropt_push(int l, int r, MESSAGE_SIMPLIFIED* x, int state) {
		iterstackl.push(l);
		iterstackr.push(r);
		iterstackx.push(x);
		iterstackstate.push(state);
	}
	void __cdmg_inner_iteropt_pop() {
		iterstackl.pop();
		iterstackr.pop();
		iterstackx.pop();
		iterstackstate.pop();
	}
	void __cdmg_inner_iteropt_get(int& l, int& r, MESSAGE_SIMPLIFIED*& x, int& state) {
		if (!iterstackstate.size())return;
		l = iterstackl.top();
		r = iterstackr.top();
		x = iterstackx.top();
		state = iterstackstate.top();
	}
	void __cdmg_inner_iterator_next() {
		int l, r, state;
		MESSAGE_SIMPLIFIED* x;
		__cdmg_inner_iteropt_get(l, r, x, state);
		if (l == r) {
			__cdmg_inner_iteropt_pop();
			__cdmg_inner_iteropt_get(l, r, x, state);
		}
		while (l != r && iterstackstate.size()) {
			//			cout << l << ' ' << r << ' ' << state << endl;
			if (state == 2) {
				__cdmg_inner_iteropt_pop();
			}
			else if (state == 0) {
				int mid = (l + r) / 2;
				iterstackstate.pop();
				iterstackstate.push(1);
				if ((*x).l != NULL && (*((*x).l)).tol < (mid - l + 1))
					__cdmg_inner_iteropt_push(l, mid, (*x).l, 0);
			}
			else if (state == 1) {
				int mid = (l + r) / 2;
				iterstackstate.pop();
				iterstackstate.push(2);
				if ((*x).r != NULL && (*((*x).r)).tol < (r - mid))
					__cdmg_inner_iteropt_push(mid + 1, r, (*x).r, 0);
			}
			__cdmg_inner_iteropt_get(l, r, x, state);
		}
	}
	bool __cdmg_inner_iterator_end() {
		return iterstackstate.size() == 0;
	}
	void __cdmg_inner_iterator_init() {
		__cdmg_inner_iteropt_push(1, max_code_store_size, first_plug, 0);
		__cdmg_inner_iterator_next();
	}
	MESSAGE __cdmg_inner_iterator_get() {
		return code_concrete[iterstackl.top()];
	}
	void __cdmg_inner_iterator_clear() {
		while (iterstackstate.size())__cdmg_inner_iteropt_pop();
	}
	int __cdmg_inner_regist(MESSAGE* tobe_rigisted, MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			if (code_concrete.size() == l)
				code_concrete.push_back(*tobe_rigisted);
			else code_concrete[l] = *tobe_rigisted;
			(*x).tol = 0;
			codesize++;
			return l;
		}
		int mid = (l + r) / 2;
		if ((*x).l == NULL) (*x).l = new MESSAGE_SIMPLIFIED(mid + 1 - l);
		if ((*x).r == NULL) (*x).r = new MESSAGE_SIMPLIFIED(r - mid);
		int ans;
		if ((*((*x).l)).tol > 0)ans = __cdmg_inner_regist(tobe_rigisted, (*x).l, l, mid);
		else ans = __cdmg_inner_regist(tobe_rigisted, (*x).r, mid + 1, r);
		(*x).tol = (*(*x).l).tol + (*(*x).r).tol;
		return ans;
	}
	bool __cdmg_inner_out(int _pos, MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			if ((*x).tol == 1) {
#if OpenLog
				LogOut << "Warming : CodeManager - Out - CODE has been logout" << endl;
				LogOut << "Warming : CodeManager - Out - Refuse to OUT" << endl;
				LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
				MessageBox(handle_window, "Warming", "Warming : CodeManager\nFunction : Out\nConcrete : CODE has been logout\n\tRefuse to OUT", MB_OK);
#endif
				return 0;
			}
			(*x).tol = 1;
			codesize--;
			return 1;
		}
		int mid = (l + r) / 2;
		if ((*x).l == NULL) (*x).l = new MESSAGE_SIMPLIFIED(mid + 1 - l);
		if ((*x).r == NULL) (*x).r = new MESSAGE_SIMPLIFIED(r - mid);
		bool ans;
		if (_pos <= mid)ans = __cdmg_inner_out(_pos, (*x).l, l, mid);
		else ans = __cdmg_inner_out(_pos, (*x).r, mid + 1, r);
		(*x).tol = (*(*x).l).tol + (*(*x).r).tol;
		return ans;
	}
	void __cdmg_inner_clearall(MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			(*x).tol = 1;
			return;
		}
		int mid = (l + r);
		if (l != NULL)__cdmg_inner_clearall((*x).l, l, mid);
		if (r != NULL)__cdmg_inner_clearall((*x).r, mid + 1, r);
		(*x).tol = (r - l + 1);
	}
	// ----------------------------------------------------------------
	// public here
public:
	CODE_MANAGER() :codesize(0) {
		MESSAGE* msg = new MESSAGE;
		EmptyMessage = *msg;
		code_concrete.push_back(*msg);
		delete msg;
		first_plug = new MESSAGE_SIMPLIFIED(max_code_store_size);
#if OpenLog
		if (!LogOut.is_open())LogOut.open("CodeManagerLog.txt");
#endif // OpenLog
	};
	int regist(MESSAGE tobe_rigisted) {
		if (codesize == max_code_store_size) {
#if OpenLog
			LogOut << "Warming : CodeManager - Regist - code size reaches MAX" << endl;
			LogOut << "Warming : CodeManager - Regist - Refuse to RIGIST" << endl;
			LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
			MessageBox(handle_window, "Warming", "Warming : CodeManager\nFunction : Regist\nConcrete : code size reaches MAX\n\tRefuse to RIGIST", MB_ICONWARNING | MB_OK);
#endif
			return -1;
		}
		return __cdmg_inner_regist(&tobe_rigisted, first_plug, 1, max_code_store_size);
	}
	bool out(int code) {
		return __cdmg_inner_out(code, first_plug, 1, max_code_store_size);
	}
	int size() { return codesize; }
	MESSAGE get(int code) {
		if (code > code_concrete.size() - 1) {
#if OpenLog
			LogOut << "Error : CodeManager - Get - invalid requirement" << endl;
			LogOut << "Error : CodeManager - Get - code out of vector size" << endl;
			LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
			MessageBox(handle_window, "Error", "Error : CodeManager\nFunction : Get\nConcrete : invalid requirement\n\tcode out of vector size", MB_ICONERROR | MB_OK);
#endif
			return EmptyMessage;
		}
		return code_concrete[code];
	}
	void AllClear() { __cdmg_inner_clearall(); codesize = 0; }
	void iterbegin() { __cdmg_inner_iterator_init(); }
	bool iterend() { return __cdmg_inner_iterator_end(); }
	void iternext() { __cdmg_inner_iterator_next(); }
	MESSAGE iterget() { return __cdmg_inner_iterator_get(); }
	void cleariter() { __cdmg_inner_iterator_clear(); }
};
template <class OBJECTCLASS> class prior_iterator {
private:
	vector<OBJECTCLASS> sequence;
	int it;
public:
	void cleariter() { sequence.clear(); }
	void add(OBJECTCLASS _obj) { sequence.push_back(_obj); }
	template <typename MANAGER_CLASS>void addall(MANAGER_CLASS& _pos) {
		for (_pos.iterbegin(); !_pos.iterend(); _pos.iternext()) {
			add(_pos.iterget());
		}
		_pos.cleariter();
	}
	void iterbegin() {
		sort(sequence.begin(), sequence.end());
		it = 0;
	}
	OBJECTCLASS iterget() { return sequence[it]; }
	bool iterend() { return it == sequence.size(); }
	void iternext() { it++; }
};
class UniWindow {
	MESSAGECODE MainBottom;
	vector<MESSAGECODE>Saving;
	vector<MESSAGE_SURF_STATE>show_state;
	vector<MESSAGE_SURF_STATE>hide_state;
	void add(MESSAGECODE to_be_saved) { Saving.push_back(to_be_saved); }
	void del(MESSAGECODE to_be_del) { Saving.erase(find(Saving.begin(), Saving.end(), to_be_del)); }
	void insert(MESSAGECODE to_be_insert, int pos) { Saving.insert(Saving.begin() + pos, to_be_insert); }
	void clear() { Saving.clear(); }

};
//windows objects
extern ofstream PriSurfLog;
extern HWND window_hwnd;
extern float fps;
extern bool MOUSE_LOCKED;
extern bool pause;
extern int key_click[300];
extern int key_down[300];
extern STAGE forestage;
extern STAGE nowstage;
//windows algorizm
extern MESSAGECODE screen_string_tree[SCREENH + 5][SCREENW * 16 + 30];
extern MESSAGECODE mouse_place;
extern int mouseX, mouseY;
//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPD3DXSPRITE spriteobj;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPDIRECT3DSURFACE9 SURFbackground;
extern LPDIRECT3DSURFACE9 SURFACE[300];
extern LPDIRECT3DTEXTURE9 SPRITE[300];
extern SPRITE_RECT SPRITE_INFO[300];
extern LPDIRECT3DSURFACE9 FLASH[21][500];
extern int FlashSize[21];
extern LPD3DXFONT TEXT[150];

extern int TEXT_KIND_SIZE;
extern int SURFACE_KIND_SIZE;
extern int SPRITE_KIND_SIZE;

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
void DrawSurface2(LPDIRECT3DSURFACE9 dest, RECT dest_rect, LPDIRECT3DSURFACE9 source);
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns, int transparency);
LPD3DXFONT MakeFont(string name, int size);
void FontPrint(string text, RECT pos, D3DCOLOR color, LPD3DXFONT font);
//DirectInput objects, devices and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_X();
int Mouse_Y();
//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();
//windows functions
int randint(int _Min, int _Max);
bool click(int key);
void update_keys();
void update_movment_key(HWND window);
void update_screen_picture();
void RenewMouseOn();
//surface deal
void PrintSurf(SURF_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state = msgSURFACE_SHOW);
void RegistSprite(SPRITE_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state = msgSPRITE_SHOW);
void RegistFlash(FLASH_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state = msgFLASH_SHOW); 
void PrintText(TEXT_TYPE type, int prior, RECT position, string message, D3DCOLOR color, OBJECTCODE object, MESSAGE_SURF_STATE state);
DELAY_MESSAGE delay_sprite_move_make(int time_point, MESSAGECODE code, RECT position);
DELAY_MESSAGE delay_sprite_continue_make(int time_point, MESSAGECODE code, int ContiSize);
DELAY_MESSAGE delay_sprite_next_make(int time_point, MESSAGECODE code, int flamex);
DELAY_MESSAGE delay_delete_make(int time_point, MESSAGECODE code);
void print_surface();
string FlashFileName(FLASH_TYPE type, int pos);

void send_message(MESSAGE_PHISICS msg, MESSAGECODE code);
void phs_msg_solve();
void delay_solve();
void sprite_effect(MESSAGECODE code, int delay, EFFECT_TYPE type);
// struct make
RECT percentMAKE(double x, double y, double height, double wide);
RECT make_rect(int left, int top, int right = 0, int bottom = 0);

//algorithm
void push_tag(int srnh, int x);
void ScreenStringtreeRenew(int srnh, MESSAGECODE p, int ll, int rr, int x = 1, int l = 1, int r = SCREENW);
MESSAGECODE CheckMouseOn(int x = 1, int l = 1, int r = SCREENW);
void AlgoScreenCoverPicture(int left, int top, int weigh, int height, MESSAGECODE msg);
void AlgoClearScreenMsg();

//get together
void init(HWND hwnd);
void update();

//test 
void test_init();

//init

//empty structs
extern MESSAGE_SURF EmptyMessageSurf;
extern RECT EmptyRect;
extern RECT FullRect;