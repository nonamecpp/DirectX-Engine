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
#include"DataStructure.h"
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
const int SCREENW = 1920;
const int SCREENH = 1080;
const int priorbase_card_inqueue = 200;
const int priorbase_background = 100;
const int priorbase_card_onstage = 300;
const int priorbase_card_use = 400;
const int priorbase_flash = 500;

const LPD3DXFONT DEFAULT_FONT = MakeFont("Consolas", 24);

typedef int MESSAGECODE;
typedef int OBJECTCODE;

//typedef
//using MESSAGECODE = int;
//using CARDCODE = int;



//struct
struct MESSAGE_SURF {
	OBJECTCODE obj;
	MESSAGECODE code;
	MESSAGE_SURF_STATE state;
	SPRITE_TYPE sprite;
	SURF_TYPE surface;
	FLASH_TYPE flash;
	TEXT_TYPE text;
	RECT rect;
	int pos;
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
extern int code_store_size;
extern int last_HighlightPos;
extern bool CardinqueueChanged;
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
extern LPDIRECT3DSURFACE9 FLASH[21][500];
extern int FlashSize[21];
extern LPDIRECT3DTEXTURE9 SPRITE[300];
extern SPRITE_RECT SPRITE_INFO[300];

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
void FontPrint(string text, RECT pos = make_rect(0, 0), D3DCOLOR color = DEFAULT_COLOR, LPD3DXFONT font = DEFAULT_FONT);
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
DELAY_MESSAGE delay_move_make(int time_point, MESSAGECODE code, RECT position);
DELAY_MESSAGE delay_continue_make(int time_point, MESSAGECODE code, int ContiSize);
DELAY_MESSAGE delay_next_make(int time_point, MESSAGECODE code, int flamex);
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