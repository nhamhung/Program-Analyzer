set(spa_srcs
        src/spa.cpp)

set(spa_headers
        src/spa.h)

set(pkb_srcs
        src/pkb/PKB.cpp
        src/pkb/templates/Table.cpp
        src/pkb/templates/TableMultiple.cpp
        src/pkb/templates/TableSingle.cpp
        src/pkb/entity_tables/AssignTable.cpp
        src/pkb/entity_tables/ProcTable.cpp
        src/pkb/entity_tables/EntityTable.cpp
        src/pkb/entity_tables/ContainerTable.cpp
        src/pkb/abstraction_tables/ModifiesTable.cpp
        src/pkb/abstraction_tables/UsesTable.cpp
        src/pkb/abstraction_tables/ParentTable.cpp
        src/pkb/abstraction_tables/ParentTTable.cpp
        src/pkb/abstraction_tables/FollowsTable.cpp
        src/pkb/abstraction_tables/FollowsTTable.cpp
        src/pkb/abstraction_tables/CallsTable.cpp
        src/pkb/abstraction_tables/NextTable.cpp
        src/pkb/abstraction_tables/AffectsTable.cpp
        src/pkb/abstraction_tables/NextBipTable.cpp
        src/pkb/abstraction_tables/AffectsBipTable.cpp
        )

set(pkb_headers
        src/pkb/PKB.h
        src/pkb/templates/Table.h
        src/pkb/templates/TableMultiple.h
        src/pkb/templates/TableSingle.h
        src/pkb/entity_tables/AssignTable.h
        src/pkb/entity_tables/ProcTable.h
        src/pkb/entity_tables/EntityTable.h
        src/pkb/entity_tables/ContainerTable.h
        src/pkb/abstraction_tables/ModifiesTable.h
        src/pkb/abstraction_tables/UsesTable.h
        src/pkb/abstraction_tables/ParentTable.h
        src/pkb/abstraction_tables/ParentTTable.h
        src/pkb/abstraction_tables/FollowsTable.h
        src/pkb/abstraction_tables/FollowsTTable.h
        src/pkb/abstraction_tables/CallsTable.h
        src/pkb/abstraction_tables/NextTable.h
        src/pkb/abstraction_tables/AffectsTable.h
        src/pkb/abstraction_tables/NextBipTable.h
        src/pkb/abstraction_tables/AffectsBipTable.h
        )

set(query_processor_srcs
        src/query_processor/QueryProcessor.cpp
        src/query_processor/commons/query/Query.cpp
        src/query_processor/commons/query/utils/QueryUtils.cpp
        src/query_processor/commons/query/entities/DesignEntity.cpp
        src/query_processor/commons/query/clause/Clause.cpp
        src/query_processor/commons/query/clause/SuchThatClause.cpp
        src/query_processor/commons/query/clause/PatternClause.cpp
        src/query_processor/commons/query/clause/WithClause.cpp
        src/query_processor/commons/query_result/QueryResult.cpp
        src/query_processor/query_parser/QueryParser.cpp
        src/query_processor/query_parser/utils/QueryRegex.cpp
        src/query_processor/query_parser/utils/QueryParserUtils.cpp
        src/query_processor/query_evaluator/QueryEvaluator.cpp
        src/query_processor/query_evaluator/ResultTable.cpp
        src/query_processor/query_evaluator/utils/QueryEvaluatorUtils.cpp
        src/query_processor/query_projector/QueryProjector.cpp
        src/query_processor/query_optimizer/QueryOptimizer.cpp
        )

set(query_processor_headers
        src/query_processor/QueryProcessor.h
        src/query_processor/commons/query/Query.h
        src/query_processor/commons/query/clause/AttributeType.h
        src/query_processor/commons/query/clause/Clause.h
        src/query_processor/commons/query/clause/ClauseParam.h
        src/query_processor/commons/query/clause/ClauseType.h
        src/query_processor/commons/query/clause/DesignAbstraction.h
        src/query_processor/commons/query/clause/SuchThatClause.h
        src/query_processor/commons/query/clause/PatternClause.h
        src/query_processor/commons/query/clause/WithClause.h
        src/query_processor/commons/query/entities/DesignEntity.h
        src/query_processor/commons/query/entities/DesignEntityType.h
        src/query_processor/commons/query/entities/SelectedEntity.h
        src/query_processor/commons/query/utils/QueryUtils.h
        src/query_processor/commons/query_result/QueryResult.h
        src/query_processor/query_parser/QueryParser.h
        src/query_processor/query_parser/utils/QueryRegex.h
        src/query_processor/query_parser/utils/QueryParserUtils.h
        src/query_processor/query_evaluator/QueryEvaluator.h
        src/query_processor/query_evaluator/ResultTable.h
        src/query_processor/query_evaluator/utils/QueryEvaluatorUtils.h
        src/query_processor/query_projector/QueryProjector.h
        src/query_processor/query_optimizer/QueryOptimizer.h
        )

