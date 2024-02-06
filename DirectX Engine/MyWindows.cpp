#include"DirectX Engine.h"
#include"MyDirectX.h"
#include"MessageEnum.h"
#include<Windows.h>
using namespace std;

#define TEXT_DEFAULT TEXT[0]
const string APPTITLE = "TestEngine";
HWND window_hwnd;
bool gameover = false;
bool MOUSE_LOCKED = false;
bool pause = false;

ofstream PriSurfLog;
STAGE forestage, nowstage;
float fps;

LPDIRECT3DSURFACE9 tempSurf;

RECT FullRect;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG msg;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(NULL, "icon.ico");
	wc.hIconSm = LoadIcon(hInstance, "iconSm.ico");
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "MainWindowClass";
	if (!RegisterClassEx(&wc))
		return false;
	HWND hwnd = CreateWindow("MainWindowClass", APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREENW, SCREENH, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	if (hwnd == 0)return 0;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	window_hwnd = hwnd;
	if (!Game_Init(hwnd))return 0;
	init(hwnd);
	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		update();
		Game_Run(hwnd);
		update_keys();
	}
	Game_End();
	return msg.wParam;
}
bool Game_Init(HWND window) {
	if (!Direct3D_Init(window, SCREENW, SCREENH, false)) {
		MessageBox(window, "Error initializing Direct3D", "Error", MB_OK);
		return false;
	}
	if (!DirectInput_Init(window)) {
		MessageBox(window, "Error initializing DirectInput", "Error", MB_OK);
		return false;
	}
	return true;
}
void Game_Run(HWND hwnd) {
	if (!d3ddev)return;
	update_screen_picture();
	update_movment_key(hwnd);
	DirectInput_Update();
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (d3ddev->BeginScene()) {
		d3ddev->StretchRect(SURFbackground, NULL, backbuffer, NULL, D3DTEXF_NONE);
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		FontPrint("Test Console", make_rect(0, 0), DCblue, TEXT_DEFAULT);
		print_surface();
		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}
void Game_End() {
	if (SURFbackground)SURFbackground->Release();
	for (int i = 1; i <= SURFACE_KIND_SIZE; i++) {
		if (SURFACE[i])SURFACE[i]->Release();
	}
	for (int i = 1; i <= SPRITE_KIND_SIZE; i++) {
		if (SURFACE[i])SPRITE[i]->Release();
	}
	for (int i = 1; i <= TEXT_KIND_SIZE; i++) {
		if (SURFACE[i])TEXT[i]->Release();
	}
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}
void update_movment_key(HWND window) {
	if (!(GetForegroundWindow() == window))return;
	if (Key_Down(VK_F4) && Key_Down(VK_MENU)) {
		gameover = 1;
	}
	//	if (click(VK_ESCAPE)) {
	//		pauser(!pause);
	//	}
}

void update_screen_picture() {
	MESSAGECODE code = CheckMouseOn();
	if (nowstage != forestage) {
		AlgoClearScreenMsg();
		switch (forestage) {
//		case STGstart: SURFbackground = SURFACE[TPbackgound_start]; break;
		}
	}
	nowstage = forestage;
}
void init(HWND hwnd) {
	FullRect = make_rect(1, SCREENW, 1, SCREENH);
	// fps
	fps = 30;

	//stage init
	nowstage = STGempty;
	forestage = STGempty;
	// data init
//	SURFACE[TPbackgound_start] = LoadSurface("main_page\\background.png");
//	SPRITE_INFO[TPstart_startgame] = make_sprite_rect(265, 401, 2);
	SURFbackground = LoadSurface("pictures\\surface\\background.png");
	TEXT[0] = MakeFont("Consolas", 24);
	//random init
	srand(time(0));
	int seed = rand() % 10000 + rand() % 10000 * 10000;
	srand(seed);
	//ready for test
	test_init();
	//	NewGameInit();
		//mouse
	mouseX = SCREENW / 2;
	mouseY = SCREENH / 2;
	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetCursorPos((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
	//network

}
void test_init() {
	PriSurfLog.open("log.txt");
	//	QueueinFlash()
//	tempSurf = LoadSurface(FlashFileName(TPflash_foggy, 0));
	//	MessageBox(window_hwnd, FlashFileName(TPflash_foggy, 0).c_str(), "foggy", MB_OK);
}
void update() {
	phs_msg_solve();
	delay_solve();
	RenewMouseOn();
	update_screen_picture();
}