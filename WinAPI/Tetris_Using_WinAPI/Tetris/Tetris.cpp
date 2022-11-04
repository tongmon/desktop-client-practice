// Tetris made by tongstar
#include <windows.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "resource.h"
using namespace std;

#define bricksize 40

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK BrickDown(HWND hwnd, UINT msg, UINT event, DWORD dwTime);

enum { Linear = 1, Mid, LEdge, REdge, LZig, RZig, Square };
enum diff { Easy = 500, Normal = 300, Hard = 100, Insane = 60, Crazy = 30 };

class Line // 테트리스 한줄
{
	int x[10] = { 0, };
public:
	Line(int a, int b, int c) // 몇 번째 부터 몇 개를 무슨 숫자로
	{
		for (int k = a; k < b; k++)
		{
			x[k] = c;
		}
	}
	int Checker() // 테트리스의 한 줄이 모두 꽉 차있는지 검사. 꽉 차있다면 10, 안 차있는 열이면 10 이하  
	{
		int count = 0;
		for (int i = 0; i < 10; i++)
		{
			if (x[i] != 0)
			{
				count++;
			}
		}
		return count;
	}
	int GetLine(int a) { return x[a]; }
	void ModLine(int a, int b)
	{
		x[a] = b;
	}
};

typedef struct _Cord // 테트리스판 좌표계
{
	int x; int y;
} Cordinate;

