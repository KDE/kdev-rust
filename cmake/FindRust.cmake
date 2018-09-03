#=============================================================================
# 2017  Emma Gospodinova <emma.gospodinova@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

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
    message(FATAL_ERROR "A nightly Rust compiler is required to build kdev-rust.
  Please read:
    https://doc.rust-lang.org/1.5.0/book/nightly-rust.html
  Or just run:
    curl -s https://static.rust-lang.org/rustup.sh | sh -s -- --channel=nightly")
endif()

execute_process(COMMAND ${RUSTC_EXECUTABLE} --print sysroot
    OUTPUT_VARIABLE RUST_SYSROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(RUST_LIBRARY_DIR "${RUST_SYSROOT}/lib/rustlib/${RUSTC_TARGET_TRIPLE}/lib")

unset(RUSTC_VERSION_OUTPUT)
