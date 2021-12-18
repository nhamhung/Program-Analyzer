#pragma once

#include <vector>

namespace design_extractor {

class GENode {
 private:
  int statement_number;
  std::vector<GENode*> children;

 public:
  const std::vector<GENode*>& GetChildren() const { return children; }
  int GetStatementNumber() const { return statement_number; }

  GENode& AddChild(GENode*);

  GENode(int);
  // ~GENode();  // TODO

  static void PrintDebugInfo(const GENode&);
};

}  // namespace design_extractor