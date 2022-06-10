#include "pch.h"
#include "ScreenObject.h"

void ScreenObject::SetObjectByType(int screenType) {
	this->screenType = screenType;
	switch (screenType) {
	case SCR_MENU:
		this->imagePath = _T("./res/screen_menu.png");
		rects = new CRect[2]; // 스크린 내의 Rect 개수만큼 동적 할당
		//memset(rects, 0, sizeof(rects));
		break;
	case SCR_BURGER:
		this->imagePath = _T("./res/screen_burger.png");
		rects = new CRect[30];
		break;
	case SCR_CHICKEN:
		this->imagePath = _T("./res/screen_chicken.png");
		rects = new CRect[28];
		break;
	case SCR_DESSERT:
		this->imagePath = _T("./res/screen_dessert.png");
		rects = new CRect[30];
		break;
	case SCR_DRINK:
		this->imagePath = _T("./res/screen_drink.png");
		rects = new CRect[30];
		break;
	case SCR_SET_DESSERT:
		this->imagePath = _T("./res/screen_set_dessert.png");
		rects = new CRect[30];
		break;
	case SCR_SET_DRINK:
		this->imagePath = _T("./res/screen_set_drink.png");
		rects = new CRect[30];
		break;
	case SCR_PAYMENT_1:
		this->imagePath = _T("./res/screen_payment_1.png");
		rects = new CRect[30];
		break;
	case SCR_PAYMENT_2:
		this->imagePath = _T("./res/screen_payment_2.png");
		rects = new CRect[30];
		break;
	default:
		break;
	}
}

CString ScreenObject::GetPath() {

	return this->imagePath;
}