set(source_processor_srcs
        src/source_processor/Lexer.cpp
        src/source_processor/Parser.cpp
        src/source_processor/ast/TNode.cpp
        src/source_processor/token/Token.cpp
        src/source_processor/token/TokenList.cpp
        src/source_processor/utils/TypeUtils.cpp
        )

set(source_processor_headers
        src/source_processor/Lexer.h
        src/source_processor/Parser.h
        src/source_processor/ast/TNode.h
        src/source_processor/ast/TNodeType.h
        src/source_processor/token/Token.h
        src/source_processor/token/TokenList.h
        src/source_processor/token/TokenType.h
        src/source_processor/utils/TypeUtils.h
        )

set(parser_utils_headers
        src/parser_utils/ParserUtils.h
        src/parser_utils/ExpressionParser.h
        )

set(parser_utils_srcs
        src/parser_utils/ParserUtils.cpp
        src/parser_utils/ExpressionParser.cpp
        )

set(design_extractor_headers
        src/design_extractor/DesignExtractor.h
        src/design_extractor/handler/ReadHandler.h
        src/design_extractor/handler/PrintHandler.h
        src/design_extractor/handler/VariableHandler.h
        src/design_extractor/handler/ProcedureHandler.h
        src/design_extractor/handler/ConstantHandler.h
        src/design_extractor/handler/FollowsHandler.h
        src/design_extractor/handler/StatementHandler.h
        src/design_extractor/handler/ParentHandler.h
        src/design_extractor/handler/AssignmentHandler.h
        src/design_extractor/handler/UsesHandler.h
        src/design_extractor/handler/ModifiesHandler.h
        src/design_extractor/handler/IfHandler.h
        src/design_extractor/handler/WhileHandler.h
        src/design_extractor/handler/EntityHandler.h
        src/design_extractor/handler/CallHandler.h
        src/design_extractor/handler/NextHandler.h
        src/design_extractor/handler/NextBipHandler.h
        src/design_extractor/handler/AffectsHandler.h
        src/design_extractor/handler/AffectsBipHandler.h
        src/design_extractor/utils/CFGHandler.h
        src/design_extractor/utils/CFGBipHandler.h
        src/design_extractor/utils/DeUtils.h
        src/design_extractor/graph_explosion/GEHandler.h
        src/design_extractor/graph_explosion/GENode.h
        )

set(design_extractor_srcs
        src/design_extractor/DesignExtractor.cpp
        src/design_extractor/handler/ReadHandler.cpp
        src/design_extractor/handler/PrintHandler.cpp
        src/design_extractor/handler/VariableHandler.cpp
        src/design_extractor/handler/ProcedureHandler.cpp
        src/design_extractor/handler/ConstantHandler.cpp
        src/design_extractor/handler/FollowsHandler.cpp
        src/design_extractor/handler/StatementHandler.cpp
        src/design_extractor/handler/ParentHandler.cpp
        src/design_extractor/handler/AssignmentHandler.cpp
        src/design_extractor/handler/UsesHandler.cpp
        src/design_extractor/handler/ModifiesHandler.cpp
        src/design_extractor/handler/IfHandler.cpp
        src/design_extractor/handler/WhileHandler.cpp
        src/design_extractor/handler/EntityHandler.cpp
        src/design_extractor/handler/CallHandler.cpp
        src/design_extractor/handler/NextHandler.cpp
        src/design_extractor/handler/NextBipHandler.cpp
        src/design_extractor/handler/AffectsHandler.cpp
        src/design_extractor/handler/AffectsBipHandler.cpp
        src/design_extractor/utils/CFGHandler.cpp
        src/design_extractor/utils/CFGBipHandler.cpp
        src/design_extractor/utils/DeUtils.cpp
        src/design_extractor/graph_explosion/GEHandler.cpp
        src/design_extractor/graph_explosion/GENode.cpp
        )

set(utils_headers
        src/utils/Extension.h
        )

set(utils_src
        src/utils/Extension.cpp
        )


add_library(spa ${spa_srcs} ${spa_headers}
                ${pkb_srcs} ${pkb_headers}
                ${query_processor_srcs} ${query_processor_headers}
                ${source_processor_srcs} ${source_processor_headers}
                ${design_extractor_headers} ${design_extractor_srcs}
                ${parser_utils_headers} ${parser_utils_srcs}
                ${utils_headers} ${utils_src})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



