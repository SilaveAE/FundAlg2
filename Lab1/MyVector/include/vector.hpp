#pragma once
#include <cmath>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <utility>

namespace my_cont {
    template <typename T>
    class Container {
    public:
        Container() = default;
        virtual ~Container() = 0;
        Container(const Container&other) = default;
        virtual Container<T>& operator=(const Container<T>&other) = default;

        virtual bool operator==(const Container& other) = 0;
        virtual bool operator!=(const Container& other) = 0;

        virtual T& operator[] (size_t index) = 0;
        virtual const T& operator[] (size_t index) const = 0;

        virtual size_t size() const = 0;
        virtual size_t max_size() const = 0;
        virtual size_t capacity() const = 0;
        virtual bool empty() const = 0;
    };

    template <typename T>
    class Vector: public Container<T> {
    private:
        T* data_ = nullptr;
        size_t size_ = 0;
        size_t capacity_ = 0;
    public:
        explicit Vector(size_t capacity = 8);
        ~Vector() override;
        Vector(const Vector<T> &other);
        Vector(Vector<T> &&other) noexcept;
        Vector(const std::initializer_list<T> &other);
        Vector<T>& operator=(std::initializer_list<T> list);
        Vector<T>& operator=(const Vector<T> &other);
        Vector<T>& operator=(Vector<T> &&other) noexcept;
        Container<T>& operator=(const Container<T> &other) override;

        T& at(size_t index);
        const T& at(size_t index) const;
        T& operator[](size_t index) override;
        const T& operator[](size_t index) const override;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        T* data();
        const T* data() const;

        void clear();
        void push_back(const T& val);
        void push_back(T&& val);
        void pop_back();

        T* insert(const T* pos, const T& val);
        T* insert(const T* pos, T&& val);
        T* insert(const T* pos, size_t count, const T& val);

        T* erase(T* pos);
        T* erase(const T* pos);
        T* erase(T* first, T* last);
        T* erase(const T* first, const T* last);

        void resize(size_t count);
        void resize(size_t count, const T& val);

        void swap(Vector<T> &other);

        size_t size() const override;
        size_t capacity() const override;
        size_t max_size() const override;
        bool empty() const override;
        void reserve (size_t new_capacity);
        void shrink_to_fit();

        T* begin();
        const T* begin() const;
        const T* cbegin() const noexcept;
        T* end();
        const T* end() const;
        const T* cend() const noexcept;

        bool operator==(const Vector<T> &other);
        bool operator!=(const Vector<T> &other);
        bool operator>(const Vector<T> & other);
        bool operator>=(const Vector<T> & other);
        bool operator<(const Vector<T> & other);
        bool operator<=(const Vector<T> & other);
        std::strong_ordering operator<=>(const Vector<T> &other);
        bool operator==(const Container<T> &other) override;
        bool operator!=(const Container<T> &other) override;
    };

template<typename T>
Vector<T>::Vector(const Vector<T> &other) : Vector(other.capacity_) {
    size_ = other.size_;
    std::copy(other.data_, other.data_ + other.size_, data_);
}

template<typename T>
Vector<T>::~Vector() {
    delete []data_;
    size_ = 0;
    capacity_ = 0;
}

template<typename T>
Vector<T>::Vector(Vector<T> &&other) noexcept :
data_{other.data_},
capacity_{other.capacity_},
size_{other.size_} {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T> &other) : Vector(int(pow(2, ceil(log2(other.size()))))) {
    std::copy(other.data_, other.data_ + other.size_, other.data_);
    size_ = other.size_;
}

template<typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> list) {
    clear();
    reserve(list.size());
    std::copy(list.begin(), list.end(), begin());
    size_ = list.size();
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &other) {
    if(this != &other) {
        resize(other.capacity_);
        std::copy(other.data_, other.data_ + other.size_, other.data_);
        size_ = other.size_;
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> &&other) noexcept {
    if(this != &other) {
        reserve(other.capacity_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        other.clear();
    }
    return *this;
}

template<typename T>
Container<T>& Vector<T>::operator=(const Container<T> &other) {
    if(this != &other) {
        resize(other.size());
        for (size_t i = 0; i < other.size(); ++i) {
            data_[i] = other[i];
        }
        size_ = other.size();
    }
    return *this;
}

template<typename T>
T& Vector<T>::at(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("at");
    } else {
        return data_[index];
    }
}

template<typename T>
const T& Vector<T>::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("at");
    } else {
        return data_[index];
    }
}

