// RectWindow.cpp
#include "RectWindow.h"
#include "DW_ToolMenu.h"

LRESULT CALLBACK RectWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

RectWindow::RectWindow(HINSTANCE hInstance)
    : hInst(hInstance), hWnd(nullptr), isRectVisible(true) {  // hWnd �ʱ�ȭ �߰�, isRectVisible�� true�� ����
}

void RectWindow::Create(HWND hParentWnd, int x, int y, int width, int height) {
    static bool isClassRegistered = false;  // Ŭ���� ��� Ȯ�ο� ���� ����

    if (!isClassRegistered) {  // Ŭ������ ��ϵ��� ���� ��쿡�� ���
        WNDCLASS wc = { };
        wc.lpfnWndProc = RectWndProc;
        wc.hInstance = hInst;
        wc.lpszClassName = L"UniqueRectWindowClass";
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

        if (!RegisterClass(&wc)) {
            MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        isClassRegistered = true;
    }

    hWnd = CreateWindowEx(
        0,
        L"UniqueRectWindowClass",
        L"Rectangle Window",
        WS_CHILD | WS_VISIBLE,
        x, y, width, height,
        hParentWnd,
        nullptr,
        hInst,
        this
    );

    if (!hWnd) {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // �ʱ� ���� �� ȭ�� ����
    InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT RectWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            isRectVisible = !isRectVisible;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect = { 200, 20, 600, 2000 };  // �簢�� ��ǥ

        if (isRectVisible) {
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
            FillRect(hdc, &rect, brush);
            FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
            DeleteObject(brush);
        }

        EndPaint(hwnd, &ps);
        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK RectWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RectWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        pThis = static_cast<RectWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<RectWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}