#pragma once
#include <d3d11.h>
#include <winrt/base.h>

class MotionBlurHelper
{
public:
    static bool Initialize();
    static bool CompileShader(const char* srcData, const char* entryPoint, const char* shaderModel, ID3DBlob** blobOut);
    static void Render(ID3D11RenderTargetView* rtv, std::vector<winrt::com_ptr<ID3D11ShaderResourceView>>& frames);

    static inline ID3D11PixelShader*      m_pixelShader;
    static inline ID3D11VertexShader*     m_vertexShader;
    static inline ID3D11InputLayout*      m_inputLayout;
    static inline ID3D11Buffer*           m_constantBuffer;
    static inline ID3D11Buffer*           m_vertexBuffer;
};
