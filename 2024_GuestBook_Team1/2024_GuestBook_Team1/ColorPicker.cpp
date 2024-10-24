#include "ColorPicker.h"

ColorPicker::ColorPicker() {
    // 16���� ���� �귯�� ����
    colorBrush[0] = CreateSolidBrush(RGB(255, 0, 0));    // ����
    colorBrush[1] = CreateSolidBrush(RGB(0, 255, 0));    // �ʷ�
    colorBrush[2] = CreateSolidBrush(RGB(0, 0, 255));    // �Ķ�
    colorBrush[3] = CreateSolidBrush(RGB(255, 255, 0));  // ���
    colorBrush[4] = CreateSolidBrush(RGB(255, 0, 255));  // ��ȫ
    colorBrush[5] = CreateSolidBrush(RGB(0, 255, 255));  // û��
    colorBrush[6] = CreateSolidBrush(RGB(128, 0, 0));    // ��ο� ����
    colorBrush[7] = CreateSolidBrush(RGB(0, 128, 0));    // ��ο� �ʷ�
    colorBrush[8] = CreateSolidBrush(RGB(0, 0, 128));    // ��ο� �Ķ�
    colorBrush[9] = CreateSolidBrush(RGB(128, 128, 0));  // ��ο� ���
    colorBrush[10] = CreateSolidBrush(RGB(128, 0, 128)); // ��ο� ��ȫ
    colorBrush[11] = CreateSolidBrush(RGB(0, 128, 128)); // ��ο� û��
    colorBrush[12] = CreateSolidBrush(RGB(192, 192, 192)); // ȸ��
    colorBrush[13] = CreateSolidBrush(RGB(128, 128, 128)); // ��ο� ȸ��
    colorBrush[14] = CreateSolidBrush(RGB(0, 0, 0));      // ����
    colorBrush[15] = CreateSolidBrush(RGB(255, 255, 255)); // ���

    // �ʱ� ���� ���� �� ���� ����
    selectedColor = RGB(0, 0, 0);
    thickness = 1;

    // �簢�� ���� ���� (4x4 �迭)
    int x = 10, y = 10;
    for (int i = 0; i < 16; i++) {
        colorRect[i] = { x, y, x + 40, y + 40 };
        x += 50;
        if ((i + 1) % 4 == 0) {
            x = 10;
            y += 50;
        }
    }

    // �̸����� ���� �� ���� ���� ����
    previewRect = { 10, 290, 250, 340 };
    thicknessRect = { 10, 230, 250, 270 };
}

ColorPicker::~ColorPicker() {
    for (int i = 0; i < 16; i++) {
        DeleteObject(colorBrush[i]);
    }
}

//void ColorPicker::showPicker(HWND parentWnd) {
//    // �÷� ��Ŀ â ���� �� ������ ũ��� ����
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
//    // �����̴� ����
//    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Thickness",
//        WS_CHILD | WS_VISIBLE | TBS_HORZ,
//        10, 230, 200, 30,
//        hWnd, NULL, GetModuleHandle(NULL), NULL);
//
//    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));  // ���� ���� 1-20 ����
//    SendMessage(hSlider, TBM_SETPOS, TRUE, thickness);          // �ʱ� ���� �� ����
//}

void ColorPicker::showPicker(HWND parentWnd) {
    // �����̴� ����
    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Thickness",
        WS_CHILD | WS_VISIBLE | TBS_HORZ,
        10, 230, 200, 30,
        parentWnd, NULL, GetModuleHandle(NULL), NULL);

    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));  // ���� ���� 1-20 ����
    SendMessage(hSlider, TBM_SETPOS, TRUE, thickness);          // �ʱ� ���� �� ����
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

        // ���� ���� �׸���
        picker->drawColors(hdc);

        // ���� ���� �ؽ�Ʈ �׸���
       // TextOut(hdc, 10, 200, L"Thickness:", 9);

        // �̸����� ���� �׸���
        picker->drawPreview(hdc);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // Ŭ���� ��ǥ�� ���� �簢�� �������� Ȯ���Ͽ� ���õ� ���� ������Ʈ
        for (int i = 0; i < 16; i++) {
            if (x >= picker->colorRect[i].left && x <= picker->colorRect[i].right &&
                y >= picker->colorRect[i].top && y <= picker->colorRect[i].bottom) {
                // ���� �귯�ÿ��� ���� ���� ������ ����
                LOGBRUSH lb;
                GetObject(picker->colorBrush[i], sizeof(LOGBRUSH), &lb);
                picker->selectedColor = lb.lbColor;  // Ŭ���� ������ ����
                InvalidateRect(hWnd, &picker->previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
                break;
            }
        }

        break;
    }

    case WM_HSCROLL: {
        if ((HWND)lParam == picker->hSlider) {
            // �����̴��� ���� �� �о ���� ������Ʈ
            picker->thickness = SendMessage(picker->hSlider, TBM_GETPOS, 0, 0);
            InvalidateRect(hWnd, &picker->previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
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
    // �̸����� ������ ������� �����
    FillRect(hdc, &previewRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // ���ο� ����� ���� �׸���
    HPEN pen = CreatePen(PS_SOLID, thickness, selectedColor);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    // �̸����� ���� �߾ӿ� �� �׸���
    int centerY = (previewRect.top + previewRect.bottom) / 2;
    MoveToEx(hdc, previewRect.left + 10, centerY, NULL);
    LineTo(hdc, previewRect.right - 10, centerY);

    // �� ���ҽ� ����
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void ColorPicker::handleColorSelection(HWND hWnd, int x, int y) {
    // Ŭ���� ��ǥ�� ���� �簢�� �������� Ȯ���Ͽ� ���õ� ���� ������Ʈ
    for (int i = 0; i < 16; i++) {
        if (x >= colorRect[i].left && x <= colorRect[i].right &&
            y >= colorRect[i].top && y <= colorRect[i].bottom) {
            LOGBRUSH lb;
            GetObject(colorBrush[i], sizeof(LOGBRUSH), &lb);
            selectedColor = lb.lbColor;  // ���õ� ���� ������Ʈ
            InvalidateRect(hWnd, &previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
            break;
        }
    }
}
