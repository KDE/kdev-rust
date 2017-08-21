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

add_library(kdevrustduchain STATIC ${kdevrustduchain_SRC})

target_link_libraries(kdevrustduchain
    KDev::Interfaces
    KDev::Language
    KF5::ThreadWeaver
    KF5::TextEditor
    ${ASTRedux_LIBRARY}
)

add_subdirectory(tests)
