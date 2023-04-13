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
public:
	// 
    vector& operator=(const vector& rhs);
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

} // namespace mystl
#endif // !MYTINYSTL_VECTOR_H_