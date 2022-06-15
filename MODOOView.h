#pragma once
#include "ScreenObject.h"
#define PLAY_TIME 100

class CMODOOView : public CView
{
protected:
	CMODOOView() noexcept;
	DECLARE_DYNCREATE(CMODOOView)

public:
	CMODOODoc* GetDocument() const;

public:

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CMODOOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint m_window_pt, m_paymentDrag_pt;
	CDC ImageDC;
	CString m_missions[4],
		m_selectedMenus[3],
		m_timeStr,
		m_scoreStr,
		m_selectedNumStr,
		m_totalPriceStr,
		m_burgerNames[8],
		m_chickenNames[6],
		m_dessertNames[8],
		m_drinkNames[8],
		m_helpImagePaths[13];
	int m_screenType,
		m_time,
		m_score,
		m_selectedNum,
		m_totalPrices[3],
		m_totalPrice,
		m_burgerPrices[8],
		m_chickenPrices[6],
		m_dessertPrices[8],
		m_drinkPrices[8],
		m_selectedSetMenu[2],
		m_isToGo,
		m_paymentType,
		m_curDragPayment,
		m_missionPaymentType,
		m_missionToGo,
		m_helpPointer;
	bool m_askSet,
		m_showPaymentRects,
		m_paymentDrag,
		m_isFinish;
	CRect m_onlyMenu,
		m_withSet,
		m_paymentCard,
		m_paymentCoupon,
		m_paymentSmartPhone,
		m_finalScore,
		m_replay,
		m_returnToHome;
	ScreenObject scr_menu;
	ScreenObject scr_burger;
	ScreenObject scr_chicken;
	ScreenObject scr_dessert;
	ScreenObject scr_drink;
	ScreenObject scr_set_dessert;
	ScreenObject scr_set_drink;
	ScreenObject scr_payment_1;
	ScreenObject scr_payment_2;
	bool printBackgroundImageToDC(CDC* pDC, CString imagePath, CPoint window_pt);
	bool printAdditionalImageToDC(CDC* pDC, CString imagePath, CPoint window_pt, CRect imageRect);
	bool printMissionInfoToDC(CDC* pDC, ScreenObject thisScr);
	bool setMission();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG
inline CMODOODoc* CMODOOView::GetDocument() const
   { return reinterpret_cast<CMODOODoc*>(m_pDocument); }
#endif