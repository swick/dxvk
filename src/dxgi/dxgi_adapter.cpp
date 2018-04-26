#include <cstdlib>
#include <cstring>

#include "dxgi_adapter.h"
#include "dxgi_device.h"
#include "dxgi_enums.h"
#include "dxgi_factory.h"
#include "dxgi_format.h"
#include "dxgi_output.h"

#include "../dxvk/vulkan/dxvk_vulkan_names.h"

namespace dxvk {

  DxgiAdapter::DxgiAdapter(
          DxgiFactory*      factory,
    const Rc<DxvkAdapter>&  adapter)
  : m_factory (factory),
    m_adapter (adapter) {
    
  }
  
  
  DxgiAdapter::~DxgiAdapter() {
    
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::QueryInterface(REFIID riid, void** ppvObject) {
    *ppvObject = nullptr;
    
    if (riid == __uuidof(IUnknown)
     || riid == __uuidof(IDXGIObject)
     || riid == __uuidof(IDXGIAdapter)
     || riid == __uuidof(IDXGIAdapter1)
     || riid == __uuidof(IDXGIVkAdapter)) {
      *ppvObject = ref(this);
      return S_OK;
    }
    
    Logger::warn("DxgiAdapter::QueryInterface: Unknown interface query");
    Logger::warn(str::format(riid));
    return E_NOINTERFACE;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetParent(REFIID riid, void** ppParent) {
    return m_factory->QueryInterface(riid, ppParent);
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::CheckInterfaceSupport(
          REFGUID                   InterfaceName,
          LARGE_INTEGER*            pUMDVersion) {
    Logger::err("DXGI: CheckInterfaceSupport: No D3D10 support");
    return DXGI_ERROR_UNSUPPORTED;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::EnumOutputs(
          UINT                      Output,
          IDXGIOutput**             ppOutput) {
    InitReturnPtr(ppOutput);
    
    if (ppOutput == nullptr)
      return DXGI_ERROR_INVALID_CALL;
    
    if (Output > 0) {
      *ppOutput = nullptr;
      return DXGI_ERROR_NOT_FOUND;
    }
    
#ifdef BUILD_LINUX_ELF
    // FIXME
#else
    // TODO support multiple monitors
    HMONITOR monitor = ::MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
    *ppOutput = ref(new DxgiOutput(this, monitor));
#endif
    return S_OK;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc(DXGI_ADAPTER_DESC* pDesc) {
    if (pDesc == nullptr)
      return DXGI_ERROR_INVALID_CALL;

    DXGI_ADAPTER_DESC1 desc1;
    HRESULT hr = this->GetDesc1(&desc1);
    
    if (SUCCEEDED(hr)) {
      std::memcpy(
        pDesc->Description,
        desc1.Description,
        sizeof(pDesc->Description));
      
      pDesc->VendorId               = desc1.VendorId;
      pDesc->DeviceId               = desc1.DeviceId;
      pDesc->SubSysId               = desc1.SubSysId;
      pDesc->Revision               = desc1.Revision;
      pDesc->DedicatedVideoMemory   = desc1.DedicatedVideoMemory;
      pDesc->DedicatedSystemMemory  = desc1.DedicatedSystemMemory;
      pDesc->SharedSystemMemory     = desc1.SharedSystemMemory;
      pDesc->AdapterLuid            = desc1.AdapterLuid;
    }
    
    return hr;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::GetDesc1(DXGI_ADAPTER_DESC1* pDesc) {
    if (pDesc == nullptr)
      return DXGI_ERROR_INVALID_CALL;
    
    auto deviceProp = m_adapter->deviceProperties();
    auto memoryProp = m_adapter->memoryProperties();
    
    // Custom Vendor ID
    const std::string customVendorID = env::getEnvVar(L"DXVK_CUSTOM_VENDOR_ID");
    const std::string customDeviceID = env::getEnvVar(L"DXVK_CUSTOM_DEVICE_ID");
    
    if (!customVendorID.empty()) {
      Logger::info("Using Custom PCI Vendor ID " + customVendorID + " instead of " + str::format(std::hex, deviceProp.vendorID));
      deviceProp.vendorID = std::stoul(customVendorID, nullptr, 16);
    }
    
    if (!customDeviceID.empty()) {
      Logger::info("Using Custom PCI Device ID " + customDeviceID + " instead of " + str::format(std::hex, deviceProp.deviceID));
      deviceProp.deviceID = std::stoul(customDeviceID, nullptr, 16);  
    }
    
    std::memset(pDesc->Description, 0, sizeof(pDesc->Description));
    std::mbstowcs(pDesc->Description, deviceProp.deviceName, std::size(pDesc->Description) - 1);
    
    VkDeviceSize deviceMemory = 0;
    VkDeviceSize sharedMemory = 0;
    
    for (uint32_t i = 0; i < memoryProp.memoryHeapCount; i++) {
      VkMemoryHeap heap = memoryProp.memoryHeaps[i];
      
      if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
        deviceMemory += heap.size;
      else
        sharedMemory += heap.size;
    }
    
    pDesc->VendorId               = deviceProp.vendorID;
    pDesc->DeviceId               = deviceProp.deviceID;
    pDesc->SubSysId               = 0;
    pDesc->Revision               = 0;
    pDesc->DedicatedVideoMemory   = deviceMemory;
    pDesc->DedicatedSystemMemory  = 0;
    pDesc->SharedSystemMemory     = sharedMemory;
    pDesc->AdapterLuid            = LUID { 0, 0 };  // TODO implement
    pDesc->Flags                  = 0;
    return S_OK;
  }
  
  
  Rc<DxvkAdapter> STDMETHODCALLTYPE DxgiAdapter::GetDXVKAdapter() {
    return m_adapter;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiAdapter::CreateDevice(
          IDXGIObject*              pContainer,
    const VkPhysicalDeviceFeatures* pFeatures,
          IDXGIVkDevice**           ppDevice) {
    InitReturnPtr(ppDevice);
    
    try {
      *ppDevice = new dxvk::DxgiDevice(pContainer, this, pFeatures);
      return S_OK;
    } catch (const dxvk::DxvkError& e) {
      dxvk::Logger::err(e.message());
      return DXGI_ERROR_UNSUPPORTED;
    }
  }
  
  
  DXGI_VK_FORMAT_INFO STDMETHODCALLTYPE DxgiAdapter::LookupFormat(
          DXGI_FORMAT               Format,
          DXGI_VK_FORMAT_MODE       Mode) {
    return GetDXGIFormatInfo(Format, Mode);
  }
  
  
  HRESULT DxgiAdapter::GetOutputFromMonitor(
          HMONITOR                  Monitor,
          IDXGIOutput**             ppOutput) {
    if (ppOutput == nullptr)
      return DXGI_ERROR_INVALID_CALL;
    
    for (uint32_t i = 0; SUCCEEDED(EnumOutputs(i, ppOutput)); i++) {
      DXGI_OUTPUT_DESC outputDesc;
      (*ppOutput)->GetDesc(&outputDesc);
      
      if (outputDesc.Monitor == Monitor)
        return S_OK;
      
      (*ppOutput)->Release();
      (*ppOutput) = nullptr;
    }
    
    // No such output found
    return DXGI_ERROR_NOT_FOUND;
  }
  
  
  HRESULT DxgiAdapter::GetOutputData(
          HMONITOR                  Monitor,
          DXGI_VK_OUTPUT_DATA*      pOutputData) {
    std::lock_guard<std::mutex> lock(m_outputMutex);
    
    auto entry = m_outputData.find(Monitor);
    if (entry == m_outputData.end())
      return DXGI_ERROR_NOT_FOUND;
    
    if (pOutputData == nullptr)
      return S_FALSE;
    
    *pOutputData = entry->second;
    return S_OK;
  }
  
  
  HRESULT DxgiAdapter::SetOutputData(
          HMONITOR                  Monitor,
    const DXGI_VK_OUTPUT_DATA*      pOutputData) {
    std::lock_guard<std::mutex> lock(m_outputMutex);
    
    m_outputData.insert_or_assign(Monitor, *pOutputData);
    return S_OK;
  }
  
}
