//#pragma once
//class DW_ColorBox
//{
//};
//
//#pragma once
//
//#include "ChildWindow.h"
//#include "Function.h"
//#include "ColorPicker.h"
//#include "PenThickness.h"
//#include <vector>
//#include <memory>
//
//class DW_ColorBox : public ChildWindow
//{
//public:
//    DW_ColorBox(HINSTANCE hInstance);
//    void CreatePop(HWND hParentWnd, int x, int y, int width, int height);
//    COLORREF getSelectedColor() const;
//    std::vector<PINFO>* penMemory;
//
//protected:
//    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
//
//private:
//    HWND bWnd;
//    RECT BoxRT;
//    std::unique_ptr<ColorPicker> colorPicker;
//    HWND closeButton;
//    HWND OKButton;
//    HINSTANCE bInst;
//
//    void drawTitleBar(HDC hdc);
//};


#include<Windows.h>
#include "MakeButton.h"
#include "DrowWindow.h"
#include "Function.h"
#include "ColorPicker.h"
#include "PenThickness.h"
#include "Struct.h"
#include <vector>
#include <memory>


class DW_ColorBox {
public:
	//DW_ColorBox(HINSTANCE hInstance);
    void CreatePop(HWND hParentWnd, int x, int y, int width, int height);
	COLORREF getSelectedColor() const;
	std::vector<PINFO>* penMemory;

private:
    HWND bWnd;
    RECT BoxRT;
    std::unique_ptr<ColorPicker> colorPicker;
    HWND closeButton;
    HWND OKButton;
    HINSTANCE bInst;

    //void drawTitleBar(HDC hdc);

};