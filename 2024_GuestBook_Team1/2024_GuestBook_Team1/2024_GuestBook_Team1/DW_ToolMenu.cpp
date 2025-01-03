#include "DW_ToolMenu.h"

#include "DW_NameBar.h"


void DrowWindow::createWindowCB(int left, int top, int right, int bottom, HWND parent)
{
    static bool isClassRegistered = false;  // 클래스가 이미 등록되었는지 확인

    if (!isClassRegistered) {
        WNDCLASS wc5 = {};
        wc5.lpfnWndProc = WndProcCB;  // 네임바 메세지 처리하는 정적 메서드
        wc5.lpszClassName = L"CustomNameWindowClass2";
        wc5.hInstance = hInst;
        wc5.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));


        if (!RegisterClass(&wc5)) {
            MessageBox(NULL, L"side 바 등록 실패", L"Error", MB_OK);
            return;
        }

        isClassRegistered = true;  // 클래스가 등록됨을 표시
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
        reinterpret_cast<LPVOID>(this)  // this 포인터 전달
    );
    if (!WndFunc::colorWnd) {
        DWORD error = GetLastError();
        wchar_t buf[256];
        wsprintf(buf, L"사이드 바 생성 실패: 오류 코드 %d", error);
        MessageBox(NULL, buf, L"Error", MB_OK);
        return;
    }
    ShowWindow(WndFunc::colorWnd, SW_SHOW);
}

