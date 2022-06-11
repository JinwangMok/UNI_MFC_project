
// MODOOView.cpp: CMODOOView 클래스의 구현

/* 현재 스크린의 타입을 나타내기 위한 변수 */


#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MODOO.h"
#endif

#include "MODOODoc.h"
#include "MODOOView.h"
#include "ScreenObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMODOOView

IMPLEMENT_DYNCREATE(CMODOOView, CView)

BEGIN_MESSAGE_MAP(CMODOOView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMODOOView 생성/소멸

CMODOOView::CMODOOView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

	/* 첫 화면 메뉴로 설정 */
	m_screenType = SCR_MENU;

	/* 스크린 객체 생성 */
	scr_menu.SetObjectByType(SCR_MENU);
	scr_burger.SetObjectByType(SCR_BURGER);
	scr_chicken.SetObjectByType(SCR_CHICKEN);
	scr_dessert.SetObjectByType(SCR_DESSERT);
	scr_drink.SetObjectByType(SCR_DRINK);
	scr_set_dessert.SetObjectByType(SCR_SET_DESSERT);
	scr_set_drink.SetObjectByType(SCR_SET_DRINK);
	scr_payment_1.SetObjectByType(SCR_PAYMENT_1);
	scr_payment_2.SetObjectByType(SCR_PAYMENT_2);

	/* 변수값 초기화 */
	srand((unsigned int)time(NULL));
	m_time = 0;
	m_score = 0;
	m_selectedNum = 0; // 메뉴 포인터 역할 겸
	m_totalPrice = 0;
	m_askSet = false;
	m_showPaymentRects = false;
	m_paymentDrag = false;
	m_selectedSetMenu[0] = -1;
	m_selectedSetMenu[1] = -1;
	m_isToGo = -1;
	m_paymentType = -1;
	m_curDragPayment = -1;
	m_missionPaymentType = -1;
	m_isFinish = false;
	m_helpPointer = 0;

	/* 도움말 화면 이미지 패스 할당 */
	CString temp;
	for (int i = 0; i < 13; i++) {
		temp.Format(_T("%d"), i + 1);
		m_helpImagePaths[i] = _T("./res/screen_help_") + temp + _T(".png");
	}

	/* 메뉴 할당 */
	int burgerPrices[8] = {4100, 4100, 4100, 3300, 7500, 6000, 2900, 6000 };
	int chickenPrices[6] = {10200, 2500, 4900, 2700, 4900, 2000};
	int dessertPrices[8] = {1600, 2100, 2100, 2600, 700, 1800, 2500, 1500};
	int drinkPrices[8] = {1800, 1800, 2200, 2500, 2100, 2100, 2500, 2500};
	CString burgerNames[8] = {L"불고기버거", L"새우버거", L"T-REX(티렉스)", L"치킨버거", L"한우불고기", L"DOUBLEX", L"데리버거", L"불고기4DX" };
	CString chickenNames[6] = { L"1인 혼닭", L"치킨1조각", L"치킨휠레-4", L"치킨휠레-2", L"화이어윙-4", L"화이어윙-2"};
	CString dessertNames[8] = { L"포테이토", L"치즈스틱", L"양념감자", L"쉑쉑치킨", L"소프트콘", L"콘샐러드", L"초코쿠키 토네이도", L"선데 허쉬초코" };
	CString drinkNames[8] = { L"콜라", L"사이다", L"아이스티", L"레몬에이드", L"아메리카노", L"아이스 아메리카노", L"카페라떼", L"아이스 카페라떼" };
	for (int i = 0; i < 8; i++) {
		m_burgerPrices[i] = burgerPrices[i];
		m_burgerNames[i] = burgerNames[i];
		if (i < 6) {
			m_chickenPrices[i] = chickenPrices[i];
			m_chickenNames[i] = chickenNames[i];
		}
		m_dessertPrices[i] =  dessertPrices[i];
		m_dessertNames[i] = dessertNames[i];
		m_drinkPrices[i] = drinkPrices[i];
		m_drinkNames[i] =  drinkNames[i];
	}
	
	m_timeStr.Format(L"%d", m_time);
	m_scoreStr.Format(L"%d", m_score);
	for (int i = 0; i < 4; i++) {
		m_missions[i] = L"";
	}
	for (int i = 0; i < 3; i++) {
		m_selectedMenus[i] = L"";
		m_totalPrices[i] = 0;
	}
	m_onlyMenu = CRect();
	m_withSet = CRect();
	m_paymentCard = CRect();
	m_paymentCoupon = CRect();
	m_paymentSmartPhone = CRect();
	m_finalScore = CRect();
	m_replay = CRect();
	m_returnToHome = CRect();
}

CMODOOView::~CMODOOView()
{
}

BOOL CMODOOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMODOOView 그리기

