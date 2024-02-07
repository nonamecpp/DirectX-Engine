#pragma once
#include"MyDirectX.h"
const D3DCOLOR DEFAULT_COLOR = D3DCOLOR_ARGB(255, 127, 127, 127);
const D3DCOLOR DCwhite = D3DCOLOR_XRGB(255, 255, 255);
const D3DCOLOR DCblack = D3DCOLOR_XRGB(0, 0, 0);
const D3DCOLOR DCred = D3DCOLOR_XRGB(255, 0, 0);
const D3DCOLOR DCgreen = D3DCOLOR_XRGB(0, 255, 0);
const D3DCOLOR DCblue = D3DCOLOR_XRGB(0, 0, 255);
D3DCOLOR ColorMake12(char rgb[3]) {
#define turnint(ch1) (isdigit(ch1))?(ch1-'0'):('a'<=ch1&&ch1<='f')?(ch1-'a'+10):('A'<=ch1&&ch1<='F')?(ch1-'A'+10):-1
	int r = turnint(rgb[0]) * 17;
	int g = turnint(rgb[1]) * 17;
	int b = turnint(rgb[2]) * 17;
	if (r < 0 || g < 0 || b < 0)return NULL;
	return D3DCOLOR_XRGB(r, g, b);
}
D3DCOLOR ColorMake24(char rgb[6]) {
#define turnint(ch1) (isdigit(ch1))?(ch1-'0'):('a'<=ch1&&ch1<='f')?(ch1-'a'+10):('A'<=ch1&&ch1<='F')?(ch1-'A'+10):-1
	int r = turnint(rgb[0]) * 16 + turnint(rgb[1]);
	int g = turnint(rgb[2]) * 17 + turnint(rgb[3]);
	int b = turnint(rgb[4]) * 17 + turnint(rgb[5]);
	if (r < 0 || g < 0 || b < 0)return NULL;
	return D3DCOLOR_XRGB(r, g, b);
}