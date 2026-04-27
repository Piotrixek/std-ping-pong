cmake_minimum_required(VERSION 3.20)

project(PingPong LANGUAGES CXX ASM_MASM)




set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)




add_executable(PingPong 
    main.cpp 
    pong_logic.asm
)




if(MSVC)
    target_compile_options(PingPong PRIVATE 
        /std:c++latest   
        /EHsc            
        /W4              
        /MP              
        /permissive-     
        /utf-8           
        $<$<CONFIG:Release>:/Ot /Oi /GL> 
    )
    
    
    target_link_options(PingPong PRIVATE
        $<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(PingPong PRIVATE 
        -std=c++23 
        -Wall 
        -Wextra 
        -pedantic
        $<$<CONFIG:Release>:-O3 -flto>
    )
endif()

target_compile_definitions(PingPong PRIVATE _PING_PONG_OFFICIAL_)