void CMODOOView::OnDraw(CDC* pDC)
{
	CMODOODoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	pDC->SetBkMode(TRANSPARENT); // 글자 배경 투명화
	CImage image;
	CPoint image_pt, rightbottom;
	CBitmap bitmap;
	CBrush transparentBrush, * oldBrush;
	CPen selectedRed, * oldPen;
	CString paymentPrices[3];
	CFont font, * oldFont;

	/* 총액 계산 부분 */
	m_totalPrice = 0;
	for (int i = 0; i < m_selectedNum; i++) { // 선택된 개수만큼만 가져옴
		m_totalPrice += m_totalPrices[i];
		
	}
	m_selectedNumStr.Format(L"%d", m_selectedNum);
	m_totalPriceStr.Format(L"%d", m_totalPrice);
	/* 배경화면 그리기 & Rect 생성 & 정보 표현 */
	if (m_isFinish) {
		/* 종료 창 띄우기 */
		image.Load(_T("./res/screen_result.png"));
		image_pt.x = image.GetWidth();
		image_pt.y = image.GetHeight();
		ImageDC.CreateCompatibleDC(pDC);
		bitmap.Attach(image.Detach());

		ImageDC.SelectObject(&bitmap);

		pDC->StretchBlt(int(m_window_pt.x * 0.35), int(m_window_pt.y * 0.25),
			int(m_window_pt.x * 0.3), int(m_window_pt.y * 0.35),
			&ImageDC, 0, 0, image_pt.x, image_pt.y, SRCCOPY);

		bitmap.DeleteObject();
		ImageDC.DeleteDC();
		m_finalScore = CRect(int(m_window_pt.x * 0.5), int(m_window_pt.y * 0.36), int(m_window_pt.x * 0.62), int(m_window_pt.y * 0.45));
		m_replay = CRect(int(m_window_pt.x * 0.375), int(m_window_pt.y * 0.49), int(m_window_pt.x * 0.49), int(m_window_pt.y * 0.575));
		m_returnToHome = CRect(int(m_window_pt.x * 0.508), int(m_window_pt.y * 0.49), int(m_window_pt.x * 0.623), int(m_window_pt.y * 0.575));

		font.CreatePointFont(192, _T("맑은고딕"));
		oldFont = pDC->SelectObject(&font);

		pDC->DrawText(m_scoreStr, &m_finalScore, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		pDC->SelectObject(oldFont);
		font.DeleteObject();
	}
	else {
		switch (m_screenType) {
		case SCR_MENU:
			scr_menu.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_menu.GetPath(), m_window_pt);
			break;
		case SCR_HELPS:
			m_returnToHome = CRect(
				int(m_window_pt.x * 0.834),
				int(m_window_pt.y * 0.046),
				int(m_window_pt.x * 0.834) + int(m_window_pt.x * 0.136),
				int(m_window_pt.y * 0.046) + int(m_window_pt.y * 0.067)
			);
			if (m_helpPointer == 12) {
				m_replay = CRect(
					int(m_window_pt.x * 0.834),
					int(m_window_pt.y * 0.117),
					int(m_window_pt.x * 0.834) + int(m_window_pt.x * 0.136),
					int(m_window_pt.y * 0.117) + int(m_window_pt.y * 0.067)
				);
				printBackgroundImageToDC(pDC, m_helpImagePaths[12], m_window_pt);
			}
			else {
				printBackgroundImageToDC(pDC, m_helpImagePaths[m_helpPointer], m_window_pt);
			}
			break;
		case SCR_BURGER:
			scr_burger.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_burger.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_burger);

			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_burger.rects[8 + i], DT_LEFT | DT_WORDBREAK);
			}
			if (m_askSet) {
				/* 메뉴 묻는 창 띄우기 */
				image.Load(_T("./res/screen_askSet.png"));
				image_pt.x = image.GetWidth();
				image_pt.y = image.GetHeight();
				ImageDC.CreateCompatibleDC(pDC);
				bitmap.Attach(image.Detach());

				ImageDC.SelectObject(&bitmap);

				pDC->StretchBlt(int(m_window_pt.x*0.35), int(m_window_pt.y*0.25),
					int(m_window_pt.x * 0.3), int(m_window_pt.y*0.35),
					&ImageDC, 0, 0, image_pt.x, image_pt.y, SRCCOPY);

				bitmap.DeleteObject();
				ImageDC.DeleteDC();

				m_onlyMenu = CRect(int(m_window_pt.x * 0.41), int(m_window_pt.y*0.372), int(m_window_pt.x * 0.49), int(m_window_pt.y*0.522));
				m_withSet = CRect(int(m_window_pt.x * 0.502), int(m_window_pt.y * 0.372), int(m_window_pt.x * 0.582), int(m_window_pt.y * 0.522));
			}
			else {
				m_onlyMenu = CRect();
				m_withSet = CRect();
				pDC->DrawText(m_selectedNumStr, scr_burger.rects[15], DT_CENTER);
				pDC->DrawText(m_totalPriceStr, scr_burger.rects[16], DT_CENTER);
			}
			break;
		case SCR_CHICKEN:
			scr_chicken.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_chicken.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_chicken);

			pDC->DrawText(m_selectedNumStr, scr_chicken.rects[15], DT_CENTER);
			pDC->DrawText(m_totalPriceStr, scr_chicken.rects[16], DT_CENTER);

			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_chicken.rects[8 + i], DT_LEFT | DT_WORDBREAK);
			}
			break;
		case SCR_DESSERT:
			scr_dessert.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_dessert.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_dessert);

			pDC->DrawText(m_selectedNumStr, scr_dessert.rects[15], DT_CENTER);
			pDC->DrawText(m_totalPriceStr, scr_dessert.rects[16], DT_CENTER);


			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_dessert.rects[8 + i], DT_LEFT | DT_WORDBREAK);
			}
			break;
		case SCR_DRINK:
			scr_drink.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_drink.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_drink);

			pDC->DrawText(m_selectedNumStr, scr_drink.rects[15], DT_CENTER);
			pDC->DrawText(m_totalPriceStr, scr_drink.rects[16], DT_CENTER);

			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_drink.rects[8 + i], DT_LEFT | DT_WORDBREAK);
			}
			break;

		case SCR_SET_DESSERT:
			scr_set_dessert.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_set_dessert.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_set_dessert);

			if (m_selectedSetMenu[0] != -1) {
				selectedRed.CreatePen(PS_SOLID, 3, RGB(250, 0, 0));
				transparentBrush.CreateStockObject(NULL_BRUSH);

				oldBrush = pDC->SelectObject(&transparentBrush);

				oldPen = pDC->SelectObject(&selectedRed);
				pDC->Rectangle(scr_set_dessert.rects[(10 + m_selectedSetMenu[0])]);
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
				DeleteObject(transparentBrush);
			}
			break;

		case SCR_SET_DRINK:
			scr_set_drink.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_set_drink.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_set_drink);

			if (m_selectedSetMenu[1] != -1) {
				selectedRed.CreatePen(PS_SOLID, 3, RGB(250, 0, 0));
				transparentBrush.CreateStockObject(NULL_BRUSH);

				oldBrush = pDC->SelectObject(&transparentBrush);

				oldPen = pDC->SelectObject(&selectedRed);
				pDC->Rectangle(scr_set_drink.rects[(10 + m_selectedSetMenu[1])]);
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
				DeleteObject(transparentBrush);
			}
			break;

		case SCR_PAYMENT_1:
			scr_payment_1.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_payment_1.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_payment_1);
		
			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_payment_1.rects[6 + i], DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_WORDBREAK);
				paymentPrices[i].Format(_T("%d"), m_totalPrices[i]);
				pDC->DrawText(paymentPrices[i], scr_payment_1.rects[9 + i], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
			}
		
			pDC->DrawText(m_totalPriceStr, scr_payment_1.rects[12], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);	//주문금액
			pDC->DrawText(L"0", scr_payment_1.rects[13], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);				//할인금액(처리 안함)
			pDC->DrawText(m_totalPriceStr, scr_payment_1.rects[14], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);	//결제할 금액

			if (m_isToGo != -1) {
				selectedRed.CreatePen(PS_SOLID, 3, RGB(250, 0, 0));
				transparentBrush.CreateStockObject(NULL_BRUSH);

				oldBrush = pDC->SelectObject(&transparentBrush);

				oldPen = pDC->SelectObject(&selectedRed);
				if (m_isToGo == 1) {
					pDC->Rectangle(scr_payment_1.rects[15]);
				}
				else {
					pDC->Rectangle(scr_payment_1.rects[16]);
				}
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
				DeleteObject(transparentBrush);
				selectedRed.DeleteObject();
			}
			break;

		case SCR_PAYMENT_2:
		
			scr_payment_2.SetRectsPosition(m_window_pt);
			printBackgroundImageToDC(pDC, scr_payment_2.GetPath(), m_window_pt);
			printMissionInfoToDC(pDC, scr_payment_2);

			for (int i = 0; i < m_selectedNum; i++) {
				pDC->DrawText(m_selectedMenus[i], scr_payment_2.rects[6 + i], DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_WORDBREAK);
				paymentPrices[i].Format(_T("%d"), m_totalPrices[i]);
				pDC->DrawText(paymentPrices[i], scr_payment_2.rects[9 + i], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
			}

			pDC->DrawText(m_totalPriceStr, scr_payment_2.rects[12], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);	//주문금액
			pDC->DrawText(L"0", scr_payment_2.rects[13], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);				//할인금액(처리 안함)
			pDC->DrawText(m_totalPriceStr, scr_payment_2.rects[14], DT_RIGHT | DT_SINGLELINE | DT_VCENTER);	//결제할 금액

			if (m_isToGo != -1) {
				selectedRed.CreatePen(PS_SOLID, 3, RGB(250, 0, 0));
				transparentBrush.CreateStockObject(NULL_BRUSH);

				oldBrush = pDC->SelectObject(&transparentBrush);

				oldPen = pDC->SelectObject(&selectedRed);
				if (m_isToGo == 1) {
					pDC->Rectangle(scr_payment_2.rects[15]);
				}
				else {
					pDC->Rectangle(scr_payment_2.rects[16]);
				}
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
				DeleteObject(transparentBrush);
				selectedRed.DeleteObject();
			}

			if (m_paymentType != -1) {
				selectedRed.CreatePen(PS_SOLID, 3, RGB(250, 0, 0));
				transparentBrush.CreateStockObject(NULL_BRUSH);

				oldBrush = pDC->SelectObject(&transparentBrush);

				oldPen = pDC->SelectObject(&selectedRed);
				pDC->Rectangle(scr_payment_2.rects[17+ m_paymentType]);
				pDC->SelectObject(oldPen);
				pDC->SelectObject(oldBrush);
				DeleteObject(transparentBrush);
				selectedRed.DeleteObject();
			}
			break;
		default:
			break;
		}
		/* 결제 수단 보이기 */
		if (m_showPaymentRects) {
			m_paymentCard = CRect(
				int(m_window_pt.x * 0.7),
				int(m_window_pt.y * 0.6),
				int(m_window_pt.x * 0.7) + int(m_window_pt.x * 0.067),
				int(m_window_pt.y * 0.6) + int(m_window_pt.y * 0.23)
			);
			m_paymentCoupon = CRect(
				int(m_window_pt.x * 0.775),
				int(m_window_pt.y * 0.6),
				int(m_window_pt.x * 0.775) + int(m_window_pt.x * 0.067),
				int(m_window_pt.y * 0.6) + int(m_window_pt.y * 0.23)
			);
			m_paymentSmartPhone = CRect(
				int(m_window_pt.x * 0.85),
				int(m_window_pt.y * 0.6),
				int(m_window_pt.x * 0.85) + int(m_window_pt.x * 0.067),
				int(m_window_pt.y * 0.6) + int(m_window_pt.y * 0.28)
			);
			if (m_paymentDrag) {
				switch (m_curDragPayment) {
				case 0:
					m_paymentCard = CRect(
						m_paymentDrag_pt.x - int(m_window_pt.x * 0.034),
						m_paymentDrag_pt.y - int(m_window_pt.y * 0.12),
						m_paymentDrag_pt.x + int(m_window_pt.x * 0.033),
						m_paymentDrag_pt.y + int(m_window_pt.y * 0.11)
					);
					break;
				case 1:
					m_paymentCoupon = CRect(
						m_paymentDrag_pt.x - int(m_window_pt.x * 0.034),
						m_paymentDrag_pt.y - int(m_window_pt.y * 0.12),
						m_paymentDrag_pt.x + int(m_window_pt.x * 0.033),
						m_paymentDrag_pt.y + int(m_window_pt.y * 0.11)
					);
					break;
				case 2:
					m_paymentSmartPhone = CRect(
						m_paymentDrag_pt.x - int(m_window_pt.x * 0.034),
						m_paymentDrag_pt.y - int(m_window_pt.y * 0.14),
						m_paymentDrag_pt.x + int(m_window_pt.x * 0.033),
						m_paymentDrag_pt.y + int(m_window_pt.y * 0.14)
					);
					break;
				default:
					break;
				}
			}
			printAdditionalImageToDC(pDC, _T("./res/payment_card.png"), m_window_pt, m_paymentCard);
			printAdditionalImageToDC(pDC, _T("./res/payment_coupon.png"), m_window_pt, m_paymentCoupon);
			printAdditionalImageToDC(pDC, _T("./res/payment_smartphone.png"), m_window_pt, m_paymentSmartPhone);	
		}
		else {
			m_paymentCard = CRect();
			m_paymentCoupon = CRect();
			m_paymentSmartPhone = CRect();
	}
	

	}
}