template<typename T>
T& Vector<T>::operator[](size_t index) {
    return data_[index];
}

template<typename T>
const T& Vector<T>::operator[](size_t index) const {
    return data_[index];
}

template<typename T>
T& Vector<T>::front() {
    return *data_;
}

template<typename T>
const T& Vector<T>::front() const {
    return *data_;
}

template<typename T>
T& Vector<T>::back() {
    return *(data_ + size_ - 1);
}

template<typename T>
const T& Vector<T>::back() const {
    return *(data_ + size_ - 1);
}

template<typename T>
T* Vector<T>::data() {
    return data_;
}

template<typename T>
const T* Vector<T>::data() const {
    return data_;
}

template<typename T>
void Vector<T>::clear() {
    size_ = 0;
}

template<typename T>
void Vector<T>::push_back(const T& val) {
    insert(data_ + size_, val);
}

template<typename T>
void Vector<T>::push_back(T&& val) {
    insert(data_ + size_, val);
}

template<typename T>
void Vector<T>::pop_back() {
    erase(data_ + size_ - 1);
}

template<typename T>
T* Vector<T>::insert(const T* pos, const T& val) {
    if(pos < data_ || pos > data_ + size_) {
        throw std::out_of_range("Pos is not in vector");
    }
    size_t ind = pos - data_;
    if(size_ == capacity_) {
        size_t stemp = size_;
        resize(capacity_ * 2);
        size_ = stemp;
    }
    T* temp = data_ + ind;
    for (auto it = data_ + size_; it != temp; --it) {
        *it = *(it - 1);
    }
    *temp = val;
    ++size_;
    return temp;
}

template<typename T>
T* Vector<T>::insert(const T* pos, T&& val) {
    if(pos < data_ || pos > data_ + size_) {
        throw std::out_of_range("Pos is not in vector");
    }
    size_t ind = pos - data_;
    if(size_ == capacity_) {
        size_t stemp = size_;
        resize(capacity_ * 2);
        size_ = stemp;
    }
    T* temp = data_ + ind;
    for (auto it = data_ + size_; it != temp; --it) {
        *it = *(it - 1);
    }
    *temp = val;
    ++size_;
    return temp;
}

template<typename T>
T* Vector<T>::insert(const T* pos, size_t count, const T& value) {
    if (pos < data_ || pos > data_ + size_) {
        throw std::out_of_range("Insert position is out of range");
    }
    size_t ind = pos - data_;
    if (size_ + count >= capacity_) {
        size_t ss = size_;
        size_t new_size = int(pow(2, ceil(log2(size_ + count))));
        resize(new_size);
        size_ = ss;
    }
    T * tmp = data_ + ind;
    for (auto it = size_ + data_ + count; it >= tmp + count; --it) {
        *it = *(it - count);
    }
    for(size_t i = 0; i < count; ++i) {
        data_[i + ind] = value;
    }
    size_ += count;
    return tmp;
}

template<typename T>
T* Vector<T>::erase(T* pos) {
    if(pos < data_ || pos > data_ + size_) {
        throw std::out_of_range("Pos is not in vector");
    }
    if(pos == data_ + size_) {
        return pos;
    }
    auto ind = pos + 1;
    for(; ind != data_ + size_; ++ind) {
        *(ind - 1) = *ind;
    }
    --size_;
    return ind - 1;
}

template<typename T>
T* Vector<T>::erase(const T *pos) {
    if(pos > end() or pos < begin()) {
        throw std::out_of_range("Erase position is out of range");
    }
    if(pos == end()) {
        return pos;
    }
    auto it = pos + 1;
    for(; it != end(); ++it) {
        *(it - 1) = *it;
    }
    --size_;
    return it - 1;
}

template<typename T>
T* Vector<T>::erase(T* first, T* last) {
    if(first < data_ || last > data_ + size_ || first > last) {
        throw std::out_of_range("Pos in not in vector");
    }
    if(first==last) {
        return last;
    }
    size_t count = last - first;
    for(size_t i = 0; i < count; ++i) {
        erase(first);
    }
    if(size_ == 0) {
        return data_ + size_;
    }
    return first;
}

