project(PowerJudge)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/powerlog/include)
add_subdirectory(src/powerlog)

ADD_EXECUTABLE(powerjudged
        src/judged/judged.cpp
        src/misc.cpp
        src/read_config.cpp)

add_subdirectory(src/judge)

target_link_libraries(powerjudged powerlog bsd curl pthread)

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
