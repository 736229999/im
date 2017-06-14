#pragma once
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
class CGraphicsRoundRectPath: public Gdiplus::GraphicsPath
{
public:
	CGraphicsRoundRectPath(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);

public:
	void AddRoundRect(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);
};

