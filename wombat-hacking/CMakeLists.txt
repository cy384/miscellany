cmake_minimum_required(VERSION 3.9)

add_application(ultra-wombat-hax-tool ultra-wombat-hax-tool.c CONSOLE)

add_compile_options(-mcpu=68020)
set_target_properties(ultra-wombat-hax-tool PROPERTIES COMPILE_FLAGS "-ffunction-sections -Wall -Wextra -Wno-unused-parameter")
set_target_properties(ultra-wombat-hax-tool PROPERTIES LINK_FLAGS "-Wl,-gc-sections -Wl,--mac-segments -Wl,${CMAKE_CURRENT_SOURCE_DIR}/demo.segmap")
target_link_libraries(ultra-wombat-hax-tool retrocrt)