class Brick // 테트리스 블록
{
public:
	int stance;
	Cordinate XY[4];
	Brick()
	{
		stance = 1;
		XY[0].x = 0; XY[0].y = 0;
		XY[1].x = 0; XY[1].y = 0;
		XY[2].x = 0; XY[2].y = 0;
		XY[3].x = 0; XY[3].y = 0;
	}
	void Reset(int a)
	{
		stance = 1;
		if (a == Linear)
		{
			XY[0].x = XY[1].x = XY[2].x = XY[3].x = 19;
			XY[0].y = 4; XY[1].y = 3; XY[2].y = 5; XY[3].y = 6;
		}
		else if (a == Mid)
		{
			XY[0].x = 19; XY[0].y = 5;
			XY[1].x = 19; XY[1].y = 4;
			XY[2].x = 19; XY[2].y = 6;
			XY[3].x = 18; XY[3].y = 5;
		}
		else if (a == LEdge)
		{
			XY[0].x = 18; XY[0].y = 5;
			XY[1].x = 18; XY[1].y = 4;
			XY[2].x = 18; XY[2].y = 6;
			XY[3].x = 19; XY[3].y = 4;
		}
		else if (a == REdge)
		{
			XY[0].x = 18; XY[0].y = 5;
			XY[1].x = 18; XY[1].y = 4;
			XY[2].x = 18; XY[2].y = 6;
			XY[3].x = 19; XY[3].y = 6;
		}
		else if (a == LZig)
		{
			XY[0].x = 18; XY[0].y = 5;
			XY[1].x = 18; XY[1].y = 4;
			XY[2].x = 19; XY[2].y = 5;
			XY[3].x = 19; XY[3].y = 6;
		}
		else if (a == RZig)
		{
			XY[0].x = 18; XY[0].y = 5;
			XY[1].x = 19; XY[1].y = 4;
			XY[2].x = 19; XY[2].y = 5;
			XY[3].x = 18; XY[3].y = 6;
		}
		else
		{
			stance = -1;
			XY[0].x = 19; XY[0].y = 4;
			XY[1].x = 19; XY[1].y = 5;
			XY[2].x = 18; XY[2].y = 4;
			XY[3].x = 18; XY[3].y = 5;
		}
	}
	void Turn(int a)
	{
		if (a == Linear)
		{
			if (stance == 1)
			{
				XY[1].y = XY[2].y = XY[3].y = XY[0].y;
				XY[1].x = XY[0].x + 1;
				XY[2].x = XY[0].x - 1;
				XY[3].x = XY[0].x - 2;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[2].x = XY[3].x = XY[0].x;
				XY[1].y = XY[0].y + 1;
				XY[2].y = XY[0].y - 1;
				XY[3].y = XY[0].y - 2;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].y = XY[2].y = XY[3].y = XY[0].y;
				XY[1].x = XY[0].x - 1;
				XY[2].x = XY[0].x + 1;
				XY[3].x = XY[0].x + 2;
				stance++;
			}
			else
			{
				XY[1].x = XY[2].x = XY[3].x = XY[0].x;
				XY[1].y = XY[0].y - 1;
				XY[2].y = XY[0].y + 1;
				XY[3].y = XY[0].y + 2;
				stance = 1;
			}
		}
		else if (a == Mid)
		{
			if (stance == 1)
			{
				XY[1].x = XY[0].x + 1; XY[1].y = XY[0].y;
				XY[2].x = XY[0].x - 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x; XY[3].y = XY[0].y - 1;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[0].x; XY[1].y = XY[0].y + 1;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y - 1;
				XY[3].x = XY[0].x + 1; XY[3].y = XY[0].y;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].x = XY[0].x - 1; XY[1].y = XY[0].y;
				XY[2].x = XY[0].x + 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x; XY[3].y = XY[0].y + 1;
				stance++;
			}
			else
			{
				XY[1].x = XY[0].x; XY[1].y = XY[0].y - 1;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y + 1;
				XY[3].x = XY[0].x - 1; XY[3].y = XY[0].y;
				stance = 1;
			}
		}
		else if (a == LEdge)
		{
			if (stance == 1)
			{
				XY[1].y = XY[2].y = XY[0].y;
				XY[3].y = XY[0].y + 1;
				XY[1].x = XY[0].x + 1;
				XY[2].x = XY[0].x - 1;
				XY[3].x = XY[0].x + 1;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[2].x = XY[0].x;
				XY[3].x = XY[0].x - 1;
				XY[1].y = XY[0].y + 1;
				XY[2].y = XY[0].y - 1;
				XY[3].y = XY[0].y + 1;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].y = XY[2].y = XY[0].y;
				XY[3].y = XY[0].y - 1;
				XY[1].x = XY[0].x - 1;
				XY[2].x = XY[0].x + 1;
				XY[3].x = XY[0].x - 1;
				stance++;
			}
			else
			{
				XY[1].x = XY[2].x = XY[0].x;
				XY[3].x = XY[0].x + 1;
				XY[1].y = XY[0].y - 1;
				XY[2].y = XY[0].y + 1;
				XY[3].y = XY[0].y - 1;
				stance = 1;
			}
		}
		else if (a == REdge)
		{
			if (stance == 1)
			{
				XY[1].y = XY[2].y = XY[0].y;
				XY[3].y = XY[0].y + 1;
				XY[1].x = XY[0].x + 1;
				XY[2].x = XY[0].x - 1;
				XY[3].x = XY[0].x - 1;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[2].x = XY[0].x;
				XY[3].x = XY[0].x - 1;
				XY[1].y = XY[0].y + 1;
				XY[2].y = XY[0].y - 1;
				XY[3].y = XY[0].y - 1;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].y = XY[2].y = XY[0].y;
				XY[3].y = XY[0].y - 1;
				XY[1].x = XY[0].x - 1;
				XY[2].x = XY[0].x + 1;
				XY[3].x = XY[0].x + 1;
				stance++;
			}
			else
			{
				XY[1].x = XY[2].x = XY[0].x;
				XY[3].x = XY[0].x + 1;
				XY[1].y = XY[0].y - 1;
				XY[2].y = XY[0].y + 1;
				XY[3].y = XY[0].y + 1;
				stance = 1;
			}
		}
		else if (a == LZig)
		{
			if (stance == 1)
			{
				XY[1].x = XY[0].x + 1; XY[1].y = XY[0].y;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y + 1;
				XY[3].x = XY[0].x - 1; XY[3].y = XY[0].y + 1;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[0].x; XY[1].y = XY[0].y + 1;
				XY[2].x = XY[0].x - 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x - 1; XY[3].y = XY[0].y - 1;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].x = XY[0].x - 1; XY[1].y = XY[0].y;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y - 1;
				XY[3].x = XY[0].x + 1; XY[3].y = XY[0].y - 1;
				stance++;
			}
			else
			{
				XY[1].x = XY[0].x; XY[1].y = XY[0].y - 1;
				XY[2].x = XY[0].x + 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x + 1; XY[3].y = XY[0].y + 1;
				stance = 1;
			}
		}
		else if (a == RZig)
		{
			if (stance == 1)
			{
				XY[1].x = XY[0].x + 1; XY[1].y = XY[0].y + 1;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y + 1;
				XY[3].x = XY[0].x - 1; XY[3].y = XY[0].y;
				stance++;
			}
			else if (stance == 2)
			{
				XY[1].x = XY[0].x - 1; XY[1].y = XY[0].y + 1;
				XY[2].x = XY[0].x - 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x; XY[3].y = XY[0].y - 1;
				stance++;
			}
			else if (stance == 3)
			{
				XY[1].x = XY[0].x - 1; XY[1].y = XY[0].y - 1;
				XY[2].x = XY[0].x; XY[2].y = XY[0].y - 1;
				XY[3].x = XY[0].x + 1; XY[3].y = XY[0].y;
				stance++;
			}
			else
			{
				XY[1].x = XY[0].x + 1; XY[1].y = XY[0].y - 1;
				XY[2].x = XY[0].x + 1; XY[2].y = XY[0].y;
				XY[3].x = XY[0].x; XY[3].y = XY[0].y + 1;
				stance = 1;
			}
		}
	}
	void Up()
	{
		for (int i = 0; i < 4; i++)
		{
			XY[i].x = XY[i].x + 1;
		}
	}
	void Down()
	{
		for (int i = 0; i < 4; i++)
		{
			XY[i].x = XY[i].x - 1;
		}
	}
	void Right()
	{
		for (int i = 0; i < 4; i++)
		{
			XY[i].y = XY[i].y + 1;
		}
	}
	void Left()
	{
		for (int i = 0; i < 4; i++)
		{
			XY[i].y = XY[i].y - 1;
		}
	}
	void copy(Brick a)
	{
		stance = a.stance;
		for (int i = 0; i < 4; i++)
		{
			XY[i].x = a.XY[i].x;
			XY[i].y = a.XY[i].y;
		}
	}
};

LPCTSTR lpszClass = TEXT("Tetris made by tongstar");
RECT cwin; // 윈도우 창 크기
HINSTANCE g_hins;
WNDCLASS WndClass;
HDC hdc;
HWND button1, button2, button3, button4, pause; // 각종 버튼들
Brick TetrisBrick;
Brick ShawdowBrick;
HBITMAP myBitmap;
vector<Line> Map; // 테트리스 판
vector<int> WaitList; // 다음 블록 대기열
int midx, midy, startx, starty, counter = 4; // 화면 중심 좌표
short starter; // 테트리스 게임 시작인지 아닌지 0이면 pause, 1이면 시작
bool turn = true, holdflag = true; // turn은 블록 한개를 맞추면 ture로 전환, holdflag는 홀드를 한 턴에 한번만 가능하게
int Diffary[5] = { Easy , Normal ,Hard, Insane, Crazy }; // 난이도
int Diffindex = 0;
int holdbuffer;
int score = 0; // 점수
int linesum = 0; // 지워진 라인
int timer = 0; // 난이도 조정용 시간 체크
static short restart;

void randnum(int* num, int size)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < size; i++)
	{
		*(num + i) = i + 1;
	}
	int tmp, a, b;
	for (int i = 0; i < (size * 2); i++)
	{
		a = rand() % size;
		b = rand() % size;
		if (a != b)
		{
			tmp = *(num + a);
			*(num + a) = *(num + b);
			*(num + b) = tmp;
		}
	}
}

