#include"MyDirectX.h"
#include"MessageEnum.h"

CODE_MANAGER<MESSAGE_SURF>SurfManager;
prior_iterator<MESSAGE_SURF>SurfSorter;

priority_queue<DELAY_MESSAGE, vector<DELAY_MESSAGE>, greater<DELAY_MESSAGE> >message_delay;
queue<MESSAGE_PHISICS>msg_phs_queue;
queue<MESSAGECODE>msg_phs_queue_obj;


LPDIRECT3DSURFACE9 SURFbackground;
LPDIRECT3DSURFACE9 SURFACE[300];
LPDIRECT3DTEXTURE9 SPRITE[300];
SPRITE_RECT SPRITE_INFO[300];
LPDIRECT3DSURFACE9 FLASH[21][500];
int FlashSize[21];
LPD3DXFONT TEXT[150];

MESSAGECODE screen_string_tree[SCREENH + 5][SCREENW * 16 + 30];
MESSAGECODE mouse_place;
int mouseX, mouseY;

MESSAGECODE PrintSurf(SURF_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state) {
	MESSAGE_SURF temp;
	temp.surface.type = type;
	temp.priority = prior;
	temp.surface.rect = position;
	temp.obj = object;
	temp.state = state;
	temp.code = SurfManager.regist(temp);
	SurfManager.code_concrete[temp.code].code = temp.code;
	return temp.code;
}
MESSAGECODE RegistSprite(SPRITE_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state) {
	MESSAGE_SURF temp;
	temp.sprite.type = type;
	temp.priority = prior;
	temp.obj = object;
	temp.state = state;
	temp.code = SurfManager.regist(temp);
	SurfManager.code_concrete[temp.code].code = temp.code;
	return temp.code;
}
MESSAGECODE RegistFlash(FLASH_TYPE type, int prior, RECT position, OBJECTCODE object, MESSAGE_SURF_STATE state) {
	MESSAGE_SURF temp;
	temp.priority = prior;
	temp.obj = object;
	temp.state = state;
	temp.code = SurfManager.regist(temp);
	SurfManager.code_concrete[temp.code].code = temp.code;
	return temp.code;
}
MESSAGECODE PrintText(TEXT_TYPE type, int prior, RECT position, string message, D3DCOLOR color, OBJECTCODE object, MESSAGE_SURF_STATE state) {
	MESSAGE_SURF temp;
	temp.text.type = type;
	temp.priority = prior;
	temp.text.rect = position;
	temp.text.msg = message;
	temp.text.color = color;
	temp.obj = object;
	temp.state = state;
	temp.code = SurfManager.regist(temp);
	SurfManager.code_concrete[temp.code].code = temp.code;
	return temp.code;
}
void DeleteSurface(MESSAGECODE code) {
	SurfManager.out(code);
}

