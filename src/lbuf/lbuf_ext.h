#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace gtl {

// LBUF External
template <typename T, typename Ti = uint16_t>
class lbuf_ext {
   public:
    lbuf_ext(T* buf = nullptr, Ti capacity = 0) {
        setBuffer(buf, capacity);
    }

    // подключить буфер
    void setBuffer(T* buf, Ti capacity) {
        buffer = buf;
        _cap = capacity;
    }

    // добавить в буфер
    void write(T val) {
        buffer[_head] = val;
        _head = (_head + 1 >= _cap) ? 0 : (_head + 1);
    }

    // запись в буфер по номеру
    void write(size_t n, T val) {
        read(n) = val;
    }

    // чтение из буфера по номеру
    T& read(size_t n) const {
        size_t i = _head + n;
        return buffer[i >= _cap ? i - _cap : i];
    }

    // чтение из буфера по номеру
    inline T& operator[](size_t n) const {
        return read(n);
    }

    // позиция первого (левого) элемента
    inline Ti getHead() const {
        return _head;
    }

    // размер буфера
    inline size_t size() const {
        return _cap;
    }

    // "очистить" буфер
    inline void clear() {
        _head = 0;
    }

    T* buffer = nullptr;

   private:
    Ti _cap = 0, _head = 0;
};

}  // namespace gtl