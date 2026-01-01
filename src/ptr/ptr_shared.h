#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace gtl {

template <typename T>
class ptr_shared {
    struct shared_data {
        T* ptr = nullptr;
        uint16_t count = 1;
    };

    shared_data* _data = nullptr;

   public:
    explicit ptr_shared(T* ptr) {
        _data = new shared_data{ptr, 1};
    }

    ptr_shared(const ptr_shared& val) {
        share(val);
    }
    ptr_shared& operator=(const ptr_shared& val) {
        share(val);
        return *this;
    }

#if __cplusplus >= 201103L
    ptr_shared(ptr_shared&& rval) noexcept {
        _data = rval._data;
        rval._data = nullptr;
    }

    ptr_shared& operator=(ptr_shared&& rval) noexcept {
        if (this != &rval) {
            release();
            _data = rval._data;
            rval._data = nullptr;
        }
        return *this;
    }
#endif

    ~ptr_shared() {
        release();
    }

    uint16_t getCount() const {
        return _data ? _data->count : 0;
    }
    T* get() const {
        return _data ? _data->ptr : nullptr;
    }
    T* operator->() const {
        return get();
    }
    T& operator*() const {
        return *get();
    }
    explicit operator bool() const {
        return get();
    }

    void share(const ptr_shared& val) noexcept {
        if (_data != val._data) {
            release();
            _data = val._data;
            if (_data) _data->count++;
        }
    }

    // удалить буфер
    void reset() {
        if (_data) {
            delete _data->ptr;
            _data->ptr = nullptr;
        }
    }

    // освободить экземпляр, удалить буфер если экземпляр был последний
    void release() {
        if (_data && --(_data->count) == 0) {
            delete _data->ptr;
            delete _data;
        }
        _data = nullptr;
    }
};

}  // namespace gtl