DELAY_MESSAGE delay_make(DELAY_TYPE type, int time_point, MESSAGECODE code, RECT position, int ContiSize, int flamex) {
	DELAY_MESSAGE t;
	t.fixed_time = time_point;
	t.code = code;
	t.rect = position;
	t.delay = type;
	t.ContiSize = ContiSize;
	t.pos = flamex;
	return t;
}
DELAY_MESSAGE delay_sprite_move_make(int time_point, MESSAGECODE code, RECT position) {
	return delay_make(delaySPRITE_MOVE, time_point, code, position, 0, 0);
}
DELAY_MESSAGE delay_sprite_continue_make(int time_point, MESSAGECODE code, int ContiSize) {
	return delay_make(delaySPRITE_CONTINUE, time_point, code, make_rect(0, 0), ContiSize, 0);
}
DELAY_MESSAGE delay_sprite_next_make(int time_point, MESSAGECODE code, int flamex) {
	return delay_make(delaySPRITE_NEXT, time_point, code, make_rect(0, 0), 0, flamex);
}
DELAY_MESSAGE delay_delete_make(int time_point, MESSAGECODE code) {
	return delay_make(delayDELETE, time_point, code, make_rect(0, 0), 0, 0);
}
void print_surface() {
	SurfSorter.addall(SurfManager);
	for (SurfSorter.iterbegin(); !SurfSorter.iterend(); SurfSorter.iternext()) {
		MESSAGE_SURF CurSurf = SurfSorter.iterget();
		if (CurSurf.state == msgSURFACE_SHOW || CurSurf.state == msgSURFACE_TRANSPARENT)
			DrawSurface2(backbuffer, CurSurf.surface.rect, SURFACE[CurSurf.surface.type]);
		if (CurSurf.state == msgSURFACE_SHOW || CurSurf.state == msgSURFACE_HIDE_CHOOSEABLE)
			AlgoScreenCoverPicture(CurSurf.surface.rect.left, CurSurf.surface.rect.top,
				CurSurf.surface.rect.right - CurSurf.surface.rect.left,
				CurSurf.surface.rect.bottom - CurSurf.surface.rect.top,
				CurSurf.code);

		if (CurSurf.state == msgSPRITE_SHOW || CurSurf.state == msgSPRITE_TRANSPARENT)
			Sprite_Draw_Frame(SPRITE[CurSurf.sprite.type], CurSurf.sprite.rect.left, CurSurf.sprite.rect.top,
				CurSurf.sprite.pos, SPRITE_INFO[CurSurf.sprite.type].framew,
				SPRITE_INFO[CurSurf.sprite.type].frameh,
				SPRITE_INFO[CurSurf.sprite.type].column,
				CurSurf.trans);
		if (CurSurf.state == msgSPRITE_SHOW || CurSurf.state == msgSPRITE_HIDE_CHOOSEABLE)
			AlgoScreenCoverPicture(CurSurf.sprite.rect.left, CurSurf.sprite.rect.top,
				SPRITE_INFO[CurSurf.sprite.type].framew, SPRITE_INFO[CurSurf.sprite.type].frameh,
				CurSurf.code);
		if (CurSurf.state == msgTEXT_SHOW || CurSurf.state == msgTEXT_TRANSPARENT)
			FontPrint(CurSurf.text.msg, CurSurf.text.rect, CurSurf.text.color, TEXT[CurSurf.text.type]);
		if (CurSurf.state == msgTEXT_SHOW || CurSurf.state == msgTEXT_HIDE_CHOOSEABLE)
			AlgoScreenCoverPicture(CurSurf.text.rect.left, CurSurf.text.rect.top,
				CurSurf.text.rect.right - CurSurf.text.rect.left,
				CurSurf.text.rect.bottom - CurSurf.text.rect.top,
				CurSurf.code);
	}
	SurfSorter.cleariter();
}

