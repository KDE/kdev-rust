cmake_minimum_required(VERSION 2.8.12)
project(kdevrust)

set(KDEVRUST_VERSION_MAJOR 0)
set(KDEVRUST_VERSION_MINOR 1)
set(KDEVRUST_VERSION_PATCH 0)

# KDevplatform dependency version
set(KDEVPLATFORM_VERSION "${KDEVRUST_VERSION_MAJOR}.${KDEVRUST_VERSION_MINOR}.${KDEVRUST_VERSION_PATCH}")

find_package(ECM 5.14.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

include(KDECompilerSettings NO_POLICY_SCOPE)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

include(ECMAddTests)
include(ECMQtDeclareLoggingCategory)

include(FeatureSummary)

include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(Qt5 REQUIRED Core Widgets Test)
find_package(KF5 REQUIRED COMPONENTS ItemModels ThreadWeaver TextEditor I18n)
find_package(KDevPlatform ${KDEVPLATFORM_VERSION} REQUIRED)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdocumentation")
endif()

add_definitions(-DTRANSLATION_DOMAIN=\"kdevrust\")

enable_testing()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(kdevrustlanguagesupport_PART_SRCS
    rustlanguagesupport.cpp
    rustparsejob.cpp
    rusthighlighting.cpp
)

ecm_qt_declare_logging_category(kdevrustlanguagesupport_PART_SRCS
    HEADER rustdebug.h
    IDENTIFIER KDEV_RUST
    CATEGORY_NAME "kdevelop.languages.rust"
)

add_subdirectory(app_templates)
add_subdirectory(duchain)
add_subdirectory(codecompletion)

kdevplatform_add_plugin(kdevrustlanguagesupport JSON kdevrustsupport.json
    SOURCES ${kdevrustlanguagesupport_PART_SRCS}
)

target_link_libraries(kdevrustlanguagesupport
    KDev::Interfaces
    KDev::Language
    KF5::ThreadWeaver
    KF5::TextEditor
    kdevrustduchain
    kdevrustcompletion
)

install(FILES kdevrustsupport.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
