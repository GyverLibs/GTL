#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../utils.h"

namespace gtl {

template <typename T>
class ptr_uniq {
    T* _ptr = nullptr;

   public:
    ptr_uniq(const ptr_uniq& val) = delete;
    ptr_uniq& operator=(const ptr_uniq& val) = delete;

    explicit ptr_uniq(T* ptr) : _ptr(ptr) {}

    ptr_uniq(ptr_uniq& val) {
        swap(_ptr, val._ptr);
    }
    ptr_uniq& operator=(ptr_uniq& val) {
        swap(_ptr, val._ptr);
        return *this;
    }

#if __cplusplus >= 201103L
    ptr_uniq(ptr_uniq&& rval) noexcept {
        swap(_ptr, rval._ptr);
    }
    ptr_uniq& operator=(ptr_uniq&& rval) noexcept {
        swap(_ptr, rval._ptr);
        return *this;
    }
#endif

    ~ptr_uniq() {
        delete _ptr;
    }

    T* get() const {
        return _ptr;
    }
    T* operator->() const {
        return _ptr;
    }
    T& operator*() const {
        return *_ptr;
    }
    explicit operator bool() const {
        return _ptr;
    }

    // переместить из другого экземпляра
    void move(ptr_uniq& val) noexcept {
        swap(_ptr, val._ptr);
    }

    // удалить
    void reset() {
        delete _ptr;
        _ptr = nullptr;
    }
};

}  // namespace gtl