#pragma once

#include <stdexcept>
#include <string>

#include "DesignEntityType.h"

namespace query_processor {
class DesignEntity {
 public:
  DesignEntity();

  DesignEntity(DesignEntityType, std::string);

  DesignEntityType &GetDesignEntityType();

  std::string GetSynonym();

  friend bool operator==(const DesignEntity &de1, const DesignEntity &de2);

 protected:
  DesignEntityType design_entity_type;
  std::string synonym;
};
}  // namespace query_processor