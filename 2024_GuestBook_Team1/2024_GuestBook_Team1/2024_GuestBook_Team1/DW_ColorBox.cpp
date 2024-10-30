#include "DW_ColorBox.h"


LRESULT CALLBACK DrowWindow::WndProcCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    DrowWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessageCB(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void DW_ColorBox::CreatePop(HWND hParentWnd, int x, int y, int width, int height)
{

    //PenThickness thicknessDialog;
    //thicknessDialog.setThicknessChangedCallback([this](int newThickness) {
    //    colorPicker->setThickness(newThickness);
    //    RECT previewRect = colorPicker->getPreviewRect();
    //    InvalidateRect(bWnd, &previewRect, TRUE);

    //    });

    //colorPicker->setThickness(PenThickness::getPenWidth());
    // ColorPicker에 펜 두께 설정
    //int initialThickness = PenThickness::getPenWidth();
    // CreateWindowEx를 사용하여 닫기 버튼을 생성하는 것은 이 위치에서 수행해야 함.

    //closeButton = CreateWindowEx(
    //    0, L"BUTTON", L"취소",
    //    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
    //    200, 350, 100, 50,  // 위치 및 크기 설정
    //    bWnd,
    //    (HMENU)IDC_CLOSE_BUTTON,  // 닫기 버튼 ID
    //    bInst, NULL
    //);
}

COLORREF DW_ColorBox::getSelectedColor() const {
   return RGB(255, 255, 255); // 기본 색상으로 흰색 사용
}


LRESULT DrowWindow::handleMessageCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    POINT mouse_position;

    switch (message) {
    case WM_CREATE: {
        //colorPicker->showPicker(pWnd);  // WM_CREATE에서 슬라이더 생성
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        //drawTitleBar(hdc);  // 추가된 표시줄 그리기

        // ColorPicker의 주요 기능 호출
        //colorPicker->drawColors(hdc);
        //colorPicker->drawPreview(hdc);
        //colorPicker->Draw(hdc);
        //TextOut(hdc, 10, 200, L"Thickness:", 9);

        // Draw Box Outline
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        HPEN hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 234, 234)));
        Rectangle(hdc, drowRT.left, drowRT.top, drowRT.right, drowRT.bottom);

        // 리소스 해제
        SelectObject(hdc, hOldBrush);
        DeleteObject(hOldPen);  // hOldPen 리소스 해제
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND: {
        //if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {  // 닫기 버튼 클릭 시 창 숨기기
        //    ShowWindow(pWnd, SW_HIDE);
        //    return 0;
        //}
        break;
    }
                   //case WM_HSCROLL: {  // 슬라이더의 위치가 변경될 때 호출
                   //    if (colorPicker && lParam == (LPARAM)colorPicker->getSliderHandle()) {
                   //        colorPicker->updateThickness();  // 선택된 굵기를 업데이트
                   //        RECT previewRect = colorPicker->getPreviewRect();  // 미리보기 영역
                   //        InvalidateRect(pWnd, &previewRect, TRUE);  // 미리보기 영역만 갱신
                   //    }
                   //    break;
                   //}
    case WM_LBUTTONUP: {
        mouse_position.x = LOWORD(lParam);
        mouse_position.y = HIWORD(lParam);

        // colorPicker->MouseUp();
        break;
    }
    case WM_LBUTTONDOWN: {
        mouse_position.x = LOWORD(lParam);
        mouse_position.y = HIWORD(lParam);

        // ColorPicker에서 색상 선택 업데이트 후 영역 다시 그리기
        //colorPicker->handleColorSelection(pWnd, mouse_position.x, mouse_position.y);
        //colorPicker->MouseDown(mouse_position);
        break;
    }

                       // 취소 버튼 스타일
    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pDis = (LPDRAWITEMSTRUCT)lParam;
        if (pDis->CtlType == ODT_BUTTON) {
            HDC hdc = pDis->hDC;
            RECT rect = pDis->rcItem;

            FillRect(hdc, &rect, (HBRUSH)(COLOR_BTNFACE + 1));

            HFONT hFont = CreateFont(
                25,            // 글꼴 높이
                0, 0, 0,
                FW_BOLD,       // 글꼴 두께 (굵기)
                FALSE,         // 이탤릭체
                FALSE,         // 밑줄
                FALSE,         // 취소선
                DEFAULT_CHARSET, // 문자 집합
                OUT_OUTLINE_PRECIS, // 출력 정밀도
                CLIP_DEFAULT_PRECIS, // 클립 정밀도
                DEFAULT_QUALITY, // 품질
                DEFAULT_QUALITY, // 이식성
                L"G마켓 산스 TTF 중간"        // 글꼴 이름
            );

            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(0, 0, 0));
            DrawText(hdc, L"취소", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont); // 글꼴 객체 해제
        }
        return TRUE;
    }


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
