#pragma once
#include <Arduino.h>

#include "list_node.h"

namespace gtl {

class list_iter {
   public:
    list_iter(list_node* last) : _p(last) {}

    list_node* next() {
        _idx++;
        _buf = _p;
        _p = _p->_prev;
        return _buf;
    }

    inline list_node* get() {
        return _buf;
    }

    inline int index() {
        return _idx;
    }

   private:
    list_node* _p = nullptr;
    list_node* _buf = nullptr;
    int _idx = -1;
};

class linked_list {
   public:
    // получить итератор
    list_iter iter() {
        return _last;
    }

    // добавить
    void add(list_node& node) {
        add(&node);
    }

    // добавить
    void add(list_node* node) {
        node->_prev = _last;
        _last = node;
    }

    // список содержит
    bool has(list_node& node) {
        return has(&node);
    }

    // список содержит
    bool has(list_node* node) {
        list_iter iter(_last);
        while (iter.next()) {
            if (iter.get() == node) return true;
        }
        return false;
    }

    // удалить
    void remove(list_node& node) {
        remove(&node);
    }

    // удалить
    void remove(list_node* node) {
        if (_last == node) {
            _last = _last->_prev;
        } else {
            list_node* p = _last;
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
        list_node* p = _last;
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
    list_node* _last = nullptr;
};

}  // namespace gtl