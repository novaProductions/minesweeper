#include <iostream>
#include <windows.h>

#include <map>
#include <cstdlib> 
#include <ctime>
#include <vector>
#include <string>

#include "Tile.h"

using namespace std;


#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GAME_STATUS 4
#define NUM_BOMBS_DISPLAY 5
#define RESTART_GAME 600

HMENU hMenu;
HWND hEdit;
Tile tileBomb;
Tile tileNotBomb;
void AddMenus(HWND hWnd);
void AddControls(HWND hWnd);
map<string, Tile> board;
HWND gameStatus;
int numOfTilesClicked;
int rowCount = 3;
int colCount = 30;
int bombCount = 1;
bool isDead = false;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void resizeMainWindow(HWND hWnd) {
	int newWindowWidth = 35 + colCount * 25;
	if (newWindowWidth <= 285) {
		newWindowWidth = 285;
	}

	SetWindowPos(hWnd, 0, 0, 0, newWindowWidth, 130 + rowCount * 25, SWP_SHOWWINDOW | SWP_NOMOVE);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreventInst, LPSTR args, int ncmdshow) {

	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	HWND mainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	resizeMainWindow(mainWindow);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void TriggerActions(int tileId) {
	Tile tile = board[to_string(tileId)];

	HWND btn = tile.getBtn();
	bool isBomb = tile.getIsBomb();
	wchar_t numOfBombsAround[256];
	wsprintfW(numOfBombsAround, L"%d", tile.getNumOfBombsAround());
	bool bombNotTriggered = true;
	if (isBomb) {
		SetWindowTextW(btn, L"B");
		SetWindowTextW(gameStatus, L"BOOM!!! Try Again");
		isDead = true;
	}
	else {
		SetWindowTextW(btn, numOfBombsAround);
	}

	numOfTilesClicked++;

	if (numOfTilesClicked == (rowCount * colCount) - bombCount && isDead == false) {
		SetWindowTextW(gameStatus, L"You Win!!");
	}
}


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (wp == RESTART_GAME) {
			numOfTilesClicked = 0;
			isDead = false;
			for (map<string, Tile>::iterator iter = board.begin(); iter != board.end(); ++iter)
			{
				Tile tile = iter->second;
				DestroyWindow(tile.getBtn());
			}
			board = map<string, Tile>();

			resizeMainWindow(hWnd);
			AddControls(hWnd);
			break;
		}
		TriggerActions(wp);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}


void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, RESTART_GAME, L"Retry");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open SubMent");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	

	SetMenu(hWnd, hMenu);
}

map<string, int> determineIfBombShouldCreated(int bombCount, int rowCount, int colCount) {
	map<string, int> tilesWithBombs;

	srand(time(NULL));
	while (tilesWithBombs.size() < bombCount) {
		
		int rowRand = rand() % rowCount;
		int colRand = rand() % colCount;
		string choosenTile = "9" + to_string(rowRand) + to_string(colRand);
		if (tilesWithBombs.count(choosenTile) == 0) {
			tilesWithBombs.insert(pair<string, int>(choosenTile, 0));
		}
		
	}

	return tilesWithBombs;
}

int calNumOfBombsAround(map<string, int> tilesWithBombs, int row, int column) {
	int numOfBombsAround = 0;

	vector<string> adjacentTiles;

	if (row > 0) {
		
		string southeast = "9" + to_string(row - 1) + to_string(column + 1);
		string south = "9" + to_string(row - 1) + to_string(column);

		adjacentTiles.push_back(southeast);
		adjacentTiles.push_back(south);
		
	}

	if (column > 0) {
		
		string northwest = "9" + to_string(row + 1) + to_string(column - 1);
		string west = "9" + to_string(row) + to_string(column - 1);
		
		adjacentTiles.push_back(northwest);
		adjacentTiles.push_back(west);
	}

	if (row > 0 && column > 0) {
		string southwest = "9" + to_string(row - 1) + to_string(column - 1);
		adjacentTiles.push_back(southwest);
	}

	string north = "9" + to_string(row + 1) + to_string(column);
	string northeast = "9" + to_string(row + 1) + to_string(column + 1);
	string east = "9" + to_string(row) + to_string(column + 1);

	adjacentTiles.push_back(north);
	adjacentTiles.push_back(northeast);
	adjacentTiles.push_back(east);
	
	
	for (int i = 0; i < adjacentTiles.size(); ++i) {
		if (tilesWithBombs.count(adjacentTiles[i]) == 1) {
			numOfBombsAround++;
		}
	}

	return numOfBombsAround;
}

void AddControls(HWND hWnd) {
	

	wstring bombDisplay = L"Bombs: ";
	bombDisplay += to_wstring(bombCount);
	
	gameStatus = CreateWindowW(L"static", L"You are still alive ...for now", WS_VISIBLE | WS_CHILD, 10, 10, 200, 24, hWnd, (HMENU)GAME_STATUS, NULL, NULL);
	CreateWindowW(L"static", bombDisplay.data(), WS_VISIBLE | WS_CHILD, 10, 40, 100, 24, hWnd, (HMENU)NUM_BOMBS_DISPLAY, NULL, NULL);


	map<string, int> tilesWithBombs = determineIfBombShouldCreated(bombCount, rowCount, colCount);

	for (int row = 0; row < rowCount; row++) {
		for (int column = 0; column < colCount; column++) {
			string tileIdStr = "9" + to_string(row) + to_string(column);
			int tileId = atoi(tileIdStr.c_str());
			HWND btnBomb = CreateWindowW(L"button", NULL, WS_VISIBLE | WS_CHILD, 10 + column * 25, 60 + row * 25, 24, 24, hWnd, (HMENU)tileId, NULL, NULL);
			
			int numOfBombsAround = calNumOfBombsAround(tilesWithBombs, row, column);
			Tile tile1 = Tile(false, btnBomb, numOfBombsAround);
			tile1.setNumOfBombsAround(numOfBombsAround);
			if (tilesWithBombs.count(tileIdStr) == 1) {
				tile1.setIsBomb(true);
			}
			
			board.insert(pair<string, Tile>(tileIdStr, tile1));
		}
	}

}