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
    bool isRectVisible;  // �簢�� ǥ�� ����
    RECT rect;  // �簢�� ��ǥ
    RECT toolRT;
};
