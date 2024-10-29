#include "DW_ColorBox.h"
#include <windows.h>

DW_ColorBox::DW_ColorBox(HINSTANCE hInstance)
    : ChildWindow(RGB(243, 243, 243)), penMemory(new std::vector<PINFO>), bInst(hInstance)
{
    BoxRT = { 0 };
    bWnd = nullptr;

    colorPicker = std::make_unique<ColorPicker>(bWnd);
    
}

void DW_ColorBox::CreatePop(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::CreatePop(hParentWnd, L"DW_SideMenuClass", L"Side Child Window", x, y, width, height);
    bWnd = cWnd;

    PenThickness thicknessDialog;
    thicknessDialog.setThicknessChangedCallback([this](int newThickness) {
        colorPicker->setThickness(newThickness);
        RECT previewRect = colorPicker->getPreviewRect();
        InvalidateRect(bWnd, &previewRect, TRUE);

        });

    colorPicker->setThickness(PenThickness::getPenWidth());
    // ColorPicker�� �� �β� ����
    int initialThickness = PenThickness::getPenWidth();
    // CreateWindowEx�� ����Ͽ� �ݱ� ��ư�� �����ϴ� ���� �� ��ġ���� �����ؾ� ��.

    closeButton = CreateWindowEx(
        0, L"BUTTON", L"���",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        200, 350, 100, 50,  // ��ġ �� ũ�� ����
        bWnd,
        (HMENU)IDC_CLOSE_BUTTON,  // �ݱ� ��ư ID
        bInst, NULL
    );
}

COLORREF DW_ColorBox::getSelectedColor() const {
    return colorPicker ? colorPicker->getSelectedColor() : RGB(255, 255, 255); // �⺻ �������� ��� ���
}



LRESULT DW_ColorBox::HandleMessage(HWND pWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    POINT mouse_position;

    switch (message) {
    case WM_CREATE: {
        colorPicker->showPicker(pWnd);  // WM_CREATE���� �����̴� ����
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(pWnd, &ps);

        drawTitleBar(hdc);  // �߰��� ǥ���� �׸���

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
        DeleteObject(hOldPen);  // hOldPen ���ҽ� ����
        EndPaint(pWnd, &ps);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {  // �ݱ� ��ư Ŭ�� �� â �����
            ShowWindow(pWnd, SW_HIDE);
            return 0;
        }
        break;
    }
    case WM_HSCROLL: {  // �����̴��� ��ġ�� ����� �� ȣ��
        if (colorPicker && lParam == (LPARAM)colorPicker->getSliderHandle()) {
            colorPicker->updateThickness();  // ���õ� ���⸦ ������Ʈ
            RECT previewRect = colorPicker->getPreviewRect();  // �̸����� ����
            InvalidateRect(pWnd, &previewRect, TRUE);  // �̸����� ������ ����
        }
        break;
    }
    case WM_LBUTTONUP: {
        mouse_position.x = LOWORD(lParam);
        mouse_position.y = HIWORD(lParam);

        colorPicker->MouseUp();
        break;
    }
    case WM_LBUTTONDOWN: {
        mouse_position.x = LOWORD(lParam);
        mouse_position.y = HIWORD(lParam);

        // ColorPicker���� ���� ���� ������Ʈ �� ���� �ٽ� �׸���
        colorPicker->handleColorSelection(pWnd, mouse_position.x, mouse_position.y);
        colorPicker->MouseDown(mouse_position);
        break;
    }

                       // ��� ��ư ��Ÿ��
    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pDis = (LPDRAWITEMSTRUCT)lParam;
        if (pDis->CtlType == ODT_BUTTON) {
            HDC hdc = pDis->hDC;
            RECT rect = pDis->rcItem;

            FillRect(hdc, &rect, (HBRUSH)(COLOR_BTNFACE + 1));

            HFONT hFont = CreateFont(
                25,            // �۲� ����
                0, 0, 0,
                FW_BOLD,       // �۲� �β� (����)
                FALSE,         // ���Ÿ�ü
                FALSE,         // ����
                FALSE,         // ��Ҽ�
                DEFAULT_CHARSET, // ���� ����
                OUT_OUTLINE_PRECIS, // ��� ���е�
                CLIP_DEFAULT_PRECIS, // Ŭ�� ���е�
                DEFAULT_QUALITY, // ǰ��
                DEFAULT_QUALITY, // �̽ļ�
                L"G���� �꽺 TTF �߰�"        // �۲� �̸�
            );

            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(0, 0, 0));
            DrawText(hdc, L"���", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont); // �۲� ��ü ����
        }
        return TRUE;
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
