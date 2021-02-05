include_directories(judgetask)
include_directories(.)

ADD_EXECUTABLE(powerjudge
        judgecommon.h
        judgecommon.cpp
        judgecommon.h
        judgetaskcontent.cpp
        judgetask/judgetaskbase.h
        judgetask/judgetaskbase.cpp
        main.cpp
        )
target_link_libraries(powerjudge powerlog mysqlclient pthread)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_command(
            TARGET powerjudge
            POST_BUILD
            COMMAND sudo chown judge:judge $<TARGET_FILE:powerjudge>
            COMMAND sudo chmod 4775 $<TARGET_FILE:powerjudge>
            COMMAND setcap cap_sys_chroot+ep $<TARGET_FILE:powerjudge>
    )

endif()