/**
@file ConnExcel.h
@brief �������� Ŭ���� ��� ���� ����
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>
#include <fstream>
#include <comdef.h>
#include <tchar.h>
#include <thread>
#include <algorithm>

#include "Resource.h"


class ConnExcel
{
public:

    void insertExcel(const std::wstring);
    std::wstring getVisitList();
    void listScroll(HWND, int, RECT);

    int getTextSize(HWND, std::wstring);
    void listScrollThread(HWND, int, RECT);

    int getTextPosX();
    void setTextPosX(int);


    static std::wstring list;

private:
    std::wstring uniVisit;
    std::string multiVisit;

    std::string wcharToChar(const std::wstring);
    std::wstring charToWchar(const std::string);

    std::thread listScrollThreadHandle;

    WCHAR text[10000];
    int textPosX;
    int textWidth = 0;


    bool isListRunning = true;

};