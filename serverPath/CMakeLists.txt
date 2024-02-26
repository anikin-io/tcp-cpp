# CMakeList.txt: проект CMake для tcp-cpp; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("server")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (server server.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET server PROPERTY CXX_STANDARD 20)
endif()

add_custom_target(run_server
    COMMAND $<TARGET_FILE:server>
    DEPENDS server
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running server"
)

add_dependencies(run_server server)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.