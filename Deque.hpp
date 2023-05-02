#pragma once
#include <iterator>
#include <memory>
#include <vector>
#include <iostream>
#include <limits>

namespace fefu_laboratory_two {
template <typename T>
class Allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  // Конструктор по умолчанию не принимает аргументов и устанавливает все переменные класса в их значения по умолчанию.
  // Таким образом, все переменные получат значения, которые определены для них по умолчанию.
  // В данном случае, поскольку все переменные типа определены через using,
  // то значения переменных будут определены их значениями по умолчанию для соответствующих типов.
  Allocator() = default;

  // Конструктор копирования по умолчанию не принимает аргументов и создает копию объекта типа Allocator.
  // Этот конструктор генерируется компилятором автоматически, если не определить его явно в классе.
  Allocator(const Allocator& other) = default;

  // Конструктор копирования для аллокаторов других типов. Это шаблонный конструктор, который позволяет создавать объекты `Allocator`
  // из других аллокаторов, которые могут иметь другой тип.
  template <class U>
  Allocator(const Allocator<U>& other){}

  // Деструктор по умолчанию. Он освобождает любые ресурсы, связанные с объектом аллокатора.
  ~Allocator() = default;

  // Возвращается указатель на выделенное место в памяти под n элементов * на размер типа.
  pointer allocate(size_type n)
  {
      return static_cast<pointer>(operator new(sizeof(T) * n ));
  }

  // Подчищается память начиная с указателя p.
  void deallocate(pointer p) noexcept
  {
      operator delete(p);
  }

  //[[nodiscard]] std::allocation_result<T*> allocate_at_least(
  // std::size_t n ); // TODO For extra points
};

// Класс Node(узел) с value, указателем на следующий элемент и предыдущий.
template <typename T>
class Node {
public:
    T value = NULL;
    Node* next = nullptr;
    Node* previous = nullptr;
};

template <typename ValueType>
class Deque_iterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t; //ptrdiff_t - целочисленный тип данных для представления разницы между указателями в памяти, достаточная размерность,знаковый и целочисленный.
  using pointer = ValueType*;
  using reference = ValueType&;
  Node<value_type> *cur;

  // Конструктор по умолчанию не принимает аргументов и устанавливает все переменные класса в их значения по умолчанию.
  // Таким образом, все переменные получат значения, которые определены для них по умолчанию.
  // В данном случае, поскольку все переменные типа определены через using,
  // то значения переменных будут определены их значениями по умолчанию для соответствующих типов.
  Deque_iterator() = default;

  // Инициализая итератора, тоесть итератор будет указывать на элемент, на который указывает указатель p
  Deque_iterator(pointer p){
      cur = p;
  }

  // Конструктор копирования, который копирует указатель на текущий элемент из другого итератора
  Deque_iterator(const Deque_iterator& other) noexcept{
      cur = other.cur;
  }

  // Оператор присваивания, который присваивает указатель на текущий элемент из другого итератора.
  Deque_iterator& operator=(const Deque_iterator& a)
  {
      cur = a.cur;
      return *this;
  }

  // Деструктор по умолчанию
  ~Deque_iterator() = default;

  // Функция-дружественный метод, который обменивает значения двух итераторов.
  // friend дает методу swap доступ к cur, который находится в private. также потому что swap не реализован в private.
  friend void swap(Deque_iterator<ValueType>& a, Deque_iterator<ValueType>& b){
      Deque_iterator<ValueType> c = a;
      a = b;
      b = c;
  }

  // Операторы сравнения, которые сравнивают текущий элемент двух итераторов на равенство и неравенство
  friend bool operator==(const Deque_iterator<ValueType>& a, const Deque_iterator<ValueType>& b){
      return a.cur == b.cur;
  }

  friend bool operator!=(const Deque_iterator<ValueType>& a, const Deque_iterator<ValueType>& b){
      return a.cur != b.cur;
  }

  // Оператор разыменования, который возвращает ссылку на текущий элемент.
  // reference не создает копию объекта, а предоставляет доступ к существующему объекту.
  reference operator*() const{
      return cur->value;
  }

  pointer operator->() const{
      return *cur -> value;
  }

  // Операторы, которые перемещают текущий элемент на следующий
  // элемент и возвращают копию текущего элемента до увеличения.
  Deque_iterator& operator++() {
      cur = cur->next;
      return *this;
  }

  //Постфикс, поэтому копируем итератор в temp и возвращаем его до увеличения.
  Deque_iterator operator++(int){
      Deque_iterator<ValueType> temp(*this);
      operator++();
      return temp;
  }


  //Смещаем указатель на прошлый элемент и return
  Deque_iterator& operator--()
  {
      cur = cur->previous;
      return *this;
  }

  //return текущего элемента, а потом делаем смещение
  Deque_iterator operator--(int)
  {
      Deque_iterator<ValueType> temp(*this);
      operator--();
      return temp;
  }

  //difference_type - это тип, который используется для представления разницы между двумя итераторами.
  //В данном контексте он используется для определения того, на сколько позиций нужно сдвинуть текущий итератор вперед или назад.
  // Размер этого типа зависит от реализации и может отличаться для разных компиляторов или архитектур.
  Deque_iterator operator+(const difference_type& a) const
  {
      return Deque_iterator<ValueType>(*this) += a;
  }

  Deque_iterator& operator+=(const difference_type& a)
  {
      cur = cur + a;
      return *this;
  }

  Deque_iterator operator-(const difference_type& a) const
  {
      return Deque_iterator<ValueType>(*this) -= a;
  }
  Deque_iterator& operator-=(const difference_type& a)
  {
      cur = cur - a;
      return *this;
  }


  //operator- предназначен для вычисления разницы между текущим элементом и элемтом другого итератора.
  //Возвращает кол-во элементов между итераторами.
  difference_type operator-(const Deque_iterator& a) const
  {
      difference_type total = cur - a.cur;
      return abs(total);
  }


  reference operator[](const difference_type& a)
  {
      return *(cur + a);
  }

  friend bool operator<(const Deque_iterator<ValueType>& a,
                        const Deque_iterator<ValueType>& b)
  {
      return a.cur < b.cur;
  }
  friend bool operator<=(const Deque_iterator<ValueType>& a,
                         const Deque_iterator<ValueType>& b)
  {
      return a.cur <= b.cur;
  }
  friend bool operator>(const Deque_iterator<ValueType>& a,
                        const Deque_iterator<ValueType>& b)
  {
      return a.cur > b.cur;
  }
  friend bool operator>=(const Deque_iterator<ValueType>& a,
                         const Deque_iterator<ValueType>& b)
  {
      return a.cur >= b.cur;
  }
};


