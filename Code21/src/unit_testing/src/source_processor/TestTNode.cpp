#include <list>
#include <string>

#include "catch.hpp"
#include "source_processor/ast/TNode.h"
#include "source_processor/ast/TNodeType.h"

using namespace source_processor;

SCENARIO("Clone works as expected") {
  GIVEN("Single node") {
    TNode* ast = new TNode(TNodeType::Call, 69, "Hola");
    TNode* cloned_ast = &TNode::Clone(*ast);

    REQUIRE(cloned_ast->GetType() == ast->GetType());
    REQUIRE(cloned_ast->GetStatementNumber() == ast->GetStatementNumber());
    REQUIRE(cloned_ast->GetValue() == ast->GetValue());
    REQUIRE(cloned_ast != ast);
  }

  GIVEN("A tree") {
    TNode* ast = new TNode(TNodeType::Call, 69, "Hola");
    ast->AddChild(new TNode(TNodeType::Assign, 20));
    ast->AddChild(&(new TNode(TNodeType::Assign, 84))->AddChild(new TNode(TNodeType::If)));
    ast->AddChild(&(new TNode(TNodeType::Assign, 29))->AddChild(new TNode(TNodeType::While)));

    TNode* cloned_ast = &TNode::Clone(*ast);

    std::list<source_processor::TNode> q1;
    q1.push_back(*ast);
    std::list<source_processor::TNode> q2;
    q2.push_back(*cloned_ast);

    while (!q1.empty()) {
      auto c1 = q1.front();
      q1.pop_front();
      auto c2 = q2.front();
      q2.pop_front();

      REQUIRE(c1.GetType() == c2.GetType());
      REQUIRE(c1.GetStatementNumber() == c2.GetStatementNumber());
      REQUIRE(c1.GetValue() == c2.GetValue());
      REQUIRE(&c1 != &c2);

      for (auto child : c1.GetChildren()) {
        q1.push_back(*child);
      }
      for (auto child : c2.GetChildren()) {
        q2.push_back(*child);
      }
    }
    REQUIRE(q2.empty());
  }
}
