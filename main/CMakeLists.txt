FILE(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/main/*.cpp)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ./include
)
