#include "Vector.h"
#include <iostream>

using namespace std;

Vector::Vector(const ValueType *rawArray, const size_t size, float coef) {
  if (coef <= 1) {
    throw invalid_argument("Coef must be >1");
  }
  _size = size;
  _capacity = size;
  _multiplicativeCoef = coef;
  _data = {new ValueType[size]}; // Указатель на наш массив
  copy(rawArray, rawArray + _size,
       _data); // Копируем элементы из массива (количеством size)
}
// Конструктор копирования
// Создаем новый вектор путем копирования переданного другого вектора
Vector::Vector(const Vector &other) {
  _size = other._size;
  _capacity = other._size;
  _multiplicativeCoef = other._multiplicativeCoef;
  _data = {new ValueType[_capacity]};
  copy(other._data, other._data + _size, _data); // Копируем элементы
}

//Оператор присваивания копированием
Vector &Vector::operator=(const Vector &other) {
  if (this != &other) {//проверка на самого себя
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    ValueType *newData = {new ValueType[_capacity]};
    copy(other._data, other._data + _size, newData); // Копируем элементы
    delete[] _data; // Освобождаем старую память
    _data = newData;
  }
  return *this;
}

// Конструктор перемещения
// Создаем новый вектор путем перемещения как бы переданного другого вектора, а исходный зануляем
Vector::Vector(Vector &&other) noexcept {
  _size = other._size;
  _capacity = other._capacity;
  _data = other._data;
  _multiplicativeCoef = other._multiplicativeCoef;

  other._data = nullptr;
  other._size = 0;
  other._capacity = 0;
}
// Оператор присваивания перемещением
Vector &Vector::operator=(Vector &&other) noexcept {
  if (this != &other) {//проверка на самого себя
    delete[] _data; // освободим старую память
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    _data = other._data;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
  }
  return *this;
}

//Деструктор
Vector::~Vector() {
  delete[] _data; 
}


// Вставка в конец
void Vector::pushBack(const ValueType &value) {
  if (_size + 1 >= _capacity) {//проверка на вместимость
    if (_capacity == 0) {
      _capacity++;
    }

    while (_size + 1 >= _capacity) {
      _capacity *= _multiplicativeCoef;//увеличение вместимости
    }

    ValueType *newData = new ValueType[_capacity];//добавление элемента
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
  }
  _data[_size] = value; // Добавляем в конец новый размер
  _size++;           
}
//Вставка в начало
void Vector::pushFront(const ValueType &value) {
  if (_size + 1 >= _capacity) {//проверка вместимости
    if (_capacity == 0) {
      _capacity++;
    }
    while (_size + 1 >= _capacity) {//увеличение
      _capacity *= _multiplicativeCoef;
    }
    ValueType *newData = new ValueType[_capacity];//вставка элемента
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
  }

  for (int i = _size; i > 0; i--) {
    _data[i] = _data[i - 1];
  }
  _data[0] = value;
  _size++;
}

//Вставка элемента value в позицию pos
void Vector::insert(const ValueType &value, size_t pos) {
  if (_size == _capacity) 
  {//расширениие при необходимости Если текущая емкость равна 0, новая емкость становится равной 1
// иначе она умножается на коэффициент расширения, затем выделяется новый участок памяти
    size_t newCapacity = _capacity == 0 ? 1 : static_cast<size_t>(_capacity * _multiplicativeCoef);
    ValueType *newData = new ValueType[newCapacity];

    if (pos > _size) {//если аозиция за пределами массива просто старые данные копирнём а элемент закинем в конец
      for (int i = 0; i < _size; i++) 
      {
        newData[i] = _data[i];
      }
      newData[_size] = value; 
    } else {//в противном случае копируем все данные до pos вставляем элемент и сдвигаем все остальные на право на 1 
      for (int i = 0; i < pos; i++) 
      {
        newData[i] = _data[i];
      }
      newData[pos] = value; // Добавляем наш элемент в конец
      for (int i = pos; i < _size; i++) 
      {
        newData[i + 1] = _data[i];
      }
    }
    _size++;                 // новый размер
    delete[] _data;          // удаляем старые данные
    _data = newData;         // новые данные
    _capacity = newCapacity; // новая граница
  } else //тут проводим те же дейтсвия просто без увеличения массива 
  {
    if (pos > _size) 
    {
      _data[_size] = value;
      _size++; 
    } else 
    {
      for (int i = _size; i > pos; i--) 
      {
        _data[i] = _data[i - 1];
      }
      _data[pos] = value; 
      _size++;
    }
  }
}

