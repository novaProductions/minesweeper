#include <iostream>
#include <windows.h>
#include "HelloWorld.h"
#include "HelloWorldClass.h"
#include "Tile.h"
#include <map>
#include <cstdlib> 
#include <ctime>
#include <vector>
using namespace std;


string samplePrint() {
	return "Hello World 2!";
}
/*
int main()
{
	cout << "Hello World!" << endl;;
	cout << samplePrint() << endl;;
	cout << printHelloWorld() << endl;;
	HelloWorldClass helloWorldClass = *new HelloWorldClass();

	HelloWorldClass board = *new HelloWorldClass(4);
	cout << board.getNum() << endl;
	board.setNum(6);
	cout << board.getNum() << endl;
	delete& board;

}
*/

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define CHANGE_TITLE 4
#define TRIGGER_TILE_00 5
#define TRIGGER_TILE_01 6
#define TRIGGER_TILE_02 7
#define TRIGGER_TILE_10 8
#define TRIGGER_TILE_12 9
#define TRIGGER_TILE_20 10
#define TRIGGER_TILE_21 11
#define TRIGGER_TILE_22 12

HMENU hMenu;
HWND hEdit;
Tile tileBomb;
Tile tileNotBomb;
void AddMenus(HWND hWnd);
void AddControls(HWND hWnd);
map<string, Tile> board;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreventInst, LPSTR args, int ncmdshow) {

	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//MessageBox(NULL, L"Sample Box", L"Sub text", MB_OK);
	return 0;
}

void TriggerActions(int tileId) {
	Tile tile = board[to_string(tileId)];

	HWND btn = tile.getBtn();
	bool isBomb = tile.getIsBomb();
	wchar_t numOfBombsAround[256];
	wsprintfW(numOfBombsAround, L"%d", tile.getNumOfBombsAround());
	if (isBomb) {
		SetWindowTextW(btn, L"B");
	}
	else {
		SetWindowTextW(btn, numOfBombsAround);
	}
}


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp) 
		{
		case FILE_MENU_NEW:
			MessageBeep(MB_OK);
			break;
		case FILE_MENU_OPEN:
			MessageBeep(MB_OK);
			break;
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case CHANGE_TITLE:
			wchar_t text[100];
			GetWindowTextW(hEdit, text, 100);
			SetWindowTextW(hWnd, text);
			break;
		default:
			TriggerActions(wp);
		}
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}


void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, CHANGE_TITLE, L"Change Title");

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
	
	int rowCount = 3;
	int colCount = 3;
	int bombCount = 3;
	map<string, int> tilesWithBombs = determineIfBombShouldCreated(bombCount, rowCount, colCount);

	for (int row = 0; row < rowCount; row++) {
		for (int column = 0; column < colCount; column++) {
			string tileIdStr = "9" + to_string(row) + to_string(column);
			int tileId = atoi(tileIdStr.c_str());
			HWND btnBomb = CreateWindowW(L"button", NULL, WS_VISIBLE | WS_CHILD, 10 + column * 25, 10 + row * 25, 24, 24, hWnd, (HMENU)tileId, NULL, NULL);
			
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