//Все тоже самое что и в Deque_iterator. Только класс Deque_const_iterator не дает возможности делать изменения элемента на который указывает итератор.
template <typename ValueType>
class Deque_const_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType*;
    using reference = ValueType&;
    Node<value_type> *cur;
    Deque_const_iterator() = default;

    Deque_const_iterator(const Deque_const_iterator& other) noexcept{
        cur = other.cur;
    }

    Deque_const_iterator& operator=(const Deque_const_iterator& a)
    {
        cur = a.cur;
        return *this;
    }

    ~Deque_const_iterator() = default;

    friend void swap(Deque_const_iterator<ValueType>& a, Deque_const_iterator<ValueType>& b){
        Deque_const_iterator<ValueType> c = a;
        a = b;
        b = c;
    }

    friend bool operator==(const Deque_const_iterator<ValueType>& a,
                           const Deque_const_iterator<ValueType>& b)
    {
        return a.cur == b.cur;
    }
    friend bool operator!=(const Deque_const_iterator<ValueType>& a,
                           const Deque_const_iterator<ValueType>& b)
    {
        return a.cur != b.cur;
    }

    reference operator*() const
    {
        return cur->value;
    }
    pointer operator->() const
    {
        return *cur ->value;
    }

    Deque_const_iterator& operator++(){
        cur = cur->next;
        return *this;
    }

    Deque_const_iterator operator++(int){
        Deque_const_iterator<ValueType> temp(*this);
        operator++();
        return temp;
    }

    Deque_const_iterator& operator--()
    {
        cur = cur->previous;
        return *this;
    }
    Deque_const_iterator operator--(int)
    {
        Deque_iterator<ValueType> temp(*this);
        operator--();
        return temp;
    }

    Deque_const_iterator operator+(const difference_type& a) const
    {
        return Deque_iterator<ValueType>(*this) += a;
    }
    Deque_const_iterator& operator+=(const difference_type& a)
    {
        cur = cur + a;
        return *this;
    }

    Deque_const_iterator operator-(const difference_type& a) const
    {
        return Deque_const_iterator<ValueType>(*this) -= a;
    }
    Deque_const_iterator& operator-=(const difference_type& a)
    {
        cur = cur - a;
        return *this;
    }
    difference_type operator-(const Deque_const_iterator& a) const
    {
        difference_type total = cur - a.cur;
        return abs(total);
    }

    reference operator[](const difference_type& a)
    {
        return *(cur + a);
    }

    friend bool operator<(const Deque_const_iterator<ValueType>& a,
                          const Deque_const_iterator<ValueType>& b)
    {
        return a.cur < b.cur;
    }
    friend bool operator<=(const Deque_const_iterator<ValueType>& a,
                           const Deque_const_iterator<ValueType>& b)
    {
        return a.cur <= b.cur;
    }
    friend bool operator>(const Deque_const_iterator<ValueType>& a,
                          const Deque_const_iterator<ValueType>& b)
    {
        return a.cur > b.cur;
    }
    friend bool operator>=(const Deque_const_iterator<ValueType>& a,
                           const Deque_const_iterator<ValueType>& b)
    {
        return a.cur >= b.cur;
    }
};

template <class Iter>
class Deque_reverse_iterator {
 public:

  //Используется iterator_traits для определения типа итератора, который мы оборачиваем. В данном классе мы хотим получить доступ к типу, который определяет
  //значение на который указывает итератор.

    //Тип итератора который мы оборачиваем в обратный итератор
  using iterator_type = Iter;
    //Тип категории итератора, который используется в тех методах где требуется знание категории
  using iterator_category = typename std::iterator_traits<Iter>::iterator_category;
    // Тип значения, на который указывает итератор.
  using value_type = typename std::iterator_traits<Iter>::value_type;
    // Тип разницы между двумя итераторами.
  using difference_type = typename std::iterator_traits<Iter>::difference_type;
    // Тип указателя на значение, на которое указывает итератор.
  using pointer = typename std::iterator_traits<Iter>::pointer;
    // Тип ссылки на значение, на которое указывает итератор.
  using reference = typename std::iterator_traits<Iter>::reference;