// Вставка массива элементов типа ValueType в позицию pos
void Vector::insert(const ValueType *values, size_t size, size_t pos) {
  if ((_size + size) > _capacity) {//если суммарный размер вектора превышает текущую ёмкость то расширяем наш вектор
    size_t newCapacity = _capacity == 0 ? size : (static_cast<size_t>(_capacity * _multiplicativeCoef) + size);
    ValueType *newData = new ValueType[newCapacity];
//теперь та же песня что и с вставкой элементов, просто вставляем в конец нашего вектора массив если позиция больше чем размер
    if (pos > _size) {
      for (int i = 0; i < _size; i++) {
        newData[i] = _data[i];
      }
      for (int i = 0; i < size; i++) {
        newData[_size + i] = values[i];
      }
    } else {//в противном случае вставляем массив на позицию и делаем с другими элементами то же самое что и в прошлой функции
      for (int i = 0; i < pos; i++) {
        newData[i] = _data[i];
      }
      for (int i = 0; i < size; i++) {
        newData[pos + i] = values[i];
      }
      for (int i = pos; i < _size; i++) {
        newData[size + i] = _data[i];
      }
    }
    _size = _size + size;    
    delete[] _data;          
    _data = newData;         
    _capacity = newCapacity; 
  } else {//и случай если суммарный размер позволяет вставить массив без расширения
    if (pos > _size) {
      for (int i = 0; i < size; i++) {
        _data[_size + i] = values[i];
      }
      _size = _size + size; 
    } else {
      for (int i = _size; i > pos; i--) {
        _data[i + size - 1] = _data[i - 1];
      }
      for (int i = 0; i < size; i++) {
        _data[pos + i] = values[i];
      }
      _size = _size + size;
    }
  }
}

// Вставка содержимого вектора в позицию pos
void Vector::insert(const Vector &vector, size_t pos) {
  int size = vector.size();
  /*тут такая же аналогичная ситуация, сначала проверяем хватает ли итогового места для вставки другого вектора в наш исходный
если нет увеличиаваем место чтоб хвататло*/
  if ((_size + size) > _capacity) {
    size_t newCapacity = _capacity == 0 ? size : (static_cast<size_t>(_capacity * _multiplicativeCoef) + size);
    ValueType *newData = new ValueType[newCapacity];
    //аналогично предыдущим двум функциям производим вставку элементов
    if (pos > _size) {
      for (int i = 0; i < _size; i++) {
        newData[i] = _data[i];
      }
      for (int i = 0; i < size; i++) {
        newData[_size + i] = vector[i];
      }
    } else {
      for (int i = 0; i < pos; i++) {
        newData[i] = _data[i];
      }
      for (int i = 0; i < size; i++) {
        newData[pos + i] = vector[i];
      }
      for (int i = pos; i < _size; i++) {
        newData[size + i] = _data[i];
      }
    }
    _size = _size + size;   
    delete[] _data;         
    _data = newData;         
    _capacity = newCapacity; 
  } else {//если раширение не требуется то вставляем вектор в наш вектор таким же путём как и в прошлых функциях
    if (pos > _size) {
      for (int i = 0; i < size; i++) {
        _data[_size + i] = vector[i];
      }
      _size = _size + size; 
    } else {
      for (int i = _size; i > pos; i--) {
        _data[i + size - 1] = _data[i - 1];
      }
      for (int i = 0; i < size; i++) {
        _data[pos + i] = vector[i];
      }
      _size = _size + size; 
    }
  }
}

//Удаление последнего элемента
void Vector::popBack() {
  if (_size == 0) {// Проверка, что вектор не пустой
    throw "Error";
  }        
  _size--;
}
//Удаление первого элемента
void Vector::popFront() {
  if (_size == 0) {//Проверка, что вектор не пустой
    throw "Error";
  }
  for (int i = 0; i < _size; i++) {//просто сдвиг данных на один элемент влево
    _data[i] = _data[i + 1];
  }
  _size--; 
}

//Удаление нескольких элементов начиная с указаной позиции 
void Vector::erase(size_t pos, size_t count) {
  if (pos + count >= _size) {//если количество запрошенное для удаление больше или равно границы удаляем все начиная с позиции 
    _size = pos;//и сдивагем границу
  } else {
    // и от позиции переносим элементы нациная с позиции
    for (int i = pos; i < _size; i++) {
      _data[i] = _data[i + count];
    }
    _size = _size - count;
  }
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
  if (beginPos > endPos || beginPos >= _size) {//проверка на то чтобы начальная позиция была меньше чем конечная 
    return;
  }
  if (endPos >= _size) {//если от позиции и до конца или больше размера, то просто сдвигаем границу
    _size = beginPos;
  } else {
    int cnt = _size - endPos - 1;//вычисляем количество элементов которые нужо будет перенести
    for (int i = 0; i < cnt; i++) {
      _data[beginPos + i] = _data[endPos + i];//и переносим элементы за конченой позициией в места элемнетов которые нужно ликвидировать
    }
    _size = _size - cnt;//уменьшаем размер на количестов перемещенных/удалённых элементов
  }
}


