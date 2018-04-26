#pragma once

#include "../util/log/log.h"
#include "../util/log/log_debug.h"

#include "../util/util_env.h"
#include "../util/util_error.h"
#include "../util/util_flags.h"
#include "../util/util_math.h"
#include "../util/util_string.h"

#include "../util/rc/util_rc.h"
#include "../util/rc/util_rc_ptr.h"

#include "../util/sync/sync_spinlock.h"

#include "./vulkan/dxvk_vulkan_extensions.h"
#include "./vulkan/dxvk_vulkan_loader.h"
#include "./vulkan/dxvk_vulkan_names.h"

#ifdef BUILD_LINUX_ELF
  #include <win/main.h>
#endif
