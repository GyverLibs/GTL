#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <string.h>

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
        clear();
    }

    // очистить
    void clear() {
        _head = 0;
        _len = 0;
    }

    // ============ WRITE ============

    // запись в буфер. Вернёт true при успешной записи
    bool write(const T& val) {
        if (_len == _cap) return false;

        Ti i = _head + _len;
        buffer[i >= _cap ? i - _cap : i] = val;
        ++_len;
        return true;
    }

    // запись в буфер. Вернёт количество записанных элементов
    Ti write(const T* buf, Ti len) {
        if (len > canWrite()) len = canWrite();

        size_t i = size_t(_head) + size_t(_len);
        if (i >= _cap) i -= _cap;

        Ti left = _cap - i;
        if (left > len) left = len;

        if (buf) {
            memcpy(buffer + i, buf, left * sizeof(T));
            memcpy(buffer, buf + left, (len - left) * sizeof(T));
        }

        _len += len;
        return len;
    }

    // ============ READ ============

    // чтение из буфера
    T& read() {
        Ti i = _head;
        if (_len) {
            if (++_head >= _cap) _head = 0;
            --_len;
        }
        return buffer[i];
    }

    // чтение из буфера. Вернёт количество прочитанных элементов
    Ti read(T* buf, Ti len) {
        if (len > _len) len = _len;

        Ti left = _cap - _head;
        if (left > len) left = len;

        if (buf) {
            memcpy(buf, buffer + _head, left * sizeof(T));
            memcpy(buf + left, buffer, (len - left) * sizeof(T));
        }

        _head += len;
        if (_head >= _cap) _head -= _cap;

        _len -= len;
        return len;
    }

    // убрать из буфера, вернёт количество
    Ti consume(Ti len) {
        return read(nullptr, len);
    }

    // ============ PREPEND ============

    // вписать один элемент в начало. Вернёт true при успехе
    bool prepend(const T& val) {
        if (_len == _cap) return false;

        _head = _head ? (_head - 1) : (_cap - 1);
        buffer[_head] = val;
        ++_len;

        return true;
    }

    // вписать блок в начало. Вернёт количество вписанных элементов
    Ti prepend(const T* buf, Ti len) {
        if (len > canWrite()) len = canWrite();

        for (Ti i = 0; i < len; i++) {
            _head = _head ? (_head - 1) : (_cap - 1);
            if (buf) buffer[_head] = buf[len - 1 - i];
        }

        _len += len;
        return len;
    }

    // ============ PEEK ============

    // возвращает крайнее значение без удаления из буфера
    T& peek() {
        return buffer[_head];
    }

    // получить указатель на непрерывный участок для чтения
    T* peekBuffer() {
        return _len ? buffer + _head : nullptr;
    }

    // получить длину непрерывного участка для чтения
    Ti peekLength() const {
        if (!_len) return 0;

        Ti left = _cap - _head;
        return left < _len ? left : _len;
    }

    // получить по индексу от начала. Отрицательный - с конца
    T& get(int i) {
        return (i >= int(_len) || i < -int(_len)) ? buffer[0] : (*this)[i];
    }

    // получить по индексу от начала без проверок. Отрицательный - с конца
    T& operator[](int i) {
        if (i < 0) i += _len;
        i += _head;
        if (i >= int(_cap)) i -= _cap;
        return buffer[i];
    }

    // получить первый
    T& first() {
        return (*this)[0];
    }

    // получить последний
    T& last() {
        return (*this)[_len ? _len - 1 : 0];
    }

    // ============ STATUS ============

    // буфер полон
    bool isFull() const {
        return _len == _cap;
    }

    // буфер пуст
    bool isEmpty() const {
        return _len == 0;
    }

    // количество непрочитанных элементов
    Ti length() const {
        return _len;
    }

    // свободных ячеек для записи
    Ti canWrite() const {
        return _cap - _len;
    }

    // размер буфера
    Ti size() const {
        return _cap;
    }

    // начало буфера
    T* data() {
        return buffer;
    }

    // начало буфера
    const T* data() const {
        return buffer;
    }

    T* buffer = nullptr;

    /////////////////////
    T& getLast() { return get(_len ? _len - 1 : 0); }
    size_t available() const { return _len; }

   private:
    Ti _cap = 0, _head = 0, _len = 0;
};

}  // namespace gtl