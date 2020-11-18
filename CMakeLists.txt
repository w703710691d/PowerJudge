project(PowerJudge)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/powerlog)
add_subdirectory(src/powerlog)

ADD_EXECUTABLE(powerjudged
        src/judged/judged.cpp
		src/powerlog/powerlog.cpp
        src/misc.cpp
        src/read_config.cpp)

ADD_EXECUTABLE(powerjudge
        src/judge/judge.cpp
        src/judge/db_updater.cpp
        src/judge/syscalls.cpp
		src/powerlog/powerlog.cpp
        src/misc.cpp
        src/read_config.cpp
        )

target_link_libraries(powerjudged powerlog bsd curl pthread)
target_link_libraries(powerjudge powerlog mysqlclient)

set(CMAKE_INSTALL_PREFIX "/usr/local")

install(
        TARGETS powerjudged powerjudge
	RUNTIME DESTINATION bin
)

install(
        CODE "execute_process(COMMAND sudo chown root:root /usr/local/bin/powerjudged)"
        CODE "execute_process(COMMAND sudo chmod 4755 /usr/local/bin/powerjudged)"
        CODE "execute_process(COMMAND sudo chown judge:judge /usr/local/bin/powerjudge)"
        CODE "execute_process(COMMAND sudo chmod 4755 /usr/local/bin/powerjudge)"
        CODE "execute_process(COMMAND sudo setcap cap_sys_chroot+ep /usr/local/bin/powerjudge)"
)

add_custom_command(
        TARGET powerjudged POST_BUILD
        COMMAND sudo chown root:root $<TARGET_FILE:powerjudged>
        COMMAND sudo chmod 4775 $<TARGET_FILE:powerjudged>
)

add_custom_command(
        TARGET powerjudge
        POST_BUILD
        COMMAND sudo chown judge:judge $<TARGET_FILE:powerjudge>
        COMMAND sudo chmod 4775 $<TARGET_FILE:powerjudge>
        COMMAND setcap cap_sys_chroot+ep $<TARGET_FILE:powerjudge>
)