bool Vaildater(Cordinate* Cord) // 블록 충돌 검사
{
	for (int i = 0; i < 4; i++)
	{
		if (Cord[i].x < 0 || Cord[i].x > 19)
		{
			return false;
		}
		if (Cord[i].y < 0 || Cord[i].y > 9)
		{
			return false;
		}
		if (Map[Cord[i].x].GetLine(Cord[i].y) != 0)
		{
			return false;
		}
	}
	return true;
}

bool Shadowater(Cordinate* Cord, vector<int>* index) // shadow 블록 충돌 검사
{
	for (int i = 0; i < 4; i++)
	{
		index->push_back(i);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (TetrisBrick.XY[i].x == ShawdowBrick.XY[j].x)
			{
				if (TetrisBrick.XY[i].y == ShawdowBrick.XY[j].y)
				{
					for (int k = 0; k < index->size(); k++)
					{
						if (index->at(k) == j)
						{
							index->erase(index->begin() + k);
							break;
						}
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < index->size(); i++)
	{
		int k = index->at(i);
		if (Cord[k].x < 0 || Cord[k].x > 19)
		{
			return false;
		}
		if (Cord[k].y < 0 || Cord[k].y > 9)
		{
			return false;
		}
		if (Map[Cord[k].x].GetLine(Cord[k].y) != 0)
		{
			return false;
		}
	}
	return true;
}

void DrawBrick(HDC hdc, int x, int y, HBITMAP hBit, int bricktype)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);
	GetObject(hBit, sizeof(BITMAP), &bit);

	if (bricktype == -1)
	{
		BitBlt(hdc, x, y, bricksize, bricksize, MemDC, bricksize * 8, 0, SRCCOPY);
	}
	else if (bricktype == -2) // shadow 블록 미리보이기
	{
		BitBlt(hdc, x, y, bricksize, bricksize, MemDC, bricksize * 9, 0, SRCCOPY);
	}
	else
	{
		BitBlt(hdc, x, y, bricksize, bricksize, MemDC, bricksize * bricktype, 0, SRCCOPY);
	}

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void Shadow(HDC hdc, int k)
{
	vector<int> index;
	if (k == 1)
	{
		while (1)
		{
			if (Shadowater(ShawdowBrick.XY, &index) == false)
			{
				ShawdowBrick.Up();
				for (int i = 0; i < index.size(); i++)
				{
					DrawBrick(hdc, startx + ShawdowBrick.XY[index[i]].y * bricksize, starty + (19 - ShawdowBrick.XY[index[i]].x) * bricksize, myBitmap, -2);
				}
				return;
			}
			else
			{
				ShawdowBrick.Down();
				index.clear();
			}
		}
	}
	else
	{
		for (int p = 0; p < 4; p++) // 그림자 블록 지우고 원상 화면 복귀
		{
			DrawBrick(hdc, startx + ShawdowBrick.XY[p].y * bricksize, starty + (19 - ShawdowBrick.XY[p].x) * bricksize, myBitmap, -1);
		}
	}
}

void Brickprint(HDC hdc, Cordinate* cord, int mode) // 특정 블록 그리기
{
	for (int i = 0; i < 4; i++)
	{
		DrawBrick(hdc, startx + cord[i].y * bricksize, starty + (19 - cord[i].x) * bricksize, myBitmap, mode);
	}
}

void DrawBorder(HDC hdc) // 외각 그리기
{
	int startx = midx - bricksize * 6, starty = midy - bricksize * 11;
	for (int i = 1; i < 11; i++)
	{
		DrawBrick(hdc, startx + i * bricksize, starty, myBitmap, 0);
		DrawBrick(hdc, startx + i * bricksize, midy + bricksize * 10, myBitmap, 0);
	}
	for (int i = 0; i < 22; i++)
	{
		DrawBrick(hdc, startx, starty + i * bricksize, myBitmap, 0);
		DrawBrick(hdc, midx + bricksize * 5, starty + i * bricksize, myBitmap, 0);
	}
}

void Redraw(HDC hdc, HBITMAP hBit) // 테트리스 내부 그리기
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Map[i].GetLine(j) == 0)
			{
				DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, hBit, -1);
			}
			else
			{
				DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, hBit, Map[i].GetLine(j));
			}
		}
	}
	DrawBorder(hdc);
}

void PartialRedraw(HDC hdc, int x, int y, int dx, int dy, int mode)
{
	for (int i = y; i <= dy; i++)
	{
		for (int j = x; j <= dx; j++)
		{
			if (mode == 0) // default 모드 걍 Map대로 다시 그리기
			{
				if (Map[i].GetLine(j) == 0)
				{
					DrawBrick(hdc, startx + j * bricksize, starty + i * bricksize, myBitmap, -1);
				}
				else
				{
					DrawBrick(hdc, startx + j * bricksize, starty + i * bricksize, myBitmap, Map[i].GetLine(j));
				}
			}
			else if (mode == 1)
			{
				DrawBrick(hdc, startx + j * bricksize, starty + i * bricksize, myBitmap, -1);
			}
		}
	}
}