  iterator_type _it; //Основной итератор. Вся логика класса построена на основе этого итератора, обратный итератор - просто обертка для основного.
  value_type* cur; //Указатель на элемент в обратном итераторе.
  constexpr Deque_reverse_iterator()= default; // Конструктор по умолчанию.


  //explicit служит для обозначения, что конструктор может быть вызван только явно,
  // нельзя использовать неявные преобразования типов,такие как инициализация, автоматическое преобразование
  constexpr explicit Deque_reverse_iterator(iterator_type x){
      _it = x;
  }

  // Конструктор копирования.
  template <class U>
  constexpr Deque_reverse_iterator(const Deque_reverse_iterator<U>& other){
      _it = other._it;
  }

  //Оператор присваивания.
  template <class U>
  Deque_reverse_iterator& operator=(const Deque_reverse_iterator<U>& other){
    _it = other._it;
  }

  //Возвращает базовый итератор.
  iterator_type base() const {return _it;}


  //Возвращаем значение элемента, на который указывает основной итератор.
  reference operator*() const {return *base();}


   //Возвращает указатель на текущий элемент.
  pointer operator->() const {return base();}


  // Возвращает ссылку на элементн находящийся на n от текущей позиции.
  reference operator[](difference_type n){
      return *(base() + n);
  }

  //префиксная форма, перегрузка оператора ++, смещаемся на одну позицию назад
  Deque_reverse_iterator& operator++() {
      --_it;
      return *this;
  }

  //постфиксная форма, перегрузка оператора ++, смещаемся на одну позицию назад
  Deque_reverse_iterator operator++(int val) {
      Deque_reverse_iterator tmp(*this);
      operator++();
      return tmp;
  }

  //Все тоже самое что с ++, но наоборот
  Deque_reverse_iterator& operator--() {
      ++_it;
      return *this;
  }
  Deque_reverse_iterator operator--(int val) {
      Deque_reverse_iterator tmp(*this);
      operator--();
      return tmp;
  }

  //Смещение на n позиций назад. Создаем новый итератор, смещенный на n позиций и вызывается конструктор от нового итератора.
  Deque_reverse_iterator operator+(difference_type n) const {
      iterator_type it = _it - n;
      return Deque_reverse_iterator<iterator_type>(it);

  }

  //Перегрузка оператора +=, смещение на n, возвращает ссылку на текущий элемент
  Deque_reverse_iterator& operator+=(difference_type n) {
      _it -= n;
      return reverse_iterator(_it);

  }


  //Все тоже самое что с оператором + и +=
  Deque_reverse_iterator operator-(difference_type n) const {
      iterator_type it = _it + n;
      return Deque_reverse_iterator<iterator_type>(it);
  }

  Deque_reverse_iterator& operator-=(difference_type n) {
      _it += n;
      return reverse_iterator(_it);
  }

  //Операторы сравнения, проверяются итераторы, равны они или нет.
  template <class Iterator1, class Iterator2>
  friend bool operator==(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() == rhs.base();
  }

  template <class Iterator1, class Iterator2>
  friend bool operator!=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() != rhs.base();
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>(const Deque_reverse_iterator<Iterator1>& lhs,
                        const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() > rhs.base();
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<(const Deque_reverse_iterator<Iterator1>& lhs,
                        const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() < rhs.base();
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() <= rhs.base();
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs){
      return lhs.base() >= rhs.base();
  }

  //Перегрузка  оператора сложения для reverse_iterators
  template <class Iterator>
  friend auto operator+(const Deque_reverse_iterator<Iterator>& lhs,
                        const Deque_reverse_iterator<Iterator>& rhs){
      return Deque_reverse_iterator<Iterator>(lhs.base() + rhs.base());
  }


  //Перегрузка оператора вычитания для reverse_iterators
  template <class Iterator>
  friend auto operator-(const Deque_reverse_iterator<Iterator>& lhs,
                        const Deque_reverse_iterator<Iterator>& rhs){
      return Deque_reverse_iterator<Iterator>(lhs.base() - rhs.base());
  }
};


template <typename T, typename Allocator = Allocator<Node<T>>>
class Deque {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = Deque_iterator<value_type>;
  using const_iterator = Deque_const_iterator<value_type>;
  using reverse_iterator = Deque_reverse_iterator<iterator>;
  using const_reverse_iterator = Deque_reverse_iterator<const_iterator>;

  Allocator alloc;
  Node<value_type>* first = nullptr;
  Node<value_type>* last = nullptr;
  value_type _size = 0;


  /// @brief Default constructor. Constructs an empty container with a
  /// default-constructed allocator.
  Deque(){}

  /// @brief Constructs an empty container with the given allocator
  /// @param alloc allocator to use for all memory allocations of this container

  //Конструктор с аллокатором, выделяется память под 1 Node. Указатели стоят на текущей Ноде.
  explicit Deque(const Allocator& alloc)
  {
     Node<value_type> *node = alloc.allocate(1);
     first = node;
     last = node;
  }

  /// @brief Constructs the container with count copies of elements with value
  /// and with the given allocator
  /// @param count the size of the container
  /// @param value the value to initialize elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container

