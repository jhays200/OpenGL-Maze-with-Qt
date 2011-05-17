#Subdirectory: QtLogic

#files needed for compilation
#set(Qt_hFiles)
#set(CPP_FILES)

QT4_WRAP_CPP(QT_MOC_SRC ${Qt_hFiles})

#Build static library to use to build project
add_library(GLWidgetLib STATIC ${QT_MOC_SRC} ${CPP_FILES})

#link in libraries
target_link_libraries(GLWidgetLib ${QT_LIBRARIES})
