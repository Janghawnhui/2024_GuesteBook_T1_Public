#pragma once

#include <windows.h>
#include <commdlg.h> // ���� ������ ����ϱ� ���� API

class ColorPalette {
private:
    COLORREF acrCustClr[16];   // ����� ���� ���� ���� ���� (16��)
    CHOOSECOLOR cc;            // ���� ���� â�� �����ϰ� ȣ���ϴ� ����ü
    HBRUSH hBrush = NULL;      // ������ �����ϴ� �귯�� (�ʱⰪ NULL)

    int penThickness;          // �� ���� ���� (�߰�)

public:
    // ������ �� �Ҹ���
    ColorPalette();

    // ���� ���� ��ȭ ���ڸ� ǥ���ϰ� ������ ������ �����ϴ� �Լ�
    void colorSelect(HWND, int);

    // ����� ������ ��ȯ�ϴ� �Լ�
    COLORREF getColor(int);

    // �� ���� ���� �� ��������
    void setPenThickness(int thickness); // �� ���� ���� (�߰�)
    int getPenThickness();               // �� ���� ��ȯ (�߰�)

    // ���� �迭 (�� ���� �⺻ ������ ����)
    static COLORREF colorArr[3];
};
