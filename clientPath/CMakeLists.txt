# CMakeList.txt: ������ CMake ��� tcp-cpp; �������� �������� ��� � �����������,
# ������� ����� ������ ��� ����������� �������.
#
cmake_minimum_required (VERSION 3.8)

# ��������� ������� ������������ ��� ������������ MSVC, ���� ��������������.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("client")

# �������� �������� � ����������� ���� ����� �������.
add_executable (client client.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET client PROPERTY CXX_STANDARD 20)
endif()

add_custom_target(run_client
    COMMAND $<TARGET_FILE:client> -s 127.0.0.1 -p 12345
    DEPENDS client
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running client"
)

add_dependencies(run_client client)

# TODO: �������� ����� � ������� �������, ���� ��� ����������.