void send_message(MESSAGE_PHISICS msg, MESSAGECODE code) {
	msg_phs_queue.push(msg);
	msg_phs_queue_obj.push(code);
}
void phs_msg_solve() {
	while (msg_phs_queue.size()) {
		MESSAGE_PHISICS msg = msg_phs_queue.front();
		MESSAGECODE code = msg_phs_queue_obj.front();
		msg_phs_queue.pop();
		msg_phs_queue_obj.pop();
		switch (msg) {

		}
	}
}
void sprite_effect(MESSAGECODE code, int delay, EFFECT_TYPE type) {
	MESSAGE_SURF data = SurfManager.code_concrete[code];
	switch (type) {

	}

}
void delay_solve() {
	while (message_delay.size() && clock() >= message_delay.top().fixed_time) {
		DELAY_MESSAGE msg = message_delay.top();
		message_delay.pop();
		MESSAGECODE code = msg.code;
		if (msg.delay == delaySPRITE_NEXT)SurfManager.code_concrete[code].sprite.pos = msg.pos;
		if (msg.delay == delayDELETE)DeleteSurface(code);
		if (msg.delay == delaySPRITE_MOVE)SurfManager.code_concrete[code].sprite.rect = msg.rect;
		if (msg.delay == delaySPRITE_CONTINUE) {
			SurfManager.code_concrete[code].sprite.pos = (SurfManager.code_concrete[code].sprite.pos + 1) % msg.ContiSize;
			msg.fixed_time += 1000 / fps;
			message_delay.push(msg);
		}
		if (msg.delay == delayFLASH_NEXT) {
			SurfManager.code_concrete[code].flash.pos = msg.pos;
			//			MessageBox(window_hwnd, "flash next", " flash", MB_OK);
			//			FLASH[code_concrete[code].flash] = LoadSurface(FlashFileName(code_concrete[code].flash, msg.pos));
		}
		if (msg.delay == delayFLASH_CONTINUE) {
			//			MessageBox(window_hwnd, "flash continue", " flash", MB_OK);
			SurfManager.code_concrete[code].flash.pos = (SurfManager.code_concrete[code].flash.pos + 1) % msg.ContiSize;
			//			FLASH[code_concrete[code].flash] = LoadSurface(FlashFileName(code_concrete[code].flash, code_concrete[code].pos));
			msg.fixed_time += 1000 / fps;
			message_delay.push(msg);
		}
	}
}
void push_tag(int srnh, int x) {
	if (screen_string_tree[srnh][x] == 0)return;
	screen_string_tree[srnh][x * 2] = screen_string_tree[srnh][x];
	screen_string_tree[srnh][x * 2 + 1] = screen_string_tree[srnh][x];
	screen_string_tree[srnh][x] = 0;
}
void ScreenStringtreeRenew(int srnh, MESSAGECODE p, int ll, int rr, int x, int l, int r) {
	if (ll <= l && r <= rr) {
		screen_string_tree[srnh][x] = p;
		return;
	}
	push_tag(srnh, x);
	int mid = (l + r) / 2;
	if (ll <= mid)ScreenStringtreeRenew(srnh, p, ll, rr, x * 2, l, mid);
	if (mid < rr)ScreenStringtreeRenew(srnh, p, ll, rr, x * 2 + 1, mid + 1, r);
}
MESSAGECODE CheckMouseOn(int x, int l, int r) {
	if (mouseX > SCREENW || mouseY > SCREENH)return -1;
	if (mouseX <= 0 || mouseY <= 0)return -1;
	if (l == r) {
		return screen_string_tree[mouseY][x];
	}
	push_tag(mouseY, x);
	int mid = (l + r) / 2;
	if (mouseX <= mid)return CheckMouseOn(x * 2, l, mid);
	if (mid < mouseX)return CheckMouseOn(x * 2 + 1, mid + 1, r);
}
void RenewMouseOn() {
	RECT rect;
	GetWindowRect(window_hwnd, &rect);
	POINT point;
	GetCursorPos(&point);
	mouseX = point.x - rect.left;
	mouseY = point.y - rect.top;
}
void AlgoScreenCoverPicture(int left, int top, int weigh, int height, MESSAGECODE msg) {
	for (int i = top; i <= top + height; i++) {
		ScreenStringtreeRenew(i, msg, left, left + weigh);
	}
}
void AlgoClearScreenMsg() {
	for (int i = 1; i <= SCREENH; i++) {
		ScreenStringtreeRenew(i, -1, 1, SCREENW);
	}
}
bool Between(int l, int r, int u, int d) {
	if (l <= mouseX && r <= mouseX && u <= mouseY && mouseY <= d)return 1;
	return 0;
}
RECT make_rect(int left, int top, int right, int bottom) {
	RECT rt;
	rt.left = left;
	rt.right = right;
	rt.top = top;
	rt.bottom = bottom;
	return rt;
}
RECT percentMAKE(double l, double u, double height, double wide) {
	return make_rect((int)(l * SCREENW), (int)(u * SCREENH), (int)((l + wide) * SCREENW), (int)((u + height) * SCREENH));
}
