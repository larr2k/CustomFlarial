#pragma once
#include "../../../Utils/Logger/Logger.hpp"
#include "../../../Utils/Utils.hpp"
#include "../D2D.hpp"
#include <wincodec.h>
#include <stack>
#include <map>
#include <vector>
#include <chrono>
#include <unordered_map>

struct WindowRect
{
    bool isMovingElement = false;
    bool hasBeenMoved = false;
    float movedX = 0;
    float movedY = 0;
    bool madeRect = false;
    float percentageX = 0;
    float percentageY = 0;
    float ratio = 0;
};

struct SliderRect
{
    bool isMovingElement = false;
    bool hasBeenMoved = false;
    float movedX = 0;
    float movedY = 0;
    float percentageX = 0;
};

struct TextBoxStruct
{
    std::string text;
    bool isActive;
    bool firstTime = true;
    bool isDeleting = false;
};

struct ColorPicker
{
    std::string oldHex;
    std::string newHex;
    bool isActive = false;
};

struct KeybindSelectorer
{
    std::string oldShi;
    std::string newShi;
    bool isActive = false;
    std::chrono::steady_clock::time_point currentOnKeyTime = std::chrono::steady_clock::now();
};


class Dimension
{
public:
    float x = 0;
    float y = 0;
    float height = 0;
    float width = 0;
};

class Notification
{
public:
    float currentPos = 0;
    float currentPosY = 0;
    std::string text;
    bool finished = false;
    bool arrived = false;
    float width = 0;
    std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
};

class ImagesClass
{
public:
    static std::map<std::string, ID2D1Bitmap*> eimages;
};

namespace FlarialGUI
{
    std::stack<Dimension> inline dimension_stack;
    std::vector<float> inline darkenAmounts(10000);
    std::vector<float> inline opacityAmounts(20);
    std::vector<float> inline toggleSpacings(20);
    std::vector<float> inline rotationAngles(40, 0);
    std::vector<D2D1_COLOR_F> inline toggleColors(20, D2D1::ColorF(D2D1::ColorF::White));
    std::vector<D2D1_COLOR_F> inline buttonColors(30, D2D1::ColorF(D2D1::ColorF::Red));


    float inline frameFactor = 1;

    inline bool isInScrollView = false;
    inline D2D1_RECT_F ScrollViewRect = D2D1::RectF();
    float inline scrollpos = 0;
    float inline scrollposmodifier = 0.f;
    float inline barscrollpos = 0;
    float inline barscrollposmodifier = 10.0f;
    inline WindowRect WindowRects[100];
    inline SliderRect SliderRects[250];
    inline TextBoxStruct TextBoxes[100];
    inline ColorPicker ColorPickers[20];
    inline KeybindSelectorer KeybindSelectors[20];
    inline std::string currentKeybind;
    std::vector<Notification> inline notifications;
    bool inline isInWindowRect = false;

    inline ID2D1Effect* blur = nullptr;
    extern std::unordered_map<std::string, ID2D1SolidColorBrush*> brushCache;


    void PushSize(float x, float y, float width, float height);
    void PopSize();
    void PopAllStack();
    bool Button(const float x, float y, const D2D_COLOR_F color, const D2D_COLOR_F textColor, const wchar_t *text, const float width = 100.0f, const float height = 160.0f);
    bool RoundedButton(const int index, const float x, float y, const D2D_COLOR_F color, const D2D_COLOR_F textColor, const wchar_t *text, const float width = 160.0f, const float height = 100.0f, const float radiusX = 10.0f, const float radiusY = 10.0f);    void RoundedRectOnlyTopCorner(const float x, float y, D2D_COLOR_F color, const float width = 160, const float height = 100, const float radiusX = 10, const float radiusY = 10);
    void RoundedRect(const float x, float y, const D2D_COLOR_F color, float width = 160.0f, const float height = 75, const float radiusX = 10.0f, const float radiusY = 10.0f);
    void LoadImageFromFile(const wchar_t *filename, ID2D1Bitmap **bitmap);
    void RoundedRectWithImageAndText(int index, const float x, float y, const float width, const float height, const D2D1_COLOR_F color, const std::string imagePath, const float imageWidth, const float imageHeight, const wchar_t *text);
    std::wstring to_wide(const std::string &multi);
    void Image(const std::string imageName, const D2D1_RECT_F rect);
    void FlarialText(const float x, float y, const wchar_t *text, D2D1_COLOR_F color, const float width, const float height, DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_CENTER);
    void SetScrollView(float x, float y, float width, float height);
    void UnsetScrollView();
    Vec2<float> GetCenterXY(float rectWidth = 160.0f, float rectHeight = 75.0f);
    void ScrollBar(float x, float y, float width, float height, float radius);
    void SetWindowRect(float x, float y, float width, float height, int currentNum);
    void UnsetWindowRect();
    Vec2<float> CalculateMovedXY(float x, float y, int num, float rectWidth = 0.0f, float rectHeight = 0.0f);
    Vec2<float> CalculateResizedXY(float x, float y, float width, float height);
    void UpdateWindowRects();

