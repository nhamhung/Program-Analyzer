set(query_processor_tests
        src/query_processor/query/TestQuery.cpp
        src/query_processor/query/clause/TestDesignEntity.cpp
        src/query_processor/query/clause/TestClause.cpp
        src/query_processor/query/clause/TestSuchThatClause.cpp
        src/query_processor/query/clause/TestPatternClause.cpp
        src/query_processor/query/clause/TestWithClause.cpp
        src/query_processor/query_parser/TestQueryParser.cpp
        src/query_processor/query_parser/TestQueryParserAnd.cpp
        src/query_processor/query_parser/TestQueryParserSelect.cpp
        src/query_processor/query_parser/TestQueryParserSuchThat.cpp
        src/query_processor/query_parser/TestQueryParserPattern.cpp
        src/query_processor/query_parser/TestQueryParserWith.cpp
        src/query_processor/query_parser/utils/TestQueryParserUtils.cpp
        src/query_processor/query_evaluator/TestQueryEvaluatorEvaluateClause.cpp
        src/query_processor/query_evaluator/TestQueryEvaluatorEvaluateQuery.cpp
        src/query_processor/query_evaluator/TestResultTable.cpp
        src/query_processor/query_evaluator/utils/TestQueryEvaluatorUtils.cpp
        src/query_processor/query_projector/TestQueryProjector.cpp
        src/query_processor/query_optimizer/TestQueryOptimizer.cpp
        )

set(source_processor_tests
        src/source_processor/TestLexer.cpp
        src/source_processor/TestParser.cpp
        src/source_processor/TestToken.cpp
        src/source_processor/TestTokenList.cpp
        src/source_processor/TestTypeUtils.cpp
        src/source_processor/TestTNode.cpp
        )

set(parser_utils_tests
        src/parser_utils/TestExpressionParser.cpp
        src/parser_utils/TestParserUtils.cpp
        )

set(pkb_tests
        src/pkb/TestPKB.cpp
        src/pkb/TestTableMultiple.cpp
        src/pkb/TestTableSingle.cpp
        src/pkb/TestProcTable.cpp
        src/pkb/TestAssignTable.cpp
        src/pkb/TestEntityTable.cpp
        src/pkb/TestFollowsTable.cpp
        src/pkb/TestFollowsTTable.cpp
        src/pkb/TestParentTable.cpp
        src/pkb/TestParentTTable.cpp
        src/pkb/TestModifiesTable.cpp
        src/pkb/TestUsesTable.cpp
        src/pkb/TestCallsTable.cpp
        src/pkb/TestNextTable.cpp
        src/pkb/TestAffectsTable.cpp
        )

set(design_extractor_utils_tests
        src/design_extractor/TestDeUtils.cpp)

set(time_complexity_tests
        src/time_complexity/TestQueryParserBigO.cpp)

set(unit_testing_general
        src/main.cpp)

add_executable(
        unit_testing
        ${unit_testing_general}
        ${query_processor_tests}
        ${source_processor_tests}
        ${parser_utils_tests}
        ${pkb_tests}
        ${design_extractor_utils_tests}
        ${time_complexity_tests})

target_link_libraries(unit_testing spa test_utils)
