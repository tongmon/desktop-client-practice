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

class Line // ��Ʈ���� ����
{
	int x[10] = { 0, };
public:
	Line(int a, int b, int c) // �� ��° ���� �� ���� ���� ���ڷ�
	{
		for (int k = a; k < b; k++)
		{
			x[k] = c;
		}
	}
	int Checker() // ��Ʈ������ �� ���� ��� �� ���ִ��� �˻�. �� ���ִٸ� 10, �� ���ִ� ���̸� 10 ����  
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

typedef struct _Cord // ��Ʈ������ ��ǥ��
{
	int x; int y;
} Cordinate;

class Brick // ��Ʈ���� ���
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
RECT cwin; // ������ â ũ��
HINSTANCE g_hins;
WNDCLASS WndClass;
HDC hdc;
HWND button1, button2, button3, button4, pause; // ���� ��ư��
Brick TetrisBrick;
Brick ShawdowBrick;
HBITMAP myBitmap;
vector<Line> Map; // ��Ʈ���� ��
vector<int> WaitList; // ���� ��� ��⿭
int midx, midy, startx, starty, counter = 4; // ȭ�� �߽� ��ǥ
short starter; // ��Ʈ���� ���� �������� �ƴ��� 0�̸� pause, 1�̸� ����
bool turn = true, holdflag = true; // turn�� ��� �Ѱ��� ���߸� ture�� ��ȯ, holdflag�� Ȧ�带 �� �Ͽ� �ѹ��� �����ϰ�
int Diffary[5] = { Easy , Normal ,Hard, Insane, Crazy }; // ���̵�
int Diffindex = 0;
int holdbuffer;
int score = 0; // ����
int linesum = 0; // ������ ����
int timer = 0; // ���̵� ������ �ð� üũ
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

bool Vaildater(Cordinate* Cord) // ��� �浹 �˻�
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

bool Shadowater(Cordinate* Cord, vector<int>* index) // shadow ��� �浹 �˻�
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
	else if (bricktype == -2) // shadow ��� �̸����̱�
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
		for (int p = 0; p < 4; p++) // �׸��� ��� ����� ���� ȭ�� ����
		{
			DrawBrick(hdc, startx + ShawdowBrick.XY[p].y * bricksize, starty + (19 - ShawdowBrick.XY[p].x) * bricksize, myBitmap, -1);
		}
	}
}

void Brickprint(HDC hdc, Cordinate* cord, int mode) // Ư�� ��� �׸���
{
	for (int i = 0; i < 4; i++)
	{
		DrawBrick(hdc, startx + cord[i].y * bricksize, starty + (19 - cord[i].x) * bricksize, myBitmap, mode);
	}
}

void DrawBorder(HDC hdc) // �ܰ� �׸���
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

