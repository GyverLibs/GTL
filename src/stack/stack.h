#pragma once
#include "../array/array.h"

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

// Base Stack
template <typename T, typename AR>
class stackT : protected AR {
   public:
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

    // добавить в конец
    bool push(const T& val) {
        if (!_fit(_len + 1)) return false;

        _buf[_len++] = val;
        return true;
    }

#if __cplusplus >= 201703L || defined(STACK_USE_FOLD) || defined(ESP32)
    // добавить в конец
    template <typename... Args>
    void pushList(const Args&... args) {
        (push(args), ...);
    }
#endif

    // добавить в конец
    inline bool operator+=(const T& val) {
        return push(val);
    }

    // получить с конца и удалить
    T& pop() {
        return _buf[length() ? --_len : 0];
    }

    // прочитать с конца не удаляя
    T& last() const {
        return _buf[length() ? _len - 1 : 0];
    }

    // прочитать с начала не удаляя
    inline T& first() const {
        return _buf[0];
    }

    // добавить в начало
    bool shift(const T& val) {
        if (!_fit(_len + 1)) return false;

        memmove((void*)(_buf + 1), (const void*)(_buf), size());
        _buf[0] = val;
        ++_len;
        return true;
    }

    // получить с начала и удалить
    T unshift() {
        if (!length()) return T();

        T t = _buf[0];
        --_len;
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
        if (uniq && pos) return false;

        return insert(pos.idx, val);
    }

    // добавить с сортировкой в bsearch_t из searchSort
    bool addSort(const T& val, bsearch_t<T>& pos) {
        return insert(pos.idx, val);
    }

    // удалить элемент. Отрицательный - с конца
    bool remove(int idx) {
        if (!length() || idx >= (int)_len || idx < -(int)_len) return false;

        if (idx < 0) idx += _len;
        memcpy((void*)(_buf + idx), (const void*)(_buf + idx + 1), (_len - idx - 1) * sizeof(T));
        --_len;
        return true;
    }

    // удалить несколько элементов, начиная с индекса
    bool remove(uint16_t from, uint16_t amount) {
        if (!length() || !amount || from >= _len) return false;

        uint16_t to = from + amount;
        if (to >= _len - 1) {
            _len = from;
            return true;
        }
        memmove((void*)(_buf + from), (const void*)(_buf + to), (_len - to) * sizeof(T));
        _len -= amount;
        return true;
    }

    // вставить элемент на индекс (отрицательный индекс - с конца, слишком большой - будет push)
    bool insert(int idx, const T& val) {
        if (idx < 0) idx += _len;
        if (idx < 0) return false;

        if (idx == 0) return shift(val);
        else if (idx >= (int)_len) return push(val);

        if (!_fit(_len + 1)) return false;

        memmove((void*)(_buf + idx + 1), (const void*)(_buf + idx), (_len - idx) * sizeof(T));
        _buf[idx] = val;
        ++_len;
        return true;
    }

    // прибавить другой массив того же типа в конец
    bool concat(const stackT& st) {
        return concat(st._buf, st._len);
    }

    // прибавить другой массив в конец
    inline bool operator+=(const stackT& st) {
        return concat(st);
    }

    // прибавить другой массив того же типа в конец
    bool concat(const T* buf, size_t len, bool pgm = false) {
        if (!len) return true;
        if (!buf || !_fit(_len + len)) return false;

        if (pgm) memcpy_P((void*)(_buf + _len), (const void*)(buf), len * sizeof(T));
        else memcpy((void*)(_buf + _len), (const void*)(buf), len * sizeof(T));

        _len += len;
        return true;
    }

    // прибавить бинарные данные, вернёт количество записанных байт
    size_t write(const void* buf, size_t len, bool pgm = false) {
        size_t wlen = (len + sizeof(T) - 1) / sizeof(T);
        if (!len || !buf || !_fit(_len + wlen)) return 0;

        if (pgm) memcpy_P((void*)(_buf + _len), buf, len);
        else memcpy((void*)(_buf + _len), buf, len);

        _len += wlen;
        return len;
    }

    // заполнить значением (на capacity)
    void fill(const T& val) {
        for (size_t i = 0; i < capacity(); i++) _buf[i] = val;
        _len = _size;
    }

    // инициализировать, вызвать конструкторы (на capacity)
    inline void init() {
        fill(T());
    }

    // очистить (установить длину 0)
    inline void clear() {
        _len = 0;
    }

    // количество элементов
    inline uint16_t length() const {
        return _buf ? _len : 0;
    }

    // текущий размер в байтах
    size_t size() const {
        return length() * sizeof(T);
    }

    // установить количество элементов (само вызовет reserve)
    bool setLength(uint16_t len) {
        if (!_fit(_len + len)) return false;

        _len = len;
        return true;
    }

    // есть место для добавления
    bool canAdd() const {
        return _buf ? (_len < _size) : 0;
    }

    // вместимость, элементов
    inline uint16_t capacity() const {
        return AR::size();
    }

    // вместимость, байт
    size_t capacityBytes() const {
        return capacity() * sizeof(T);
    }

    // позиция элемента (-1 если не найден)
    int indexOf(const T& val) const {
        for (size_t i = 0; i < length(); i++) {
            if (_buf[i] == val) return i;
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

    // получить элемент под индексом без проверок
    inline T& _get(int idx) const {
        return _buf[idx];
    }

    // получить элемент под индексом. Отрицательный - с конца
    inline T& operator[](int idx) const {
        return get(idx);
    }

    // буфер существует
    inline bool valid() const {
        return _buf;
    }

    // буфер существует
    explicit inline operator bool() const {
        return _buf;
    }

    // буфер
    using AR::buf;

    // legacy
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
    using AR::_buf;
    using AR::_size;
    uint16_t _len = 0;
    uint16_t _oversize = 8;

    // зарезервировать, элементов (установить новый размер буфера)
    inline bool reserve(uint16_t size) {
        return _size < size ? AR::resize(size) : true;
    }

    // освободить незанятое зарезервированное место
    inline void shrink() {
        AR::resize(_len);
    }

    // зарезервировать, элементов (добавить к текущему размеру буфера)
    inline bool addCapacity(uint16_t size) {
        return _fit(_size + size);
    }

    // установить увеличение размера для уменьшения количества мелких реаллокаций. Умолч. 8
    inline void setOversize(uint16_t oversize) {
        _oversize = oversize;
    }

   private:
    bool _fit(uint16_t size) {
        return (size <= _size || reserve(size + _oversize)) && _buf;
    }
};

// Dynamic Stack
template <typename T>
class stack : public stackT<T, array<T>> {
    typedef stackT<T, array<T>> ST;

   public:
    using ST::addCapacity;
    using ST::reserve;
    using ST::setOversize;
    using ST::shrink;

    // освободить память
    void reset() {
        ST::reset();
        ST::_len = 0;
    }

    stack() {}

    stack(const stack& st) : ST(st) {}

    stack(stack&& st) noexcept {
        move(st);
    }

    stack& operator=(stack st) {
        move(st);
        return *this;
    }

    void move(stack& st) noexcept {
        ST::move(st);
        swap(ST::_len, st._len);
        swap(ST::_oversize, st._oversize);
    }
};

// legacy
template <typename T>
using stack_uniq = stack<T>;

template <typename T>
using stack_copy = stack<T>;

}  // namespace gtl