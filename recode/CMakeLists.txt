
set (recode_sources
    recode.cc
    util.cc
    impl_libunistring.cc
    impl_libiconv.cc
)
if (WIN32)
    list (APPEND recode_sources
        impl_win32.cc
    )
endif()

add_library(recode SHARED
    ${recode_sources}
)
target_link_libraries(recode PUBLIC
    Iconv::Iconv
    "${UNISTRING_LIBRARY}"
)
target_include_directories(
    recode
    AFTER PRIVATE
    "${UNISTRING_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
if (WIN32)
    target_compile_definitions(recode PUBLIC _UNICODE UNICODE)
endif()
if (MSVC)
    target_compile_options(recode PUBLIC "/utf-8")
endif()

################################################################################

add_executable(Test01 Tests/Test01.cc)
target_link_libraries(Test01 PUBLIC recode)
target_include_directories(
    Test01
    AFTER PRIVATE
    "${CMAKE_SOURCE_DIR}/recode/include"
)
if (WIN32)
    target_compile_definitions(Test01 PUBLIC _UNICODE UNICODE)
endif()
if (MSVC)
    target_compile_options(Test01 PRIVATE "/utf-8")
endif()

add_test(NAME SimpleTest COMMAND Test01)