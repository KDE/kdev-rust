set(kdevrustcompletion_SRC
    completionmodel.cpp
    completionworker.cpp
    completioncontext.cpp
)

ecm_qt_declare_logging_category(kdevrustcompletion_SRC
    HEADER rustdebug.h
    IDENTIFIER KDEV_RUST
    CATEGORY_NAME "kdevelop.languages.rust.completion"
)

add_library(kdevrustcompletion SHARED ${kdevrustcompletion_SRC})

generate_export_header(kdevrustcompletion BASE_NAME kdevrustcompletion EXPORT_MACRO_NAME KDEVRUSTCOMPLETION_EXPORT)
target_link_libraries(kdevrustcompletion LINK_PRIVATE
    KDev::Interfaces
    KDev::Language
)

install(TARGETS kdevrustcompletion DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

#add_subdirectory(tests)
