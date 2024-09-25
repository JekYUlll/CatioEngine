cmake_minimum_required(VERSION 3.22)

add_executable(SandBox Main.cpp)

target_link_libraries(SandBox PRIVATE catio_core)
# target_link_libraries(SandBox PRIVATE catio_platform) # ? 之前有链接错误，加上之后正常了。作者似乎没加这一句？玄学。