  //Конструктор, создает дек с count элементами, каждый из которых инициализирован value
  Deque(size_type count, const T& value, const Allocator& alloc = Allocator())
  {
      for(size_type i = 0; i < count; i++){
          Node<value_type> *node = alloc.allocate(1);
          push_back(value);
      }
  }

  /// @brief Constructs the container with count default-inserted instances of
  /// T. No copies are made.
  /// @param count the size of the container
  /// @param alloc allocator to use for all memory allocations of this container

  //Конструктор с count элементами, каждый из которых имеет значение по умолчанию для элементов типа value_type
  explicit Deque(size_type count, const Allocator& alloc = Allocator())
  {
      for(value_type i = 0; i < count; i++){
          push_back(value_type());
      }
  }

  /// @brief Constructs the container with the contents of the range [first,
  /// last).
  /// @tparam InputIt Input Iterator
  /// @param first, last 	the range to copy the elements from
  /// @param alloc allocator to use for all memory allocations of this container

  //Конструктор с итераторами first,last. push_back добавляет элементы *i, разименовываем элементы на которые указывает итератор.
  template <class InputIt>
  Deque(InputIt first, InputIt last, const Allocator& alloc = Allocator()){
      for(InputIt i = first; i != last; i++){
          push_back(*i);
      }
  }

  /// @brief Copy constructor. Constructs the container with the copy of the
  /// contents of other.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  //Конструктор копирования, пробегаемся пр всем элементам контейнера other. На каждой итерации выделяется память для нового узла Node
  //с помощью allocate() из аллокатора связанного с other. Затем значение элемента i контейнера other копируется в значение поля value нового узла Node
  //Если Other пустой то first и last указывают на новый узел Node, иначе новый узел добавляется в конец контейнера.
  //В конце устанавливается размер нового контейнера _size = other.size()
  Deque(const Deque& other)
  {
      for(value_type i = 0; i < other.size(); i++){
          Node<value_type> *node = other.get_allocator().allocate(1);
          node->value = other[i];
          if(empty()){
              this->first = node;
              this->last = node;
          }
          else{
              last -> next = node;
              last = node;
          }
      }
      _size = other.size();
  }

  /// @brief Constructs the container with the copy of the contents of other,
  /// using alloc as the allocator.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container

  //Конструктор копирования, который является копией другого объекта other. alloc для выделения памяти для каждого узла.
  //Для каждого i в other выделяется память для нового узла, устанавливается значение other[i]
  //Если новый дек пустой,, то first и last указывают только на созданный узел
  //Иначе
  Deque(const Deque& other, const Allocator& alloc)
  {
      for(value_type i = 0; i < other.size();i++){
          Node<value_type> *node = alloc.allocate(1);
          node->value = other[i];
          if(empty()){
              first = node;
              last = node;
          }
          else{
              node->previous = last; //предыдущий элемент для текущего это last
              last -> next = node; // предыдущий указывает на новый
              last = node; // last становится node
              first->previous = last; //прошлый для first указывает на last элемент
          }
      }
      _size = other.size();
  }

  /**
   * @brief Move constructor.
   *
   * Constructs the container with the contents of other using move semantics.
   * Allocator is obtained by move-construction from the allocator belonging to
   * other.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   */
  //rvalue параметр, для того чтобы забрать ресурсы из other. Используется для оптимизации работы с большими объектами
  //noexcept не генерирует исключения.
  Deque(Deque&& other) noexcept
  {
      for(value_type i = 0; i < other.size();i++){
          Node<value_type> *node = other.get_allocator().allocate(1);
          node->value = other[i];
          if(empty()){
              first = node;
              last = node;
          }
          else{
              node->previous = last;
              last -> next = node;
              last = node;
              first->previous = last;
          }
      }
      _size = other.size();
  }

  /**
   * @brief Allocator-extended move constructor.
   * Using alloc as the allocator for the new container, moving the contents
   * from other; if alloc != other.get_allocator(), this results in an
   * element-wise move.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   * @param alloc allocator to use for all memory allocations of this container
   */

  //rvalue параметр, для того чтобы забрать ресурсы из other. Используется для оптимизации работы с большими объектами,
  //передаем в него новый аллокатор, далее переносим через Move все элементы в новый дек, first и last не  нужно изменять
  //т.к они настроены в other deque.
  Deque(Deque&& other, const Allocator& alloc)
  {
      this->alloc = alloc;
      for(size_type i = 0; i < other.size();i++){
          push_back(std::move(other[i]));
      }
  }

  /// @brief Constructs the container with the contents of the initializer list
  /// init.
  /// @param init initializer list to initialize the elements of the container
  /// with
  /// @param alloc allocator to use for all memory allocations of this container
  //Конструктор с инициализацией дека через list init. push back init.
  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()){
      for(auto i: init){
          push_back(i);
      }
  }

  /// @brief Destructs the deque.


  //Подчистка памяти в деструкторе
  ~Deque(){
      Node<value_type>* current = first;
      while(current != nullptr) {
          Node<value_type>* next = current->next;
          alloc.deallocate(current);
          current = next;
      }
      if (std::uncaught_exception()) {
          std::cerr << "~Deque() stack unwinding, not throwing\n";
      } else {
          std::cerr << "~Deque() normal case, throwing\n";
      }
  }

