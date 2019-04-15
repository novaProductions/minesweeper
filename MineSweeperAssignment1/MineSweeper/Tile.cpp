#include "Tile.h"

Tile::Tile()
{
	Tile::isBomb = NULL;
	Tile::btn = NULL;
}

Tile::Tile(bool isBomb, HWND btn)
{
	Tile::isBomb = isBomb;
	Tile::btn = btn;
}

bool Tile::getIsBomb() {
	return isBomb;
}

bool Tile::setIsBomb(bool isBomb) {
	Tile::isBomb = isBomb;
	return Tile::isBomb;
}

HWND Tile::getBtn() {
	return btn;
}

HWND Tile::setBtn(HWND btn) {
	Tile::btn = btn;
	return Tile::btn;
}