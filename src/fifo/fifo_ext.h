#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace gtl {

// FIFO External
template <typename T, typename Ti = uint16_t>
class fifo_ext {
   public:
    fifo_ext(T* buf = nullptr, Ti capacity = 0) {
        setBuffer(buf, capacity);
    }

    // подключить буфер
    void setBuffer(T* buf, Ti capacity) {
        buffer = buf;
        _cap = capacity;
    }

    // запись в буфер. Вернёт true при успешной записи
    bool write(const T& val) {
        if (_len < _cap) {
            Ti i = _head + _len;
            buffer[i >= _cap ? i - _cap : i] = val;
            ++_len;
            return true;
        }
        return false;
    }

    // буфер полон
    bool isFull() const {
        return _len == _cap;
    }

    // буфер пуст
    bool isEmpty() const {
        return _len == 0;
    }

    // чтение из буфера
    T read() {
        Ti i = _head;
        if (_len) {
            _head = (_head + 1 >= _cap) ? 0 : (_head + 1);
            --_len;
        }
        return buffer[i];
    }

    // возвращает крайнее значение без удаления из буфера
    T peek() {
        return buffer[_head];
    }

    // количество непрочитанных элементов
    size_t available() {
        return _len;
    }

    // размер буфера
    size_t size() const {
        return _cap;
    }

    // очистить
    void clear() {
        _len = 0;
    }

    T* buffer = nullptr;

   private:
    Ti _cap = 0, _head = 0, _len = 0;
};

}  // namespace gtl