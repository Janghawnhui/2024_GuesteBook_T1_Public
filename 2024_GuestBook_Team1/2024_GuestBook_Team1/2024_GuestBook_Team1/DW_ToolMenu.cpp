#include "DW_ToolMenu.h"

#include "DW_NameBar.h"


void DrowWindow::createWindowCB(int left, int top, int right, int bottom, HWND parent)
{
    static bool isClassRegistered = false;  // Ŭ������ �̹� ��ϵǾ����� Ȯ��

    if (!isClassRegistered) {
        WNDCLASS wc5 = {};
        wc5.lpfnWndProc = WndProcCB;  // ���ӹ� �޼��� ó���ϴ� ���� �޼���
        wc5.lpszClassName = L"CustomNameWindowClass2";
        wc5.hInstance = hInst;
        wc5.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));


        if (!RegisterClass(&wc5)) {
            MessageBox(NULL, L"side �� ��� ����", L"Error", MB_OK);
            return;
        }

        isClassRegistered = true;  // Ŭ������ ��ϵ��� ǥ��
    }


    WndFunc::colorWnd = CreateWindow(
        L"CustomNameWindowClass2",
        L"Name Window",
        WS_CHILD | WS_VISIBLE,
        left, top,
        right,
        bottom,
        parent,
        nullptr,
        hInst,
        reinterpret_cast<LPVOID>(this)  // this ������ ����
    );
    if (!WndFunc::colorWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"���̵� �� ���� ����: ���� �ڵ� %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::colorWnd, SW_SHOW);
}

/// ���� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
        return pThis->handleMessageTB(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


/// �� ���� ��ư ����     (â ũ�� �����̳� ���÷��̽ÿ��� ��ġ ���� ���� �����ڷ� ��ǥ �ʱ�ȭ)
MakeButton basicPenButton(10, 10, 40, 40);
MakeButton pencilButton(50, 10, 80, 40);
MakeButton brushButton(90, 10, 120, 40);
MakeButton sprayButton(130, 10, 160, 40);
MakeButton rectpenButton(170, 10, 200, 40);
MakeButton waterpenButton(210, 10, 240, 40);

/// ���� ���� ��ư ����   (WM_PAINT�� ó��)
MakeButton colorButton1;
MakeButton colorButton2;
MakeButton colorButton3;

std::unique_ptr<DW_ColorBox> dwColorBox;
COLORREF selectedColor1 = RGB(255, 0, 0); // �ʱ� ����
COLORREF selectedColor2 = RGB(0, 255, 0);
COLORREF selectedColor3 = RGB(0, 0, 255);

/// �Ʒ� 3���� ��ư�� ���÷��̽� or â ũ�� ���� �� ��ġ �̵��� �־� 
/// DW_ToolMenu::Create���� �ʱ� ��ǥ ����
/// DW_ToolMenu::HandleMessage�� WM_SIZE�� WM_LBUTTONDOWN�� ����, ���÷��� ��ư�� ��ǥ �̵�


MakeButton eraseButton;      /// ���찳 ��ư ���� 
MakeButton playButton;      /// �÷��� ��ư ����
MakeButton stopButton;      /// ���� ��ư ����
//MakeButton saveButton;      /// ���̺� ��ư ����

//ColorPalette colorGet;      /// ���� �޾ƿ���� �ν��Ͻ� 

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

RECT a;         /// IntersectRect ��ȯ�� RECT
RECT mouse;      /// ���콺 ��ǥ ���� RECT ����

/// LBUTTONDOWN�� ���� selectedBrushButton�� selectedIcon�� ������Ʈ
/// ����Ʈ ó���� WM_PAINT���� ó��
/// Ŭ���� selectedBrushButton ������ �ش� RECT ������ ����
/// buttonLoadImage �޼���� �ش� Rect�� ����Ʈ ����
MakeButton* selectedBrushButton = nullptr;  /// ���� ���õ� ��ư
int selectedIcon = 0;                  /// ���� ���õ� ������ ID

/// Ŭ���� ���� ������ ����
MakeButton* selectedColorButton = nullptr;  // ���� ���õ� ���� ��ư

bool replayStay = false;   /// ���÷��� ���� �� ȭ�� ��ȿȭ �߻��ص� ���� ��ǥ ���� ���� ����


/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        //function = make_unique<Function>();
        //colorPalette = make_unique<ColorPalette>();

        int midPoint = WndFunc::wndSize.right / 2;


        /// ���α׷� ���� �� ����Ʈ ������ ������ ����
        selectedBrushButton = &basicPenButton;   /// �⺻ �� ��ư ����
        selectedIcon = IDI_PEN_ICON;         /// �⺻ �� ������ ����
        selectedColorButton = &colorButton1;   /// ���α׷� ���� �� �÷� ��ư1 �⺻��

        eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
        playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
        stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);



        break;
    }
    case WM_LBUTTONDOWN:
    {
        HDC hdc = GetDC(hWnd);

        /// ���� ���콺 ��ǥ�� �簢�� ����
        mouse.left = LOWORD(lParam);
        mouse.top = HIWORD(lParam);
        mouse.right = mouse.left + 1;
        mouse.bottom = mouse.top + 1;

        /// �⺻ ��
        if (IntersectRect(&a, &mouse, &basicPenButton.rectButton)) {
            //function->setBShape(BASIC);

            selectedBrushButton = &basicPenButton;   /// ���õ� ��ư ��Ͽ� ������ ���� ��ü ����
            selectedIcon = IDI_PEN_ICON;         /// ���õ� ������ �� ����
        }
        /// �潽
        else if (IntersectRect(&a, &mouse, &pencilButton.rectButton)) {
            //function->setBShape(PENCIL);

            selectedBrushButton = &pencilButton;
            selectedIcon = IDI_PENCIL_ICON;
        }
        /// �귯��
        else if (IntersectRect(&a, &mouse, &brushButton.rectButton)) {
            //function->setBShape(BRUSH);

            selectedBrushButton = &brushButton;
            selectedIcon = IDI_BRUSH_ICON;
        }
        /// ��������
        else if (IntersectRect(&a, &mouse, &sprayButton.rectButton)) {
            //function->setBShape(SPRAY);

            selectedBrushButton = &sprayButton;
            selectedIcon = IDI_SPRAY_ICON;
        }
        /// �׸���
        else if (IntersectRect(&a, &mouse, &rectpenButton.rectButton)) {
            //function->setBShape(MARKER);

            selectedBrushButton = &rectpenButton;
            selectedIcon = IDI_RECTPEN_ICON;

        }
        /// ����
        else if (IntersectRect(&a, &mouse, &waterpenButton.rectButton)) {
            //function->setBShape(WATERCOLOR);

            selectedBrushButton = &waterpenButton;
            selectedIcon = IDI_WATERPEN_ICON;
        }

        if (IntersectRect(&a, &mouse, &colorButton1.rectButton)) {
            createWindowCB(50, 50, 300, 400,WndFunc::drowWnd);  // DW_ColorBox�� ��� ���� ����
            selectedColor1 = dwColorBox->getSelectedColor();  // getSelectedColor() ���
            selectedColorButton = &colorButton1;
        }
        /// ���� ��ư 2
        else if (IntersectRect(&a, &mouse, &colorButton2.rectButton)) {
            dwColorBox->CreatePop(hWnd, 50, 50, 300, 400);
            selectedColor2 = dwColorBox->getSelectedColor();
            selectedColorButton = &colorButton2;
        }
        /// ���� ��ư 3
        else if (IntersectRect(&a, &mouse, &colorButton3.rectButton)) {
            dwColorBox->CreatePop(hWnd, 50, 50, 300, 400);
            selectedColor3 = dwColorBox->getSelectedColor();
            selectedColorButton = &colorButton3;
        }

        /// ���찳 ��ư 
        else if (IntersectRect(&a, &mouse, &eraseButton.rectButton)) {
            /*
            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_CLEAR_BT, 0);
            }
            */
        }

        /// ���÷��� ��ư
        else if (IntersectRect(&a, &mouse, &playButton.rectButton)) {
            
            //if (function->getDrawLInfoEmpty()) { break; }

            replayStay = true;   /// replay�� true�� �����Ͽ� WM_SIZE ������ �����
            playButton.toggleState = !playButton.toggleState;   /// ��ư ���������� �̹��� ��ü ���� �� ����
            /*
            if (pCnt)
            {
                if (!function->getIsReplay())
                {
                    int midPoint = toolRT.right / 2;   /// �߽� ��ǥ ���

                    SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 0);

                    /// ���÷��� �� �귯�� ��ư ������ �ٸ� ��ư�� �Ⱥ��̰� ����
                    //saveButton.setCoordinate(-50, -50, -50, -50);
                    eraseButton.setCoordinate(-50, -50, -50, -50);
                    colorButton1.setCoordinate(-50, -50, -50, -50);
                    colorButton2.setCoordinate(-50, -50, -50, -50);
                    colorButton3.setCoordinate(-50, -50, -50, -50);

                    /// ���, ���� ��ư ��ġ ����
                    playButton.setCoordinate(midPoint - 50, 10, midPoint - 20, 40);
                    stopButton.setCoordinate(midPoint + 20, 10, midPoint + 50, 40);

                    pCnt = false;
                    *tCnt = false;

                    InvalidateRect(tWnd, NULL, true);
                }
            }
            else {
                SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 1);
                pCnt = true;
            }

            InvalidateRect(tWnd, NULL, true);
            UpdateWindow(tWnd);

        }
        /// ���� ��ư
        else if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {

            InvalidateRect(tWnd, nullptr, true);

            if (function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
            }
            else {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

                toolRT = ChildWindow::GetRT();      /// â�� ���ο� ũ�� ��������
                int midPoint = toolRT.right / 2;   /// ������ �߽� ��ǥ ���

                /// ������ư Ŭ�� ��, ���÷��� ������ ���찳, ���� ��ư ���� �ڸ��� ����
                eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
                playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
                stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

                //saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);
                playButton.toggleState = false;      /// ��� �� �����ص� �ʱ� ���������� ����
            }
            replayStay = false;      /// ���÷��� ���� ����
            pCnt = true;
            *tCnt = true;

            InvalidateRect(tWnd, NULL, true);
            */
        }
        /// ���� ��ư
        else if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {
            playButton.toggleState = false;      /// ��� �� �����ص� �ʱ� ���������� ��
            /*
            InvalidateRect(tWnd, nullptr, true);

            if (function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
            }
            else {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

                toolRT = ChildWindow::GetRT();      /// â�� ���ο� ũ�� ��������
                int midPoint = toolRT.right / 2;   /// ������ �߽� ��ǥ ���

                /// ������ư Ŭ�� ��, ���÷��� ������ ���찳, ���� ��ư ���� �ڸ��� ����
                eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
                playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
                stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

                //saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);
                ��
            }
            replayStay = false;      /// ���÷��� ���� ����
            pCnt = true;
            *tCnt = true;

            InvalidateRect(tWnd, NULL, true);
            */
            }

        /// ���� ��ư

        /*
        else if (IntersectRect(&a, &mouse, &saveButton.rectButton)) {
           MessageBox(tWnd, L"���� ������", L"SOS", MB_OK);
        }
        */
        InvalidateRect(hWnd, NULL, true);  // ȭ�� ����
        ReleaseDC(hWnd, hdc);
        
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        int midPoint = WndFunc::wndSize.right / 2;

        /// ��ư�� �̹��� �׸���
        basicPenButton.drawRectButton(hdc, IDI_PEN_ICON);
        pencilButton.drawRectButton(hdc, IDI_PENCIL_ICON);
        brushButton.drawRectButton(hdc, IDI_BRUSH_ICON);
        sprayButton.drawRectButton(hdc, IDI_SPRAY_ICON);
        rectpenButton.drawRectButton(hdc, IDI_RECTPEN_ICON);
        waterpenButton.drawRectButton(hdc, IDI_WATERPEN_ICON);
        eraseButton.drawRectButton(hdc, IDI_ERASE_ICON);
        playButton.doubleImgButton(hdc, IDI_PAUSE_ICON, IDI_PLAY_ICON);
        stopButton.drawRectButton(hdc, IDI_STOP_ICON);
        //saveButton.drawRectButton(memDC, IDI_SAVE_ICON);

        /// ���õ� �귯�� ��ư�� ����Ʈ ����
        if (selectedBrushButton != nullptr) {
            selectedBrushButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, hdc);
        }

        /// �÷� ��ư �׸��� (Ellipse�� ����� ���� ���� WM_PAINT���� �׸��� + ��ǥó��)
        colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
        colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
        colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

        /// ���õ� �÷� ��ư�� ����Ʈ ����
        /// ����Ʈ ���� �׸� �� ���� ��ư�� �׸�(drawEllipseButton)
        if (selectedColorButton != nullptr) {
            selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, hdc);
        }

        colorButton1.drawEllipseButton(hdc, RGB(0,0,0));   /// ���� ��ư 1 �̸�����
        colorButton2.drawEllipseButton(hdc, RGB(0, 0, 0));   /// ���� ��ư 2 �̸�����
        colorButton3.drawEllipseButton(hdc, RGB(0, 0, 0));   /// ���� ��ư 3 �̸�����

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



