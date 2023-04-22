#pragma once
#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

#include <initializer_list>
#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
#include "algo.h"

namespace mystl
{
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

// vector template class
template <class T>
class vector
{
	static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned in mystl");

private:
    iterator begin_;  // head of current using space
    iterator end_;    // tail of current using space 
    iterator cap_;    // tail of current store space


public:
	// constructor and destructor
    vector() noexcept
    {
        try_init();
    }

    explicit vector(size_type n)
    {
        fill_init(n, value_type());
    }

    vector(size_type n, const value_type& value)
    {
        fill_init(n, value);
    }


    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs) noexcept;

    vector& operator=(std::initializer_list<value_type> ilist)
    {
        vector tmp(ilist.begin(), ilist.end());
        swap(tmp);
        return *this;
    }

    ~vector()
    {
        destroy_and_recover(begin_, end_, cap_ - begin_);
        begin_ = end_ = cap_ = nullptr;
    }



private:
    // util functions
    
    // initialize / destroy
    void      try_init() noexcept;

    void      init_space(size_type size, size_type cap);

    void      fill_init(size_type n, const value_type& value);
    template <class Iter>
    void      range_init(Iter first, Iter last);

    void      destroy_and_recover(iterator first, iterator last, size_type n);
};


// overloading operator =
template <class T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
	if (this != &rhs)
  {
    const auto len = rhs.size();
    if (len > capacity())
    { 
      vector tmp(rhs.begin(), rhs.end());
      swap(tmp);
    }
    else if (size() >= len)
    {
      auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
      data_allocator::destroy(i, end_);
      end_ = begin_ + len;
    }
    else
    { 
      mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
      mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
      cap_ = end_ = begin_ + len;
    }
  }
  return *this;
}

// overloading operator 
template <class T>
vector<T>& vector<T>::operator=(vector&& rhs) noexcept
{
    destroy_and_recover(begin_, end_, cap_ - begin_);
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    cap_ = rhs.cap_;
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
    return *this;
}

// overloading compare operator
template <class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs)
{
    return lhs.size() == rhs.size() &&
        mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs)
{
    return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs)
{
    return rhs < lhs;
}

template <class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs < rhs);
}

} // namespace mystl
#endif // !MYTINYSTL_VECTOR_H_