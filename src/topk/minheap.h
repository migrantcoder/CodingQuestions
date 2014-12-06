// vim: set ts=4 sw=4 tw=132 expandtab
// Copyright 2014 Migrant Coder 

#ifndef TOPK_MINHEAP_H
#define TOPK_MINHEAP_H

#include <queue>

namespace topk {

/// A minimum binary heap backed by a \c std::deque.
template <typename T>
class minheap {
public:
    minheap() = default;
    ~minheap() = default;
    minheap(const minheap&) = default;
    minheap(minheap&& o) : _heap(std::move(o._heap)) {}
    minheap& operator=(const minheap&) = default;
    minheap& operator=(minheap&& o) { _heap = std::move(o._heap); }

    bool empty() const { return _heap.empty(); }
    void pop();
    void push(const T&);
    void push(T&&);
    size_t size() const { return _heap.size(); }
    const T& top() const;

    /// \exception \c std::logic_error if the object invariants don't hold.
    void validate_properties() const;

private:
    void bubble_last();                          /// Bubble up the last element.
    void sift_first();                           /// Sift down the first element.

    // Note that we don't need to handle overflow, as std::vector::push_back will raise an exception first.
    size_t left_child_index(const size_t i) const { return (2 * i) + 1; }
    size_t parent_index(const size_t i) const { return (i - 1) / 2; }
    // Note that we don't need to handle overflow, as std::vector::push_back will raise an exception first.
    size_t right_child_index(const size_t i) const { return (2 * i) + 2; }

    void validate_order(size_t index) const;     /// \exception \c std::logic_error if the object order property doesn't hold.

    std::deque<T> _heap;
};

template <typename T>
void minheap<T>::pop()
{
    assert(!_heap.empty());

    if (_heap.size() == 1) {
        _heap.pop_front();
        return;
    }

    // Replace the first element with the last, breaking the ordering, but preserving the shape property. Sift the first element
    // down to restore the heap ordering property.
    std::swap(_heap[0], _heap[_heap.size() - 1]);
    _heap.pop_back();
    sift_first();
}

template <typename T>
void minheap<T>::sift_first()
{
    assert(!_heap.empty());

    size_t i = 0;
    while (true) {
        auto& element = _heap[i];
        const size_t l = left_child_index(i);
        const size_t r = right_child_index(i);

        // Base cases.
        if (l >= _heap.size()) {
            // The end of the heap has been reached.
            break;
        }
        auto& leftchild = _heap[l];
        if (r >= _heap.size()) {
            // The left child is the last element in the heap.
            if (leftchild < element) {
                std::swap(leftchild, element);
            }
            break;
        }
        auto& rightchild = _heap[r];
        if (element < leftchild && element < rightchild) {
            // The heap invariant holds again.
            break;
        }

        // Sift left or right depending on child priority.
        if (leftchild < rightchild) {
            std::swap(leftchild, element);
            i = l;
        } else {
            std::swap(rightchild, element);
            i = r;
        }
    }
}

template <typename T>
void minheap<T>::push(const T& e)
{
    // Append to the heap and bubble up.
    _heap.push_back(e);
    bubble_last();
}

template <typename T>
void minheap<T>::push(T&& e)
{
    // Append to the heap and bubble up.
    _heap.emplace_back(e);
    bubble_last();
}

template <typename T>
void minheap<T>::bubble_last()
{
    assert(!_heap.empty());

    size_t i = _heap.size() - 1;
    if (i < 1) {
        return;
    }
    size_t p = parent_index(i);
    while (i > 0 && _heap[i] < _heap[p]) {
        std::swap(_heap[i], _heap[p]);
        i = p;
        p = parent_index(i);
    }
}


template <typename T>
const T& minheap<T>::top() const
{
    return _heap.front();
}

template <typename T>
void minheap<T>::validate_properties() const
{
    validate_order(0);
}

template <typename T>
void minheap<T>::validate_order(const size_t i) const
{
    const size_t l = left_child_index(i);
    const size_t r = right_child_index(i);
    if (l >= _heap.size()) {
        return;
    }
    if (_heap[i] > _heap[l]) {
        throw std::logic_error("heap order property does not hold");
    }
    validate_order(l);
    if (r >= _heap.size()) {
        return;
    }
    if (_heap[i] > _heap[r]) {
        throw std::logic_error("heap order property does not hold");
    }
    validate_order(r);
}

} // namespace topk

#endif // TOPK_MINHEAP_H
