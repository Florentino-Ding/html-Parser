//
// Created by 丁麓然 on 9/22/23.
//

#ifndef STRING_OPERATION_CUSTOMLIST_H
#define STRING_OPERATION_CUSTOMLIST_H

#include <initializer_list>
#include <iostream>

namespace custom {
template <typename T> class list {
private:
  struct _Node {
    T data;
    _Node *next;

    _Node() : next(nullptr) {}

    _Node(const T d) : next(nullptr) { data = d; }

    _Node(const T d, _Node *next) : next(next) { data = d; }
  };

  _Node *_head;
  _Node *_tail;
  T _seg;
  long _size;

public:
  list() : _head(nullptr), _tail(nullptr), _size(0) {}

  list(T *arr, int length) {
    _head = nullptr;
    _tail = nullptr;
    for (int i = 0; i < length; i++) {
      push_back(arr[i]);
    }
  }

  list(const list<T> &l) {
    _head = nullptr;
    _tail = nullptr;
    _Node *p = l._head;
    while (p != nullptr) {
      push_back(p->data);
      p = p->next;
    }
  }

  list(const std::initializer_list<T> &l) {
    _head = nullptr;
    _tail = nullptr;
    for (auto &i : l) {
      push_back(i);
    }
  }

  ~list() {
    while (not empty()) {
      pop_front();
    }
    _tail = nullptr;
    _size = 0;
  }

  list<T> &operator=(const list<T> &l) {
    /**
     * Assigns the contents of the given list to this list.
     *
     * This operator overloads the assignment operator (=) to assign the
     * contents of the given list (l) to this list. If the given list is the
     * same as this list, no action is taken and the current list is returned.
     * Otherwise, the current list is cleared and then each element from the
     * given list is added to this list using push_back. The elements are added
     * in the same order as they appear in the given list. The assigned list is
     * returned.
     *
     * @param l The list to assign from.
     * @return A reference to this list after assignment.
     *
     * Example:
     * list<int> list1 = {1, 2, 3};
     * list<int> list2;
     * list2 = list1;
     * // list2 now contains {1, 2, 3}
     */
    if (this == &l) {
      return *this;
    }
    clear();
    _Node *p = l._head;
    while (p != nullptr) {
      push_back(p->data);
      p = p->next;
    }
    return *this;
  }

  T &operator[](int index) {
    /**
     * Accesses the element at the specified index.
     *
     * This function allows accessing the element at the specified index in the
     * list using the subscript operator ([]). If the index is out of range
     * (less than 0 or greater than or equal to the size of the list), an
     * std::out_of_range exception is thrown. Otherwise, the function traverses
     * the list to find the node at the specified index and returns its data.
     *
     * @param index The index of the element to access.
     * @return The data of the element at the specified index.
     * @throws std::out_of_range if the index is out of range.
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4, 5};
     * int element = myList[2];
     * // element is now 3
     */
    if (index < 0 or index >= size()) {
      throw std::out_of_range("Index out of range");
    }
    _Node *p = _head;
    for (int i = 0; i < index; i++) {
      p = p->next;
    }
    return p->data;
  }

  T operator[](int index) const {
    /**
     * Accesses the element at the specified index.
     *
     * This function allows accessing the element at the specified
     * index in the list using the subscript operator ([]). If the
     * index is out of range (less than 0 or greater than or equal to
     * the size of the list), an std::out_of_range exception is
     * thrown. Otherwise, the function traverses the list to find the
     * node at the specified index and returns its data.
     *
     * @param index The index of the element to access.
     * @return The data of the element at the specified index.
     * @throws std::out_of_range if the index is out of range.
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4, 5};
     * int element = myList[2];
     * // element is now 3
     */
    if (index < 0 or index >= size()) {
      throw std::out_of_range("Index out of range");
    }
    _Node *p = _head;
    for (int i = 0; i < index; i++) {
      p = p->next;
    }
    return p->data;
  }

  friend list<T> operator+(const list<T> &l1, const list<T> &l2) {
    /**
     * Concatenates two lists and returns a new list.
     *
     * This friend function overloads the addition operator (+) to concatenate
     * two lists (l1 and l2) and return a new list. It creates a temporary list
     * (temp) initialized with the contents of l1. Then, it uses the compound
     * assignment operator (+=) to append the elements of l2 to temp. Finally,
     * it returns the resulting concatenated list (temp).
     *
     * @param l1 The first list to concatenate.
     * @param l2 The second list to concatenate.
     * @return A new list that is the concatenation of l1 and l2.
     *
     * Example:
     * list<int> list1 = {1, 2, 3};
     * list<int> list2 = {4, 5, 6};
     * list<int> result = list1 + list2;
     * // result now contains {1, 2, 3, 4, 5, 6}
     */

    list<T> temp(l1);
    temp += l2;
    return temp;
  }

  void operator+=(const list<T> &l) {
    /**
     * Traverses a linked list and performs an action on each node.
     *
     * This code snippet traverses a linked list starting from the head node
     * (l._head) and performs an action on each node. Inside the loop, it calls
     * the push_back function to add the data of the current node (p->data) to
     * the list. Then, it moves to the next node by updating the pointer (p) to
     * point to the next node (p->next).
     *
     */
    _Node *p = l._head;
    while (p != nullptr) {
      push_back(p->data);
      p = p->next;
    }
  }

  bool operator==(const list<T> &l) const {
    /**
     * Checks if the current list is equal to the given list.
     *
     * This member function overloads the equality operator (==) to compare the
     * current list with the given list (l). It first checks if the sizes of the
     * two lists are different. If so, it returns false. Then, it traverses both
     * lists simultaneously, comparing the data of each node. If any data pair
     * is not equal, it returns false. If all data pairs are equal and the
     * traversal reaches the end of both lists, it returns true.
     *
     * @param l The list to compare with.
     * @return True if the lists are equal, false otherwise.
     *
     * Example:
     * list<int> list1 = {1, 2, 3};
     * list<int> list2 = {1, 2, 3};
     * bool result = list1 == list2;
     * // result is true
     */

    if (size() != l.size()) {
      return false;
    }
    _Node *p = _head;
    _Node *q = l._head;
    while (p != nullptr) {
      if (p->data != q->data) {
        return false;
      }
      p = p->next;
      q = q->next;
    }
    return true;
  }

  bool operator!=(const list<T> &l) const { return not(*this == l); }

  friend std::wostream &operator<<(std::wostream &os, const list<T> &l) {
    _Node *p = l._head;
    while (p != nullptr) {
      os << p->data << l._seg;
      p = p->next;
    }
    return os;
  }

  friend std::wistream &operator>>(std::wistream &is, list<T> &l) {
    wchar_t c;
    while (is.get(c)) {
      l.push_back(c);
    }
    return is;
  }

  operator T const() const {
    T result;
    _Node *p = _head;
    while (p != nullptr) {
      result += p->data;
      result += _seg;
      p = p->next;
    }
    return result;
  }

  int size() const { return _size; }

  bool empty() const { return _size == 0; }

  void clear() {
    while (not empty()) {
      pop_front();
    }
    _tail = nullptr;
    _size = 0;
  }

  T front() const {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return _head->data;
  }

  T back() const {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return _tail->data;
  }

  void push_back(const T &data) {
    /**
     * Appends a new node with the given data to the end of the list.
     *
     * This code snippet appends a new node with the given data to the end of
     * the list. It first attempts to allocate memory for the new node using the
     * `new` operator. If the memory allocation fails, it throws a
     * `std::system_error` exception with a descriptive error message. If the
     * list is empty (head is nullptr), the new node becomes both the head and
     * tail of the list. Otherwise, the new node is linked to the current tail
     * node, and the tail pointer is updated to the new node. The size of the
     * list is incremented.
     *
     */
    _Node *new_node = nullptr;
    try {
      new_node = new _Node(data);
    } catch (std::exception &e) {
      throw std::system_error(std::error_code(1, std::generic_category()),
                              "Memory allocation failed");
    }
    if (_head == nullptr) {
      _head = new_node;
      _tail = new_node;
    } else {
      _tail->next = new_node;
      _tail = new_node;
    }
    _size++;
  }

  void push_front(const T &data) {
    /**
     * Inserts a new node with the given data at the front of the list.
     *
     * This function inserts a new node with the given data at the front of the
     * list. It first attempts to allocate memory for the new node using the
     * `new` operator. If the memory allocation fails, it throws a
     * `std::system_error` exception with a descriptive error message. If the
     * list is empty (head is nullptr), the new node becomes both the head and
     * tail of the list. Otherwise, the new node is linked to the current head
     * node, and the head pointer is updated to the new node. The size of the
     * list is incremented.
     *
     * @param data The data to be inserted at the front of the list.
     * @throws std::system_error if memory allocation fails.
     *
     * Example:
     * list<int> myList = {2, 3, 4};
     * myList.push_front(1);
     * // myList now contains {1, 2, 3, 4}
     */

    _Node *new_node = nullptr;
    try {
      new_node = new _Node(data);
    } catch (std::exception &e) {
      throw std::system_error(std::error_code(1, std::generic_category()),
                              "Memory allocation failed");
    }
    if (_head == nullptr) {
      _head = new_node;
      _tail = new_node;
    } else {
      new_node->next = _head;
      _head = new_node;
    }
    _size++;
  }

  void pop_back() {
    /**
     * Removes the last node from the list.
     *
     * This function removes the last node from the list.
     * If the list is empty, it throws an `std::out_of_range` exception with the
     * message "List is empty". If there is only one node in the list, it is
     * deleted and both the head and tail pointers are set to nullptr.
     * Otherwise, it traverses the list to find the second-to-last node (p->next
     * == _tail), deletes the last node, updates the tail pointer to the
     * second-to-last node, and sets its next pointer to nullptr. The size of
     * the list is decremented.
     *
     * @throws std::out_of_range if the list is empty.
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4};
     * myList.pop_back();
     * // myList now contains {1, 2, 3}
     */
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    if (_head == _tail) {
      delete _head;
      _head = nullptr;
      _tail = nullptr;
    } else {
      _Node *p = _head;
      while (p->next != _tail) {
        p = p->next;
      }
      delete _tail;
      _tail = p;
      _tail->next = nullptr;
    }
    _size--;
  }

  void pop_front() {
    /**
     * Removes the first node from the list.
     *
     * This function removes the first node from the list.
     * If the list is empty, it throws an `std::out_of_range` exception with the
     * message "List is empty". If there is only one node in the list, it is
     * deleted and both the head and tail pointers are set to nullptr.
     * Otherwise, it updates the head pointer to point to the second node,
     * deletes the first node, and updates the size of the list.
     *
     * @throws std::out_of_range if the list is empty.
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4};
     * myList.pop_front();
     * // myList now contains {2, 3, 4}
     */
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    if (_head == _tail) {
      delete _head;
      _head = nullptr;
      _tail = nullptr;
    } else {
      _Node *p = _head;
      _head = _head->next;
      delete p;
    }
    _size--;
  }

  void insert(int index, const T &data) {
    /**
     * Inserts an element at the specified index in the list.
     *
     * This function inserts the given element (data) at the specified index in
     * the list. If the index is less than 0 or greater than the size of the
     * list, it throws an `std::out_of_range` exception with the message "Index
     * out of range". If the index is 0, the element is inserted at the front of
     * the list using `push_front`. If the index is equal to the size of the
     * list, the element is inserted at the end of the list using `push_back`.
     * Otherwise, it creates a new node with the given data, inserts it at the
     * specified index by updating the appropriate pointers, and increments the
     * size of the list.
     *
     * @param index The index at which to insert the element.
     * @param data The element to be inserted.
     * @throws std::out_of_range if the index is out of range.
     * @throws std::system_error if memory allocation fails.
     *
     * Example:
     * list<int> myList = {1, 2, 3};
     * myList.insert(1, 4);
     * // myList now contains {1, 4, 2, 3}
     */
    if (index < 0 or index > size()) {
      throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
      push_front(data);
    } else if (index == size()) {
      push_back(data);
    } else {
      _Node *new_node = nullptr;
      try {
        new_node = new _Node(data);
      } catch (std::exception &e) {
        throw std::system_error(std::error_code(1, std::generic_category()),
                                "Memory allocation failed");
      }
      _Node *p = _head;
      for (int i = 0; i < index - 1; i++) {
        p = p->next;
      }
      new_node->next = p->next;
      p->next = new_node;
      _size++;
    }
  }

  void insert(int index, const list<T> &l) {
    if (index < 0 or index > size()) {
      throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
      *this = l + *this;
    } else if (index == size()) {
      *this = *this + l;
    } else {
      _Node *p = _head;
      for (int i = 0; i < index - 1; i++) {
        p = p->next;
      }
      _Node *q = p->next;
      p->next = l._head;
      l._tail->next = q;
      _size += l._size;
    }
  }

  void set_segment(const T &seg) { _seg = seg; }

  void highlight_show(const list<int> &to_highlight,
                      const char mode = 0) const {
    /**
     * Displays the elements of the list with optional highlighting.
     *
     * This function displays the elements of the list, with the option to
     * highlight specific elements based on the provided list (to_highlight).
     * The highlighting mode can be specified using the 'mode' parameter
     * (default is 0). In mode 0, the elements are displayed as-is, but the
     * highlighted elements are displayed with a background color. In mode 1,
     * the elements are displayed with their corresponding indices, and the
     * highlighted elements are displayed with both the index and a background
     * color. The function iterates through the list, checking if each element's
     * index exists in the to_highlight list. If an element is highlighted, it
     * is displayed with the appropriate formatting. The elements are separated
     * by spaces.
     *
     * @param to_highlight The list of indices to highlight.
     * @param mode The highlighting mode (0 or 1).
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4, 5};
     * list<int> highlightIndices = {1, 3};
     * myList.highlight_show(highlightIndices, 1);
     * // Displays: "0 1 2 3 4 5" with elements 1 and 3 highlighted.
     */

    _Node *p = _head;
    int index = 0;
    switch (mode) {
    case 0:
      while (p != nullptr) {
        if (to_highlight.exist(index)) {
          std::wcout << L"\033[7m" << p->data << L"\033[0m";
        } else {
          std::wcout << p->data;
        }
        std::wcout << L" ";
        p = p->next;
        index++;
      }
    case 1:
      while (p != nullptr) {
        if (to_highlight.exist(index)) {
          std::wcout << L"\033[7m" << L"\033[2m" << index << L"\033[0m"
                     << L"\033[7m" << p->data << L"\033[0m";
        } else {
          std::wcout << L"\033[2m" << index << L"\033[0m" << p->data;
        }
        std::wcout << L" ";
        p = p->next;
        index++;
      }
    }
  }

  void show(const char mode = 0) const {
    _Node *p = _head;
    switch (mode) {
    case 0:
      std::wcout << *this;
      break;
    case 1:
      int index = 0;
      while (p != nullptr) {
        std::wcout << L"\033[2m" << index << L"\033[0m" << p->data << L"\t";
        p = p->next;
        index++;
      }
    }
  }

  void remove(int index) {
    /**
     * Removes the element at the specified index from the list.
     *
     * This function removes the element at the specified index in the list.
     * If the index is less than 0 or greater than or equal to the size of the
     * list, it throws an `std::out_of_range` exception with the message "Index
     * out of range". If the index is 0, the element is removed from the front
     * of the list using `pop_front`. If the index is equal to the size of the
     * list minus 1, the element is removed from the back of the list using
     * `pop_back`. Otherwise, it traverses the list to find the node before the
     * node to be removed, updates the pointers to bypass the node to be
     * removed, deletes the node, and decrements the size of the list.
     *
     * @param index The index of the element to remove.
     * @throws std::out_of_range if the index is out of range.
     *
     * Example:
     * list<int> myList = {1, 2, 3, 4, 5};
     * myList.remove(2);
     * // myList now contains {1, 2, 4, 5}
     */

    if (index < 0 or index >= size()) {
      throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
      pop_front();
    } else if (index == size() - 1) {
      pop_back();
    } else {
      _Node *p = _head;
      for (int i = 0; i < index - 1; i++) {
        p = p->next;
      }
      _Node *q = p->next;
      p->next = q->next;
      delete q;
      _size--;
    }
  }

  void remove(const T &data) {
    bool idx = exist(data);
    if (not idx) {
      throw std::out_of_range("Data not found");
    }
    for (int i = 0; i < _size; i++) {
      if ((*this)[i] == data) {
        remove(i);
        i--;
      }
    }
  }

  void remove(int index, int length) {
    if (index < 0 or index >= size()) {
      throw std::out_of_range("Index out of range");
    }
    while (_size > index and 0 < length) {
      remove(index);
      length--;
    }
  }

  bool exist(const T data) const {
    _Node *p = _head;
    int index = 0;
    while (p != nullptr) {
      if (p->data == data) {
        return true;
      }
      p = p->next;
      index++;
    }
    return false;
  }

  list<int> find(const T &data) const {
    _Node *p = _head;
    list<int> result;
    int index = 0;
    while (p != nullptr) {
      if (p->data == data) {
        result.push_back(index);
      }
      p = p->next;
      index++;
    }
    return result;
  }

  void replace(int index, const T &data) { this->operator[](index) = data; }

  void reverse() {
    /**
     * CustomList class for managing a linked list of elements.
     *
     * This class provides functionality for managing a linked list of elements.
     * It supports operations such as adding elements, removing elements,
     * accessing elements by index, and counting occurrences of elements. The
     * class also includes functions for reversing the list and checking if it
     * is empty.
     *
     * Example:
     * CustomList<int> myList;
     * myList.push_back(1);
     * myList.push_back(2);
     * myList.push_back(3);
     * myList.reverse();
     * // myList now contains {3, 2, 1}
     * int count = myList.count(2);
     * // count is 1
     */
    if (empty()) {
      return;
    }
    _Node *p = _head;
    _Node *q = _head->next;
    _Node *r = nullptr;
    while (q != nullptr) {
      p->next = r;
      r = p;
      p = q;
      q = q->next;
    }
    p->next = r;
    _tail = _head;
    _head = p;
  }

  int count(const T &data) const {
    /**
     * Counts the occurrences of a specific element in the list.
     *
     * This function counts the number of occurrences of the given element
     * (data) in the list. It initializes a counter (cnt) to 0 and starts
     * traversing the list from the head node (_head). For each node, it checks
     * if the data of the node is equal to the given element. If there is a
     * match, it increments the counter. After traversing the entire list, it
     * returns the final count.
     *
     * @param data The element to count occurrences of.
     * @return The number of occurrences of the element in the list.
     *
     * Example:
     * CustomList<int> myList = {1, 2, 2, 3, 2, 4};
     * int count = myList.count(2);
     * // count is 3
     */
    int cnt = 0;
    _Node *p = _head;
    while (p != nullptr) {
      if (p->data == data) {
        cnt++;
      }
      p = p->next;
    }
    return cnt;
  }
};

} // namespace custom

#endif // STRING_OPERATION_CUSTOMLIST_H