void ScreenObject::SetRectsPosition(CPoint window_pt) {
	
	if (this->screenType == SCR_MENU) { //메뉴화면
		this->rects[0] = CRect(		// 게임방법
			int(window_pt.x * 0.25),
			int(window_pt.y * 0.64),
			int(window_pt.x * 0.25) + int(window_pt.x * 0.22),
			int(window_pt.y * 0.64) + int(window_pt.y * 0.1)
		);
		this->rects[1] = CRect(		// 게임시작
			int(window_pt.x * 0.53),
			int(window_pt.y * 0.64),
			int(window_pt.x * 0.53) + int(window_pt.x * 0.22),
			int(window_pt.y * 0.64) + int(window_pt.y * 0.1)
		);
	}
	else { //플레이 화면
		/* 플레이 화면 공통(누를 필요 없는 Rect) */
		this->rects[0] = CRect(		// 시간
			int(window_pt.x * 0.052),
			int(window_pt.y * 0.115),
			int(window_pt.x * 0.052) + int(window_pt.x * 0.05),
			int(window_pt.y * 0.115) + int(window_pt.y * 0.065)
		);
		this->rects[1] = CRect(		// 점수
			int(window_pt.x * 0.927),
			int(window_pt.y * 0.065),
			int(window_pt.x * 0.927) + int(window_pt.x * 0.05),
			int(window_pt.y * 0.065) + int(window_pt.y * 0.065)
		);
		this->rects[2] = CRect(		// 미션1
			int(window_pt.x * 0.15),
			int(window_pt.y * 0.483),
			int(window_pt.x * 0.15) + int(window_pt.x * 0.11),
			int(window_pt.y * 0.483) + int(window_pt.y * 0.035)
		);
		this->rects[3] = CRect(		// 미션2
			int(window_pt.x * 0.15),
			int(window_pt.y * 0.538),
			int(window_pt.x * 0.15) + int(window_pt.x * 0.11),
			int(window_pt.y * 0.538) + int(window_pt.y * 0.035)
		);
		this->rects[4] = CRect(		// 미션3
			int(window_pt.x * 0.15),
			int(window_pt.y * 0.593),
			int(window_pt.x * 0.15) + int(window_pt.x * 0.11),
			int(window_pt.y * 0.593) + int(window_pt.y * 0.035)
		);
		this->rects[5] = CRect(		// 미션 - 결제 방법
			int(window_pt.x * 0.15),
			int(window_pt.y * 0.648),
			int(window_pt.x * 0.15) + int(window_pt.x * 0.11),
			int(window_pt.y * 0.648) + int(window_pt.y * 0.07)
		);
		if (this->screenType == SCR_PAYMENT_1 || this->screenType == SCR_PAYMENT_2) { // 결제화면
			for (int i = 0; i < 3; i++) {
				this-> rects[6+i] = CRect( //주문내역 1, 2, 3
					int(window_pt.x * 0.387),
					int(window_pt.y * 0.176) + int((window_pt.y * 0.05) * i),
					int(window_pt.x * 0.387) + int(window_pt.x * 0.07),
					int(window_pt.y * 0.176) + int((window_pt.y * 0.05) * (i+1))
				);
				this->rects[9 + i] = CRect( // 금액 1, 2, 3
					int(window_pt.x * 0.45),
					int(window_pt.y * 0.178) + int((window_pt.y * 0.051) * i),
					int(window_pt.x * 0.45) + int(window_pt.x * 0.05),
					int(window_pt.y * 0.178) + int((window_pt.y * 0.051) * (i + 1))
				);
			}
			this->rects[12] = CRect( // 주문금액
				int(window_pt.x * 0.44),
				int(window_pt.y * 0.426),
				int(window_pt.x * 0.44) + int(window_pt.x * 0.055),
				int(window_pt.y * 0.426) + int(window_pt.y * 0.032)
			);
			this->rects[13] = CRect( // 할인금액
				int(window_pt.x * 0.44),
				int(window_pt.y * 0.46),
				int(window_pt.x * 0.44) + int(window_pt.x * 0.055),
				int(window_pt.y * 0.46) + int(window_pt.y * 0.032)
			);
			this->rects[14] = CRect( // 결제할 금액
				int(window_pt.x * 0.456),
				int(window_pt.y * 0.508),
				int(window_pt.x * 0.456) + int(window_pt.x * 0.038),
				int(window_pt.y * 0.508) + int(window_pt.y * 0.029)
			);
			this->rects[15] = CRect( // 포장
				int(window_pt.x * 0.508),
				int(window_pt.y * 0.172),
				int(window_pt.x * 0.508) + int(window_pt.x * 0.05),
				int(window_pt.y * 0.172) + int(window_pt.y * 0.142)
			);
			this->rects[16] = CRect( // 매장
				int(window_pt.x * 0.558),
				int(window_pt.y * 0.172),
				int(window_pt.x * 0.558) + int(window_pt.x * 0.05),
				int(window_pt.y * 0.172) + int(window_pt.y * 0.142)
			);
			this->rects[17] = CRect( // 신용체크카드
				int(window_pt.x * 0.51),
				int(window_pt.y * 0.36),
				int(window_pt.x * 0.51) + int(window_pt.x * 0.03),
				int(window_pt.y * 0.36) + int(window_pt.y * 0.235)
			);
			this->rects[18] = CRect( // 모바일 바코드
				int(window_pt.x * 0.542),
				int(window_pt.y * 0.36),
				int(window_pt.x * 0.542) + int(window_pt.x * 0.03),
				int(window_pt.y * 0.36) + int(window_pt.y * 0.235)
			);
			this->rects[19] = CRect( // 삼성페이
				int(window_pt.x * 0.575),
				int(window_pt.y * 0.36),
				int(window_pt.x * 0.575) + int(window_pt.x * 0.03),
				int(window_pt.y * 0.36) + int(window_pt.y * 0.235)
			);
			this->rects[20] = CRect(		// 바코드
				int(window_pt.x * 0.465),
				int(window_pt.y * 0.737),
				int(window_pt.x * 0.465) + int(window_pt.x * 0.073),
				int(window_pt.y * 0.737) + int(window_pt.y * 0.243)
			);
			this->rects[21] = CRect(		// 카드투입구
				int(window_pt.x * 0.538),
				int(window_pt.y * 0.737),
				int(window_pt.x * 0.538) + int(window_pt.x * 0.073),
				int(window_pt.y * 0.737) + int(window_pt.y * 0.114)
			);
			this->rects[22] = CRect(		// 삼성페이
				int(window_pt.x * 0.538),
				int(window_pt.y * 0.852),
				int(window_pt.x * 0.538) + int(window_pt.x * 0.073),
				int(window_pt.y * 0.852) + int(window_pt.y * 0.115)
			);
			this->rects[23] = CRect(		// 결제취소
				int(window_pt.x * 0.497),
				int(window_pt.y * 0.662),
				int(window_pt.x * 0.497) + int(window_pt.x * 0.051),
				int(window_pt.y * 0.662) + int(window_pt.y * 0.026)
			);
			this->rects[24] = CRect(		// 추가주문
				int(window_pt.x * 0.55),
				int(window_pt.y * 0.662),
				int(window_pt.x * 0.55) + int(window_pt.x * 0.051),
				int(window_pt.y * 0.662) + int(window_pt.y * 0.026)
			);
		}//@
		else {
			/* 메뉴 화면 공통 Rect */
			this->rects[6] = CRect(		// 왼쪽 <
				int(window_pt.x * 0.385),
				int(window_pt.y * 0.06),
				int(window_pt.x * 0.385) + int(window_pt.x * 0.015),
				int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
			);
			this->rects[7] = CRect(		// 오른쪽 > 
				int(window_pt.x * 0.6),
				int(window_pt.y * 0.06),
				int(window_pt.x * 0.6) + int(window_pt.x * 0.015),
				int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
			);
			
			if (this->screenType == SCR_SET_DESSERT || this->screenType == SCR_SET_DRINK) {
				/* 세트 메뉴 선택 화면 */
				this->rects[8] = CRect(		// 세트_디저트
					int(window_pt.x * 0.417),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.417) + int(window_pt.x * 0.067),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				this->rects[9] = CRect(		// 세트_음료
					int(window_pt.x * 0.507),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.507) + int(window_pt.x * 0.067),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				for (int i = 0; i < 8; i++) { // 메뉴
					this->rects[10 + i] = CRect(
						int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * (i % 2)),
						int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * (i / 2)),
						int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * ((i % 2) + 1)),
						int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * ((i / 2) + 1))
					);
				}
				this->rects[18] = CRect( // 취소하기
					int(window_pt.x * 0.4),
					int(window_pt.y * 0.655),
					int(window_pt.x * 0.4) + int(window_pt.x * 0.067),
					int(window_pt.y * 0.655) + int(window_pt.y * 0.059)
				);
				this->rects[19] = CRect( // 선택하기
					int(window_pt.x * 0.533),
					int(window_pt.y * 0.655),
					int(window_pt.x * 0.533) + int(window_pt.x * 0.067),
					int(window_pt.y * 0.655) + int(window_pt.y * 0.059)
				);
			}
			else {
				/* 기본 메뉴 선택 화면 */
				for (int i = 0; i < 3; i++) {
					this->rects[8+i] = CRect(		// 주문내역1,2,3
						int(window_pt.x * 0.4),
						int(window_pt.y * 0.596) + int(window_pt.y * 0.03 * i),
						int(window_pt.x * 0.4) + int(window_pt.x * 0.227),
						int(window_pt.y * 0.596) + int(window_pt.y * 0.03 * (i+1))
					);
				}
				this->rects[11] = CRect(		// 햄버거
					int(window_pt.x * 0.41),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.41) + int(window_pt.x * 0.045),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				this->rects[12] = CRect(		// 치킨
					int(window_pt.x * 0.455),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.455) + int(window_pt.x * 0.045),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				this->rects[13] = CRect(		// 디저트
					int(window_pt.x * 0.5),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.5) + int(window_pt.x * 0.045),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				this->rects[14] = CRect(		// 음료/커피
					int(window_pt.x * 0.545),
					int(window_pt.y * 0.06),
					int(window_pt.x * 0.545) + int(window_pt.x * 0.045),
					int(window_pt.y * 0.06) + int(window_pt.y * 0.075)
				);
				this->rects[15] = CRect(		// 주문내역(개)
					int(window_pt.x * 0.445),
					int(window_pt.y * 0.557),
					int(window_pt.x * 0.445) + int(window_pt.x * 0.04),
					int(window_pt.y * 0.557) + int(window_pt.y * 0.03)
				);
				this->rects[16] = CRect(		// 금액(숫자)
					int(window_pt.x * 0.5),
					int(window_pt.y * 0.557),
					int(window_pt.x * 0.5) + int(window_pt.x * 0.11),
					int(window_pt.y * 0.557) + int(window_pt.y * 0.03)
				);
				this->rects[17] = CRect(		// 취소하기
					int(window_pt.x * 0.5),
					int(window_pt.y * 0.687),
					int(window_pt.x * 0.5) + int(window_pt.x * 0.052),
					int(window_pt.y * 0.687) + int(window_pt.y * 0.031)
				);
				this->rects[18] = CRect(		// 결제하기
					int(window_pt.x * 0.556),
					int(window_pt.y * 0.687),
					int(window_pt.x * 0.556) + int(window_pt.x * 0.052),
					int(window_pt.y * 0.687) + int(window_pt.y * 0.031)
				);
				this->rects[19] = CRect(		// 바코드
					int(window_pt.x * 0.463),
					int(window_pt.y * 0.737),
					int(window_pt.x * 0.463) + int(window_pt.x * 0.073),
					int(window_pt.y * 0.737) + int(window_pt.y * 0.243)
				);
				this->rects[20] = CRect(		// 카드투입구
					int(window_pt.x * 0.536),
					int(window_pt.y * 0.737),
					int(window_pt.x * 0.536) + int(window_pt.x * 0.073),
					int(window_pt.y * 0.737) + int(window_pt.y * 0.114)
				);
				this->rects[21] = CRect(		// 삼성페이
					int(window_pt.x * 0.536),
					int(window_pt.y * 0.852),
					int(window_pt.x * 0.536) + int(window_pt.x * 0.073),
					int(window_pt.y * 0.852) + int(window_pt.y * 0.115)
				);
				if (this->screenType == SCR_CHICKEN) { // 메뉴 6개
					for (int i = 0; i < 6; i++) {
						this->rects[22 + i] = CRect(
							int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * (i % 2)),
							int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * (i / 2)),
							int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * ((i % 2) + 1)),
							int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * ((i / 2) + 1))
						);
					}
				}
				else { // 메뉴 8개
					for (int i = 0; i < 8; i++) {
						this->rects[22 + i] = CRect(
							int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * (i % 2)),
							int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * (i / 2)),
							int(window_pt.x * 0.385) + (int(window_pt.x * 0.113) * ((i % 2) + 1)),
							int(window_pt.y * 0.142) + (int(window_pt.y * 0.096) * ((i / 2) + 1))
						);
					}
				}
			}
		}
		
	}
	
	//switch (this->screenType) {
	//case SCR_MENU:
	//	
	//	break;
	//case SCR_BURGER:
	//	
	//	break;
	//case SCR_CHICKEN:
	//	break;
	//case SCR_DESSERT:
	//	break;
	//case SCR_DRINK:
	//	break;
	//default:
	//	break;
	//}
}