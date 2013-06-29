#include <functional>

template<typename T, typename M, template<typename> class C = std::less>
struct method_comparer : std::binary_function<T, T, bool>
{
  explicit method_comparer(M (T::*p)() const) : p_(p) {}

  bool operator ()(T const& lhs, T const& rhs) const
  {
    return C<M>()((lhs.*p_)(), (rhs.*p_)());
  }

  private:
    M (T::*p_)() const;

};

template<typename T, typename M>
method_comparer<T, M> make_method_comparer(M (T::*p)() const)
{
    return method_comparer<T, M>(p);
}

template<template<typename> class C, typename T, typename M>
method_comparer<T, M, C> make_method_comparer2(M (T::*p)() const)
{
    return method_comparer<T, M, C>(p);
}