/// 툴바 정적 메서드
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
        return pThis->handleMessageTB(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


/// 펜 종류 버튼 생성     (창 크기 변경이나 리플레이시에도 위치 변경 없어 생성자로 좌표 초기화)
MakeButton basicPenButton(10, 10, 40, 40);
MakeButton pencilButton(50, 10, 80, 40);
MakeButton brushButton(90, 10, 120, 40);
MakeButton sprayButton(130, 10, 160, 40);
MakeButton rectpenButton(170, 10, 200, 40);
MakeButton waterpenButton(210, 10, 240, 40);

/// 색상 종류 버튼 생성   (WM_PAINT로 처리)
MakeButton colorButton1;
MakeButton colorButton2;
MakeButton colorButton3;

std::unique_ptr<DW_ColorBox> dwColorBox;
COLORREF selectedColor1 = RGB(255, 0, 0); // 초기 색상
COLORREF selectedColor2 = RGB(0, 255, 0);
COLORREF selectedColor3 = RGB(0, 0, 255);

/// 아래 3개의 버튼은 리플레이시 or 창 크기 변경 시 위치 이동이 있어 
/// DW_ToolMenu::Create에서 초기 좌표 생성
/// DW_ToolMenu::HandleMessage의 WM_SIZE나 WM_LBUTTONDOWN의 정지, 리플레이 버튼시 좌표 이동


MakeButton eraseButton;      /// 지우개 버튼 생성 
MakeButton playButton;      /// 플레이 버튼 생성
MakeButton stopButton;      /// 중지 버튼 생성
//MakeButton saveButton;      /// 세이브 버튼 생성

//ColorPalette colorGet;      /// 색상 받아오기용 인스턴스 

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

RECT a;         /// IntersectRect 반환용 RECT
RECT mouse;      /// 마우스 좌표 기준 RECT 생성

/// LBUTTONDOWN시 마다 selectedBrushButton과 selectedIcon이 업데이트
/// 이펙트 처리는 WM_PAINT에서 처리
/// 클릭시 selectedBrushButton 변수에 해당 RECT 데이터 저장
/// buttonLoadImage 메서드로 해당 Rect에 이펙트 생성
MakeButton* selectedBrushButton = nullptr;  /// 현재 선택된 버튼
int selectedIcon = 0;                  /// 현재 선택된 아이콘 ID

/// 클릭한 색상 추적용 변수
MakeButton* selectedColorButton = nullptr;  // 현재 선택된 색상 버튼

bool replayStay = false;   /// 리플레이 중지 시 화면 무효화 발생해도 현재 좌표 유지 위해 선언


/// 네임 바 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        //function = make_unique<Function>();
        //colorPalette = make_unique<ColorPalette>();

        int midPoint = WndFunc::wndSize.right / 2;


        /// 프로그램 실행 시 이펙트 적용할 아이콘 설정
        selectedBrushButton = &basicPenButton;   /// 기본 펜 버튼 설정
        selectedIcon = IDI_PEN_ICON;         /// 기본 펜 아이콘 설정
        selectedColorButton = &colorButton1;   /// 프로그램 실행 시 컬러 버튼1 기본값

        eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
        playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
        stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);



        break;
    }
    case WM_LBUTTONDOWN:
    {
        HDC hdc = GetDC(hWnd);

        /// 현재 마우스 좌표로 사각형 생성
        mouse.left = LOWORD(lParam);
        mouse.top = HIWORD(lParam);
        mouse.right = mouse.left + 1;
        mouse.bottom = mouse.top + 1;

        /// 기본 펜
        if (IntersectRect(&a, &mouse, &basicPenButton.rectButton)) {
            //function->setBShape(BASIC);

            selectedBrushButton = &basicPenButton;   /// 선택된 버튼 기록용 변수에 현재 객체 저장
            selectedIcon = IDI_PEN_ICON;         /// 선택된 아이콘 값 저장
        }
        /// 펜슬
        else if (IntersectRect(&a, &mouse, &pencilButton.rectButton)) {
            //function->setBShape(PENCIL);

            selectedBrushButton = &pencilButton;
            selectedIcon = IDI_PENCIL_ICON;
        }
        /// 브러시
        else if (IntersectRect(&a, &mouse, &brushButton.rectButton)) {
            //function->setBShape(BRUSH);

            selectedBrushButton = &brushButton;
            selectedIcon = IDI_BRUSH_ICON;
        }
        /// 스프레이
        else if (IntersectRect(&a, &mouse, &sprayButton.rectButton)) {
            //function->setBShape(SPRAY);

            selectedBrushButton = &sprayButton;
            selectedIcon = IDI_SPRAY_ICON;
        }
        /// 네모펜
        else if (IntersectRect(&a, &mouse, &rectpenButton.rectButton)) {
            //function->setBShape(MARKER);

            selectedBrushButton = &rectpenButton;
            selectedIcon = IDI_RECTPEN_ICON;

        }
        /// 물펜
        else if (IntersectRect(&a, &mouse, &waterpenButton.rectButton)) {
            //function->setBShape(WATERCOLOR);

            selectedBrushButton = &waterpenButton;
            selectedIcon = IDI_WATERPEN_ICON;
        }

        if (IntersectRect(&a, &mouse, &colorButton1.rectButton)) {
            createWindowCB(50, 50, 300, 400,WndFunc::drowWnd);  // DW_ColorBox를 열어서 색상 선택
            selectedColor1 = dwColorBox->getSelectedColor();  // getSelectedColor() 사용
            selectedColorButton = &colorButton1;
        }
        /// 색상 버튼 2
        else if (IntersectRect(&a, &mouse, &colorButton2.rectButton)) {
            dwColorBox->CreatePop(hWnd, 50, 50, 300, 400);
            selectedColor2 = dwColorBox->getSelectedColor();
            selectedColorButton = &colorButton2;
        }
        /// 색상 버튼 3
        else if (IntersectRect(&a, &mouse, &colorButton3.rectButton)) {
            dwColorBox->CreatePop(hWnd, 50, 50, 300, 400);
            selectedColor3 = dwColorBox->getSelectedColor();
            selectedColorButton = &colorButton3;
        }

        /// 지우개 버튼 
        else if (IntersectRect(&a, &mouse, &eraseButton.rectButton)) {
            /*
            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_CLEAR_BT, 0);
            }
            */
        }

        /// 리플레이 버튼
        else if (IntersectRect(&a, &mouse, &playButton.rectButton)) {
            
            //if (function->getDrawLInfoEmpty()) { break; }

            replayStay = true;   /// replay시 true로 설정하여 WM_SIZE 조절을 멈춘다
            playButton.toggleState = !playButton.toggleState;   /// 버튼 누를때마다 이미지 교체 위해 값 반점
            /*
            if (pCnt)
            {
                if (!function->getIsReplay())
                {
                    int midPoint = toolRT.right / 2;   /// 중심 좌표 계산

                    SendMessage(Function::hWnd, WM_COMMAND, TL_PLAY_BT, 0);

                    /// 리플레이 시 브러시 버튼 제외한 다른 버튼들 안보이게 설정
                    //saveButton.setCoordinate(-50, -50, -50, -50);
                    eraseButton.setCoordinate(-50, -50, -50, -50);
                    colorButton1.setCoordinate(-50, -50, -50, -50);
                    colorButton2.setCoordinate(-50, -50, -50, -50);
                    colorButton3.setCoordinate(-50, -50, -50, -50);

                    /// 재생, 중지 버튼 위치 조정
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
        /// 중지 버튼
        else if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {

            InvalidateRect(tWnd, nullptr, true);

            if (function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
            }
            else {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

                toolRT = ChildWindow::GetRT();      /// 창의 새로운 크기 가져오기
                int midPoint = toolRT.right / 2;   /// 윈도우 중심 좌표 계산

                /// 정지버튼 클릭 즉, 리플레이 중지시 지우개, 저장 버튼 원래 자리로 복귀
                eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
                playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
                stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

                //saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);
                playButton.toggleState = false;      /// 재생 중 정지해도 초기 아이콘으로 설정
            }
            replayStay = false;      /// 리플레이 상태 종료
            pCnt = true;
            *tCnt = true;

            InvalidateRect(tWnd, NULL, true);
            */
        }
        /// 중지 버튼
        else if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {
            playButton.toggleState = false;      /// 재생 중 정지해도 초기 아이콘으로 설
            /*
            InvalidateRect(tWnd, nullptr, true);

            if (function->getIsReplay()) {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 0);
            }
            else {
                SendMessage(Function::hWnd, WM_COMMAND, TL_RESET_BT, 1);

                toolRT = ChildWindow::GetRT();      /// 창의 새로운 크기 가져오기
                int midPoint = toolRT.right / 2;   /// 윈도우 중심 좌표 계산

                /// 정지버튼 클릭 즉, 리플레이 중지시 지우개, 저장 버튼 원래 자리로 복귀
                eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
                playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
                stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

                //saveButton.setCoordinate(toolRT.right - 50, 10, toolRT.right - 20, 40);
                정
            }
            replayStay = false;      /// 리플레이 상태 종료
            pCnt = true;
            *tCnt = true;

            InvalidateRect(tWnd, NULL, true);
            */
            }

        /// 저장 버튼

        /*
        else if (IntersectRect(&a, &mouse, &saveButton.rectButton)) {
           MessageBox(tWnd, L"저장 구현중", L"SOS", MB_OK);
        }
        */
        InvalidateRect(hWnd, NULL, true);  // 화면 갱신
        ReleaseDC(hWnd, hdc);
        
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        int midPoint = WndFunc::wndSize.right / 2;

        /// 버튼과 이미지 그리기
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

        /// 선택된 브러시 버튼에 이펙트 적용
        if (selectedBrushButton != nullptr) {
            selectedBrushButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, hdc);
        }

        /// 컬러 버튼 그리기 (Ellipse로 색상과 구현 위해 WM_PAINT에서 그리기 + 좌표처리)
        colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
        colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
        colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

        /// 선택된 컬러 버튼에 이펙트 적용
        /// 이펙트 먼저 그린 후 색상 버튼을 그림(drawEllipseButton)
        if (selectedColorButton != nullptr) {
            selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, hdc);
        }

        colorButton1.drawEllipseButton(hdc, RGB(0,0,0));   /// 색상 버튼 1 미리보기
        colorButton2.drawEllipseButton(hdc, RGB(0, 0, 0));   /// 색상 버튼 2 미리보기
        colorButton3.drawEllipseButton(hdc, RGB(0, 0, 0));   /// 색상 버튼 3 미리보기

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




