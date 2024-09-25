cmake_minimum_required(VERSION 3.22)

add_library(catio_core
    Private/CtApplication.cpp
)

target_link_libraries(catio_core PUBLIC catio_platform) # 原来之前是少了这个链接?
