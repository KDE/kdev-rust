include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(ASTRedux MODULE)
set_package_properties(ASTRedux PROPERTIES TYPE REQUIRED)

ecm_add_test(duchaintest.cpp
    TEST_NAME duchain
    LINK_LIBRARIES
    Qt5::Test
    KDev::Language
    KDev::Tests
    kdevrustduchain
    ${ASTRedux_LIBRARY}
)
