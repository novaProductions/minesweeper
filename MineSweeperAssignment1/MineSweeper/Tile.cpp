#include "Tile.h"

Tile::Tile()
{
	Tile::isBomb = NULL;
	Tile::btn = NULL;
	Tile::numOfBombsAround = NULL;
}

Tile::Tile(bool isBomb, HWND btn, int numOfBombsArround)
{
	Tile::isBomb = isBomb;
	Tile::btn = btn;
	Tile::numOfBombsAround = numOfBombsAround;
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

int Tile::getNumOfBombsAround() {
	return numOfBombsAround;
}
int Tile::setNumOfBombsAround(int numOfBombsAround) {
	Tile::numOfBombsAround = numOfBombsAround;
	return Tile::numOfBombsAround;
}