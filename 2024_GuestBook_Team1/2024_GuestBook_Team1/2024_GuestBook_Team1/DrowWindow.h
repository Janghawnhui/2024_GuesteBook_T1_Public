#pragma once

#include <Windows.h>
#include <memory>
#include "Resource.h"
#include "WndFunc.h"

class DrowWindow {
public:
    DrowWindow(int mode, HINSTANCE hInst);

    //void createWindow(int width, int height, HWND parent); // â ���� �޼���
    virtual LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���

    void createWindowNB(int left, int top, int right, int bottom, HWND parent);
    void createWindowTB(int left, int top, int right, int bottom, HWND parent);
    void createWindowCV(int left, int top, int right, int bottom, HWND parent);
    void createWindowSB(int left, int top, int right, int bottom, HWND parent);
    void createWindowCB(int left, int top, int right, int bottom, HWND parent);

    /// ���ӹ� ���� ������
    static LRESULT CALLBACK WndProcNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    /// ���ӹ� �޼��� ó�� �޼���
    virtual LRESULT handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��


    /// ���� ���� ������
    static LRESULT CALLBACK WndProcTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    /// ���� �޼��� ó�� �޼���
    virtual LRESULT handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��


    /// ĵ���� ���� ���� ������
    static LRESULT CALLBACK WndProcCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    /// ĵ���� ���� ó�� �޼���
    virtual LRESULT handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��
    
    /// ���̵� �޴� ���� ������
    static LRESULT CALLBACK WndProcSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    /// ���̵� �޴�  ó�� �޼���
    virtual LRESULT handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��

    // �÷��ڽ� ���� ������
    static LRESULT CALLBACK WndProcCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    // �÷��ڽ�  ó�� �޼���
    virtual LRESULT handleMessageCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��


private:
    HWND hwnd;
    HINSTANCE hInst;
    WNDCLASS wc;
    int mode;
    RECT drowRT;
    COLORREF bkColor;
    std::unique_ptr<WndFunc> wndFunc;
 
};