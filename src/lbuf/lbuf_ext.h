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
        get(n) = val;
    }

    // чтение из буфера по номеру. Отрицательный - с конца
    T& get(int i) {
        return (i >= int(_cap) || i < -int(_cap)) ? buffer[0] : (*this)[i];
    }

    // получить по индексу от начала без проверок. Отрицательный - с конца
    T& operator[](int i) {
        if (i < 0) i += _cap;
        i += _head;
        return buffer[(i >= int(_cap)) ? i - _cap : i];
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

    // legacy
    T& read(int i) {
        return get(i);
    }

   private:
    Ti _cap = 0, _head = 0;
};

}  // namespace gtl