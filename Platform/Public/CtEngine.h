//
// Created by JekYUlll on 2024/9/23.
//

#ifndef CTENGINE_H
#define CTENGINE_H
// common
#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <unordered_map>

#include <filesystem>
#include <fstream>
#include <sstream>

#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(a[0]))
// #define __FILENAME__        (strrchr(__FILE__, '/') + 1)

#ifdef CT_ENGINE_PLATFORM_WIN32
// Windows

#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#elif CT_ENGINE_PLATFORM_LINUX
// Linux

#define VK_USE_PLATFORM_XCB_KHR
#elif CT_ENGINE_PLATFORM_MACOS
// Mac

#define VK_USE_PLATFORM_MACOS_MVK
#else
    #error Unsuppots this Platform
#endif

#define CT_ENGINE_GRAPHIC_API_VULKAN

#endif
