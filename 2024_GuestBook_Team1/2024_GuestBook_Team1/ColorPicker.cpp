#include "ColorPicker.h"
#
using namespace Gdiplus;

ColorPicker::ColorPicker(HWND hWnd) {

    this->hWnd = hWnd;

    is_color_picker_open_ = true;
    palette_width_ = 200;
    palette_height_ = 200;
    hue_slider_width_ = 30;
    hue_slider_height_ = 200;

    s_ = 0;
    v_ = 1.0f;

    current_color_ = HSVToRGB(360.0f - h_, s_, 1.0f - v_);
    thickness = PenThickness::getPenWidth();
    // �ʱ� ���� ���� �� ���� ����
    selectedColor = RGB(0, 0, 0);
    thickness = 1;

    // �̸����� ���� �� ���� ���� ����
    previewRect = { 10, 290, 250, 340 };
    thicknessRect = { 10, 230, 250, 270 };
}


void ColorPicker::MouseUp()
{
    if (is_color_picker_open_)
    {
        if (is_palette_click_ || is_hue_slider_click_)
        {
            is_palette_click_ = false;    // �ȷ�Ʈ Ŭ�� ����
            is_hue_slider_click_ = false; // ���� �����̴� Ŭ�� ����
        }
    }
}

// ���콺�� ������ �� ó��
void ColorPicker::MouseDown(POINT mouse_position)
{
    if (is_color_picker_open_)
    {
        // �ȷ�Ʈ �������� Ŭ���� �߻��ߴ��� Ȯ��
        if (PtInRect(&palette_area_, mouse_position))
        {
            PaletteControl(mouse_position); // �ȷ�Ʈ ����
            is_palette_click_ = true; // �ȷ�Ʈ Ŭ�� ���·� ����
        }
        // ���� �����̴� �������� Ŭ���� �߻��ߴ��� Ȯ��
        else if (PtInRect(&hue_slider_area_, mouse_position))
        {
            HueSliderControl(mouse_position); // ���� �����̴� ����
            is_hue_slider_click_ = true; // ���� �����̴� Ŭ�� ���·� ����
        }
    }
}

// ���콺 �̵� �� ó��
void ColorPicker::MouseMove(POINT mouse_position)
{
    if (is_color_picker_open_)
    {
        if (is_palette_click_)
        {
            // �ȷ�Ʈ ���� ������ ����� Ŭ�� ���� ����
            if (!PtInRect(&palette_area_, mouse_position))
            {
                MouseUp();
                return;
            }

            PaletteControl(mouse_position); // �ȷ�Ʈ ����
        }

        if (is_hue_slider_click_)
        {
            // ���� �����̴� ���� ������ ����� Ŭ�� ���� ����
            if (!PtInRect(&hue_slider_area_, mouse_position))
            {
                MouseUp();
                return;
            }

            HueSliderControl(mouse_position); // ���� �����̴� ����
        }
    }
}

void ColorPicker::PaletteControl(POINT mouse_position)
{
    s_ = min(max(((mouse_position.x - palette_x_) * 1.0f) / palette_width_, 0), 1.0f);
    v_ = min(max(((mouse_position.y - palette_y_) * 1.0f) / palette_height_, 0), 1.0f);

    // ���õ� HSV ������ RGB�� ��ȯ�Ͽ� selectedColor ������Ʈ
    Color rgbColor = HSVToRGB(360.0f - h_, s_, 1.0f - v_);
    selectedColor = RGB(rgbColor.GetR(), rgbColor.GetG(), rgbColor.GetB());

    InvalidateRect(hWnd, NULL, FALSE);
}