void MoveBrick(HDC hdc, int key)
{
	int flag = -1;
	for (int i = 0; i < 4; i++)
	{
		Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, 0);
	}

	if (key == 1)
	{
		TetrisBrick.Left();
	}
	else if (key == 2)
	{
		TetrisBrick.Right();
	}
	else
	{
		if (WaitList[0] == Linear)
		{
			if ((TetrisBrick.XY[0].y == 0 && TetrisBrick.stance == 4) || (TetrisBrick.XY[0].y == 1 && TetrisBrick.stance == 2))
			{
				flag = 1;
				TetrisBrick.Right();
			}
			else if (TetrisBrick.XY[0].y == 0 && TetrisBrick.stance == 2)
			{
				flag = 3;
				TetrisBrick.Right(); TetrisBrick.Right();
			}
			else if (TetrisBrick.XY[0].y == 9 && TetrisBrick.stance == 4)
			{
				flag = 4;
				TetrisBrick.Left(); TetrisBrick.Left();
			}
			else if ((TetrisBrick.XY[0].y == 9 && TetrisBrick.stance == 2) || (TetrisBrick.XY[0].y == 8 && TetrisBrick.stance == 4))
			{
				flag = 2;
				TetrisBrick.Left();
			}
			else if (TetrisBrick.XY[0].x == 0 && TetrisBrick.stance == 2)
			{
				flag = 5;
				TetrisBrick.Down(); TetrisBrick.Down();
			}
			else if ((TetrisBrick.XY[0].x == 1 && TetrisBrick.stance == 2) && (TetrisBrick.XY[0].x == 0 && TetrisBrick.stance == 4))
			{
				flag = 6;
				TetrisBrick.Down();
			}
		}
		else if (TetrisBrick.XY[0].y == 0)
		{
			flag = 1;
			TetrisBrick.Right();
		}
		else if (TetrisBrick.XY[0].y == 9)
		{
			flag = 2;
			TetrisBrick.Left();
		}
		TetrisBrick.Turn(WaitList[0]);
	}

	if (Vaildater(TetrisBrick.XY) == false) // 유효하지 않다면
	{
		if (key == 1)
		{
			TetrisBrick.Right();
		}
		else if (key == 2)
		{
			TetrisBrick.Left();
		}
		else // 원상복귀
		{
			if (flag == 1)
			{
				TetrisBrick.Left();
			}
			else if (flag == 2)
			{
				TetrisBrick.Right();
			}
			else if (flag == 3)
			{
				TetrisBrick.Left(); TetrisBrick.Left();
			}
			else if (flag == 4)
			{
				TetrisBrick.Right(); TetrisBrick.Right();
			}
			else if (flag == 5)
			{
				TetrisBrick.Up(); TetrisBrick.Up();
			}
			else if (flag == 6)
			{
				TetrisBrick.Up();
			}
			TetrisBrick.Turn(WaitList[0]);
			TetrisBrick.Turn(WaitList[0]);
			TetrisBrick.Turn(WaitList[0]);
		}
		for (int i = 0; i < 4; i++)
		{
			Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
		}
	}
	else
	{
		if (key == 1)
		{
			TetrisBrick.Right();
		}
		else if (key == 2)
		{
			TetrisBrick.Left();
		}
		else
		{
			if (flag == 1)
			{
				TetrisBrick.Left();
			}
			else if (flag == 2)
			{
				TetrisBrick.Right();
			}
			else if (flag == 3)
			{
				TetrisBrick.Left(); TetrisBrick.Left();
			}
			else if (flag == 4)
			{
				TetrisBrick.Right(); TetrisBrick.Right();
			}
			else if (flag == 5)
			{
				TetrisBrick.Up(); TetrisBrick.Up();
			}
			else if (flag == 6)
			{
				TetrisBrick.Up();
			}
			TetrisBrick.Turn(WaitList[0]);
			TetrisBrick.Turn(WaitList[0]);
			TetrisBrick.Turn(WaitList[0]);
		}

		for (int i = 0; i < 4; i++)
		{
			DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, -1);
		}

		if (key == 1)
		{
			TetrisBrick.Left();
		}
		else if (key == 2)
		{
			TetrisBrick.Right();
		}
		else
		{
			if (flag == 1)
			{
				TetrisBrick.Right();
			}
			else if (flag == 2)
			{
				TetrisBrick.Left();
			}
			else if (flag == 3)
			{
				TetrisBrick.Right(); TetrisBrick.Right();
			}
			else if (flag == 4)
			{
				TetrisBrick.Left(); TetrisBrick.Left();
			}
			else if (flag == 5)
			{
				TetrisBrick.Down(); TetrisBrick.Down();
			}
			else if (flag == 6)
			{
				TetrisBrick.Down();
			}
			TetrisBrick.Turn(WaitList[0]);
		}

		for (int i = 0; i < 4; i++)
		{
			Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
			DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, WaitList[0]);
		}

		if (key == 1)
		{
			for (int i = TetrisBrick.XY[0].x, k = 0; k < 2; i++, k++)
			{
				for (int j = TetrisBrick.XY[0].y, l = 0; l < 3; j++, l++)
				{
					if (i < 20 && i >= 0 && j < 10 && j >= 0)
					{
						if (Map[i].GetLine(j) == 0)
						{
							DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, myBitmap, -1);
						}
						else
						{
							DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, myBitmap, Map[i].GetLine(j));
						}
					}
				}
			}
		}
		else if (key == 2)
		{
			for (int i = TetrisBrick.XY[0].x, k = 0; k < 2; i++, k++)
			{
				for (int j = TetrisBrick.XY[0].y, l = 0; l < 3; j--, l++)
				{
					if (i < 20 && i >= 0 && j < 10 && j >= 0)
					{
						if (Map[i].GetLine(j) == 0)
						{
							DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, myBitmap, -1);
						}
						else
						{
							DrawBrick(hdc, startx + j * bricksize, starty + (19 - i) * bricksize, myBitmap, Map[i].GetLine(j));
						}
					}
				}
			}
		}
	}
}