// CMODOOView 인쇄

BOOL CMODOOView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMODOOView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMODOOView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMODOOView 진단

#ifdef _DEBUG
void CMODOOView::AssertValid() const
{
	CView::AssertValid();
}

void CMODOOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMODOODoc* CMODOOView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMODOODoc)));
	return (CMODOODoc*)m_pDocument;
}
#endif //_DEBUG

bool CMODOOView::printBackgroundImageToDC(CDC* pDC, CString imagePath, CPoint window_pt) {

	CImage image;
	if (FAILED(image.Load(imagePath)))

	{

		return FALSE;

	}
	
	CPoint image_pt;
	image_pt.x = image.GetWidth();
	image_pt.y = image.GetHeight();

	ImageDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	ImageDC.SelectObject(&bitmap);

	pDC->StretchBlt(0, 0, window_pt.x, window_pt.y, &ImageDC, 0, 0, image_pt.x, image_pt.y, SRCCOPY);

	bitmap.DeleteObject();
	ImageDC.DeleteDC();

	return true;
}

bool CMODOOView::printAdditionalImageToDC(CDC* pDC, CString imagePath, CPoint window_pt, CRect imageRect) {
	CImage image;
	if (FAILED(image.Load(imagePath)))

	{

		return FALSE;

	}
	CPoint image_pt;
	image_pt.x = image.GetWidth();
	image_pt.y = image.GetHeight();
	ImageDC.CreateCompatibleDC(pDC);
	
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	ImageDC.SelectObject(&bitmap);

	pDC->StretchBlt(
		imageRect.TopLeft().x, imageRect.TopLeft().y,
		imageRect.Width(), imageRect.Height(),
		&ImageDC, 0, 0, image_pt.x, image_pt.y, SRCCOPY);

	bitmap.DeleteObject();
	ImageDC.DeleteDC();
	return true;
}

