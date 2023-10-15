namespace custom {
template <typename T> class stack {
private:
  struct _Node {
    T data;
    _Node *next;
    _Node(const T &data) : data(data), next(nullptr) {}
    _Node(const T &data, _Node *next) : data(data), next(next) {}
  };
};
} // namespace custom