void NextPrint(HDC hdc, int mode)
{
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
	HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc, midx + bricksize * 7 + bricksize / 2, starty + bricksize, "NEXT", lstrlen("NEXT"));
	SelectObject(hdc, oldfont);
	DeleteObject(hFont);
	PartialRedraw(hdc, 12, 3, 15, 12, 1);
	if (mode == 0)
	{
		return;
	}
	Brick bufbrick;
	for (int i = 1; i < 4; i++)
	{
		bufbrick.Reset(WaitList[i]);
		if (WaitList[i] != Mid)
		{
			bufbrick.XY[0].x = starty + bricksize * 12 - (4 - i) * bricksize * 3 + 2 * bricksize;
		}
		else
		{
			bufbrick.XY[0].x = starty + bricksize * 12 - (4 - i) * bricksize * 3 + bricksize;
		}
		bufbrick.XY[0].y = midx + bricksize * 9;
		if (WaitList[i] != Square)
		{
			bufbrick.Turn(WaitList[i]);
			bufbrick.Turn(WaitList[i]);
			if (WaitList[i] != Linear)
			{
				bufbrick.Turn(WaitList[i]);
				bufbrick.Turn(WaitList[i]);
			}
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x, myBitmap, WaitList[i]);
			for (int j = 1; j < 4; j++)
			{
				int xgap = bufbrick.XY[0].x - bufbrick.XY[j].x;
				int ygap = bufbrick.XY[0].y - bufbrick.XY[j].y;
				DrawBrick(hdc, bufbrick.XY[0].y - ygap * bricksize, bufbrick.XY[0].x + xgap * bricksize, myBitmap, WaitList[i]);
			}
		}
		else
		{
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x, myBitmap, WaitList[i]);
			DrawBrick(hdc, bufbrick.XY[0].y - bricksize, bufbrick.XY[0].x - bricksize, myBitmap, WaitList[i]);
			DrawBrick(hdc, bufbrick.XY[0].y - bricksize, bufbrick.XY[0].x, myBitmap, WaitList[i]);
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x - bricksize, myBitmap, WaitList[i]);
		}
	}
}

void HoldPrint(HDC hdc, int mode)
{
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
	HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
	TextOut(hdc, startx - bricksize * 6 + 10, starty + bricksize, "HOLD", lstrlen("HOLD"));
	SelectObject(hdc, oldfont);
	DeleteObject(hFont);
	PartialRedraw(hdc, -6, 3, -3, 6, 1);
	if (mode == 0)
	{
		return;
	}
	Brick bufbrick;
	if (holdbuffer != -1)
	{
		bufbrick.Reset(holdbuffer);
		if (holdbuffer != Mid)
		{
			bufbrick.XY[0].x = starty + 5 * bricksize;
		}
		else
		{
			bufbrick.XY[0].x = starty + 4 * bricksize;
		}
		bufbrick.XY[0].y = midx - 9 * bricksize;
		if (holdbuffer != Square)
		{
			bufbrick.Turn(holdbuffer);
			bufbrick.Turn(holdbuffer);
			if (holdbuffer != Linear)
			{
				bufbrick.Turn(holdbuffer);
				bufbrick.Turn(holdbuffer);
			}
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x, myBitmap, holdbuffer);
			for (int j = 1; j < 4; j++)
			{
				int xgap = bufbrick.XY[0].x - bufbrick.XY[j].x;
				int ygap = bufbrick.XY[0].y - bufbrick.XY[j].y;
				DrawBrick(hdc, bufbrick.XY[0].y - ygap * bricksize, bufbrick.XY[0].x + xgap * bricksize, myBitmap, holdbuffer);
			}
		}
		else
		{
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x, myBitmap, holdbuffer);
			DrawBrick(hdc, bufbrick.XY[0].y - bricksize, bufbrick.XY[0].x - bricksize, myBitmap, holdbuffer);
			DrawBrick(hdc, bufbrick.XY[0].y - bricksize, bufbrick.XY[0].x, myBitmap, holdbuffer);
			DrawBrick(hdc, bufbrick.XY[0].y, bufbrick.XY[0].x - bricksize, myBitmap, holdbuffer);
		}
	}
}

void ScorePrint(HDC hdc, int mode)
{
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
	HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
	if (mode == 1)
	{
		TextOut(hdc, startx - bricksize * 16, starty + bricksize * 12, "                       ", lstrlen("                       "));
		TextOut(hdc, startx - bricksize * 16, starty + bricksize * 12 + 60, "                       ", lstrlen("                       "));
		TextOut(hdc, startx - bricksize * 16, starty + bricksize * 12 + 120, "                       ", lstrlen("                       "));
	}
	else
	{
		char scorebuffer[256];
		char linesumbuffer[10];
		char levelbuffer[10];
		_itoa_s(score, scorebuffer, 10);
		_itoa_s(linesum, linesumbuffer, 10);
		TextOut(hdc, startx - bricksize * 13, starty + bricksize * 12, "Score: ", lstrlen("Score: "));
		TextOut(hdc, startx - bricksize * 9, starty + bricksize * 12, "                  ", lstrlen("                  "));
		TextOut(hdc, startx - bricksize * 9, starty + bricksize * 12, scorebuffer, lstrlen(scorebuffer));
		TextOut(hdc, startx - bricksize * 13, starty + bricksize * 12 + 60, "Line: ", lstrlen("Line: "));
		TextOut(hdc, startx - bricksize * 10, starty + bricksize * 12 + 60, "                    ", lstrlen("                    "));
		TextOut(hdc, startx - bricksize * 10, starty + bricksize * 12 + 60, linesumbuffer, lstrlen(linesumbuffer));
		TextOut(hdc, startx - bricksize * 13, starty + bricksize * 12 + 120, "Level: ", lstrlen("Level: "));
		if (Diffindex == 0)
		{
			strcpy_s(levelbuffer, "EASY");
		}
		else if (Diffindex == 1)
		{
			strcpy_s(levelbuffer, "NORMAL");
		}
		else if (Diffindex == 2)
		{
			strcpy_s(levelbuffer, "HARD");
		}
		else if (Diffindex == 3)
		{
			strcpy_s(levelbuffer, "INSANE");
		}
		else if (Diffindex == 4)
		{
			strcpy_s(levelbuffer, "CRAZY");
		}
		TextOut(hdc, startx - bricksize * 9, starty + bricksize * 12 + 120, "                  ", lstrlen("                  "));
		TextOut(hdc, startx - bricksize * 9, starty + bricksize * 12 + 120, levelbuffer, lstrlen(levelbuffer));
	}
	SelectObject(hdc, oldfont);
	DeleteObject(hFont);
}

