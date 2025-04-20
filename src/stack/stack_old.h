#pragma once
#include "../array/array_copy.h"
#include "../array/array_shared.h"
#include "../array/array_uniq.h"
// #include "stack_ext.h"

namespace gtl {

template <typename T>
struct bsearch_t {
    // индекс в стеке
    int idx;

    // указатель на данные, если найдены
    T* ptr;

    // данные найдены
    explicit operator bool() {
        return ptr;
    }
};

// External Static Stack
template <typename T>
class stack_ext {
   public:
    stack_ext() {}

    stack_ext(T* arr, size_t capacity, size_t length = 0) {
        setBuffer(arr, capacity, length);
    }

    // экспортировать в файл
    template <typename FS>
    bool writeToFile(FS& fs, const char* path) {
        auto f = fs.open(path, "w");
        return f ? writeTo(f) : false;
    }

    // импортировать из файла
    template <typename FS>
    bool readFromFile(FS& fs, const char* path) {
        auto f = fs.open(path, "r");
        return f ? readFrom(f) : false;
    }

    // экспортировать в Stream (напр. файл)
    template <typename TS>
    bool writeTo(TS& stream) {
        return stream.write((uint8_t*)_buf, size()) == size();
    }

    // импортировать из Stream (напр. файл)
    template <typename TS>
    bool readFrom(TS& stream) {
        clear();
        while (stream.available()) {
            size_t rlen = stream.available() / sizeof(T);
            if (!rlen ||
                !_fit(_len + rlen) ||
                stream.readBytes((char*)(_buf + _len), rlen * sizeof(T)) != rlen * sizeof(T)) {
                clear();
                return false;
            }
            _len += rlen;
        }
        return true;
    }

    // подключить буфер
    void setBuffer(T* arr, size_t capacity, size_t length = 0) {
        _buf = arr;
        _cap = capacity;
        _len = min(length, capacity);
    }

    // добавить в конец
    bool push(const T& val) {
        if (!_fit(_len + 1)) return 0;
        _buf[_len++] = val;
        return 1;
    }

#if __cplusplus >= 201703L
    // добавить в конец
    template <typename... Args>
    void pushList(const Args&... args) {
        (push(args), ...);
    }
#endif

    // добавить в конец
    bool operator+=(const T& val) {
        return push(val);
    }

    // получить с конца и удалить
    T& pop() {
        return _buf[(_len--) - 1];
    }

    // прочитать с конца не удаляя
    T& last() const {
        return _buf[_len - 1];
    }

    // прочитать с начала не удаляя
    T& first() const {
        return _buf[0];
    }

    // добавить в начало
    bool shift(const T& val) {
        if (!_fit(_len + 1)) return 0;
        memmove((void*)(_buf + 1), (const void*)(_buf), size());
        _buf[0] = val;
        _len++;
        return 1;
    }

    // получить с начала и удалить
    T unshift() {
        if (!length()) return T();
        T t = _buf[0];
        _len--;
        memmove((void*)(_buf), (const void*)(_buf + 1), size());
        return t;
    }

    // бинарный поиск в отсортированном стеке
    bsearch_t<T> searchSort(const T& val) {
        if (!length()) return bsearch_t<T>{0, nullptr};
        int mid, low = 0, high = length() - 1;
        while (low <= high) {
            mid = low + ((high - low) >> 1);
            if (_buf[mid] == val) return bsearch_t<T>{mid, &_buf[mid]};
            if (_buf[mid] < val) low = mid + 1;
            else high = mid - 1;
        }
        return bsearch_t<T>{low, nullptr};
    }

    // добавить с сортировкой. Флаг uniq - не добавлять если элемент уже есть
    bool addSort(const T& val, bool uniq = false) {
        bsearch_t<T> pos = searchSort(val);
        if (uniq && pos) return 0;
        return insert(pos.idx, val);
    }

    // добавить с сортировкой в bsearch_t из searchSort
    bool addSort(const T& val, bsearch_t<T>& pos) {
        return insert(pos.idx, val);
    }

    // удалить элемент. Отрицательный - с конца
    bool remove(int idx) {
        if (!length() || idx >= (int)_len || idx < -(int)_len) return 0;

        if (idx < 0) idx += _len;
        memcpy((void*)(_buf + idx), (const void*)(_buf + idx + 1), (_len - idx - 1) * sizeof(T));
        _len--;
        return 1;
    }

