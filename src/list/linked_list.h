#pragma once
#include <Arduino.h>

#include "node.h"

namespace gtl {

class list_iter {
   public:
    list_iter(node* last) : _p(last) {}

    node* next() {
        node* buf = _p;
        _p = _p->_prev;
        return buf;
    }

   private:
    node* _p;
};

class linked_list {
   public:
    // получить итератор
    list_iter iter() {
        return _last;
    }

    // добавить
    void add(node& node) {
        add(&node);
    }

    // добавить
    void add(node* node) {
        node->_prev = _last;
        _last = node;
    }

    // удалить
    void remove(node& node) {
        remove(&node);
    }

    // удалить
    void remove(node* node) {
        if (_last == node) {
            _last = _last->_prev;
        } else {
            gtl::node* p = _last;
            while (p) {
                if (p->_prev == node) {
                    p->_prev = p->_prev->_prev;
                    break;
                }
                p = p->_prev;
            }
        }
    }

    // длина списка
    size_t length() {
        size_t len = 0;
        node* p = _last;
        while (p) {
            len++;
            p = p->_prev;
        }
        return len;
    }

    // очистить список
    void clear() {
        _last = nullptr;
    }

   private:
    node* _last = nullptr;
};

}  // namespace gtl