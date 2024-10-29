#include "DW_ColorBox.h"
#include <windows.h>

DW_ColorBox::DW_ColorBox(HINSTANCE hInstance)
    : ChildWindow(RGB(243, 243, 243)), penMemory(new std::vector<PINFO>), bInst(hInstance)
{
    BoxRT = { 0 };
    bWnd = nullptr;

    colorPicker = std::make_unique<ColorPicker>(bWnd);

    // 닫기 버튼 생성
    closeButton = CreateWindowEx(
        0, L"BUTTON", L"닫기",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 5, 50, 25,  // 위치 및 크기 설정
        bWnd,
        (HMENU)IDC_CLOSE_BUTTON,  // 닫기 버튼 ID
        bInst, NULL
    );
    // 확인 버튼 생성
    OKButton = CreateWindowEx(
        0, L"BUTTON", L"확인",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 5, 50, 25,
        bWnd,
        (HMENU)IDC_OK_BUTTON,
        bInst, NULL
    );
    
}

void DW_ColorBox::CreatePop(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::CreatePop(hParentWnd, L"DW_SideMenuClass", L"Side Child Window", x, y, width, height);
    bWnd = cWnd;

    // CreateWindowEx를 사용하여 닫기 버튼을 생성하는 것은 이 위치에서 수행해야 함.
    closeButton = CreateWindowEx(
        0, L"BUTTON", L"닫기",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 5, 50, 25,  // 위치 및 크기 설정
        bWnd,
        (HMENU)IDC_CLOSE_BUTTON,  // 닫기 버튼 ID
        bInst, NULL
    );
    OKButton = CreateWindowEx(
        0, L"BUTTON", L"확인",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 5, 50, 25,  // 위치 및 크기 설정
        bWnd,
        (HMENU)IDC_OK_BUTTON,  // 확인 버튼 ID
        bInst, NULL
    );
}

COLORREF DW_ColorBox::getSelectedColor() const {
    return colorPicker ? colorPicker->getSelectedColor() : RGB(255, 255, 255); // 기본 색상으로 흰색 사용
}

COLORREF DW_ColorBox::getSelectedColor() {
    return colorPicker->getSelectedColor();
}

LRESULT DW_ColorBox::HandleMessage(HWND pWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    /*case WM_KILLFOCUS:
        ShowWindow(pWnd, false);
        break;*/

    case WM_CREATE: {
        // `WM_CREATE`에서 슬라이더 생성
        colorPicker->showPicker(pWnd);
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(pWnd, &ps);

        drawTitleBar(hdc); // 추가된 표시줄 그리기

        // ColorPicker의 주요 기능 호출
        colorPicker->drawColors(hdc);
        colorPicker->drawPreview(hdc);
        colorPicker->Draw(hdc);
        TextOut(hdc, 10, 200, L"Thickness:", 9);

        // Draw Box Outline
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        HPEN hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 234, 234)));
        Rectangle(hdc, BoxRT.left, BoxRT.top, BoxRT.right, BoxRT.bottom);

        // 리소스 해제
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        EndPaint(pWnd, &ps);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {  // 닫기 버튼을 클릭했을 때 창 닫기
            ShowWindow(pWnd, SW_HIDE);
            return 0;
        }
        //else if (LOWORD(wParam) == IDC_OK_BUTTON) {  // 확인 버튼을 클릭했을 때
        //    // 적용할 작업을 여기에 추가합니다.
        //    // 예를 들어, 설정을 저장하는 코드

        //    // 창을 닫기
        //    ShowWindow(pWnd, SW_HIDE);
        //    return 0;
        //}
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // ColorPicker에서 색상 선택 업데이트 후 영역 다시 그리기
        colorPicker->handleColorSelection(pWnd, x, y);
        InvalidateRect(pWnd, NULL, TRUE);  // 전체 창 갱신
        break;
    }

    default:
        return ChildWindow::HandleMessage(pWnd, message, wParam, lParam);
    }
    return 0;
}

void DW_ColorBox::drawTitleBar(HDC hdc) {
    RECT titleRect = { 0, 0, BoxRT.right, 30 };
    HBRUSH titleBrush = CreateSolidBrush(RGB(200, 200, 200));  // 표시줄 배경색
    FillRect(hdc, &titleRect, titleBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    DrawText(hdc, L"색상 및 굵기 선택", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    DeleteObject(titleBrush);
}

