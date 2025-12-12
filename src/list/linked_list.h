#pragma once
#include <Arduino.h>

#include "list_node.h"

namespace gtl {

class list_iter {
   public:
    list_iter(list_node* last) : _p(last) {}

    list_node* next() {
        if (!_p) return nullptr;
        ++_idx;
        _buf = _p;
        _p = _p->getPrev();
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
        return list_iter(_last);
    }

    // добавить
    bool add(list_node& node) {
        return add(&node);
    }

    // добавить
    bool add(list_node* node) {
        if (!node || has(node)) return false;
        node->_prev = _last;
        _last = node;
        return true;
    }

    // список содержит
    bool has(list_node& node) {
        return has(&node);
    }

    // список содержит
    bool has(list_node* node) {
        list_node* p = _last;
        while (p) {
            if (p == node) return true;
            p = p->_prev;
        }
        return false;
    }

    // удалить
    bool remove(list_node& node) {
        return remove(&node);
    }

    // удалить
    bool remove(list_node* node) {
        if (!node) return false;
        if (_last == node) {
            _last = _last->_prev;
            return true;
        } else {
            list_node* p = _last;
            while (p) {
                if (p->_prev == node) {
                    p->_prev = p->_prev->_prev;
                    return true;
                }
                p = p->_prev;
            }
        }
        return false;
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

    // получить последний элемент в списке
    inline list_node* getLast() {
        return _last;
    }

   private:
    list_node* _last = nullptr;
};

}  // namespace gtl