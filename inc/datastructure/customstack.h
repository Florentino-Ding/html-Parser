//
// Created by 丁麓然 on 10/15/23.
//

#ifndef HTML_PARSER_CUSTOMSTACK_H
#define HTML_PARSER_CUSTOMSTACK_H

#include <cstddef>
#include <new>
#include <stdexcept>

namespace custom {
    template<typename T>
    class stack {
    private:
        struct _Node {
            T data;
            _Node *next;

            _Node(const T &data) : data(data), next(nullptr) {}

            _Node(const T &data, _Node *next) : data(data), next(next) {}
        };

        _Node *_top;
        size_t _size;

    public:
        stack() : _top(nullptr), _size(0) {}
        ~stack() {
            while (not empty()) {
                pop();
            }
        }

        void push(const T &data) {
            _Node *newNode;
            try {
                newNode = new _Node(data, _top);
            } catch (std::bad_alloc &e) {
                throw e;
            }
            _top = newNode;
            _size++;
        }

        void pop(void) {
            if (_size == 0) {
                throw std::out_of_range("Stack underflow");
            }
            _Node *temp = _top;
            _top = _top->next;
            delete temp;
            _size--;
        }

        T top(void) {
            if (_size == 0) {
                throw std::out_of_range("Stack underflow");
            }
            return _top->data;
        }

        size_t size(void) {
            return _size;
        }

        bool empty(void) {
            return _size == 0;
        }
    };
} // namespace custom

#endif //HTML_PARSER_CUSTOMSTACK_H
