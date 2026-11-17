option(PLAYRUNTIME_WARNINGS_AS_ERRORS "Treat PlayRuntime warnings as errors" OFF)
option(PLAYRUNTIME_ENABLE_SANITIZERS "Enable ASan and UBSan for PlayRuntime on Linux" OFF)

if(PLAYRUNTIME_ENABLE_SANITIZERS)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
       NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
        message(FATAL_ERROR "PlayRuntime sanitizers require Linux with GCC or Clang")
    endif()
endif()

# Apply only to our targets, never to fetched dependencies or parent projects.
function(playruntime_configure_target target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
        if(PLAYRUNTIME_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(PLAYRUNTIME_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()

    if(PLAYRUNTIME_ENABLE_SANITIZERS)
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined -fno-sanitize-recover=all)
    endif()
endfunction()
