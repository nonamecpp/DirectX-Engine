#pragma once
#include"MyDirectX.h"
const D3DCOLOR DEFAULT_COLOR = D3DCOLOR_ARGB(255, 127, 127, 127);
const D3DCOLOR DCwhite = D3DCOLOR_XRGB(255, 255, 255);
const D3DCOLOR DCblack = D3DCOLOR_XRGB(0, 0, 0);
const D3DCOLOR DCred = D3DCOLOR_XRGB(255, 0, 0);
const D3DCOLOR DCgreen = D3DCOLOR_XRGB(0, 255, 0);
const D3DCOLOR DCblue = D3DCOLOR_XRGB(0, 0, 255);

D3DCOLOR ColorMake12(char rgb[3]);
D3DCOLOR ColorMake24(char rgb[6]);