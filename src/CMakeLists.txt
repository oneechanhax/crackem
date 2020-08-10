
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_custom_target(crackem)

macro(ApplyCommon in_target)
    add_dependencies(crackem ${in_target})
    set_property(TARGET ${in_target} PROPERTY OUTPUT_NAME crackem)
    set_property(TARGET ${in_target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET ${in_target} PROPERTY CXX_STANDARD 17)

    target_include_directories(${in_target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")
    target_include_directories(${in_target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")
    if (UNIX AND NOT APPLE)
        target_link_libraries(${in_target} PUBLIC rt pthread)
    endif ()

    install(TARGETS ${in_target} DESTINATION lib)
endmacro()

if(CRACKEM_BUILD_SHARED)
    add_library(crackem_shared SHARED ${sources})
    ApplyCommon(crackem_shared)
endif(CRACKEM_BUILD_SHARED)
if(CRACKEM_BUILD_STATIC)
    add_library(crackem_static STATIC ${sources})
    ApplyCommon(crackem_static)
endif(CRACKEM_BUILD_STATIC)


install(FILES ${headers} DESTINATION include/crackem)
