#pragma once
#include <windows.h>

class Tile
{
public:
	Tile();
	Tile(bool isBomb, HWND btn, int numOfBombsAround);
	bool getIsBomb();
	bool setIsBomb(bool isBomb);
	HWND getBtn();
	HWND setBtn(HWND isBomb);
	int getNumOfBombsAround();
	int setNumOfBombsAround(int numOfBombsAround);
private:
	bool isBomb;
	HWND btn;
	int numOfBombsAround;
};