bool  CMODOOView::printMissionInfoToDC(CDC* pDC, ScreenObject thisScr) {
	
	pDC->DrawText(m_timeStr, thisScr.rects[0], DT_CENTER);
	pDC->DrawText(m_scoreStr, thisScr.rects[1], DT_CENTER);
	
	CFont font, *oldFont;
	font.CreatePointFont(72, _T("맑은고딕"));
	oldFont = pDC->SelectObject(&font);

	pDC->DrawText(m_missions[0], thisScr.rects[2], DT_LEFT | DT_NOCLIP);
	pDC->DrawText(m_missions[1], thisScr.rects[3], DT_LEFT | DT_NOCLIP);
	pDC->DrawText(m_missions[2], thisScr.rects[4], DT_LEFT | DT_NOCLIP);
	pDC->DrawText(m_missions[3], thisScr.rects[5], DT_CENTER);

	pDC->SelectObject(oldFont);
	font.DeleteObject();

	return true;
}

bool CMODOOView::setMission() {
	int type, idx, set, both;
	for (int i = 0; i < 3; i++) {
		type = rand() % 4;
		switch (type) {
		case 0:
			idx = rand() % 8;
			m_missions[i] = m_burgerNames[idx];

			set = rand() % 2;
			if (set == 1) {

				both = rand() % 3;
				if (both == 0) {
					idx = rand() % 8;
					m_missions[i] += L"+" + m_dessertNames[idx];
				}
				else if (both == 1) {
					idx = rand() % 8;
					m_missions[i] += L"+" + m_drinkNames[idx];
				}
				else if (both == 2) {
					idx = rand() % 8;
					m_missions[i] += L"+" + m_dessertNames[idx];
					idx = rand() % 8;
					m_missions[i] += L"+" + m_drinkNames[idx];
				}
			}
			break;
		case 1:
			idx = rand() % 6;
			m_missions[i] = m_chickenNames[idx];
			break;
		case 2:
			idx = rand() % 8;
			m_missions[i] = m_dessertNames[idx];
			break;
		case 3:
			idx = rand() % 8;
			m_missions[i] = m_drinkNames[idx];
			break;
		default:
			break;
		}
	}
	m_missionToGo = rand() % 2;
	m_missionPaymentType = rand() % 3;
	switch (m_missionPaymentType) {
	case 0:
		if (m_missionToGo == 0) {
			m_missions[3] = L"위 메뉴들을\n매장 이용, 신용(체크)카드로\n결제하세요!";
		}
		else if (m_missionToGo == 1) {
			m_missions[3] = L"위 메뉴들을\n포장 이용, 신용(체크)카드로\n결제하세요!";
		}
		break;
	case 1:
		if (m_missionToGo == 0) {
			m_missions[3] = L"위 메뉴들을\n매장 이용, 모바일 바코드로\n결제하세요!";
		}
		else if (m_missionToGo == 1) {
			m_missions[3] = L"위 메뉴들을\n포장 이용, 모바일 바코드로\n결제하세요!";
		}
		break;
	case 2:
		if (m_missionToGo == 0) {
			m_missions[3] = L"위 메뉴들을\n매장 이용, 삼성 페이로\n결제하세요!";
		}
		else if (m_missionToGo == 1) {
			m_missions[3] = L"위 메뉴들을\n포장 이용, 삼성 페이로\n결제하세요!";
		}
		break;
	default:
		break;
	}
	Invalidate(false);
	return true;
}

