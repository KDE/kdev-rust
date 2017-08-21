include(FindPackageHandleStandardArgs)

find_program(RUSTC_EXECUTABLE rustc)

find_package_handle_standard_args(Rust DEFAULT_MSG RUSTC_EXECUTABLE)

mark_as_advanced(RUSTC_EXECUTABLE)

execute_process(COMMAND ${RUSTC_EXECUTABLE} -Vv
    OUTPUT_VARIABLE RUSTC_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX MATCH "host:[ \t]([A-Za-z0-9_-]*)\n" RUSTC_TARGET_TRIPLE "${RUSTC_VERSION_OUTPUT}")
string(REGEX REPLACE "host:[ \t]([A-Za-z0-9_-]*)\n" "\\1" RUSTC_TARGET_TRIPLE "${RUSTC_TARGET_TRIPLE}")

string(REGEX MATCH "release:[ \t]([A-Za-z0-9\\._-]*)\n" RUSTC_RELEASE "${RUSTC_VERSION_OUTPUT}")
string(REGEX REPLACE "release:[ \t]([A-Za-z0-9\\._-]*)\n" "\\1" RUSTC_RELEASE "${RUSTC_RELEASE}")

mark_as_advanced(RUSTC_TARGET_TRIPLE)
mark_as_advanced(RUSTC_RELEASE)

if(NOT ${RUSTC_RELEASE} MATCHES "nightly")
    message(FATAL_ERROR "A nightly Rust compiler is required to build kdev-rust")
endif()

execute_process(COMMAND ${RUSTC_EXECUTABLE} --print sysroot
    OUTPUT_VARIABLE RUST_SYSROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(RUST_LIBRARY_DIR "${RUST_SYSROOT}/lib/rustlib/${RUSTC_TARGET_TRIPLE}/lib")

unset(RUSTC_VERSION_OUTPUT)
