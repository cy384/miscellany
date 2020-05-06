cmake_minimum_required(VERSION 3.9)

add_application(tcp-demo tcp-demo.c CONSOLE)

set_target_properties(tcp-demo PROPERTIES COMPILE_OPTIONS -ffunction-sections)

IF(CMAKE_SYSTEM_NAME MATCHES Retro68)
# for 68k
set_target_properties(tcp-demo PROPERTIES LINK_FLAGS "-Wl,-gc-sections -Wl,--mac-strip-macsbug -Wl,--mac-segments -Wl,${CMAKE_CURRENT_SOURCE_DIR}/tcp-demo.segmap")
target_link_libraries(tcp-demo -lRetroConsole -lOpenTransport -lOpenTransportApp -lOpenTptInet)
ELSE()
# for PPC
set_target_properties(tcp-demo PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
target_link_libraries(tcp-demo -lRetroConsole -lOpenTransportAppPPC -lOpenTransportLib -lOpenTptInternetLib -lThreadsLib)
ENDIF()