// CMODOOView 메시지 처리기

void CMODOOView::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_window_pt.x = cx;
	m_window_pt.y = cy;
	CView::OnSize(nType, cx, cy);
}


void CMODOOView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	/* 화면별 Rectangle 클릭 이벤트 제어 */
	switch (m_screenType) {
	case SCR_MENU:
		if (scr_menu.rects[0].PtInRect(point)) {
			m_screenType = SCR_HELPS;
			m_helpPointer = 0;
		}
		else if (scr_menu.rects[1].PtInRect(point)) {
			m_screenType = SCR_BURGER;
			m_time = PLAY_TIME; // 게임 시간 1분
			setMission();
			SetTimer(0, 1000, NULL);
		} 
		break;
	case SCR_HELPS:
		if (m_helpPointer < 12) {
			m_helpPointer++;
		}

		if (m_returnToHome.PtInRect(point)) {
			m_screenType = SCR_MENU;
			m_helpPointer = 0;
			m_returnToHome = CRect();
		}
		else if (m_replay.PtInRect(point)) {
			m_helpPointer = 0;
			m_replay = CRect();
		}
		
		break;
	case SCR_BURGER: /* 버거 메뉴 */
		if (m_askSet) {
			if (m_onlyMenu.PtInRect(point)) {
				m_askSet = false;
			}
			else if (m_withSet.PtInRect(point)) {
				m_screenType = SCR_SET_DESSERT;
				m_askSet = false;
			}
		}
		else {
			if (scr_burger.rects[6].PtInRect(point)) { // <
				m_screenType = SCR_BURGER;
			}
			else if (scr_burger.rects[7].PtInRect(point)) { // >
				m_screenType = SCR_CHICKEN;
			}
			else if (scr_burger.rects[11].PtInRect(point)) { // 햄버거
				m_screenType = SCR_BURGER;
			}
			else if (scr_burger.rects[12].PtInRect(point)) { // 치킨
				m_screenType = SCR_CHICKEN;
			}
			else if (scr_burger.rects[13].PtInRect(point)) { // 디저트
				m_screenType = SCR_DESSERT;
			}
			else if (scr_burger.rects[14].PtInRect(point)) { // 음료
				m_screenType = SCR_DRINK;
			}
			else if (scr_burger.rects[17].PtInRect(point)) { // 취소하기
				if (m_selectedNum > 0) { m_selectedNum--; }
			}
			else if (scr_burger.rects[18].PtInRect(point)) { // 결제하기
				if (m_selectedNum > 0) {
					m_screenType = SCR_PAYMENT_1;
				}
			}
			else { // 각 버거 메뉴 선택
				for (int i = 0; i < 8; i++) {
					if (scr_burger.rects[22 + i].PtInRect(point)) {
						if (m_selectedNum < 3) {
							m_totalPrices[m_selectedNum] = m_burgerPrices[i];

							m_selectedMenus[m_selectedNum] = m_burgerNames[i];
							m_selectedNum++;

							m_askSet = true;
						}
					}
				}
			}
		}		
		break;

	case SCR_CHICKEN: /* 치킨 메뉴 */
		if (scr_chicken.rects[6].PtInRect(point)) { // <
			m_screenType = SCR_BURGER;
		}
		else if (scr_chicken.rects[7].PtInRect(point)) { // >
			m_screenType = SCR_DESSERT;
		}
		else if (scr_chicken.rects[11].PtInRect(point)) { // 햄버거
			m_screenType = SCR_BURGER;
		}
		else if (scr_chicken.rects[12].PtInRect(point)) { // 치킨
			m_screenType = SCR_CHICKEN;
		}
		else if (scr_chicken.rects[13].PtInRect(point)) { // 디저트
			m_screenType = SCR_DESSERT;
		}
		else if (scr_chicken.rects[14].PtInRect(point)) { // 음료
			m_screenType = SCR_DRINK;
		}
		else if (scr_chicken.rects[17].PtInRect(point)) { // 취소하기
			if (m_selectedNum > 0) { m_selectedNum--; }
		}
		else if (scr_chicken.rects[18].PtInRect(point)) { // 결제하기
			if (m_selectedNum > 0) {
				m_screenType = SCR_PAYMENT_1;
			}
		}
		else { // 각 치킨 메뉴 선택
			for (int i = 0; i < 6; i++) {
				if (scr_chicken.rects[22 + i].PtInRect(point)) {
					if (m_selectedNum < 3) {
						m_totalPrices[m_selectedNum] = m_chickenPrices[i];

						m_selectedMenus[m_selectedNum] = m_chickenNames[i];
						m_selectedNum++;
					}
				}
			}
		}
		break;
	case SCR_DESSERT: /* 디저트 메뉴 */
		if (scr_dessert.rects[6].PtInRect(point)) { // <
			m_screenType = SCR_CHICKEN;
		}
		else if (scr_dessert.rects[7].PtInRect(point)) { // >
			m_screenType = SCR_DRINK;
		}
		else if (scr_dessert.rects[11].PtInRect(point)) { // 햄버거
			m_screenType = SCR_BURGER;
		}
		else if (scr_dessert.rects[12].PtInRect(point)) { // 치킨
			m_screenType = SCR_CHICKEN;
		}
		else if (scr_dessert.rects[13].PtInRect(point)) { // 디저트
			m_screenType = SCR_DESSERT;
		}
		else if (scr_dessert.rects[14].PtInRect(point)) { // 음료
			m_screenType = SCR_DRINK;
		}
		else if (scr_dessert.rects[17].PtInRect(point)) { // 취소하기
			if (m_selectedNum > 0) { m_selectedNum--; }
		}
		else if (scr_dessert.rects[18].PtInRect(point)) { // 결제하기
			if (m_selectedNum > 0) {
				m_screenType = SCR_PAYMENT_1;
			}
		}
		else { // 각 디저트 메뉴 선택
			for (int i = 0; i < 8; i++) {
				if (scr_dessert.rects[22 + i].PtInRect(point)) {
					if (m_selectedNum < 3) {
						m_totalPrices[m_selectedNum] = m_dessertPrices[i];

						m_selectedMenus[m_selectedNum] = m_dessertNames[i];
						m_selectedNum++;
					}
				}
			}
		}
		break;

	case SCR_DRINK: /* 음료 메뉴 */
		if (scr_drink.rects[6].PtInRect(point)) { // <
			m_screenType = SCR_DESSERT;
		}
		else if (scr_drink.rects[7].PtInRect(point)) { // >
			m_screenType = SCR_DRINK;
		}
		else if (scr_drink.rects[11].PtInRect(point)) { // 햄버거
			m_screenType = SCR_BURGER;
		}
		else if (scr_drink.rects[12].PtInRect(point)) { // 치킨
			m_screenType = SCR_CHICKEN;
		}
		else if (scr_drink.rects[13].PtInRect(point)) { // 디저트
			m_screenType = SCR_DESSERT;
		}
		else if (scr_drink.rects[14].PtInRect(point)) { // 음료
			m_screenType = SCR_DRINK;
		}
		else if (scr_drink.rects[17].PtInRect(point)) { // 취소하기
			if (m_selectedNum > 0) { m_selectedNum--; }
		}
		else if (scr_drink.rects[18].PtInRect(point)) { // 결제하기
			if (m_selectedNum > 0) {
				m_screenType = SCR_PAYMENT_1;
			}
		}
		else { // 각 음료 메뉴 선택
			for (int i = 0; i < 8; i++) {
				if (scr_drink.rects[22 + i].PtInRect(point)) {
					if (m_selectedNum < 3) {
						m_totalPrices[m_selectedNum] = m_drinkPrices[i];

						m_selectedMenus[m_selectedNum] = m_drinkNames[i];
						m_selectedNum++;
					}
				}
			}
		}
		break;

	case SCR_SET_DESSERT:
		if (scr_set_dessert.rects[6].PtInRect(point)) { // <
			m_screenType = SCR_SET_DESSERT;
		}
		else if (scr_set_dessert.rects[7].PtInRect(point)) { // >
			m_screenType = SCR_SET_DRINK;
		}
		else if (scr_set_dessert.rects[8].PtInRect(point)) { // 세트_디저트
			m_screenType = SCR_SET_DESSERT;
		}
		else if (scr_set_dessert.rects[9].PtInRect(point)) { // 세트_음료
			m_screenType = SCR_SET_DRINK;
		}
		else if (scr_set_dessert.rects[18].PtInRect(point)) { // 취소하기
			m_screenType = SCR_BURGER;
			m_selectedSetMenu[0] = -1;
			m_selectedSetMenu[1] = -1;
		}
		else if (scr_set_dessert.rects[19].PtInRect(point)) { // 선택하기
			if (m_selectedSetMenu[0] != -1) {
				m_totalPrices[m_selectedNum - 1] += m_dessertPrices[m_selectedSetMenu[0]];
				m_selectedMenus[m_selectedNum - 1] += L"+" + m_dessertNames[m_selectedSetMenu[0]];
			}
			if (m_selectedSetMenu[1] != -1) {
				m_totalPrices[m_selectedNum - 1] += m_drinkPrices[m_selectedSetMenu[1]];
				m_selectedMenus[m_selectedNum - 1] += L"+" + m_drinkNames[m_selectedSetMenu[1]];
			}
			m_screenType = SCR_BURGER;
			m_selectedSetMenu[0] = -1;
			m_selectedSetMenu[1] = -1;
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (scr_set_dessert.rects[10 + i].PtInRect(point)) {
					m_selectedSetMenu[0] = i;
				}
			}
		}
		break;

	case SCR_SET_DRINK:
		if (scr_set_drink.rects[6].PtInRect(point)) { // <
			m_screenType = SCR_SET_DESSERT;
		}
		else if (scr_set_drink.rects[7].PtInRect(point)) { // >
			m_screenType = SCR_SET_DRINK;
		}
		else if (scr_set_drink.rects[8].PtInRect(point)) { // 세트_디저트
			m_screenType = SCR_SET_DESSERT;
		}
		else if (scr_set_drink.rects[9].PtInRect(point)) { // 세트_음료
			m_screenType = SCR_SET_DRINK;
		}
		else if (scr_set_drink.rects[18].PtInRect(point)) { // 취소하기
			m_screenType = SCR_BURGER;
			m_selectedSetMenu[0] = -1;
			m_selectedSetMenu[1] = -1;
		}
		else if (scr_set_drink.rects[19].PtInRect(point)) { // 선택하기
			if (m_selectedSetMenu[0] != -1) {
				m_totalPrices[m_selectedNum - 1] += m_dessertPrices[m_selectedSetMenu[0]];
				m_selectedMenus[m_selectedNum - 1] += L"+" + m_dessertNames[m_selectedSetMenu[0]];
			}
			if (m_selectedSetMenu[1] != -1) {
				m_totalPrices[m_selectedNum - 1] += m_drinkPrices[m_selectedSetMenu[1]];
				m_selectedMenus[m_selectedNum - 1] += L"+" + m_drinkNames[m_selectedSetMenu[1]];
			}
			m_screenType = SCR_BURGER;
			m_selectedSetMenu[0] = -1;
			m_selectedSetMenu[1] = -1;
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (scr_set_drink.rects[10 + i].PtInRect(point)) {
					m_selectedSetMenu[1] = i;
				}
			}
		}
		break;
	case SCR_PAYMENT_1:
		if (scr_payment_1.rects[15].PtInRect(point)) {
			m_isToGo = 1; // true
			m_screenType = SCR_PAYMENT_2;
		}
		else if (scr_payment_1.rects[16].PtInRect(point)) {
			m_isToGo = 0; // false
			m_screenType = SCR_PAYMENT_2;
		}
		else if (scr_payment_1.rects[23].PtInRect(point) || scr_payment_1.rects[24].PtInRect(point)) {
			m_isToGo = -1;
			m_paymentType = -1;
			m_screenType = SCR_BURGER;
			m_showPaymentRects = false;
		}
		break;
	case SCR_PAYMENT_2:
		if (scr_payment_2.rects[15].PtInRect(point)) {
			m_isToGo = 1;
			m_screenType = SCR_PAYMENT_2;
		}
		else if (scr_payment_2.rects[16].PtInRect(point)) {
			m_isToGo = 0;
			m_screenType = SCR_PAYMENT_2;
		}
		else if (scr_payment_2.rects[23].PtInRect(point) || scr_payment_2.rects[24].PtInRect(point)) {
			m_isToGo = -1;
			m_paymentType = -1;
			m_screenType = SCR_BURGER;
			m_showPaymentRects = false;
		}
		else if (scr_payment_2.rects[17].PtInRect(point)) { // 신용카트 결제
			m_paymentType = 0;
			m_showPaymentRects = true;
		}
		else if (scr_payment_2.rects[18].PtInRect(point)) { // 모바일 바코드 결제
			m_paymentType = 1;
			m_showPaymentRects = true;
		}
		else if (scr_payment_2.rects[19].PtInRect(point)) { // 삼성페이 결제
			m_paymentType = 2;
			m_showPaymentRects = true;
		}
		else if (m_paymentCard.PtInRect(point)) {
			m_paymentDrag = true;
			m_paymentDrag_pt = point;
			m_curDragPayment = 0;
		}
		else if (m_paymentCoupon.PtInRect(point)) {
			m_paymentDrag = true;
			m_paymentDrag_pt = point;
			m_curDragPayment = 1;
		}
		else if (m_paymentSmartPhone.PtInRect(point)) {
			m_paymentDrag = true;
			m_paymentDrag_pt = point;
			m_curDragPayment = 2;
		}
		break;
	default:
		break;
	}
	if (m_isFinish) {
		if (m_replay.PtInRect(point)) {
			m_screenType = SCR_BURGER;
			m_time = PLAY_TIME;
			setMission();
			SetTimer(0, 1000, NULL);
		}
		else if (m_returnToHome.PtInRect(point)) {
			m_screenType = SCR_MENU;
		}

		m_score = 0;
		m_selectedNum = 0;
		m_totalPrice = 0;
		m_askSet = false;
		m_showPaymentRects = false;
		m_paymentDrag = false;
		m_selectedSetMenu[0] = -1;
		m_selectedSetMenu[1] = -1;
		m_isToGo = -1;
		m_paymentType = -1;
		m_curDragPayment = -1;
		m_missionPaymentType = -1;
		m_isFinish = false;

		m_timeStr.Format(L"%d", m_time);
		m_scoreStr.Format(L"%d", m_score);
		for (int i = 0; i < 3; i++) {
			m_selectedMenus[i] = L"";
			m_totalPrices[i] = 0;
		}
		m_onlyMenu = CRect();
		m_withSet = CRect();
		m_paymentCard = CRect();
		m_paymentCoupon = CRect();
		m_paymentSmartPhone = CRect();
		m_finalScore = CRect();
		m_replay = CRect();
		m_returnToHome = CRect();
	}
	Invalidate(false); // 추후 이상하면 수정
	CView::OnLButtonDown(nFlags, point);
}


