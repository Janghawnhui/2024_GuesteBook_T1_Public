#include "DW_ColorBox.h"
#include <windows.h>

DW_ColorBox::DW_ColorBox(HINSTANCE hInstance)
    : ChildWindow(RGB(243, 243, 243)), penMemory(new std::vector<PINFO>), bInst(hInstance)
{
    BoxRT = { 0 };
    bWnd = nullptr;

    colorPicker = std::make_unique<ColorPicker>(bWnd);

    // �ݱ� ��ư ����
    closeButton = CreateWindowEx(
        0, L"BUTTON", L"�ݱ�",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 5, 50, 25,  // ��ġ �� ũ�� ����
        bWnd,
        (HMENU)IDC_CLOSE_BUTTON,  // �ݱ� ��ư ID
        bInst, NULL
    );
    // Ȯ�� ��ư ����
    OKButton = CreateWindowEx(
        0, L"BUTTON", L"Ȯ��",
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

    // CreateWindowEx�� ����Ͽ� �ݱ� ��ư�� �����ϴ� ���� �� ��ġ���� �����ؾ� ��.
    closeButton = CreateWindowEx(
        0, L"BUTTON", L"�ݱ�",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        150, 5, 50, 25,  // ��ġ �� ũ�� ����
        bWnd,
        (HMENU)IDC_CLOSE_BUTTON,  // �ݱ� ��ư ID
        bInst, NULL
    );
    OKButton = CreateWindowEx(
        0, L"BUTTON", L"Ȯ��",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 5, 50, 25,  // ��ġ �� ũ�� ����
        bWnd,
        (HMENU)IDC_OK_BUTTON,  // Ȯ�� ��ư ID
        bInst, NULL
    );
}

COLORREF DW_ColorBox::getSelectedColor() const {
    return colorPicker ? colorPicker->getSelectedColor() : RGB(255, 255, 255); // �⺻ �������� ��� ���
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
        // `WM_CREATE`���� �����̴� ����
        colorPicker->showPicker(pWnd);
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(pWnd, &ps);

        drawTitleBar(hdc); // �߰��� ǥ���� �׸���

        // ColorPicker�� �ֿ� ��� ȣ��
        colorPicker->drawColors(hdc);
        colorPicker->drawPreview(hdc);
        colorPicker->Draw(hdc);
        TextOut(hdc, 10, 200, L"Thickness:", 9);

        // Draw Box Outline
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        HPEN hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 234, 234)));
        Rectangle(hdc, BoxRT.left, BoxRT.top, BoxRT.right, BoxRT.bottom);

        // ���ҽ� ����
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        EndPaint(pWnd, &ps);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {  // �ݱ� ��ư�� Ŭ������ �� â �ݱ�
            ShowWindow(pWnd, SW_HIDE);
            return 0;
        }
        //else if (LOWORD(wParam) == IDC_OK_BUTTON) {  // Ȯ�� ��ư�� Ŭ������ ��
        //    // ������ �۾��� ���⿡ �߰��մϴ�.
        //    // ���� ���, ������ �����ϴ� �ڵ�

        //    // â�� �ݱ�
        //    ShowWindow(pWnd, SW_HIDE);
        //    return 0;
        //}
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // ColorPicker���� ���� ���� ������Ʈ �� ���� �ٽ� �׸���
        colorPicker->handleColorSelection(pWnd, x, y);
        InvalidateRect(pWnd, NULL, TRUE);  // ��ü â ����
        break;
    }

    default:
        return ChildWindow::HandleMessage(pWnd, message, wParam, lParam);
    }
    return 0;
}

void DW_ColorBox::drawTitleBar(HDC hdc) {
    RECT titleRect = { 0, 0, BoxRT.right, 30 };
    HBRUSH titleBrush = CreateSolidBrush(RGB(200, 200, 200));  // ǥ���� ����
    FillRect(hdc, &titleRect, titleBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    DrawText(hdc, L"���� �� ���� ����", -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    DeleteObject(titleBrush);
}

