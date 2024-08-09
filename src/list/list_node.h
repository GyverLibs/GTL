#pragma once
#include <Arduino.h>

namespace gtl {

class list_node {
   public:
    list_node* _prev = nullptr;
};

}  // namespace gtl