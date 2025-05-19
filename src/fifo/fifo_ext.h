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
    inline bool isFull() const {
        return _len == _cap;
    }

    // буфер пуст
    inline bool isEmpty() const {
        return _len == 0;
    }

    // чтение из буфера
    T& read() {
        Ti i = _head;
        if (_len) {
            _head = (_head + 1 >= _cap) ? 0 : (_head + 1);
            --_len;
        }
        return buffer[i];
    }

    // возвращает крайнее значение без удаления из буфера
    inline T& peek() const {
        return buffer[_head];
    }

    // получить по индексу от начала
    T& get(Ti i) const {
        return buffer[(_head + i >= _cap) ? (_head + i - _cap) : (_head + i)];
    }

    // получить по индексу от начала
    inline T& operator[](Ti i) const {
        return get(i);
    }

    // получить последний
    T& getLast() const {
        return get(_len ? _len - 1 : 0);
    }

    // количество непрочитанных элементов
    inline size_t length() const {
        return _len;
    }

    // размер буфера
    inline size_t size() const {
        return _cap;
    }

    // очистить
    inline void clear() {
        _len = 0;
    }

    T* buffer = nullptr;

    inline size_t available() const {
        return _len;
    }

   private:
    Ti _cap = 0, _head = 0, _len = 0;
};

}  // namespace gtl