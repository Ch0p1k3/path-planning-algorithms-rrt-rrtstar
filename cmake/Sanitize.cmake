# Sanitizers

# --------------------------------------------------------------------

# UB Sanitizer
# https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html

set(UBSAN_COMPILE_FLAGS -fsanitize=undefined -fno-sanitize-recover=all)
set(UBSAN_LINK_FLAGS -fsanitize=undefined)

if(UBSAN)
    message(STATUS "Sanitize with UB Sanitizer (UBSAN)")
    add_compile_options(${UBSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${UBSAN_LINK_FLAGS}")
endif()

# --------------------------------------------------------------------

# Address Sanitizer
# https://clang.llvm.org/docs/AddressSanitizer.html

set(ASAN_COMPILE_FLAGS -fsanitize=address,undefined -fno-sanitize-recover=all)
set(ASAN_LINK_FLAGS -fsanitize=address,undefined)

if(ASAN)
    message(STATUS "Sanitize with Address Sanitizer (ASAN)")
    add_compile_options(${ASAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
endif()

# --------------------------------------------------------------------

# Thread Sanitizer
# https://clang.llvm.org/docs/ThreadSanitizer.html

set(TSAN_COMPILE_FLAGS -fsanitize=thread -fno-sanitize-recover=all)
set(TSAN_LINK_FLAGS -fsanitize=thread)

if(TSAN)
    message(STATUS "Sanitize with Thread Sanitizer (TSAN)")
    add_compile_options(${TSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_LINK_FLAGS}")
endif()

