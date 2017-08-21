set(kdevrustduchain_SRC
    contextbuilder.cpp
    declarationbuilder.cpp
    usebuilder.cpp
    rustnode.cpp
    rustducontext.cpp
    parsesession.cpp
    astredux.h
    nodetraits.h
)

find_package(Rust MODULE REQUIRED)
find_package(ASTRedux MODULE REQUIRED)

link_directories(${RUST_LIBRARY_DIR})

add_library(kdevrustduchain SHARED ${kdevrustduchain_SRC})

generate_export_header(kdevrustduchain BASE_NAME kdevrustduchain EXPORT_MACRO_NAME KDEVRUSTDUCHAIN_EXPORT)
target_link_libraries(kdevrustduchain LINK_PRIVATE
    KDev::Interfaces
    KDev::Language

    LINK_PUBLIC
    ${ASTRedux_LIBRARY}
)

install(TARGETS kdevrustduchain DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(tests)