void CMODOOView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_time > 0) {
		m_time -= 1;
		m_timeStr.Format(L"%d", m_time);
		InvalidateRect(scr_burger.rects[0], false);
	}
	else {
		KillTimer(0);
		m_isFinish = true;
		Invalidate();
	}
	CView::OnTimer(nIDEvent);
}


void CMODOOView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_paymentDrag) {
		m_paymentDrag_pt = point;
		Invalidate(false);
	}
	CView::OnMouseMove(nFlags, point);
}


void CMODOOView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_paymentDrag) {
		if ((m_curDragPayment == 0 && scr_payment_2.rects[21].PtInRect(point)) ||
			(m_curDragPayment == 1 && scr_payment_2.rects[20].PtInRect(point)) ||
			(m_curDragPayment == 2 && scr_payment_2.rects[22].PtInRect(point))) {
			if ((m_selectedMenus[0].CompareNoCase(m_missions[0]) == 0) &&
				(m_selectedMenus[1].CompareNoCase(m_missions[1]) == 0) &&
				(m_selectedMenus[2].CompareNoCase(m_missions[2]) == 0) &&
				(m_curDragPayment == m_missionPaymentType)&&
				(m_isToGo == m_missionToGo)) {
				/* 성공 */
				m_score += 1;
				m_scoreStr.Format(L"%d", m_score);
			}
			else {
				/*실패*/
			}
			
			/* 초기화 */
			m_screenType = SCR_BURGER;
			m_selectedNum = 0;
			m_totalPrice = 0;
			m_showPaymentRects = false;
			m_selectedSetMenu[0] = -1;
			m_selectedSetMenu[1] = -1;
			m_isToGo = -1;
			m_paymentType = -1;
			m_curDragPayment = -1;
			for (int i = 0; i < 3; i++) {
				m_selectedMenus[i] = L"";
				m_totalPrices[i] = 0;
			}
		}
		m_paymentDrag = false;
		setMission();
		Invalidate(false);
	}
	CView::OnLButtonUp(nFlags, point);
}
