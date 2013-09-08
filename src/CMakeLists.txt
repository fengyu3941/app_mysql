ADD_EXECUTABLE(app_mysql app_test.cpp app_mysql.cpp app_mysql.h otlv4.h)

FIND_PACKAGE(ODBC REQUIRED)

IF(ODBC_FOUND)
    INCLUDE_DIRECTORIES(${ODBC_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(app_mysql ${ODBC_LIBRARY})
    MESSAGE("ODBC pakage founded")
ELSE()
    MESSAGE(FATAL_ERROR "ODBC pakage not found")
ENDIF()

