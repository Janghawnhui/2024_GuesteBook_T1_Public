#include "ColorPicker.h"

ColorPicker::ColorPicker() {
    // 16개의 색상 브러시 생성
    colorBrush[0] = CreateSolidBrush(RGB(255, 0, 0));    // 빨강
    colorBrush[1] = CreateSolidBrush(RGB(0, 255, 0));    // 초록
    colorBrush[2] = CreateSolidBrush(RGB(0, 0, 255));    // 파랑
    colorBrush[3] = CreateSolidBrush(RGB(255, 255, 0));  // 노랑
    colorBrush[4] = CreateSolidBrush(RGB(255, 0, 255));  // 자홍
    colorBrush[5] = CreateSolidBrush(RGB(0, 255, 255));  // 청록
    colorBrush[6] = CreateSolidBrush(RGB(128, 0, 0));    // 어두운 빨강
    colorBrush[7] = CreateSolidBrush(RGB(0, 128, 0));    // 어두운 초록
    colorBrush[8] = CreateSolidBrush(RGB(0, 0, 128));    // 어두운 파랑
    colorBrush[9] = CreateSolidBrush(RGB(128, 128, 0));  // 어두운 노랑
    colorBrush[10] = CreateSolidBrush(RGB(128, 0, 128)); // 어두운 자홍
    colorBrush[11] = CreateSolidBrush(RGB(0, 128, 128)); // 어두운 청록
    colorBrush[12] = CreateSolidBrush(RGB(192, 192, 192)); // 회색
    colorBrush[13] = CreateSolidBrush(RGB(128, 128, 128)); // 어두운 회색
    colorBrush[14] = CreateSolidBrush(RGB(0, 0, 0));      // 검정
    colorBrush[15] = CreateSolidBrush(RGB(255, 255, 255)); // 흰색

    // 초기 선택 색상 및 굵기 설정
    selectedColor = RGB(0, 0, 0);
    thickness = 1;

    // 사각형 영역 설정 (4x4 배열)
    int x = 10, y = 10;
    for (int i = 0; i < 16; i++) {
        colorRect[i] = { x, y, x + 40, y + 40 };
        x += 50;
        if ((i + 1) % 4 == 0) {
            x = 10;
            y += 50;
        }
    }

    // 미리보기 영역 및 굵기 영역 정의
    previewRect = { 10, 290, 250, 340 };
    thicknessRect = { 10, 230, 250, 270 };
}

ColorPicker::~ColorPicker() {
    for (int i = 0; i < 16; i++) {
        DeleteObject(colorBrush[i]);
    }
}

//void ColorPicker::showPicker(HWND parentWnd) {
//    // 컬러 피커 창 생성 및 고정된 크기로 설정
//    WNDCLASS wc = { 0 };
//    wc.lpfnWndProc = ColorPickerWndProc;
//    wc.hInstance = GetModuleHandle(NULL);
//    wc.lpszClassName = L"ColorPickerClass";
//    RegisterClass(&wc);
//
//    hWnd = CreateWindowEx(0, L"ColorPickerClass", L"Color Picker",
//        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
//        parentWnd, NULL, GetModuleHandle(NULL), this);
//
//    // 슬라이더 생성
//    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Thickness",
//        WS_CHILD | WS_VISIBLE | TBS_HORZ,
//        10, 230, 200, 30,
//        hWnd, NULL, GetModuleHandle(NULL), NULL);
//
//    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));  // 굵기 범위 1-20 설정
//    SendMessage(hSlider, TBM_SETPOS, TRUE, thickness);          // 초기 굵기 값 설정
//}

void ColorPicker::showPicker(HWND parentWnd) {
    // 슬라이더 생성
    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Thickness",
        WS_CHILD | WS_VISIBLE | TBS_HORZ,
        10, 230, 200, 30,
        parentWnd, NULL, GetModuleHandle(NULL), NULL);

    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));  // 굵기 범위 1-20 설정
    SendMessage(hSlider, TBM_SETPOS, TRUE, thickness);          // 초기 굵기 값 설정
}

LRESULT CALLBACK ColorPicker::ColorPickerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    ColorPicker* picker = reinterpret_cast<ColorPicker*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message) {
    case WM_CREATE:
        picker = reinterpret_cast<ColorPicker*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)picker);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 색상 영역 그리기
        picker->drawColors(hdc);

        // 굵기 영역 텍스트 그리기
       // TextOut(hdc, 10, 200, L"Thickness:", 9);

        // 미리보기 영역 그리기
        picker->drawPreview(hdc);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // 클릭된 좌표가 색상 사각형 내부인지 확인하여 선택된 색상 업데이트
        for (int i = 0; i < 16; i++) {
            if (x >= picker->colorRect[i].left && x <= picker->colorRect[i].right &&
                y >= picker->colorRect[i].top && y <= picker->colorRect[i].bottom) {
                // 색상 브러시에서 직접 색상 정보를 얻어옴
                LOGBRUSH lb;
                GetObject(picker->colorBrush[i], sizeof(LOGBRUSH), &lb);
                picker->selectedColor = lb.lbColor;  // 클릭한 색상을 선택
                InvalidateRect(hWnd, &picker->previewRect, TRUE);  // 미리보기 영역 다시 그리기
                break;
            }
        }

        break;
    }

    case WM_HSCROLL: {
        if ((HWND)lParam == picker->hSlider) {
            // 슬라이더의 현재 값 읽어서 굵기 업데이트
            picker->thickness = SendMessage(picker->hSlider, TBM_GETPOS, 0, 0);
            InvalidateRect(hWnd, &picker->previewRect, TRUE);  // 미리보기 영역 다시 그리기
        }
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void ColorPicker::drawColors(HDC hdc) {
    for (int i = 0; i < 16; i++) {
        FillRect(hdc, &colorRect[i], colorBrush[i]);
    }
}

void ColorPicker::drawPreview(HDC hdc) {
    // 미리보기 영역을 흰색으로 지우기
    FillRect(hdc, &previewRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // 새로운 굵기로 선을 그리기
    HPEN pen = CreatePen(PS_SOLID, thickness, selectedColor);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    // 미리보기 영역 중앙에 선 그리기
    int centerY = (previewRect.top + previewRect.bottom) / 2;
    MoveToEx(hdc, previewRect.left + 10, centerY, NULL);
    LineTo(hdc, previewRect.right - 10, centerY);

    // 펜 리소스 정리
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void ColorPicker::handleColorSelection(HWND hWnd, int x, int y) {
    // 클릭된 좌표가 색상 사각형 내부인지 확인하여 선택된 색상 업데이트
    for (int i = 0; i < 16; i++) {
        if (x >= colorRect[i].left && x <= colorRect[i].right &&
            y >= colorRect[i].top && y <= colorRect[i].bottom) {
            LOGBRUSH lb;
            GetObject(colorBrush[i], sizeof(LOGBRUSH), &lb);
            selectedColor = lb.lbColor;  // 선택된 색상 업데이트
            InvalidateRect(hWnd, &previewRect, TRUE);  // 미리보기 영역 다시 그리기
            break;
        }
    }
}
