#include <windows.h>
#include <iostream>

class RECTWrapper {
private:
    RECT rect;

public:
    // 기본 생성자
    RECTWrapper() {
        SetRect(&rect, 0, 0, 0, 0);
    }

    // 사용자 정의 생성자
    RECTWrapper(int left, int top, int right, int bottom) {
        SetRect(&rect, left, top, right, bottom);
    };