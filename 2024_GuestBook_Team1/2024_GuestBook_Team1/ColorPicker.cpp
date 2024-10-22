#include "ColorPicker.h"

    // 기본 생성자
    RECTWrapper::RECTWrapper() {
        SetRect(&rect, 0, 0, 0, 0);
    }

    RECTWrapper::RECTWrapper(int left, int top, int right, int bottom) {
        SetRect(&rect, left, top, right, bottom);
    }