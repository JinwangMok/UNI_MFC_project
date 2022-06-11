#pragma once
#define SCR_MENU 1001
#define SCR_BURGER 1002
#define SCR_CHICKEN 1003
#define SCR_DESSERT 1004
#define SCR_DRINK 1005
#define SCR_SET_DESSERT 1006
#define SCR_SET_DRINK 1007
#define SCR_PAYMENT_1 1008
#define SCR_PAYMENT_2 1009
#define SCR_HELPS 1010

class ScreenObject
{
protected:
	int screenType;
	CString imagePath;

public:
	CRect* rects;
	CString GetPath();
	void SetPath(CString imagePath);
	void SetObjectByType(int screenType);
	void SetRectsPosition(CPoint window_pt);
};