    // удалить несколько элементов, начиная с индекса
    bool remove(size_t from, size_t amount) {
        if (!length() || !amount || from >= _len) return 0;
        size_t to = from + amount;
        if (to >= _len - 1) {
            _len = from;
            return 1;
        }

        memmove((void*)(_buf + from), (const void*)(_buf + to), (_len - to) * sizeof(T));
        _len -= amount;
        return 1;
    }

    // вставить элемент на индекс (отрицательный индекс - с конца, слишком большой - будет push)
    bool insert(int idx, const T& val) {
        if (idx < 0) idx += _len;
        if (idx < 0) return 0;
        if (idx == 0) return shift(val);
        else if (idx >= (int)_len) return push(val);

        if (!_fit(_len + 1)) return 0;
        memmove((void*)(_buf + idx + 1), (const void*)(_buf + idx), (_len - idx) * sizeof(T));
        _buf[idx] = val;
        _len++;
        return 1;
    }

    // прибавить другой массив в конец
    bool concat(const stack_ext& st) {
        return concat(st._buf, st._len);
    }

    // прибавить другой массив того же типа в конец
    bool concat(const T* buf, size_t len, bool pgm = false) {
        if (!len) return 1;
        if (!buf || !_fit(_len + len)) return 0;

        if (pgm) {
            memcpy_P((void*)(_buf + _len), (const void*)(buf), len * sizeof(T));
        } else {
            memcpy((void*)(_buf + _len), (const void*)(buf), len * sizeof(T));
        }
        _len += len;
        return 1;
    }

    // прибавить бинарные данные
    size_t write(const void* buf, size_t len, bool pgm = false) {
        if (!len || !buf || !_fit(_len + len)) return 0;

        if (pgm) memcpy_P((void*)(_buf + _len), buf, len);
        else memcpy((void*)(_buf + _len), buf, len);
        _len += len;
        return len;
    }

    // прибавить другой массив в конец
    bool operator+=(const stack_ext& st) {
        return concat(st);
    }

    // заполнить значением (на capacity)
    void fill(const T& val) {
        for (size_t i = 0; i < capacity(); i++) _buf[i] = val;
        _len = _cap;
    }

    // инициализировать, вызвать конструкторы (на capacity)
    void init() {
        fill(T());
    }

    // очистить (установить длину 0)
    void clear() {
        _len = 0;
    }

    // количество элементов
    size_t length() const {
        return _buf ? _len : 0;
    }

    // текущий размер в байтах
    size_t size() const {
        return length() * sizeof(T);
    }

    // установить количество элементов (само вызовет reserve)
    bool setLength(size_t len) {
        if (!_fit(_len + len)) return 0;
        _len = len;
        return 1;
    }

    // есть место для добавления
    bool canAdd() const {
        return _buf ? (_len < _cap) : 0;
    }

    // вместимость, элементов
    size_t capacity() const {
        return _buf ? _cap : 0;
    }

    // вместимость, байт
    size_t capacityBytes() const {
        return capacity() * sizeof(T);
    }

    // позиция элемента (-1 если не найден)
    int indexOf(const T& val) const {
        if (_buf) {
            for (size_t i = 0; i < _len; i++) {
                if (_buf[i] == val) return i;
            }
        }
        return -1;
    }

    // содержит элемент
    bool has(const T& val) const {
        return indexOf(val) != -1;
    }

    // удалить по значению (true если элемента нет)
    bool removeByVal(const T& val) {
        int i = indexOf(val);
        return (i >= 0) ? remove(i) : true;
    }

    // получить элемент под индексом. Отрицательный - с конца
    T& get(int idx) const {
        if (idx < 0) idx += _len;
        if (idx < 0 || !_len) return _buf[0];
        return ((size_t)idx < _len) ? _buf[idx] : _buf[_len - 1];
    }

    // получить элемент под индексом. Отрицательный - с конца
    T& operator[](int idx) const {
        return get(idx);
    }

    // доступ к буферу
    inline T* buf() const {
        return _buf;
    }

    // буфер существует
    inline bool valid() const {
        return _buf;
    }