    bool RoundedRadioButton(int index, float x, float y, const D2D_COLOR_F color, const D2D_COLOR_F textColor, const wchar_t *text,
                            const float width, const float height, float radiusX, float radiusY, const std::string& radioName, const std::string& currentRadio);

    void ApplyGaussianBlur(float blurIntensity);

    void ApplyPaintEffect(float blurIntensity);
    void ApplyAestheticDeepFry();
    void ApplyHue(float Hue);
    void ApplyDeepFry(float intensity);
    void ApplyCombinedDeepFry();


    void CopyBitmap(ID2D1Bitmap1 *from, ID2D1Bitmap **to);

    bool Toggle(int index, float x, float y, const D2D1_COLOR_F enabledColor, const D2D1_COLOR_F disabledColor, D2D1_COLOR_F circleColor, bool isEnabled);

    float Slider(int index, float x, float y, const D2D1_COLOR_F color, const D2D1_COLOR_F disabledColor,
           const D2D1_COLOR_F circleColor, const float startingPoint = 50.0f, const float maxValue = 100.0f);

    void Circle(float x, float y, const D2D1_COLOR_F &color, float radius);

    std::string TextBox(int index, std::string text, int limit, float x, float y, float width, float height);

    void ColorWheel(float x, float y, float radius);

    float HueToRGB(float p, float q, float t);

    void HSLToRGB(float h, float s, float l, float &r, float &g, float &b);

    bool CursorInRect(float rectX, float rectY, float width, float height);

    std::string ColorFToHex(const D2D1_COLOR_F &color);

    void
    RoundedHollowRect(float x, float y, float borderWidth, const D2D_COLOR_F color, const float width, const float height, float radiusX,
                      float radiusY);

    template <typename T>
    void lerp(T &a, const T &b, float t);

    D2D_COLOR_F LerpColor(D2D_COLOR_F color1, D2D_COLOR_F color2, float percentage);

    void ColorPicker(int index, float x, float y, std::string &hex, float &opacity);

    void ColorPickerWindow(int index, std::string &hex, float &opacity);

    D2D1::ColorF HexToColorF(const std::string &hexString);

    void
    FlarialTextWithFont(float x, float y, const wchar_t *text, D2D1_COLOR_F color, const float width,
                        const float height,
                        const DWRITE_TEXT_ALIGNMENT alignment, const float fontSize, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_REGULAR);

    void ResetShit();

    void AddShadowRect(const D2D1_POINT_2F &obj_min, const D2D1_POINT_2F &obj_max, D2D1_COLOR_F shadow_col,
                       float shadow_thickness, const D2D1_POINT_2F &shadow_offset, float obj_rounding);

    void KeybindSelector(const int index, float x, float y, std::string &keybind);

    void Notify(std::string text);

    void BlurRect(D2D1_ROUNDED_RECT rect, float intensity);

    void ApplyGaussianBlurToTarget(ID2D1BitmapRenderTarget *target, float blurIntensity);

    void NotifyHeartbeat();

    std::string TextBoxVisual(int index, std::string& text, int limit, float x, float y, std::string real = "Text Format");

    ID2D1SolidColorBrush* getBrush(D2D1_COLOR_F color);

    extern IDWriteFactory *writeFactory;

    extern ID2D1ImageBrush* blurbrush;
    extern ID2D1Factory* factory;
    extern ID2D1ImageBrush* shadowbrush;
    extern std::unordered_map<std::string, ID2D1Image*> cachedBitmaps;

    void ShadowRect(D2D1_ROUNDED_RECT rect, D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0, 0.75f));

    void ApplySusGaussianBlur(float blurIntensity);

    void AllahBlur(float intensity);

    void InnerShadowRect(D2D1_ROUNDED_RECT rect, float howbig, D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0, 0.75f));
};