  /// @brief Copy assignment operator. Replaces the contents with a copy of the
  /// contents of other.
  /// @param other another container to use as data source
  /// @return *this

  //Перегрузка оператора присваивания.
  Deque& operator=(const Deque& other){
      clear();
      for(size_type i = 0; i < other.size();i++){
          push_back(other[i]);
      }
  }

  /**
   * Move assignment operator.
   *
   * Replaces the contents with those of other using move semantics
   * (i.e. the data in other is moved from other into this container).
   * other is in a valid but unspecified state afterwards.
   *
   * @param other another container to use as data source
   * @return *this
   */

  //Перегрузка оператора присваивания используя rvalue
  Deque& operator=(Deque&& other){
      clear();
      for(size_type i = 0; i < other.size();i++){
          push_back(std::move(other[i]));
      }
  }

  /// @brief Replaces the contents with those identified by initializer list
  /// ilist.
  /// @param ilist
  /// @return this

  //Присваивание нового list в Deque
  Deque& operator=(std::initializer_list<T> ilist){
        clear();
        for(auto i: ilist){
            push_back(*i);
        }
  }

  /// @brief Replaces the contents with count copies of value
  /// @param count
  /// @param value

  //Замена элементов, count - колво, value - значение
  void assign(size_type count, const T& value){
      resize(count, value);
  }

  /// @brief Replaces the contents with copies of those in the range [first,
  /// last).
  /// @tparam InputIt
  /// @param first
  /// @param last
  //Очистка дека, передаем first и last итератор и Push back *i
  template <class InputIt>
  void assign(InputIt first, InputIt last){
      clear();
      for(InputIt i = first; i != last; i++){
          push_back(*i);
      }
  }

  /// @brief Replaces the contents with the elements from the initializer list
  /// ilis
  /// @param ilist
  void assign(std::initializer_list<T> ilist){
      clear();
      for(auto i: ilist){
          push_back(i);
      }
  }

  /// @brief Returns the allocator associated with the container.
  /// @return The associated allocator.
  allocator_type get_allocator() const noexcept
  {
      return alloc;
  }

  /// ELEMENT ACCESS

  /// @brief Returns a reference to the element at specified location pos, with
  /// bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.
  /// @throw std::out_of_range

  //Возвращает ссылку на элемент по индексу pos
  //Если pos+1>_size то исключение, _size размер контейнера
  reference at(size_type pos){
      if(pos+1 > _size) throw std::out_of_range("index out of range");
      else return operator[](pos);
  }

  /// @brief Returns a const reference to the element at specified location pos,
  /// with bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.
  /// @throw std::out_of_range

  //Возвращаем const ссылку на элемент.
  const_reference at(size_type pos) const{
      if(pos+1 > _size) throw std::out_of_range("index out of range");
      else return operator[](pos);
  }

  /// @brief Returns a reference to the element at specified location pos. No
  /// bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.

  //Возвращает ссылку на [pos] элемент. Обходим узлы и проверяем значение value в каждом.
  reference operator[](size_type pos){
      size_type it = 0;
      for(Node<value_type>* cur = first; cur != nullptr;cur = cur->next)
      {
          if(pos == it)
          {
              return cur->value;
          }
          it++;
      }
  }

  /// @brief Returns a const reference to the element at specified location pos.
  /// No bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.

  //Все тоже самое только const ссылка.
  //const нужно для того чтобы предотвратить изменение значений вне класса.
  const_reference operator[](size_type pos) const{
      size_type it = 0;
      for(Node<value_type>* cur = first; cur != nullptr;cur = cur->next)
      {
          if(pos == it)
          {
              return cur->value;
          }
          it++;
      }
  }

  /// @brief Returns a reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Reference to the first element
  reference front(){
      return first->value;
  }

  /// @brief Returns a const reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Const reference to the first element
  const_reference front() const{
      return first->value;
  }

  /// @brief Returns a reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Reference to the last element.
  reference back(){
      return last->value;
  }

  /// @brief Returns a const reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Const Reference to the last element.
  const_reference back() const{
      return last->value;
  }

  /// ITERATORS

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  iterator begin() noexcept{
      iterator a;
      a.cur = first;
      return a;
  }

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  const_iterator begin() const noexcept{
      const_iterator a;
      a.cur = first;
      return a;
  }

  /// @brief Same to begin()
   const_iterator cbegin() const noexcept{
      const_iterator a;
      a.cur = first;
      return a;
  }

  /// @brief Returns an iterator to the element following the last element of
  /// the deque. This element acts as a placeholder; attempting to access it
  /// results in undefined behavior.
  /// @return Iterator to the element following the last element.
  iterator end() noexcept{
      iterator a;
      a.cur = last;
      return a;
  }

  /// @brief Returns an constant iterator to the element following the last
  /// element of the deque. This element acts as a placeholder; attempting to
  /// access it results in undefined behavior.
  /// @return Constant Iterator to the element following the last element.
  const_iterator end() const noexcept{
      const_iterator a;;
      a.cur = last;
      return a;
  }

  /// @brief Same to end()
  const_iterator cend() const noexcept{
      const_iterator a;
      a.cur = last;
      return a;
  }