void ColorPicker::HueSliderControl(POINT mouse_position)
{
    h_ = min(max(((mouse_position.y - hue_slider_y_) * 360.0f) / hue_slider_height_, 0), 360.0f);

    // ���õ� HSV ������ RGB�� ��ȯ�Ͽ� selectedColor ������Ʈ
    Color rgbColor = HSVToRGB(360.0f - h_, s_, 1.0f - v_);
    selectedColor = RGB(rgbColor.GetR(), rgbColor.GetG(), rgbColor.GetB());

    InvalidateRect(hWnd, NULL, FALSE);
}
void ColorPicker::Draw(HDC hdc)
{

        Graphics graphics(hdc);

        Pen black_pen(Color(255, 0, 0, 0));                 //�ȷ�Ʈ ���� �ٱ��� ������
        Pen white_pen(Color(255, 255, 255, 255));           // �ȷ�Ʈ ���� ���� ������
        Pen thumb_contour_pen(Color(255, 149, 149, 149));   //�����̴� ������
        SolidBrush white_brush(Color(255, 255, 255, 255));
        SolidBrush black_brush(Color(255, 0, 0, 0));
        SolidBrush white_alpha_brush(Color(50, 255, 255, 255));
        SolidBrush background_brush(Color(255, 238, 238, 238));

        palette_x_ = 10;
        palette_y_ = 10;
        SolidBrush palette_background(Color(255, 255, 255, 255));
        graphics.FillRectangle(&palette_background, palette_x_, palette_y_, palette_width_, palette_height_);

        LinearGradientBrush palette_horizontal(
            Point(palette_x_, palette_y_),
            Point(palette_x_ + palette_width_, palette_y_),
            Color(0, 255, 255, 255),
            HSVToRGB(360.0f - h_, 1, 1));
        graphics.FillRectangle(&palette_horizontal, palette_x_, palette_y_, palette_width_, palette_height_);

        // �ȷ�Ʈ �׶��̼� (���Ʒ�: ���� ~ ����)
        LinearGradientBrush palette_vertical(
            Point(palette_x_, palette_y_),
            Point(palette_x_, palette_y_ + palette_height_),
            Color(0, 0, 0, 0),
            Color(255, 0, 0, 0));
        graphics.FillRectangle(&palette_vertical, palette_x_, palette_y_, palette_width_, palette_height_);

        // ���õ� ������ ���� ǥ��
        int palette_circle_x = palette_x_ + (int)(s_ * palette_width_);
        int palette_circle_y = palette_y_ + (int)(v_ * palette_height_);
        graphics.DrawEllipse(&black_pen, palette_circle_x - 5, palette_circle_y - 5, 10, 10);
        graphics.DrawEllipse(&white_pen, palette_circle_x - 4, palette_circle_y - 4, 8, 8);
        hue_slider_x_ = palette_x_ + palette_width_ + 20;
        hue_slider_y_ = palette_y_;

        graphics.FillRectangle(&white_brush, hue_slider_x_, hue_slider_y_, hue_slider_width_, hue_slider_height_);

        Image hue_slider_image(L"../ImgResource/Hue.png");
        graphics.DrawImage(&hue_slider_image, hue_slider_x_, hue_slider_y_, hue_slider_width_, hue_slider_height_);

        Point left_points[] = {
           Point(hue_slider_x_, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_),
           Point(hue_slider_x_ - 5, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_ + 5),
           Point(hue_slider_x_ - 5, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_ - 5) };

        graphics.FillPolygon(&white_brush, left_points, 3);
        graphics.DrawPolygon(&thumb_contour_pen, left_points, 3);

        Point right_points[] = {
            Point(hue_slider_x_ + hue_slider_width_, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_),
            Point(hue_slider_x_ + hue_slider_width_ + 5, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_ + 5),
            Point(hue_slider_x_ + hue_slider_width_ + 5, hue_slider_y_ + (h_ / 360.0f) * hue_slider_height_ - 5) };


        graphics.FillPolygon(&white_brush, right_points, 3);
        graphics.DrawPolygon(&thumb_contour_pen, right_points, 3);

        palette_area_ = { palette_x_ - 10, palette_y_ - 10, palette_x_ + palette_width_ + 10, palette_y_ + palette_height_ + 10 };
        hue_slider_area_ = { hue_slider_x_, hue_slider_y_ - 10, hue_slider_x_ + hue_slider_width_, hue_slider_y_ + hue_slider_height_ + 10 };
}
ColorPicker::~ColorPicker() {
    
}


void ColorPicker::showPicker(HWND parentWnd) {
    // �����̴� ����
    hSlider = CreateWindowEx(0, TRACKBAR_CLASS, L"Thickness",
        WS_CHILD | WS_VISIBLE | TBS_HORZ,
        10, 230, 200, 30,
        parentWnd, NULL, GetModuleHandle(NULL), NULL);

    SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELONG(1, 20));  // ���� ���� 1-20 ����
    SendMessage(hSlider, TBM_SETPOS, TRUE, thickness);          // �ʱ� ���� �� ����
}

