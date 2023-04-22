function(add_third_party THIRD_PARTY_NAME)
    add_compile_options(-Wno-error -Wno-all -Wno-extra -Wno-pedantic -Wno-unused-but-set-variable)
    FetchContent_Declare(
        ${THIRD_PARTY_NAME}
        ${ARGN}
    )
    FetchContent_MakeAvailable(${THIRD_PARTY_NAME})
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wunused-but-set-variable)
endfunction()
