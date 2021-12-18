add_executable(integration_testing
        src/TestQueryProcessor.cpp
        src/TestQueryEvaluatorPKB.cpp
        src/TestDesignExtractorSingleProcedure.cpp
        src/TestDesignExtractorMultipleProcedures.cpp
        src/TestDesignExtractorExtensions.cpp
        src/TestCFGHandler.cpp
        src/TestCFGBipHandler.cpp
        src/TestGEHandler.cpp
        src/main.cpp)

target_link_libraries(integration_testing test_utils spa)
