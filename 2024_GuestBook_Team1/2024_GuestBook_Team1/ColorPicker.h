#pragma once
#include <windows.h>
#include <commctrl.h>

class ColorPicker {
private:
    HWND hWnd;
    HWND hSlider;              // �����̴� �ڵ�
    RECT colorRect[16];        // ���� ���� ����
    RECT previewRect;          // �̸����� ����
    RECT thicknessRect;        // ���� ���� ����

    COLORREF selectedColor;    // ���õ� ����
    int thickness;             // ���õ� ����

    HBRUSH colorBrush[16];     // ���� ������ ���� �귯�� �迭

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