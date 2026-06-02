This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/GTL.svg?color=brightgreen)](https://github.com/GyverLibs/GTL/releases/latest/download/GTL.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GTL.svg)](https://registry.platformio.org/libraries/gyverlibs/GTL)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GTL?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Gyver Template Library
A set of template tools
- Smart pointers
- Dynamic arrays
- Buffers
- Related list

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Use of use](#usage)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Use of use
### Allocator
`array_x`/`stack_x`cherry`realloc`to resize. This allows you to change the size without creating holes in the memory, as in other vector-like libraries, the memory is used very efficiently. But at the same time **GTL arrays do not cause constructors and destructors**, so it is strongly recommended not to use them with objects that contain dynamic data (for example String) or copy/move semantics!!!

### move
`gtl::array`and`gtl::stack`support copy/move semantics, there is a function`gtl::move(x)`:

- `gtl::array<> a(b)` - copy
- `gtl::array<> a(gtl::move(b))` - move

### gtl::array
Dynamic Array

```cpp
// buffering
T* buf();
operator T*();

// dimension
uint16_t size();

// byte size
size_t sizeBytes();

// clean (fill in zeros)
void clear();

// resize the number of elements T
bool resize(uint16_t size);

// buffer
void reset();

// Move (swap) from another instance
void move(array& other);
```

### gtl::stack_x
Dynamic array, stores data linearly, allows you to add and remove them. There are several options:
- `stack`- dynamic buffer
- `stack_ext`- external buffer
- `stack_static`- internal static buffer

```cpp
// file
bool writeToFile(FS& fs, const char* path);

// file
bool readFromFile(FS& fs, const char* path);

// export to Stream (e.g. file)
bool writeTo(TS& stream);

// Import from Stream (e.g. file)
bool readFrom(TS& stream);

// finish
bool push(const T& val, ...);

// add if there is no element with this value n
bool pushUniq(const T& val);

// finish
bool operator+=(const T& val);

// finish off
T pop();

// read out
T& last();

// begin
bool shift(const T& val);

// get out
T unshift();

// read from the beginning without removing
T& first();

// remove the element. Negative from the end.
bool remove(int idx);

// Remove several elements, starting with the index
bool remove(size_t from, size_t amount);

// insert an item on an index (length() index is allowed)
bool insert(int idx, const T& val);

// add another array
bool concat(const stack_ext& st);
bool concat(const T* buf, size_t len, bool pgm = false);

// add another array
bool operator+=(const stack_ext& st);

// add binary
size_t write(const void* buf, size_t len, bool pgm = false);

// fill in the value (on capacity)
void fill(const T& val);

// Initialize, call constructors (to capacity)
void init();

// clean (set length 0)
void clear();

// byte size
size_t size();

// number
size_t length();

// There is free space, elements
uint16_t left();

// set up
bool setLength(size_t len);

// add
bool addLength(size_t len);

// there is room to add
bool canAdd();

// capacity, elements
size_t capacity();

// capacity, byte
size_t capacityBytes();

// element position (-1 if not found)
int indexOf(const T& val);

// component
bool has(const T& val);

// Remove the value (true if there is no element)
bool removeByVal(const T& val);

// Get an item under the index. Negative from the end.
T& get(int idx);

// Get the index from the beginning without checking. Negative from the end.
T& operator[](int idx);

// buffering
T* buf();

// pointer
T* end();

// buffer exists
bool valid();

// buffer exists
explicit operator bool();

// Binary search in a sorted stack
bsearch_t<T> searchSort(const T& val);

// add sorting. uniq flag - do not add if the element already exists
bool addSort(const T& val, bool uniq = false);

// add sorted to bsearch t from searchSort
bool addSort(const T& val, bsearch_t<T>& pos);

// stack
void sort();

// FOR DYNAMIC
// Increase the size to reduce the number of small locations. Shut up. 8.
void setOversize(uint16_t oversize);

// reserve elements (set a new buffer size)
bool reserve(size_t size);

// vacate the vacant reserved seat
void shrink();

// reserve elements (add to the current length)
bool addCapacity(size_t size);

// buffer
void reset();
```

### gtl::fifo_x
FIFO buffer
- `fifo_ext`- external buffer
- `fifo_static`- internal static buffer

```cpp
// plug in
void setBuffer(T* buf, Ti capacity);

// Buffer entry. Return true upon successful recording
bool write(const T& val);

// Buffer entry. Return the number of recorded elements
Ti write(const T* vals, Ti len);

// buffer
bool isFull();

// buffer
bool isEmpty();

// buffering
T& read();

// Buffer reading. Return the number of items read
Ti read(T* vals, Ti len);

// remove from the buffer, return the quantity
Ti consume(Ti len);

// returns extreme value without removal from buffer
T& peek();

// Get a pointer to a continuous reading area
T* peekBuffer();

// Get the length of a continuous reading area
Ti peekLength();

// Get the index from the beginning. Negative from the end.
T& get(int i);

// Get the index from the beginning without checking. Negative from the end.
T& operator[](int i);

// last
T& getLast();

// number of unread elements
size_t length();

// buffer
size_t size();

// record-free
size_t canWrite();

// cleanse
void clear();

T* buffer;
```

### gtl::lbuf_x
Linear buffer with overflow and sequential reading
- `lbuf_ext`- external buffer
- `lbuf_static`- internal static buffer

```cpp
// plug in
void setBuffer(T* buf, Ti capacity);

// buffer
void write(T val);

// buffering
void write(size_t n, T val);

// number-reading
T& get(size_t n);

// Get the index from the beginning without checking. Negative from the end.
T& operator[](size_t n);

// first (left) position
Ti getHead();

// buffer
size_t size();

// "clean" the buffer
void clear();

T* buffer = nullptr;
```

```cpp
  lbuf_static<uint8_t, 8> buf;

  for (int i = 0; i < 12; i++) {
    buf.write(i);
    for (int j = 0; j < 8; j++) {
      Serial.print(buf.read(j));
      Serial.print(',');
    }
    Serial.println();
  }
```

### gtl::linked_list
Related list for creating a dynamic array of objects in the stack. See example in examples
```cpp
// iterate
list_iter iter();

// add
bool add(list_node& node);

// add
bool add(list_node* node);

// remove
void remove(list_node& node);

// remove
void remove(list_node* node);

// list
bool has(list_node& node);

// list
bool has(list_node* node);

// length
size_t length();

// clear out
void clear();

// get the last item in the list
list_node* getLast();
```

<a id="versions"></a>

## Versions
- v1.0
- v1.1 - Added list

<a id="install"></a>
## Installation
- The library can be found under the name **GTL** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/GTL/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
