#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>  
#include <functional>
/// �� ���⸦ �����ϱ� ���� ��ȭ ����
class PenThickness {
public:
    PenThickness();
    ~PenThickness();

    /// �� ���� ���� ��ȭ ���ڸ� ǥ��
    void show(HINSTANCE hInst, HWND hWndParent);
    /// ���� ������ �� ���⸦ ��ȯ
    static int getPenWidth();
    /// ���⸦ ���� �����ϵ��� �޼��� �߰�
    static void setPenWidth(int width);
    // �β� ���� �� ȣ��� �ݹ� �Լ� ���
    void setThicknessChangedCallback(std::function<void(int)> callback);


private:
    /// ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /// (�����̴� ����ϱ� ���� ���̺귯���� �ʱ�ȭ
    void initializeCommonControls();

    /// �߰� ���� ����
    static int penWidth;

    static std::function<void(int)> thicknessChangedCallback; // �ݹ� �Լ� ������
};

#endif
