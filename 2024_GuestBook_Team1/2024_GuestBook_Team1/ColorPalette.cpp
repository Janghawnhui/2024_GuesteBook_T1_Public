#include "ColorPalette.h"

void CustomizeColorDialog(HWND hWnd) {
    HWND hColorDialog = FindWindowA(NULL, "Select Color ");
    if (hColorDialog) {
        SetWindowPos(hColorDialog, HWND_TOP, 100, 100, 600, 500, SWP_SHOWWINDOW);

    }
}
// 기본 색상 배열 초기화
COLORREF ColorPalette::colorArr[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0) };

// 생성자
ColorPalette::ColorPalette() : penThickness(1) { // 기본 펜 굵기 설정
}

// 색상 선택 대화 상자 표시 함수
void ColorPalette::colorSelect(HWND hWnd, int penNum) {
    CHOOSECOLOR cc;
    COLORREF acrCustClr[16]; // 사용자 정의 색상 배열
    ZeroMemory(&cc, sizeof(cc));

    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = RGB(0, 0, 0);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        // 색상이 선택된 후 창 크기 조정
        CustomizeColorDialog(hWnd);
    }
}

// 색상 반환 함수
COLORREF ColorPalette::getColor(int penNum) {
    return ColorPalette::colorArr[penNum];
}

//void ColorPalette::ShowColorPicker(HWND hWnd) {
//    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_COLORPICKER), hWnd, ColorPickerProc);
//}