
// MODOOView.h: CMODOOView 클래스의 인터페이스
//
#pragma once
#include "ScreenObject.h"
#define PLAY_TIME 100

class CMODOOView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMODOOView() noexcept;
	DECLARE_DYNCREATE(CMODOOView)

// 특성입니다.
public:
	CMODOODoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMODOOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/* Author:목진왕 */
	CPoint m_window_pt, m_paymentDrag_pt;		// 윈도우 가로, 세로 길이를 저장하기 위한 CPoint 변수
	CDC ImageDC;			// 이미지를 pDC에 등록하기 위한 디바이스 컨텍스트 변수
	CString m_missions[4],	// 미션 문자열
		m_selectedMenus[3],	// 선택된 메뉴 문자열
		m_timeStr,				// 제한 시간 문자열
		m_scoreStr,			// 점수 문자열
		m_selectedNumStr,	// 주문내역(개) 문자열
		m_totalPriceStr,	// 총 주문 금액 문자열
		m_burgerNames[8],
		m_chickenNames[6],
		m_dessertNames[8],
		m_drinkNames[8];
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
		m_missionToGo;
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

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MODOOView.cpp의 디버그 버전
inline CMODOODoc* CMODOOView::GetDocument() const
   { return reinterpret_cast<CMODOODoc*>(m_pDocument); }
#endif