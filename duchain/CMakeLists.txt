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
find_package(ASTRedux MODULE)

link_directories(${RUST_LIBRARY_DIR})

ecm_qt_declare_logging_category(kdevrustduchain_SRC
    HEADER rustdebug.h
    IDENTIFIER KDEV_RUST
    CATEGORY_NAME "kdevelop.languages.rust.duchain"
)

add_library(kdevrustduchain SHARED ${kdevrustduchain_SRC})

if(NOT ${ASTRedux_FOUND})
    include(ExternalProject)
    ExternalProject_Add(ASTRedux
        GIT_REPOSITORY "git://anongit.kde.org/scratch/egospodinova/ast-redux"
        GIT_TAG "master"
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/3rdparty"
        SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/ast-redux"
        INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/ast-redux/target/release"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cargo build --release
        BUILD_BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/src/ast-redux/target/release/libast_redux.so"
        INSTALL_COMMAND "")

    ExternalProject_Get_Property(ASTRedux BUILD_BYPRODUCTS)
    set(ASTRedux_LIBRARY ${BUILD_BYPRODUCTS})
    add_dependencies(kdevrustduchain ASTRedux)
endif()

generate_export_header(kdevrustduchain BASE_NAME kdevrustduchain EXPORT_MACRO_NAME KDEVRUSTDUCHAIN_EXPORT)
target_link_libraries(kdevrustduchain LINK_PRIVATE
    KDev::Interfaces
    KDev::Language

    LINK_PUBLIC
    ${ASTRedux_LIBRARY}
)

install(TARGETS kdevrustduchain DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(tests)