template<typename T>
T* Vector<T>::erase(const T* first, const T* last) {
    if(first < data_ || last > data_ + size_ || first > last) {
        throw std::out_of_range("Pos in not in vector");
    }
    if(first==last) {
        return last;
    }
    size_t count = last - first;
    for(size_t i = 0; i < count; ++i) {
        erase(first);
    }
    if(size_ == 0) {
        return data_ + size_;
    }
    return first;
}

template<typename T>
void Vector<T>::resize(size_t count) {
    if(count > capacity_) {
        size_t new_cap = std::max(count, capacity_ * 2);
        T* new_data = new T[new_cap];
        std::copy_n(data_, size_, new_data);
        delete []data_;
        data_ = new_data;
        capacity_ = new_cap;
    }
    size_ = count;
}

template<typename T>
void Vector<T>::resize(size_t count, const T& val){
    if(count > capacity_) {
        size_t new_cap = std::max(count, capacity_ * 2);
        T* new_data = new T[new_cap];
        std::copy_n(data_, size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }
    for(size_t i = size_; i < count; ++i) {
        data_[i] = val;
    }
    size_ = count;
}

template<typename T>
void Vector<T>::swap(Vector<T> &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
size_t Vector<T>::size() const {
    return size_;
}

template<typename T>
size_t Vector<T>::capacity() const {
    return capacity_;
}

template<typename T>
size_t Vector<T>::max_size() const {
    return std::numeric_limits<size_t>::max() / sizeof(T);
}

template<typename T>
bool Vector<T>::empty() const {
    return (size_ == 0);
}

template<typename T>
void Vector<T>::reserve(size_t new_capacity) {
    if(capacity_ < new_capacity) {
        size_t new_capacity_t = std::max(new_capacity, capacity_ * 2);
        T* new_data = new T[new_capacity_t];
        std::copy_n(data_, size_, new_data);
        delete []data_;
        data_ = new_data;
        capacity_ = new_capacity_t;
    }
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    if(size_ < capacity_) {
        T* new_data = new T[size_];
        std::copy_n(data_, size_, new_data);
        delete []data_;
        data_ = new_data;
        capacity_ = size_;
    }
}

template<typename T>
T* Vector<T>::begin() {
    return data_;
}

template<typename T>
const T* Vector<T>::begin() const {
    return data_;
}

template<typename T>
const T* Vector<T>::cbegin() const noexcept {
    return data_;
}

template<typename T>
T* Vector<T>::end() {
    return data_ + size_;
}

template<typename T>
const T* Vector<T>::end() const {
    return data_ + size_;
}

template<typename T>
const T* Vector<T>::cend() const noexcept {
    return data_ + size_;
}

template<typename T>
bool Vector<T>::operator==(const Vector<T> &other) {
    return (*this <=> &other) == std::strong_ordering::equal;
}

template<typename T>
bool Vector<T>::operator!=(const Vector<T> &other) {
    return (*this <=> &other) == std::strong_ordering::equal;
}

template<typename T>
bool Vector<T>::operator>(const Vector<T> &other){
    retunr (*this <=> &other) == std::strong_ordering::greater;
}

template<typename T>
bool Vector<T>::operator>=(const Vector<T> &other) {
    return (*this <=> &other) == std::strong_ordering::greater or (*this <=> &other) == std::strong_ordering::equal;
}

template<typename T>
bool Vector<T>::operator<(const Vector<T> &other) {
    return (*this <=> &other) == std::strong_ordering::less;
}

template<typename T>
bool Vector<T>::operator<=(const Vector<T> &other) {
    return (*this <=> &other) == std::strong_ordering::less or (*this <=> &other) == std::strong_ordering::equal;
}

template<typename T>
bool Vector<T>::operator==(const Container<T> &other) {
    if(this->size() != other.size()) return false;
    for(size_t i = 0; i < this->size(); ++i) {
        if(data_[i] != other[i]) return false;
    }
    return true;
}

template<typename T>
bool Vector<T>::operator!=(const Container<T> &other) {
    return !(*this == other);
}

template<typename T>
std::strong_ordering Vector<T>::operator<=>(const Vector<T> &other) {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
}

} //my_cont
