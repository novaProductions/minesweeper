#pragma once
#include <windows.h>

class Tile
{
public:

	Tile();
	Tile(bool isBomb, HWND btn);
	bool getIsBomb();
	bool setIsBomb(bool isBomb);
	HWND getBtn();
	HWND setBtn(HWND isBomb);
private:
	bool isBomb;
	HWND btn;
};

