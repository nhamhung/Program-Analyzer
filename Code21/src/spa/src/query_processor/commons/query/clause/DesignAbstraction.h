#pragma once

enum class DesignAbstraction {
  FOLLOWS,
  FOLLOWS_T,
  PARENT,
  PARENT_T,
  MODIFIES,
  USES,
  CALLS,
  CALLS_T,
  NEXT,
  NEXT_T,
  AFFECTS,
  AFFECTS_T,
  NEXTBIP,
  NEXTBIP_T,
  AFFECTSBIP,
  AFFECTSBIP_T
};