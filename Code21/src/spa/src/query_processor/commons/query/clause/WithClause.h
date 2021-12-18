#pragma once

#include <utility>

#include "AttributeType.h"
#include "ClauseParam.h"

namespace query_processor {
class WithClause {
 protected:
  std::pair<ClauseParam, AttributeType> lhs_pair;
  std::pair<ClauseParam, AttributeType> rhs_pair;

 public:
  WithClause(){};
  WithClause(std::pair<ClauseParam, AttributeType>, std::pair<ClauseParam, AttributeType>);
  const ClauseParam& GetLHSParam() const;
  const ClauseParam& GetRHSParam() const;
  AttributeType GetLHSAttributeType();
  AttributeType GetRHSAttributeType();
  bool IsValidClause();
  friend bool operator==(const WithClause& clause1, const WithClause& clause2);
};
}  // namespace query_processor