LRESULT CALLBACK ColorPicker::ColorPickerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    ColorPicker* picker = reinterpret_cast<ColorPicker*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message) {
    case WM_CREATE:
        picker = reinterpret_cast<ColorPicker*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)picker);

        // �����̴��� �� ���� �����ϵ��� showPicker ȣ��
        picker->showPicker(hWnd);
        break;


    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // Ŭ���� ��ǥ�� ���� �簢�� �������� Ȯ���Ͽ� ���õ� ���� ������Ʈ
        for (int i = 0; i < 16; i++) {
            if (x >= picker->colorRect[i].left && x <= picker->colorRect[i].right &&
                y >= picker->colorRect[i].top && y <= picker->colorRect[i].bottom) {
                // ���� �귯�ÿ��� ���� ���� ������ ����
                LOGBRUSH lb;
                GetObject(picker->colorBrush[i], sizeof(LOGBRUSH), &lb);
                picker->selectedColor = lb.lbColor;  // Ŭ���� ������ ����
                InvalidateRect(hWnd, &picker->previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
                break;
            }
        }

        break;
    }

    case WM_HSCROLL: {
        if ((HWND)lParam == picker->hSlider) {
            // �����̴��� ���� �� �о ���� ������Ʈ
            picker->thickness = SendMessage(picker->hSlider, TBM_GETPOS, 0, 0);
            InvalidateRect(hWnd, &picker->previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
        }
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void ColorPicker::drawColors(HDC hdc) {
    for (int i = 0; i < 16; i++) {
        FillRect(hdc, &colorRect[i], colorBrush[i]);
    }
}

void ColorPicker::drawPreview(HDC hdc) {
    // �̸����� ������ ������� �����
    FillRect(hdc, &previewRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // selectedColor�� ����Ͽ� �� ����
    HPEN pen = CreatePen(PS_SOLID, thickness, selectedColor);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    // �̸����� ���� �߾ӿ� �� �׸���
    int centerY = (previewRect.top + previewRect.bottom) / 2;
    MoveToEx(hdc, previewRect.left + 10, centerY, NULL);
    LineTo(hdc, previewRect.right - 10, centerY);

    // �� ���ҽ� ����
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}
void ColorPicker::handleColorSelection(HWND hWnd, int x, int y) {
    // Ŭ���� ��ǥ�� ���� �簢�� �������� Ȯ���Ͽ� ���õ� ���� ������Ʈ
    for (int i = 0; i < 16; i++) {
        if (x >= colorRect[i].left && x <= colorRect[i].right &&
            y >= colorRect[i].top && y <= colorRect[i].bottom) {
            LOGBRUSH lb;
            GetObject(colorBrush[i], sizeof(LOGBRUSH), &lb);
            selectedColor = lb.lbColor;  // ���õ� ���� ������Ʈ
            InvalidateRect(hWnd, &previewRect, TRUE);  // �̸����� ���� �ٽ� �׸���
            break;
        }
    }
}

// HSV ���� RGB ������ ��ȯ
Color ColorPicker::HSVToRGB(double h, double s, double v)
{
    double r = 0;
    double g = 0;
    double b = 0;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        int i;
        double f, p, q, t;

        if (h == 360)
        {
            h = 0;
        }
        else
        {
            h = h / 60;
        }

        i = (int)trunc(h);
        f = h - i;

        p = v * (1.0 - s);
        q = v * (1.0 - (s * f));
        t = v * (1.0 - (s * (1.0 - f)));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break;
        }
    }

    return Color(255, (BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
COLORREF ColorPicker::getSelectedColor() {
    return selectedColor;
}

// �����̴� �ڵ��� ��ȯ�ϴ� �޼���
HWND ColorPicker::getSliderHandle() {
    return hSlider;
}

// �̸����� ������ RECT�� ��ȯ�ϴ� �޼���
RECT ColorPicker::getPreviewRect() const {
    return previewRect;
}

// �����̴� ��ġ�� ���� ���� ���� ������Ʈ�ϴ� �޼���
void ColorPicker::updateThickness() {
    thickness = SendMessage(hSlider, TBM_GETPOS, 0, 0);
    PenThickness::setPenWidth(thickness);  // PenThickness Ŭ������ ���� �� ����
    InvalidateRect(hWnd, &previewRect, TRUE);  // �̸����� ���� �ٽ� �׸��� ��û
}
void ColorPicker::setThickness(int thickness) {
    this->thickness = thickness;
    InvalidateRect(hWnd, &previewRect, TRUE);
}