    // буфер существует
    explicit operator bool() const {
        return _buf;
    }

    //
    bool includes(const T& val) const __attribute__((deprecated)) {
        return has(val);
    }
    T& peek() const {
        return last();
    }
    T& unpeek() const {
        return first();
    }

   protected:
    T* _buf = nullptr;
    size_t _len = 0;
    size_t _cap = 0;
    uint16_t _oversize = 8;

    inline bool _fit(size_t size) {
        return (size <= _cap || reserve(size + _oversize)) && _buf;
    }

    // зарезервировать, элементов (установить новый размер буфера)
    virtual bool reserve(size_t size) {
        return 0;
    }

    // зарезервировать, элементов (добавить к текущему размеру буфера)
    bool addCapacity(size_t size) {
        return _fit(_cap + size);
    }

    // установить увеличение размера для уменьшения количества мелких реаллокаций. Умолч. 8
    void setOversize(uint16_t oversize) {
        _oversize = oversize;
    }
};

// stackBase
template <typename T, template <typename> class array_t>
class stackBase : public stack_ext<T> {
   public:
    // зарезервировать память в кол-ве элементов
    stackBase() {}
    stackBase(size_t size) {
        reserve(size);
    }

    stackBase(const stack_ext<T>& st) = delete;
    stackBase(stack_ext<T>&& st) = delete;
    stackBase& operator=(const stack_ext<T>& st) = delete;
    stackBase& operator=(stack_ext<T>&& st) = delete;

    stackBase(const stackBase<T, array_t>& st) {
        if (this == &st) return;
        _array = st._array;
        _len = st._len;
        _update();
    }
    stackBase& operator=(const stackBase<T, array_t>& st) {
        if (this == &st) return *this;
        _array = st._array;
        _len = st._len;
        _update();
        return *this;
    }

#if __cplusplus >= 201103L
    stackBase(stackBase<T, array_t>&& st) noexcept {
        _array = st._array;
        _len = st._len;
        _update();
    }
    stackBase& operator=(stackBase<T, array_t>&& st) noexcept {
        _array = st._array;
        _len = st._len;
        _update();
        return *this;
    }
#endif

    // зарезервировать память в кол-ве элементов
    bool reserve(size_t cap) {
        bool ok = _array.resize(cap);
        _update();
        return ok;
    }

    // удалить буфер
    void reset() {
        _array.reset();
        _update();
    }

    // есть место для добавления (всегда true)
    bool canAdd() {
        return 1;
    }

   protected:
    array_t<T> _array;
    using stack_ext<T>::setBuffer;
    using stack_ext<T>::_buf;
    using stack_ext<T>::_cap;
    using stack_ext<T>::_len;

    void _update() {
        _buf = _array.buf();
        _cap = _array.size();
    }
};

// stack_copy
template <typename T>
class stack_copy : public stackBase<T, array_copy> {
    using stackBase<T, array_copy>::_update;
    using stackBase<T, array_copy>::_array;

   protected:
    using stackBase<T, array_copy>::_len;

   public:
    stack_copy() {}
    using stackBase<T, array_copy>::stackBase;

    void move(stack_copy<T>& st) {
        if (this == &st) return;
        _array = st._array;
        _len = st._len;
        _update();
    }
};

// stack_shared
template <typename T>
class stack_shared : public stackBase<T, array_shared> {
   public:
    using stackBase<T, array_shared>::stackBase;
    stack_shared() {}
};

// stack_uniq
template <typename T>
class stack_uniq : public stackBase<T, array_uniq> {
    using stackBase<T, array_uniq>::_array;
    using stackBase<T, array_uniq>::_update;

   protected:
    using stackBase<T, array_uniq>::_len;

   public:
    stack_uniq() {}
    using stackBase<T, array_uniq>::stackBase;

    stack_uniq(const stack_uniq<T>& st) = delete;
    stack_uniq& operator=(const stack_uniq<T>& st) = delete;

    stack_uniq(stack_uniq<T>& st) {
        move(st);
    }
    stack_uniq& operator=(stack_uniq<T>& st) {
        move(st);
        return *this;
    }

    void move(stack_uniq<T>& st) noexcept {
        if (this == &st) return;
        _array = st._array;
        _len = st._len;
        st._update();
        _update();
    }
};

}  // namespace gtl