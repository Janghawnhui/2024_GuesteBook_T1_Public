#include "ColorPalette.h"

void CustomizeColorDialog(HWND hWnd) {
    HWND hColorDialog = FindWindowA(NULL, "Select Color ");
    if (hColorDialog) {
        SetWindowPos(hColorDialog, HWND_TOP, 100, 100, 600, 500, SWP_SHOWWINDOW);

    }
}
// �⺻ ���� �迭 �ʱ�ȭ
COLORREF ColorPalette::colorArr[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0) };

// ������
ColorPalette::ColorPalette() : penThickness(1) { // �⺻ �� ���� ����
}

// ���� ���� ��ȭ ���� ǥ�� �Լ�
void ColorPalette::colorSelect(HWND hWnd, int penNum) {
    CHOOSECOLOR cc;
    COLORREF acrCustClr[16]; // ����� ���� ���� �迭
    ZeroMemory(&cc, sizeof(cc));

    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = RGB(0, 0, 0);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        // ������ ���õ� �� â ũ�� ����
        CustomizeColorDialog(hWnd);
    }
}

// ���� ��ȯ �Լ�
COLORREF ColorPalette::getColor(int penNum) {
    return ColorPalette::colorArr[penNum];
}

//void ColorPalette::ShowColorPicker(HWND hWnd) {
//    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_COLORPICKER), hWnd, ColorPickerProc);
//}