void Redraw(HDC hdc, HBITMAP hBit) // ��Ʈ���� ���� �׸���
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
			if (mode == 0) // default ��� �� Map��� �ٽ� �׸���
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

	if (Vaildater(TetrisBrick.XY) == false) // ��ȿ���� �ʴٸ�
	{
		if (key == 1)
		{
			TetrisBrick.Right();
		}
		else if (key == 2)
		{
			TetrisBrick.Left();
		}
		else // ���󺹱�
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
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
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
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
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
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
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
			// ��Ʈ���� �� �ʱ�ȭ
			Map.clear();
			WaitList.clear();
			PushList();
			for (int k = 0; k < 20; k++)
			{
				Map.push_back(Line(0, 10, 0));
			}
			char buffer[256];
			wsprintf(buffer, "���� ���ھ�: %d\n���ӿ���! �ٽ� �����Ϸ��� '��', �޴� �̵��� '�ƴϿ�'�� ��������.", score);
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
	for (int i = 0; i < 4; i++) // ����� ��ǥ�� ���� ���� ���̸� ���Ѵ�.
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
	holdflag = true; // Ȧ�� �ٽ� �����ϰ�
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
{ //szCmdLine : Ŀ��Ʈ���� �󿡼� ���α׷� ���� �� ���޵� ���ڿ�
	HWND hwnd; //iCmdShow : �����찡 ȭ�鿡 ��µ� ����
	MSG msg;
	g_hins = hInstance;

	GetWindowRect(GetDesktopWindow(), &cwin); // ��üȭ�� ���ϱ�
	midx = (cwin.right - cwin.left) / 2;
	midy = (cwin.bottom - cwin.top) / 2;

	WndClass.style = CS_HREDRAW | CS_VREDRAW; //��½�Ÿ�� : ����/������ ��ȭ�� �ٽ� �׸�
	WndClass.lpfnWndProc = WndProc; //���ν��� �Լ���
	WndClass.cbClsExtra = 0; //O/S ��� ���� �޸� (Class)
	WndClass.cbWndExtra = 0; //O/s ��� ���� �޸� (Window)
	WndClass.hInstance = hInstance; //���� ���α׷� ID
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//����   
	WndClass.lpszMenuName = NULL; //�޴� �̸�
	WndClass.lpszClassName = lpszClass; //Ŭ���� �̸�
	RegisterClass(&WndClass); //�ռ� ������ ������ Ŭ������ �ּ�

	// ��Ʈ���� ���� ���� ������
	hwnd = CreateWindow(lpszClass, //�����찡 �����Ǹ� �ڵ�(hwnd)�� ��ȯ
		lpszClass, //������ Ŭ����, Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		CW_USEDEFAULT, //������ ��ġ, x��ǥ
		CW_USEDEFAULT, //������ ��ġ, y��ǥ
		CW_USEDEFAULT, //������ ��   
		CW_USEDEFAULT, //������ ����   
		NULL, //�θ� ������ �ڵ� 
		NULL, //�޴� �ڵ�
		hInstance,     //���� ���α׷� ID
		NULL      //������ ������ ����
	);
	ShowWindow(hwnd, SW_MAXIMIZE); //�������� ȭ�� ���
	UpdateWindow(hwnd); //O/S �� WM_PAINT �޽��� ����

	while (GetMessage(&msg, NULL, 0, 0)) //WinProc()���� PostQuitMessage() ȣ�� ������ ó��
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
		InvalidateRect(hwnd, NULL, FALSE); // hold, next ȭ�� �ٲٱ�
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
		if (Vaildater(TetrisBrick.XY) == false) // ��� �浹�� ����
		{
			TetrisBrick.Up();
			for (int i = 0; i < 4; i++) // ���� ����
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
		srand((unsigned int)time(NULL)); // ������ ���� �ʱ�ȭ
		Diffindex = timer = linesum = score = starter = 0;
		holdbuffer = -1; turn = true; holdflag = true;
		for (int i = 0; i < 20; i++)
		{
			Map.push_back(Line(0, 0, 0));
		}
		PushList();
		ShawdowBrick.Reset(WaitList[0]);
		button1 = CreateWindow("button", "���ӽ���", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 100, bwidth, bheight, hwnd, (HMENU)1, g_hins, NULL);
		button2 = CreateWindow("button", "���ӹ��", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 50, bwidth, bheight, hwnd, (HMENU)2, g_hins, NULL);
		button3 = CreateWindow("button", "����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2, bwidth, bheight, hwnd, (HMENU)3, g_hins, NULL);
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
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000) // �Ʒ� ȭ��ǥ
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
					if (Vaildater(TetrisBrick.XY) == false) // ��� �浹�� ����
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
		if ((char)wParam == 'c' && turn == false && holdflag == true && holdbuffer != WaitList[0]) // hold ���
		{
			holdflag = false;
			if (holdbuffer == -1) // ���� ���� ó������ Ȧ�� ���
			{
				holdbuffer = WaitList[0];
				WaitList.erase(WaitList.begin());
				PushList();
			}
			else // �� �ܿ� ���
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
			HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
			HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
			TextOut(hdc, startx + 2 * bricksize + 33, starty + 4 * bricksize, "             ", lstrlen("             "));
			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
			oldfont = (HFONT)SelectObject(hdc, hFont);
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 6 * bricksize, "-> �·� �̵�", lstrlen("-> �·� �̵�"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 7 * bricksize, "<- ��� �̵�", lstrlen("<- ��� �̵�"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 8 * bricksize, "Up ��� ȸ��", lstrlen("Up ��� ȸ��"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 9 * bricksize, "Down ������ ������", lstrlen("Down ������ ������"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 10 * bricksize, "Space �ٷ� ������", lstrlen("Space �ٷ� ������"));
			TextOut(hdc, startx + 1 * bricksize + 33, starty + 11 * bricksize, "c ��� Ȧ��", lstrlen("c ��� Ȧ��"));
			SelectObject(hdc, oldfont);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc);
			break;
		}
		case 3:
			PostQuitMessage(0);
			break;
		case 4: // Pause ������
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (starter == 1 && counter == 4)
				{
					KillTimer(hwnd, 2);
					KillTimer(hwnd, 3);
					starter = 0; // pause ����
					EnableWindow(pause, FALSE);

					if (MessageBox(hwnd, "������ �簳�Ϸ��� '��', �޴��� �̵��ϰ� �ʹٸ� '�ƴϿ�'�� ��������.", "Paused", MB_YESNO) == IDYES)
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
			button1 = CreateWindow("button", "���ӽ���", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 100, bwidth, bheight, hwnd, (HMENU)1, g_hins, NULL);
			button2 = CreateWindow("button", "���ӹ��", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2 - 50, bwidth, bheight, hwnd, (HMENU)2, g_hins, NULL);
			button3 = CreateWindow("button", "����", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, (cwin.right - cwin.left - bwidth) / 2, (cwin.bottom - cwin.top - bheight) / 2, bwidth, bheight, hwnd, (HMENU)3, g_hins, NULL);
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
			HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
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
				SetTimer(hwnd, 2, Diffary[Diffindex], BrickDown); // ��Ʈ���� ����, �簳
				BrickDown(hwnd, iMsg, 0, 0);
				SetTimer(hwnd, 3, 1000, NULL); // Ÿ�̸� �簳
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
		HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����");
		HFONT oldfont = (HFONT)SelectObject(hdc, hFont);
		if (starter == 0 && restart == 0)
		{
			TextOut(hdc, startx + 2 * bricksize + 33, starty + 4 * bricksize, "TETRIS", lstrlen("TETRIS"));
		}
		else if (starter == 1)
		{
			// ���� ��� ǥ��
			NextPrint(hdc, 1);
			HoldPrint(hdc, 1);
			ScorePrint(hdc, 0);
			int indexbuffer = timer / 120; // 2�и��� ���̵� ���
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
	return DefWindowProc(hwnd, iMsg, wParam, lParam); //CASE���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}