void PushList()
{
	if (WaitList.size() < 4)
	{
		int bary[7]; randnum(bary, 7);
		for (int i = 0; i < 7; i++)
		{
			WaitList.push_back(bary[i]);
		}
	}
}

int GoverChecker(HWND hwnd, HDC hdc)
{
	short a = 0;
	for (int i = 3; i < 7; i++)
	{
		if (Map[19].GetLine(i) != 0) // gameover
		{
			KillTimer(hwnd, 2);
			for (int k = 0; k < 4; k++)
			{
				DrawBrick(hdc, startx + ShawdowBrick.XY[k].y * bricksize, starty + (19 - ShawdowBrick.XY[k].x) * bricksize, myBitmap, Map[19].GetLine(i));
			}
			ReleaseDC(hwnd, hdc);
			// 테트리스 맵 초기화
			Map.clear();
			WaitList.clear();
			PushList();
			for (int k = 0; k < 20; k++)
			{
				Map.push_back(Line(0, 10, 0));
			}
			char buffer[256];
			wsprintf(buffer, "최종 스코어: %d\n게임오버! 다시 도전하려면 '예', 메뉴 이동은 '아니요'를 누르세요.", score);
			Diffindex = timer = linesum = score = starter = 0;
			holdbuffer = -1; turn = true; holdflag = true;
			ShawdowBrick.Reset(WaitList[0]);
			if (MessageBox(hwnd, buffer, "Gameover", MB_YESNO) == IDYES)
			{
				restart = 1;
				SetTimer(hwnd, 1, 1000, NULL);
				SendMessage(hwnd, WM_TIMER, 1, 0);
			}
			else
			{
				DestroyWindow(pause);
				SendMessage(hwnd, WM_CREATE, 0, 0);
				InvalidateRect(hwnd, NULL, TRUE);
			}
			a = 1;
			return a;
		}
	}
	return a;
}

void Delline(HWND hwnd, HDC hdc)
{
	int k = -1, flag = 0, sum = 0;
	for (int i = 0; i < 4; i++) // 블록의 좌표중 가장 높은 높이를 구한다.
	{
		if (TetrisBrick.XY[i].x > k)
		{
			k = TetrisBrick.XY[i].x;
		}
	}
	for (int i = k, j = 0; j < 4; j++, i--)
	{
		if (i >= 0 && Map[i].Checker() == 10)
		{
			Map.erase(Map.begin() + i);
			Map.push_back(Line(0, 0, 0));
			flag = 1;
			linesum++;
			sum++;
		}
	}
	int a = GoverChecker(hwnd, hdc);
	if (a == 1)
	{
		return;
	}
	if (flag > 0)
	{
		Redraw(hdc, myBitmap);
	}
	holdflag = true; // 홀드 다시 가능하게
	if (sum == 1)
	{
		score = score + 100;
	}
	else if (sum == 2)
	{
		score = score + 300;
	}
	else if (sum == 3)
	{
		score = score + 500;
	}
	else if (sum == 4)
	{
		score = score + 1000;
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{ //szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
	HWND hwnd; //iCmdShow : 윈도우가 화면에 출력될 형태
	MSG msg;
	g_hins = hInstance;

	GetWindowRect(GetDesktopWindow(), &cwin); // 전체화면 구하기
	midx = (cwin.right - cwin.left) / 2;
	midy = (cwin.bottom - cwin.top) / 2;

	WndClass.style = CS_HREDRAW | CS_VREDRAW; //출력스타일 : 수직/수평의 변화시 다시 그림
	WndClass.lpfnWndProc = WndProc; //프로시저 함수명
	WndClass.cbClsExtra = 0; //O/S 사용 여분 메모리 (Class)
	WndClass.cbWndExtra = 0; //O/s 사용 여분 메모리 (Window)
	WndClass.hInstance = hInstance; //응용 프로그램 ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 유형
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 유형
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//배경색   
	WndClass.lpszMenuName = NULL; //메뉴 이름
	WndClass.lpszClassName = lpszClass; //클래스 이름
	RegisterClass(&WndClass); //앞서 정의한 윈도우 클래스의 주소

	// 테트리스 진행 메인 윈도우
	hwnd = CreateWindow(lpszClass, //윈도우가 생성되면 핸들(hwnd)이 반환
		lpszClass, //윈도우 클래스, 타이틀 이름
		WS_OVERLAPPEDWINDOW, //윈도우 스타일
		CW_USEDEFAULT, //윈도우 위치, x좌표
		CW_USEDEFAULT, //윈도우 위치, y좌표
		CW_USEDEFAULT, //윈도우 폭   
		CW_USEDEFAULT, //윈도우 높이   
		NULL, //부모 윈도우 핸들 
		NULL, //메뉴 핸들
		hInstance,     //응용 프로그램 ID
		NULL      //생성된 윈도우 정보
	);
	ShowWindow(hwnd, SW_MAXIMIZE); //윈도우의 화면 출력
	UpdateWindow(hwnd); //O/S 에 WM_PAINT 메시지 전송

	while (GetMessage(&msg, NULL, 0, 0)) //WinProc()에서 PostQuitMessage() 호출 때까지 처리
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); //WinMain -> WinProc  
	}
	return (int)msg.wParam;
}

