#pragma once

#include "resource.h"

#define Vv velocity
#define PI 3.141593

typedef struct coord_s
{
	int X;
	int Y;
} coord_t;

class Snake
{
private:
	int XVal, YVal, OldX, OldY;
	int XBody, YBody, XTail, YTail;
	int HeadRadius, BodyRadius, TailRadius;
	int Length;
	coord_t velocity;

	int FindOutlyingCoords();
	void UpdateVals();
	void Animate(HDC hdc, HWND hWnd, HBRUSH brush);

public:
	void Idle(HDC hdc, HWND hWnd);
	void MoveToward(HDC hdc, HWND hWnd, POINT dest);
	int GetCoord(bool bY);
	void GetVelocity(coord_t &v);

	Snake(int Xinit, int Yinit, int Head, int Body, int Tail) : XVal(Xinit), YVal(Yinit),
	TailRadius(Tail), BodyRadius(Body), HeadRadius(Head) { velocity.X = 5; velocity.Y = 5; }	//VS2013: non static data member initalization is incomplete, so must initialize within method Vv{5,5}

	Snake(int Xinit, int Yinit, int Head, int Body, int Tail, int NewLength, int XV, int YV) : XVal(Xinit), YVal(Yinit),
		TailRadius(Tail), BodyRadius(Body), HeadRadius(Head), Length(NewLength) { velocity.X = XV; velocity.Y = YV; }

	~Snake() {};
};

void UnitVector(coord_t &vec);
void UnitVector(float &X, float &Y);

template<class T> void LogTypeData(T a, T b, char *id);
