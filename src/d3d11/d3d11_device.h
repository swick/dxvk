#pragma once

#include "d3d11_include.h"

#include "../util/com/com_private_data.h"

namespace dxvk {
  
  class DxgiAdapter;
  class D3D11DeviceContext;
  
  class D3D11Device : public ComObject<ID3D11Device> {
    
  public:
    
    D3D11Device(
            D3D_FEATURE_LEVEL   featureLevel,
            UINT                featureFlags);
    ~D3D11Device();
    
    HRESULT QueryInterface(
            REFIID riid,
            void **ppvObject) final;
    
    HRESULT CreateBuffer(
      const D3D11_BUFFER_DESC*      pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
            ID3D11Buffer**          ppBuffer) final;
    
    HRESULT CreateTexture1D(
      const D3D11_TEXTURE1D_DESC*   pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
            ID3D11Texture1D**       ppTexture1D) final;
    
    HRESULT CreateTexture2D(
      const D3D11_TEXTURE2D_DESC*   pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
            ID3D11Texture2D**       ppTexture2D) final;
    
    HRESULT CreateTexture3D(
      const D3D11_TEXTURE3D_DESC*   pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
            ID3D11Texture3D**       ppTexture3D) final;
    
    HRESULT CreateShaderResourceView(
            ID3D11Resource*                   pResource,
      const D3D11_SHADER_RESOURCE_VIEW_DESC*  pDesc,
            ID3D11ShaderResourceView**        ppSRView) final;
    
    HRESULT CreateUnorderedAccessView(
            ID3D11Resource*                   pResource,
      const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc,
            ID3D11UnorderedAccessView**       ppUAView) final;
    
    HRESULT CreateRenderTargetView(
            ID3D11Resource*                   pResource,
      const D3D11_RENDER_TARGET_VIEW_DESC*    pDesc,
            ID3D11RenderTargetView**          ppRTView) final;
    
    HRESULT CreateDepthStencilView(
            ID3D11Resource*                   pResource,
      const D3D11_DEPTH_STENCIL_VIEW_DESC*    pDesc,
            ID3D11DepthStencilView**          ppDepthStencilView) final;
    
    HRESULT CreateInputLayout(
      const D3D11_INPUT_ELEMENT_DESC*   pInputElementDescs,
            UINT                        NumElements,
      const void*                       pShaderBytecodeWithInputSignature,
            SIZE_T                      BytecodeLength,
            ID3D11InputLayout**         ppInputLayout) final;
    
    HRESULT CreateVertexShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11VertexShader**        ppVertexShader) final;
    
    HRESULT CreateGeometryShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11GeometryShader**      ppGeometryShader) final;
    
    HRESULT CreateGeometryShaderWithStreamOutput(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
      const D3D11_SO_DECLARATION_ENTRY* pSODeclaration,
            UINT                        NumEntries,
      const UINT*                       pBufferStrides,
            UINT                        NumStrides,
            UINT                        RasterizedStream,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11GeometryShader**      ppGeometryShader) final;
    
    HRESULT CreatePixelShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11PixelShader**         ppPixelShader) final;
    
    HRESULT CreateHullShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11HullShader**          ppHullShader) final;
    
    HRESULT CreateDomainShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11DomainShader**        ppDomainShader) final;
    
    HRESULT CreateComputeShader(
      const void*                       pShaderBytecode,
            SIZE_T                      BytecodeLength,
            ID3D11ClassLinkage*         pClassLinkage,
            ID3D11ComputeShader**       ppComputeShader) final;
    
    HRESULT CreateClassLinkage(
            ID3D11ClassLinkage**        ppLinkage) final;
    
    HRESULT CreateBlendState(
      const D3D11_BLEND_DESC*           pBlendStateDesc,
            ID3D11BlendState**          ppBlendState) final;
    
    HRESULT CreateDepthStencilState(
      const D3D11_DEPTH_STENCIL_DESC*   pDepthStencilDesc,
            ID3D11DepthStencilState**   ppDepthStencilState) final;
    
    HRESULT CreateRasterizerState(
      const D3D11_RASTERIZER_DESC*      pRasterizerDesc,
            ID3D11RasterizerState**     ppRasterizerState) final;
    
    HRESULT CreateSamplerState(
      const D3D11_SAMPLER_DESC*         pSamplerDesc,
            ID3D11SamplerState**        ppSamplerState) final;
    
    HRESULT CreateQuery(
      const D3D11_QUERY_DESC*           pQueryDesc,
            ID3D11Query**               ppQuery) final;
    
    HRESULT CreatePredicate(
      const D3D11_QUERY_DESC*           pPredicateDesc,
            ID3D11Predicate**           ppPredicate) final;
    
    HRESULT CreateCounter(
      const D3D11_COUNTER_DESC*         pCounterDesc,
            ID3D11Counter**             ppCounter) final;
    
    HRESULT CreateDeferredContext(
            UINT                        ContextFlags,
            ID3D11DeviceContext**       ppDeferredContext) final;
    
    HRESULT OpenSharedResource(
            HANDLE      hResource,
            REFIID      ReturnedInterface,
            void**      ppResource) final;
    
    HRESULT CheckFormatSupport(
            DXGI_FORMAT Format,
            UINT*       pFormatSupport) final;
    
    HRESULT CheckMultisampleQualityLevels(
            DXGI_FORMAT Format,
            UINT        SampleCount,
            UINT*       pNumQualityLevels) final;
    
    void CheckCounterInfo(
            D3D11_COUNTER_INFO* pCounterInfo) final;
    
    HRESULT CheckCounter(
      const D3D11_COUNTER_DESC* pDesc,
            D3D11_COUNTER_TYPE* pType,
            UINT*               pActiveCounters,
            LPSTR               szName,
            UINT*               pNameLength,
            LPSTR               szUnits,
            UINT*               pUnitsLength,
            LPSTR               szDescription,
            UINT*               pDescriptionLength) final;
    
    HRESULT CheckFeatureSupport(
            D3D11_FEATURE Feature,
            void*         pFeatureSupportData,
            UINT          FeatureSupportDataSize) final;
    
    HRESULT GetPrivateData(
            REFGUID   guid,
            UINT*     pDataSize,
            void*     pData) final;
    
    HRESULT SetPrivateData(
            REFGUID   guid,
            UINT      DataSize,
      const void*     pData) final;
    
    HRESULT SetPrivateDataInterface(
            REFGUID   guid,
      const IUnknown* pData) final;
    
    D3D_FEATURE_LEVEL GetFeatureLevel() final;
    
    UINT GetCreationFlags() final;
    
    HRESULT GetDeviceRemovedReason() final;
    
    void GetImmediateContext(
            ID3D11DeviceContext** ppImmediateContext) final;
    
    HRESULT SetExceptionMode(UINT RaiseFlags) final;
    
    UINT GetExceptionMode() final;
    
  private:
    
    const D3D_FEATURE_LEVEL m_featureLevel;
    const UINT              m_featureFlags;
    
    ComPrivateData m_privateData;
    
  };
  
}