//возвращаем количество элементов
size_t Vector::size() const { 
  return _size; 
  }

//возвращем вмстимость вектора
size_t Vector::capacity() const { 
  return _capacity; 
  }

//возвращаем загруженность вектора
// Расчитаем как отношение текущего размера к общедоступному
// При этом проверим, чтобы не делим на ноль, ни у приведем к типу double
double Vector::loadFactor() const {
  return _capacity == 0 ? 0 : static_cast<double>(_size) / _capacity;
}


//Доступ к элементу по индексу
//Проверим, что запрос из доступного диапазона, иначе ошибка
ValueType &Vector::operator[](size_t idx) {
  if (idx >= _size){
    throw out_of_range("Vector::operator[] ERROR: Index out of range");
  }
  return _data[idx];
}

//Доступ к элементу по индексу константая версия
const ValueType &Vector::operator[](size_t idx) const {
  if (idx >= _size)
    throw out_of_range("Vector::operator[] ERROR: Index out of range");
  return _data[idx];
}

//Поиск первого вхождения значения, будем возвращать индекс элемента
long long Vector::find(const ValueType &value) const {
  for (int i = 0; i < _size; i++) {
    if (_data[i] == value) {
      return i;
    }
  }
  return -1;
}

//изменение/резервирование вместимости вектора
void Vector::reserve(size_t capacity) {
  if (capacity > _capacity) {//проверка на то что вообще нужно ли изменять вместимость
    ValueType *newData = new ValueType[capacity];
    for (int i = 0; i < _size; i++) {
      newData[i] = _data[i];
    }
    delete[] _data;       // удаляем старые данные
    _data = newData;      // новые данные
    _capacity = capacity; // новая граница
  }
}

//Уменьшить capacity до size
void Vector::shrinkToFit() {
  _capacity = _size;
  ValueType *newData = new ValueType[_size];
  for (int i = 0; i < _size; i++) {
    newData[i] = _data[i];
  }
  delete[] _data;
  _data = newData;
}


//класс ИТЕРАТОР

//конструктор, который создаёт новый объект Iterator, инициализируя его внутренний указатель _ptr предоставленным адресом ptr
Vector::Iterator::Iterator(ValueType *ptr) {
  _ptr = ptr; // OK
}

//оператор разыменования, нужен для предоставления доступа к значению на которое указывает итератор
ValueType &Vector::Iterator::operator*() {
  return *_ptr; 
}
//онстантная версия оператора разыменования
const ValueType &Vector::Iterator::operator*() const {
  return *_ptr;
}


//оператор доступа к членам объекта
ValueType *Vector::Iterator::operator->() { 
  return _ptr; 
  }
//и константная версия
const ValueType *Vector::Iterator::operator->() const { 
  return _ptr; 
  }

//оператор перемещает итератор на следующий элемент. Оператор инткремента
//префиксная форма означает, что инкремент выполняется сразу, и возвращается обновленное значение
Vector::Iterator Vector::Iterator::operator++() {
  _ptr++;
  return *this;
}

//Этот оператор определяет постфиксную форму оператора инкремента (например, it++). 
//При использовании постфиксной формы итератор сначала возвращает своё текущее значение до инкремента,
//а затем увеличивает внутренний указатель _ptr на один элемент вектора. 
Vector::Iterator Vector::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}
//Этот оператор сравнивает два итератора на равенство. 
//Итераторы считаются равными, если они указывают на один и тот же элемент вектора 
bool Vector::Iterator::operator==(const Iterator &other) const {
  return _ptr == other._ptr; // OK
}
//Этот оператор — логическое отрицание оператора равенства ==. 
//Он возвращает true, если итераторы указывают на разные элементы вектора, 
//и false — если на один и тот же. 
bool Vector::Iterator::operator!=(const Iterator &other) const {
  return _ptr != other._ptr; // OK
}

//Получить итератор на начало вектора
Vector::Iterator Vector::begin() {
  return Iterator(&_data[0]);
}

//Получить итератор на конец вектора
Vector::Iterator Vector::end() {
  return Iterator(&_data[_size]);
}