set (CMAKE_VERBOSE_MAKEFILE on)


#--- BoostRegex ---
find_package(LibBoostRegex REQUIRED)
include_directories(${LIBBOOSTREGEX_INCLUDE_DIRS})
set (LIBS ${LIBS} ${LIBBOOSTREGEX_LIBRARIES})


#set (CMAKE_C_COMPILER ${MPI_C_COMPILER})


set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} TaskStartException.cpp TaskStartException.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ConfigurationAttribute.cpp ConfigurationAttribute.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ConfigurationObj.cpp ConfigurationObj.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ConfigurationObjWrapper.cpp ConfigurationObjWrapper.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ConfigurationReader.cpp ConfigurationReader.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} Controller.cpp Controller.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} Prog3Settings.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} SemanticParseException.cpp SemanticParseException.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ServiceProcessor.cpp ServiceProcessor.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ServiceReader.cpp ServiceReader.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} ServiceRequest.cpp ServiceRequest.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} Task.cpp Task.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} TaskDescriptionReader.cpp TaskDescriptionReader.h)
set (LIBCONTROLLER_SRCS ${LIBCONTROLLER_SRCS} TaskProcessor.cpp TaskProcessor.h)



add_library (controller ${LIBCONTROLLER_SRCS})
target_link_libraries(controller ${LIBS})


#------ Make the TestProgram ---------
set (CONTROLLERTEST_SRCS Prog3.cpp)
set (CONTROLLERTESTLIBS ${LIBS} controller)
add_executable(prog3_parseNscedule ${CONTROLLERTEST_SRCS})
target_link_libraries(prog3_parseNscedule ${CONTROLLERTESTLIBS})

#------ INSTALL RULES ----------------
install (TARGETS controller DESTINATION /Rational/lib)
install (FILES "${PROJECT_SOURCE_DIR}/src/Controller.h" DESTINATION /Rational/include)
