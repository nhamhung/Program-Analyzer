#pragma once

#include <stdexcept>

#include "DesignEntity.h"
#include "query_processor/commons/query/utils/QueryUtils.h"

namespace query_processor {
enum class SelectedEntityType { DESIGN_ENTITY,
                                BOOLEAN,
                                ATTRIBUTE };
struct SelectedEntity {
  DesignEntity design_entity;
  SelectedEntityType entity_type;
  std::pair<DesignEntity, AttributeType> attribute;

  SelectedEntity() = default;

  SelectedEntity(DesignEntity de) {
    this->design_entity = de;
    this->entity_type = SelectedEntityType::DESIGN_ENTITY;
  }

  // This constructor should only be used when selecting BOOLEANS.
  SelectedEntity(SelectedEntityType type) {
    if (type != SelectedEntityType::BOOLEAN) {
      throw std::runtime_error("SelectedEntity can only be constructed with SelectedEntityType BOOLEAN.");
    }
    this->entity_type = type;
  }

  SelectedEntity(std::pair<DesignEntity, AttributeType> attribute) {
    if (!QueryUtils::IsValidAttributeType(attribute.first, attribute.second)) {
      throw std::runtime_error("Invalid attribute type for the given design entity");
    }
    this->attribute = attribute;
    this->entity_type = SelectedEntityType::ATTRIBUTE;
  }
};
}  // namespace query_processor