  /// @brief Returns a reverse iterator to the first element of the reversed
  /// deque. It corresponds to the last element of the non-reversed deque. If
  /// the deque is empty, the returned iterator is equal to rend().
  /// @return Reverse iterator to the first element.

  //Создается временный объект класса deque reverse iterator который  инициализируется итератором на начало контейнера
  //begin() должен возвращать итератор на первый элемент контейнера, а deque reverse iterator - обертка над deque iteraеtor
  reverse_iterator rbegin() noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(begin());
      return a;
  }

  /// @brief Returns a const reverse iterator to the first element of the
  /// reversed deque. It corresponds to the last element of the non-reversed
  /// deque. If the deque is empty, the returned iterator is equal to rend().
  /// @return Const Reverse iterator to the first element.

  //Возвращение константного итератора
  const_reverse_iterator rbegin() const noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(begin());
      return a;
  }

  /// @brief Same to rbegin()
  //Возвращение константного итератора
  const_reverse_iterator crbegin() const noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(begin());
      return a;
  }

  /// @brief Returns a reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Reverse iterator to the element following the last element.
  reverse_iterator rend() noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(end());
      return a;
  }

  /// @brief Returns a const reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Const Reverse iterator to the element following the last element.
  const_reverse_iterator rend() const noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(end());
      return a;
  }

  /// @brief Same to rend()
  const_reverse_iterator crend() const noexcept{
      Deque_reverse_iterator<Deque_iterator<value_type>> a(end());
      return a;
  }

  /// CAPACITY

  /// @brief Checks if the container has no elements
  /// @return true if the container is empty, false otherwise
  //Проверка на пустоту дека
  bool empty() const noexcept{
      return first == nullptr;
  }

  /// @brief Returns the number of elements in the container
  /// @return The number of elements in the container.
  size_type size() const noexcept{
      return _size;
  }

  /// @brief Returns the maximum number of elements the container is able to
  /// hold due to system or library implementation limitations
  /// @return Maximum number of elements.

  //Возвращает максимально возможное число колва элементов, которое может содержать контейнер
  size_type max_size() const noexcept{
      return std::numeric_limits<size_t>::max();
  }

  /// @brief Requests the removal of unused capacity.
  /// It is a non-binding request to reduce the memory usage without changing
  /// the size of the sequence. All iterators and references are invalidated.
  /// Past-the-end iterator is also invalidated.
  //Запрос на освобождение памяти
  void shrink_to_fit(){}

  /// MODIFIERS

  /// @brief Erases all elements from the container.
  /// nvalidates any references, pointers, or iterators referring to contained
  /// elements. Any past-the-end iterators are also invalidated.
  //Удаление всех элементов контейнера
  void clear() noexcept{
      while (size() != 0){
          pop_back();
      }
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  //Вставляем value перед pos. Создаем новый узел
  iterator insert(const_iterator pos, const T& value){
        Node<value_type>* cur = alloc.allocate(1);
        cur->value = value;
        cur->next = pos.cur;
        cur->previous = pos.cur->previous;
        pos.cur->previous->next = cur;
        pos.cur->previous = cur;
        _size++;
        iterator a;
        a.cur = cur;
        return a;
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  //Вставка value перед pos. Создаем новый узел.
  iterator insert(const_iterator pos, T&& value){
      Node<value_type>* cur = alloc.allocate(1);
      cur->value = std::move(value);
      cur->next = pos.cur;
      cur->previous = pos.cur->previous;
      pos.cur->previous->next = cur;
      pos.cur->previous = cur;
      _size++;
      iterator a;
      a.cur = cur;
      return a;
  }

  /// @brief Inserts count copies of the value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param count number of elements to insert
  /// @param value element value to insert
  /// @return Iterator pointing to the first element inserted, or pos if count
  /// == 0.
  //Инсерт нескольких элементов перед pos. 
  iterator insert(const_iterator pos, size_type count, const T& value){
      Node<value_type>* cur_;
      for(size_type i = 0; i < count; i++){
          Node<value_type>* cur = alloc.allocate(1);
          cur->value = value;
          cur->next = pos.cur;
          cur->previous = pos.cur->previous;
          pos.cur->previous->next = cur;
          pos.cur->previous = cur;
          _size++;
          cur_ = cur;
      }
      iterator a;
      a.cur = cur_;
      return a;
  }

  /// @brief Inserts elements from range [first, last) before pos.
  /// @tparam InputIt Input Iterator
  /// @param pos iterator before which the content will be inserted.
  /// @param first,last the range of elements to insert, can't be iterators into
  /// container for which insert is called
  /// @return Iterator pointing to the first element inserted, or pos if first
  /// == last.
  //insert элементов от итераторов first до last перед позицией pos
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last){
      Node<value_type>* cur_;
      for(InputIt i = first; i != last; i++){
          Node<value_type>* cur = alloc.allocate(1);
          cur->value = *i;
          cur->next = pos.cur;
          cur->previous = pos.cur->previous;
          pos.cur->previous->next = cur;
          pos.cur->previous = cur;
          _size++;
          cur_ = cur;
      }
      iterator a;
      a.cur = cur_;
      return a;
  }

  /// @brief Inserts elements from initializer list before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param ilist initializer list to insert the values from
  /// @return Iterator pointing to the first element inserted, or pos if ilist
  /// is empty.
  //insert элементов из ilist перед pos.
  iterator insert(const_iterator pos, std::initializer_list<T> ilist){
      Node<value_type>* cur_;
      for(auto i: ilist){
          Node<value_type>* cur = alloc.allocate(1);
          cur->value = i;
          cur->next = pos.cur;
          cur->previous = pos.cur->previous;
          pos.cur->previous->next = cur;
          pos.cur->previous = cur;
          _size++;
          cur_ = cur;
      }
      iterator a;
      a.cur = cur_;
      return a;
  }

  /// @brief Inserts a new element into the container directly before pos.
  /// @param pos iterator before which the new element will be constructed
  /// @param ...args arguments to forward to the constructor of the element
  /// @return terator pointing to the emplaced element.
  //move rvalue args перед pos.
  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args){
      Node<value_type>* cur = alloc.allocate(1);
      cur->value = std::move(value_type(args...));
      cur->next = pos.cur;
      cur->previous = pos.cur->previous;
      pos.cur->previous->next = cur;
      pos.cur->previous = cur;
      _size++;
      iterator a;
      a.cur = cur;
      return a;
  }

  /// @brief Removes the element at pos.
  /// @param pos iterator to the element to remove
  /// @return Iterator following the last removed element.
  //Удаляем элемент на который указывает итератор pos, проверяем разные случае, когда в деке 1 элемент, когда несколько,
  //также, куда указывает итератор pos, на какой элемент, случаи когда указывает на first или last
  iterator erase(const_iterator pos){
      if(pos.cur == first){
          if(_size == 1){
              clear();
          }
          else {
              Node<value_type> *cur = first;
              first = first->next;
              first->previous = nullptr;
              _size--;
              alloc.deallocate(cur);
          }
          iterator a;
          a.cur = first;
          return a;
      }
      if(pos.cur == last){
          Node<value_type> *cur = last;
          last = last->previous;
          last->next = nullptr;
          iterator a;
          a.cur = last;
          _size--;
          return a;
      }
      Node<value_type> *cur = pos.cur->next;
      pos.cur->previous->next = pos.cur->next;
      pos.cur->next->previous = pos.cur->previous;
      alloc.deallocate(pos.cur);
      _size--;
      iterator a;
      a.cur = cur;
      return a;
  }

  /// @brief Removes the elements in the range [first, last).
  /// @param first,last range of elements to remove
  /// @return Iterator following the last removed element.
  //Удаляем элементы от first до last, проверяем также случаи, куда указывает first и last.
  iterator erase(const_iterator first, const_iterator last){
      if(first == cbegin() && last == cend()){
          clear();
          iterator a;
          a.cur = this->first;
          return a;
      }
      if(first == cbegin()) {
          this->first = last.cur;
          this->first->previous = nullptr;
      }
      const_iterator cur = first;
      while (cur.cur != last.cur){
          erase(cur);
          cur++;
          _size--;
      }
      iterator a;
      a.cur = last.cur;
      return a;
  }

  /// @brief Appends the given element value to the end of the container.
  /// The new element is initialized as a copy of value.
  /// @param value the value of the element to append
  //Реализуем метод push_back, передается value по ссылке, выделяем память под новый узел
  //переставляем указатели.
  void push_back(const T& value){
      Node<value_type> *node = get_allocator().allocate(1);
      node->value = value;
      node->next = nullptr;
      node->previous = last;
      if(empty()){
          first = node;
          last = node;
      }
      else{
          last -> next = node;
          last = node;
      }
      _size++;
  }

  /// @brief Appends the given element value to the end of the container.
  /// Value is moved into the new element.
  /// @param value the value of the element to append
  //Все тоже самое что сверху, только передаем rvalue значение, используем move.
  void push_back(T&& value){
      Node<value_type> *node = get_allocator().allocate(1);
      node->value = std::move(value);
      node->next = nullptr;
      node->previous = last;
      if(empty()){
          first = node;
          last = node;
      }
      else{
          last -> next = node;
          last = node;
      }
      _size++;
  }

  /// @brief Appends a new element to the end of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  //Размещаем Args в конец.
  template <class... Args>
  reference emplace_back(Args&&... args){
      push_back(value_type(args...));
      return last->value;
  }

  /// @brief Removes the last element of the container.
  //Удаляем последний элемент. Проверяем если first == last, то подчищаем память и случай когда first != last.
  void pop_back(){
      if(last == first){
          alloc.deallocate(first);
          last = nullptr;
          first = nullptr;
          _size--;
          return;
      }
      Node<value_type> *del = last;
      last = last->previous;
      last->next = nullptr;
      get_allocator().deallocate(del);
      _size--;
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value the value of the element to prepend
  //Реализуем push_front, передаем ссылку на value, переносим все указатели куда нужно.
  void push_front(const T& value){
      Node<value_type>* node = get_allocator().allocate(1);
      node -> value = value;
      node->next = first;
      node->previous = nullptr;
      if (empty())
      {
          first = node;
          last = node;
          return;
      }
      first->previous = node;
      first = node;
      _size++;
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value moved value of the element to prepend
  //тоже самое, только передаем rvalue value.
  void push_front(T&& value){
      Node<value_type>* node = get_allocator().allocate(1);
      node -> value = std::move(value);
      node->next = first;
      node->previous = nullptr;
      if (empty())
      {
          first = node;
          last = node;
          return;
      }
      first->previous = node;
      first = node;
      _size++;
  }

  /// @brief Inserts a new element to the beginning of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  //Размещаем в начало Args.
  template <class... Args>
  reference emplace_front(Args&&... args){
      push_front(value_type(args...));
      return first->value;
  }

  /// @brief Removes the first element of the container.
  //Удаляем элемент с начала. Проверяем, если контейнер из 1 элемента или нет.
  void pop_front(){
      if(last == first){
          alloc.deallocate(first);
          last = nullptr;
          first = nullptr;
          _size--;
          return;
      }
      Node<value_type> *del = first;
      first = first->next;
      first->previous = nullptr;
      get_allocator().deallocate(del);
      _size--;
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// default-inserted elements are appended
  /// @param count new size of the container
  //Метод изменения размера контейнера, если _size больше count то удаляем элементы.
  void resize(size_type count){
      while (_size > count){
          pop_back();
      }
      while (count > _size){
          push_back(value_type());
      }
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// copies of value are appended.
  /// @param count new size of the container
  /// @param value the value to initialize the new elements with
  //Тоже самое, только передаем еще сам элемент, которым мы будем заполнять недостающие i.
  void resize(size_type count, const value_type& value){
      while (_size > count){
          pop_back();
      }
      while (count > _size){
          push_back(value);
      }
  }

  /// @brief Exchanges the contents of the container with those of other.
  /// Does not invoke any move, copy, or swap operations on individual elements.
  /// All iterators and references remain valid. The past-the-end iterator is
  /// invalidated.
  /// @param other container to exchange the contents with
  void swap(Deque& other){
      Node<value_type>* f = first;
      Node<value_type>* l = last;
      first = other.first;
      last = other.last;
      other.first = f;
      other.last = l;
  }

  /// COMPARISIONS

  /// @brief Checks if the contents of lhs and rhs are equal
  /// @param lhs,rhs deques whose contents to compare

  //Перегрузка оператора ==, для проверки равны ли деки
  template <class U, class Alloc>
  friend bool operator==(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs){
      if(lhs.size() != rhs.size()) return false;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] != lhs[i]) return false;
      }
      return true;
  }

  /// @brief Checks if the contents of lhs and rhs are not equal
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator!=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs){
      if(lhs.size() != rhs.size()) return true;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] != lhs[i]) return true;
      }
      return false;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs){
      if(lhs.size() > rhs.size()) return true;
      else if(lhs.size() < rhs.size()) return false;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] == lhs[i]) continue;
          if(lhs[i] > rhs[i]) return true;
          return false;
      }
      return false;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs){
      if(lhs.size() < rhs.size()) return true;
      else if(lhs.size() < rhs.size()) return false;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] == lhs[i]) continue;
          if(lhs[i] < rhs[i]) return true;
          return false;
      }
      return false;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs){
      if(lhs.size() > rhs.size()) return true;
      else if(lhs.size() < rhs.size()) return false;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] == lhs[i]) continue;
          if(lhs[i] > rhs[i]) return true;
          return false;
      }
      return true;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs){
      if(lhs.size() < rhs.size()) return true;
      else if(lhs.size() < rhs.size()) return false;
      for(size_type i = 0; i < lhs.size(); i++){
          if(rhs[i] == lhs[i]) continue;
          if(lhs[i] < rhs[i]) return true;
          return false;
      }
      return true;
  }

  //Получаем iterator значения val, пробегаемся по деку, ищем наш элемент и возвращаем итератор указывающий на него
  const_iterator get_iter(value_type val){
      size_type it = 0;
      for(Node<value_type>* cur = first; cur != nullptr;cur = cur->next)
      {
          if(cur->value == val)
          {
              const_iterator a;
              a.cur = cur;
              return a;
          }
          it++;
      }
  }

  // operator <=> will be handy
};

