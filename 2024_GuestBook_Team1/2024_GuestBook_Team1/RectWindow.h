// RectWindow.h
#pragma once
#include <windows.h>

class RectWindow {
public:
    RectWindow(HINSTANCE hInstance);
    void Create(HWND hParentWnd, int x, int y, int width, int height);
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND hWnd;
    HINSTANCE hInst;
    bool isRectVisible;  // 사각형 표시 여부
    RECT rect;  // 사각형 좌표
    RECT toolRT;
};
