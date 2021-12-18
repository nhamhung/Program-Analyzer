set (headers
        src/TestUtils.h
        src/BuildPKBUtils.h
        src/PKBStub.h)
set (src
        src/TestUtils.cpp
        src/BuildPKBUtils.cpp
        src/PKBStub.cpp)

add_library(test_utils ${headers} ${src})
target_link_libraries(test_utils spa)
target_include_directories(test_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)