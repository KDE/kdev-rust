include(FindPackageHandleStandardArgs)

find_library(ASTRedux_LIBRARY NAMES ast-redux)

find_package_handle_standard_args(ASTRedux DEFAULT_MSG ASTRedux_LIBRARY)

# Set package properties if FeatureSummary was included
if (COMMAND set_package_properties)
    set_package_properties(ASTRedux PROPERTIES DESCRIPTION "ast-redux - a Rust library")
endif()
