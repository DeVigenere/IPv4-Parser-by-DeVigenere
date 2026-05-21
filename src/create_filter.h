#pragma once
#include "filter.h"
#include <initializer_list>
#include <memory>
#include <string>
#include "filter_rule.h"
#include <vector>

std::unique_ptr<filter> create_filter(const std::vector<filter_rule>& list);