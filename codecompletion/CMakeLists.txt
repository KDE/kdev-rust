set(kdevrustcompletion_SRC
    completionmodel.cpp
    completionworker.cpp
    completioncontext.cpp
)

add_library(kdevrustcompletion STATIC ${kdevrustcompletion_SRC})

target_link_libraries(kdevrustcompletion
    KDev::Interfaces
    KDev::Language
)

#add_subdirectory(tests)
