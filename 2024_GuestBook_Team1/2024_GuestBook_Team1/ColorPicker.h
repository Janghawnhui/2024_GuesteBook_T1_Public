#pragma once
#include <windows.h>
#include <commctrl.h>

class ColorPicker {
private:
    HWND hWnd;
    HWND hSlider;              // 슬라이더 핸들
    RECT colorRect[16];        // 색상 선택 영역
    RECT previewRect;          // 미리보기 영역
    RECT thicknessRect;        // 굵기 선택 영역

    COLORREF selectedColor;    // 선택된 색상
    int thickness;             // 선택된 굵기

    HBRUSH colorBrush[16];     // 색상 선택을 위한 브러시 배열

public:
    ColorPicker();
    ~ColorPicker();
    void showPicker(HWND parentWnd);
    static LRESULT CALLBACK ColorPickerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void drawColors(HDC hdc);
    void drawPreview(HDC hdc);
    void drawThickness(HDC hdc);
    void handleColorSelection(HWND hWnd, int x, int y);
};