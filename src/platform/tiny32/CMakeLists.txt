file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-sm4
ahb_sm4.cpp
ahbreg_sm4.cpp
sm4.cpp
tlm_SM4.cpp
tlmAHBSocket.cpp
${HEADERS})

add_library(platform-sm3
messagecompress.cpp
sm3_control.cpp
reg_ahb.cpp
top_SM3.cpp
tlmSM3.cpp
tlmAHBSocket.cpp
${HEADERS})

add_definitions("-g")

target_include_directories(platform-sm4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(platform-sm3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tiny32-vp
        tiny32_main.cpp
        ${HEADERS})

target_link_libraries(tiny32-vp rv32 platform-sm4 platform-sm3 platform-common gdb-mc
	${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS tiny32-vp RUNTIME DESTINATION bin)
