//
// Created by Jolyn Tan on 3/2/21.
//
#include "DesignEntity.h"

#include <stdexcept>

#include "query_processor/commons/query/utils/QueryUtils.h"

namespace query_processor {
DesignEntity::DesignEntity(){};

DesignEntity::DesignEntity(DesignEntityType design_entity, std::string synonym_string) {
  this->design_entity_type = design_entity;
  if (synonym_string.empty()) {
    throw std::runtime_error("Design entity synonym cannot be empty");
  }
  this->synonym = synonym_string;
}

DesignEntityType &DesignEntity::GetDesignEntityType() {
  return this->design_entity_type;
}

std::string DesignEntity::GetSynonym() {
  return this->synonym;
}

bool operator==(const DesignEntity &de1, const DesignEntity &de2) {
  return de1.design_entity_type == de2.design_entity_type && de1.synonym == de2.synonym;
}

}  // namespace query_processor