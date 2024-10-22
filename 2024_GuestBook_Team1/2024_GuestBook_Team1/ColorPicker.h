#include <windows.h>
#include <iostream>

class RECTWrapper {
private:
    RECT rect;

public:
    // �⺻ ������
    RECTWrapper() {
        SetRect(&rect, 0, 0, 0, 0);
    }

    // ����� ���� ������
    RECTWrapper(int left, int top, int right, int bottom) {
        SetRect(&rect, left, top, right, bottom);
    };