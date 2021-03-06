#pragma once

#include "dxvk_include.h"

namespace dxvk {
  
  /**
   * \brief Pipeline cache
   * 
   * Allows the Vulkan implementation to
   * re-use previously compiled pipelines.
   */
  class DxvkPipelineCache : public RcObject {
    
  public:
    
    DxvkPipelineCache(const Rc<vk::DeviceFn>& vkd);
    ~DxvkPipelineCache();
    
    /**
     * \brief Pipeline cache handle
     * \returns Pipeline cache handle
     */
    VkPipelineCache handle() const {
      return m_handle;
    }
    
  private:
    
    Rc<vk::DeviceFn> m_vkd;
    VkPipelineCache  m_handle;
    
  };
  
}