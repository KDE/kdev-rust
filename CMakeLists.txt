cmake_minimum_required(VERSION 3.0)
project(kdevrust)

set(KDEVRUST_VERSION_MAJOR 0)
set(KDEVRUST_VERSION_MINOR 1)
set(KDEVRUST_VERSION_PATCH 0)

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

set(QT_MIN_VERSION "5.5.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets Test)
set(KF5_DEP_VERSION "5.15.0")
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS ItemModels ThreadWeaver TextEditor I18n)
set(KDEVPLATFORM_DEP_VERSION "5.1.0")
find_package(KDevPlatform ${KDEVPLATFORM_DEP_VERSION} CONFIG)
set_package_properties(KDevPlatform PROPERTIES
    TYPE REQUIRED
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdocumentation")
endif()

add_definitions(-DTRANSLATION_DOMAIN=\"kdevrust\")

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050500
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_STRICT_ITERATORS
    -DQT_USE_QSTRINGBUILDER
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
