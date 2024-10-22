#pragma once

#include <windows.h>
#include <commdlg.h> // 색상 선택을 사용하기 위한 API

class ColorPalette {
private:
    COLORREF acrCustClr[16];   // 사용자 정의 색상 저장 공간 (16개)
    CHOOSECOLOR cc;            // 색상 선택 창을 설정하고 호출하는 구조체
    HBRUSH hBrush = NULL;      // 색상을 저장하는 브러시 (초기값 NULL)

    int penThickness;          // 펜 굵기 저장 (추가)

public:
    // 생성자 및 소멸자
    ColorPalette();

    // 색상 선택 대화 상자를 표시하고 선택한 색상을 저장하는 함수
    void colorSelect(HWND, int);

    // 저장된 색상을 반환하는 함수
    COLORREF getColor(int);

    // 펜 굵기 설정 및 가져오기
    void setPenThickness(int thickness); // 펜 굵기 설정 (추가)
    int getPenThickness();               // 펜 굵기 반환 (추가)

    // 색상 배열 (세 가지 기본 색상을 저장)
    static COLORREF colorArr[3];
};