void CALLBACK BrickDown(HWND hwnd, UINT msg, UINT event, DWORD dwTime)
{
	hdc = GetDC(hwnd);
	if (turn == true)
	{
		InvalidateRect(hwnd, NULL, FALSE); // hold, next 화면 바꾸기
		TetrisBrick.Reset(WaitList[0]);
		ShawdowBrick.Reset(WaitList[0]);
		for (int i = 0; i < 4; i++)
		{
			Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
			DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, WaitList[0]);
		}
		turn = false;
		Shadow(hdc, 1);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, 0);
		}
		TetrisBrick.Down();
		if (Vaildater(TetrisBrick.XY) == false) // 블록 충돌이 나면
		{
			TetrisBrick.Up();
			for (int i = 0; i < 4; i++) // 원상 복구
			{
				Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
			}
			turn = true;
			WaitList.erase(WaitList.begin());
			PushList();
			Delline(hwnd, hdc);
			BrickDown(hwnd, msg, event, dwTime);
		}
		else
		{
			TetrisBrick.Up();
			for (int i = 0; i < 4; i++)
			{
				DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, -1);
			}
			TetrisBrick.Down();
			for (int i = 0; i < 4; i++)
			{
				Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
				DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, WaitList[0]);
			}
		}
	}
	DrawBorder(hdc);
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	int bwidth = 200, bheight = 30;

	switch (iMsg)
	{
	case WM_CREATE:
	{
		myBitmap = LoadBitmap(g_hins, MAKEINTRESOURCE(IDB_BITMAP1));
		startx = midx - bricksize * 5, starty = midy - bricksize * 10;
		restart = 0;
		srand((unsigned int)time(NULL)); // 난수를 위한 초기화
		Diffindex = timer = linesum = score = starter = 0;
		holdbuffer = -1; turn = true; holdflag = true;
		for (int i = 0; i < 20; i++)
		{
			Map.push_back(Line(0, 0, 0));
		}
		PushList();
		ShawdowBrick.Reset(WaitList[0]);
		button1 = CreateWindow("button", "게임시작", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 100, bwidth, bheight, hwnd, (HMENU)1, g_hins, NULL);
		button2 = CreateWindow("button", "게임방법", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 50, bwidth, bheight, hwnd, (HMENU)2, g_hins, NULL);
		button3 = CreateWindow("button", "종료", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2, bwidth, bheight, hwnd, (HMENU)3, g_hins, NULL);
		pause = CreateWindow("button", "Pause", WS_CHILD | BS_DEFPUSHBUTTON, cwin.right - 200, cwin.bottom - 150, 50, 50, hwnd, (HMENU)4, g_hins, NULL);
		button4 = CreateWindow("button", "Go Back", WS_CHILD | BS_DEFPUSHBUTTON, cwin.right - 220, cwin.bottom - 150, 70, 50, hwnd, (HMENU)5, g_hins, NULL);

		InvalidateRect(hwnd, NULL, FALSE);
		break;
	}

	case WM_KEYDOWN:
	{
		hdc = GetDC(hwnd);
		if (starter == 1)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				Shadow(hdc, 0);
				MoveBrick(hdc, 0);
				ShawdowBrick.copy(TetrisBrick);
				Shadow(hdc, 1);
				Brickprint(hdc, TetrisBrick.XY, WaitList[0]);
			}
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000) // 아래 화살표
			{
				score = score + 5;
				BrickDown(hwnd, iMsg, (UINT)wParam, (DWORD)lParam);
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				Shadow(hdc, 0);
				MoveBrick(hdc, 1);
				ShawdowBrick.copy(TetrisBrick);
				Shadow(hdc, 1);
				Brickprint(hdc, TetrisBrick.XY, WaitList[0]);
			}
			else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				Shadow(hdc, 0);
				MoveBrick(hdc, 2);
				ShawdowBrick.copy(TetrisBrick);
				Shadow(hdc, 1);
				Brickprint(hdc, TetrisBrick.XY, WaitList[0]);
			}
			else if (wParam == VK_SPACE)
			{
				int hight = TetrisBrick.XY[0].x;
				int low = 0;
				while (1)
				{
					for (int i = 0; i < 4; i++)
					{
						Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, 0);
					}
					TetrisBrick.Down();
					if (Vaildater(TetrisBrick.XY) == false) // 블록 충돌이 나면
					{
						TetrisBrick.Up();
						low = TetrisBrick.XY[0].x;
						score = score + (hight - low) * 10;
						for (int i = 0; i < 4; i++)
						{
							Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, WaitList[0]);
							DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, WaitList[0]);
						}
						turn = true;
						WaitList.erase(WaitList.begin());
						PushList();
						Delline(hwnd, hdc);
						break;
					}
					else
					{
						TetrisBrick.Up();
						Brickprint(hdc, TetrisBrick.XY, -1);
						TetrisBrick.Down();
					}
				}
				BrickDown(hwnd, iMsg, (UINT)wParam, (DWORD)lParam);
			}
		}
		ReleaseDC(hwnd, hdc);
		break;
	}

	case WM_CHAR:
	{
		hdc = GetDC(hwnd);
		if ((char)wParam == 'c' && turn == false && holdflag == true && holdbuffer != WaitList[0]) // hold 기능
		{
			holdflag = false;
			if (holdbuffer == -1) // 게임 시작 처음으로 홀드 사용
			{
				holdbuffer = WaitList[0];
				WaitList.erase(WaitList.begin());
				PushList();
			}
			else // 그 외에 사용
			{
				int temp = WaitList[0];
				WaitList.erase(WaitList.begin());
				WaitList.insert(WaitList.begin(), holdbuffer);
				holdbuffer = temp;
			}
			for (int i = 0; i < 4; i++)
			{
				Map[TetrisBrick.XY[i].x].ModLine(TetrisBrick.XY[i].y, 0);
				DrawBrick(hdc, startx + TetrisBrick.XY[i].y * bricksize, starty + (19 - TetrisBrick.XY[i].x) * bricksize, myBitmap, -1);
				DrawBrick(hdc, startx + ShawdowBrick.XY[i].y * bricksize, starty + (19 - ShawdowBrick.XY[i].x) * bricksize, myBitmap, -1);
			}
			turn = true;
			BrickDown(hwnd, iMsg, 0, 0);
			InvalidateRect(hwnd, NULL, FALSE);
		}
		ReleaseDC(hwnd, hdc);
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			DestroyWindow(button1);
			DestroyWindow(button2);
			DestroyWindow(button3);
			SetTimer(hwnd, 1, 1000, NULL);
			SendMessage(hwnd, WM_TIMER, 1, 0);
			break;
		case 2:
		{
			DestroyWindow(button1);
			DestroyWindow(button2);
			DestroyWindow(button3);
			ShowWindow(button4, SW_SHOW);
			hdc = GetDC(hwnd);
			HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
			HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
			TextOut(hdc, startx + 2 * bricksize + 33, starty + 4 * bricksize, "             ", lstrlen("             "));
			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
			oldfont = (HFONT)SelectObject(hdc, hFont);
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 6 * bricksize, "-> 좌로 이동", lstrlen("-> 좌로 이동"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 7 * bricksize, "<- 우로 이동", lstrlen("<- 우로 이동"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 8 * bricksize, "Up 블록 회전", lstrlen("Up 블록 회전"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 9 * bricksize, "Down 빠르게 내리기", lstrlen("Down 빠르게 내리기"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 10 * bricksize, "Space 바로 내리기", lstrlen("Space 바로 내리기"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 11 * bricksize, "c 블록 홀드", lstrlen("c 블록 홀드"));
			SelectObject(hdc, oldfont);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc);
			break;
		}
		case 3:
			PostQuitMessage(0);
			break;
		case 4: // Pause 누르면
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (starter == 1 && counter == 4)
				{
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					starter = 0; // pause 상태
					EnableWindow(pause, FALSE);

					if (MessageBox(hwnd, "게임을 재개하려면 '예', 메뉴로 이동하고 싶다면 '아니요'를 누르세요.", "Paused", MB_YESNO) == IDYES)
					{
						SetTimer(hwnd, 1, 1000, NULL);
						SendMessage(hwnd, WM_TIMER, 1, 0);
					}
					else
					{
						Map.clear();
						WaitList.clear();
						DestroyWindow(pause);
						InvalidateRect(hwnd, NULL, TRUE);
						SendMessage(hwnd, WM_CREATE, 0, 0);
					}
				}
				break;
			default:
				break;
			}
			break;
		case 5:
			InvalidateRect(hwnd, NULL, TRUE);
			button1 = CreateWindow("button", "게임시작", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 100, bwidth, bheight, hwnd, (HMENU)1, g_hins, NULL);
			button2 = CreateWindow("button", "게임방법", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 50, bwidth, bheight, hwnd, (HMENU)2, g_hins, NULL);
			button3 = CreateWindow("button", "종료", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2, bwidth, bheight, hwnd, (HMENU)3, g_hins, NULL);
			ShowWindow(button4, SW_HIDE);
			break;
		default:
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
		{
			hdc = GetDC(hwnd);
			HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
			HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
			if (counter == 4)
			{
				ShowWindow(pause, SW_SHOW);
				NextPrint(hdc, 0);
				HoldPrint(hdc, 0);
				ScorePrint(hdc, 0);
				Redraw(hdc, myBitmap);
			}
			counter--;
			SetTextColor(hdc, RGB(80, 188, 233));
			SetBkMode(hdc, TRANSPARENT);
			if (counter != 0)
			{
				EnableWindow(pause, FALSE);
				char buffer[2];
				_itoa_s(counter, buffer, 10);
				PartialRedraw(hdc, 3, 6, 5, 8, 0);
				TextOut(hdc, midx - 30, midy - 150, buffer, lstrlen(buffer));
			}
			else if (counter == 0)
			{
				PartialRedraw(hdc, 3, 6, 5, 8, 0);
				counter = 4;
				KillTimer(hwnd, 1);
				SetTextColor(hdc, RGB(0, 0, 0));
				NextPrint(hdc, 1);
				HoldPrint(hdc, 1);
				int a = 0;
				for (int i = 0; i < 4; i++)
				{
					if (ShawdowBrick.XY[i].x == 19)
					{
						a = 1;
						break;
					}
				}
				if (a == 0)
				{
					Brickprint(hdc, ShawdowBrick.XY, -2);
				}
				SetTimer(hwnd, 2, Diffary[Diffindex], BrickDown); // 테트리스 시작, 재개
				BrickDown(hwnd, iMsg, 0, 0);
				SetTimer(hwnd, 3, 1000, NULL); // 타이머 재개
				SendMessage(hwnd, WM_TIMER, 3, 0);
				EnableWindow(pause, TRUE);
				starter = 1;
			}
			SelectObject(hdc, oldfont);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc);
			break;
		}
		case 3:
			timer++;
			break;
		}
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋움");
		HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
		if (starter == 0 && restart == 0)
		{
			TextOut(hdc, startx + 2 * bricksize + 33, starty + 4 * bricksize, "TETRIS", lstrlen("TETRIS"));
		}
		else if (starter == 1)
		{
			// 다음 블록 표시
			NextPrint(hdc, 1);
			HoldPrint(hdc, 1);
			ScorePrint(hdc, 0);
			int indexbuffer = timer / 120; // 2분마다 난이도 상승
			if (indexbuffer > 4)
			{
				indexbuffer = 4;
			}
			if (indexbuffer != Diffindex)
			{
				Diffindex = indexbuffer;
				KillTimer(hwnd, 2);
				SetTimer(hwnd, 2, Diffary[Diffindex], BrickDown);
				BrickDown(hwnd, iMsg, 0, 0);
			}
		}
		SelectObject(hdc, oldfont);
		DeleteObject(hFont);
		EndPaint(hwnd, &ps);
		break;
	}

	case WM_DESTROY:
		DeleteObject(myBitmap);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}