/// NON-MEMBER FUNCTIONS

/// @brief  Swaps the contents of lhs and rhs.
/// @param lhs,rhs containers whose contents to swap
template <class T, class Alloc>
void swap(Deque<T, Alloc>& lhs, Deque<T, Alloc>& rhs){
    Node<T> *first = lhs.first;
    Node<T> *last = lhs.last;
    lhs.first = rhs.first;
    lhs.last = rhs.last;
    rhs.first = first;
    rhs.last = last;
}

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param value value to be removed
/// @return The number of erased elements.
//erase элемента value, полученного по ссылке в Deque c
template <class T, class Alloc, typename U>
typename Deque<T, Alloc>::size_type erase(Deque<T, Alloc>& c, const U& value){
    for(size_t i = 0; i < c.size();i++){
        if(c[i] == value){
            c.erase(c.get_iter(value));
            return i;
        }
    }
}

/// @brief Erases all elements that compare equal to value from the container.era
/// @param c container from which to erase
/// @param pred unary predicate which retgit urns ​true if the element should be
/// erased.
/// @return The number of erased elements.
//erase if, отличается от erase тем, что pred хранит в себе true, если элемент должен быть удален
template <class T, class Alloc, class Pred>
typename Deque<T, Alloc>::size_type erase_if(Deque<T, Alloc>& c, Pred pred){
        for(size_t i = 0; i < c.size();i++){
        if(pred[i] == true) {
            c.erase(pred[i]);
            return i;
        }
    }
}
}  // namespace fefu_laboratory_two
