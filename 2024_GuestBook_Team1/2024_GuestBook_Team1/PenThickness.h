#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>  
#include <functional>
/// 펜 굵기를 설정하기 위한 대화 상자
class PenThickness {
public:
    PenThickness();
    ~PenThickness();

    /// 펜 굵기 설정 대화 상자를 표시
    void show(HINSTANCE hInst, HWND hWndParent);
    /// 현재 설정된 펜 굵기를 반환
    static int getPenWidth();
    /// 굵기를 직접 설정하도록 메서드 추가
    static void setPenWidth(int width);
    // 두께 변경 시 호출될 콜백 함수 등록
    void setThicknessChangedCallback(std::function<void(int)> callback);


private:
    /// 대화 상자에서 발생하는 메시지를 처리
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /// (슬라이더 사용하기 위해 라이브러리를 초기화
    void initializeCommonControls();

    /// 추가 정적 변수
    static int penWidth;

    static std::function<void(int)> thicknessChangedCallback; // 콜백 함수 포인터
};

#endif
