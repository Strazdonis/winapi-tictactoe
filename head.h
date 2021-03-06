#include <iostream>
#include <fstream>
#include <gdiplus.h>
#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED
using namespace std;
enum Tiles {Blank, X, O};
Tiles playerTurn = X;
Tiles GameBoard[3][3] =
	{{Blank, Blank, Blank},
	{Blank, Blank, Blank},
	{Blank, Blank, Blank}};
const int windowWidth = 600;
const int lineLen = windowWidth / 3;

HICON XIcon = (HICON) LoadImage(GetModuleHandle(NULL), TEXT("X_ICON"), IMAGE_ICON, 256, 256, 0);
HICON OIcon = (HICON) LoadImage(GetModuleHandle(NULL), TEXT("O_ICON"), IMAGE_ICON, 256, 256, 0);

void DrawBoard(HDC hdc, int windowWidth, int lineLen)
{
	HPEN hPenOld;
	// Draw the board lines
	HPEN hLinePen;
	COLORREF qLineColor;
	qLineColor = RGB(0, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Draw two vertical lines
	for (int i = lineLen; i < windowWidth; i += lineLen) {
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, windowWidth);
	}
	// Draw two horizontal lines
	for (int j = lineLen; j < windowWidth; j += lineLen) {
		MoveToEx(hdc, 0, j, NULL);
		LineTo(hdc, windowWidth, j);
	}

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void DrawImage(HDC hdc, int X, int Y, HICON hIcon)
{
    int left = windowWidth / 3 * X + 10;
    int top = windowWidth / 3 * Y + 10;
    DrawIconEx(hdc, left, top, hIcon, 180, 180, 0, NULL, DI_NORMAL);
}

bool ValidateMove(LPARAM lParam, Tiles mover, int lineLen) {
	int posX = LOWORD(lParam);
	int posY = HIWORD(lParam);
	int sqX = (posX/lineLen);
	int sqY = (posY/lineLen);
    if ((sqX < 3) && (sqY < 3) &&
		(GameBoard[sqX][sqY]) == Blank)
	{
		GameBoard[sqX][sqY] = mover;
		return true;
	}
	return false;
}

bool HasWon() {
	// Check for win through upper left
	if (GameBoard[0][0] != Blank) {
		if (GameBoard[0][1] == GameBoard[0][0] &&
			GameBoard[0][2] == GameBoard[0][0])
		{
			return true;
		}
		if (GameBoard[1][0] == GameBoard[0][0] &&
			GameBoard[2][0] == GameBoard[0][0])
		{
			return true;
		}
	}
	// Check for win through center
	if (GameBoard[1][1] != Blank) {
		if (GameBoard[0][0] == GameBoard[1][1] &&
			GameBoard[2][2] == GameBoard[1][1])
		{
			return true;
		}
		if (GameBoard[0][2] == GameBoard[1][1] &&
			GameBoard[2][0] == GameBoard[1][1])
		{
			return true;
		}
		if (GameBoard[1][0] == GameBoard[1][1] &&
			GameBoard[1][2] == GameBoard[1][1])
		{
			return true;
		}
		if (GameBoard[0][1] == GameBoard[1][1] &&
			GameBoard[2][1] == GameBoard[1][1])
		{
			return true;
		}
	}
	// Check for win through lower right
	if (GameBoard[2][2] != Blank) {
		if (GameBoard[2][0] == GameBoard[2][2] &&
			GameBoard[2][1] == GameBoard[2][2])
		{
			return true;
		}
		if (GameBoard[0][2] == GameBoard[2][2] &&
			GameBoard[1][2] == GameBoard[2][2])
		{
			return true;
		}
	}
	return false;
}

bool IsBoardFull()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (GameBoard[i][j] == Blank) {
				return false;
			}
		}
	}
	return true;
}

void ResetGame()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			GameBoard[i][j] = Blank;
		}
	}
	playerTurn = X;
}


void SaveToFile(string msg) {
    ofstream file;
    file.open ("hiscores.txt", ios::app);
    file << msg << endl;
    file.close();
}

#endif // HEAD_H_INCLUDED
