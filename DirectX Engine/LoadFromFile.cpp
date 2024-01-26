#include"MyDirectX.h"
int TEXT_KIND_SIZE;
int SURFACE_KIND_SIZE;
int SPRITE_KIND_SIZE;

void LoadTextSettings(string FileName) {
	ifstream fin;
	string name;
	int size;
	fin.open(FileName.c_str(), ios::in);
	for (TEXT_KIND_SIZE = 0; fin >> name >> size; TEXT_KIND_SIZE++) {
		TEXT[TEXT_KIND_SIZE + 1] = MakeFont(name, size);
	}
	fin.close();
}

void LoadSurfaceSettings(string FileName) {
	ifstream fin;
	string name;
	fin.open(FileName.c_str(), ios::in);
	for (SURFACE_KIND_SIZE = 0; fin >> name; SURFACE_KIND_SIZE++) {
		SURFACE[SURFACE_KIND_SIZE + 1] = LoadSurface(name);
	}
	fin.close();
}
void LoadSpriteSettings(string FileName) {
	ifstream fin;
	string name;
	int a, r, g, b, tag;
	int wid, hei, ml;
	fin.open(FileName.c_str(), ios::in);
	for (SPRITE_KIND_SIZE = 0; fin >> name >> tag; SPRITE_KIND_SIZE++) {
		if (tag)fin >> a >> r >> g >> b;
		fin >> wid >> hei >> ml;
		SPRITE_INFO[SPRITE_KIND_SIZE + 1].framew = wid;
		SPRITE_INFO[SPRITE_KIND_SIZE + 1].frameh = hei;
		SPRITE_INFO[SPRITE_KIND_SIZE + 1].column = ml;
		SPRITE[SPRITE_KIND_SIZE + 1] = LoadTexture(name, D3DCOLOR_ARGB(a, r, g, b));
	}
	fin.close();
}