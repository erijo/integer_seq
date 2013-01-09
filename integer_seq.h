#ifndef REDI_INTEGER_SEQ_H
#define REDI_INTEGER_SEQ_H

// Copyright Jonathan Wakely 2012-2013
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <type_traits>

namespace redi
{
  /// A type that represents a parameter pack of zero or more integers.
  template<typename T, T... I>
    struct integer_seq
    {
      static_assert( std::is_integral<T>::value, "Integral type" );

      using type = T;

      static constexpr T size = sizeof...(I);

      /// Generate an integer_seq with an additional element.
      template<T N>
        using append = integer_seq<T, I..., N>;

      using next = append<size>;
    };

  template<typename T, T... I>
    constexpr T integer_seq<T, I...>::size;

  template<int... I>
    using int_seq = integer_seq<int, I...>;

  template<unsigned... I>
    using uint_seq = integer_seq<unsigned, I...>;

  template<std::size_t... I>
    using index_seq = integer_seq<std::size_t, I...>;

  namespace detail
  {
    // Metafunction that generates an integer_seq of T containing [0, N)
    template<typename T, unsigned N>
      struct iota
      {
        using type = typename iota<T, N-1>::type::next;
      };

    // Terminal case of the recursive metafunction.
    template<typename T>
      struct iota<T, 0u>
      {
        using type = integer_seq<T>;
      };
  }


  // make_integer_seq<T, N> is an alias for integer_seq of T containing [0, N)
  template<typename T, unsigned N>
    using make_integer_seq = typename detail::iota<T, N>::type;

  template<int N>
    using make_int_seq = make_integer_seq<int, N>;

  template<unsigned N>
    using make_uint_seq = make_integer_seq<unsigned, N>;

  template<int N>
    using make_index_seq = make_integer_seq<std::size_t, N>;


  template<typename... Args>
    using int_seq_of = make_int_seq<sizeof...(Args)>;

  template<typename... Args>
    using uint_seq_of = make_uint_seq<sizeof...(Args)>;

  template<typename... Args>
    using index_seq_of = make_index_seq<sizeof...(Args)>;

}  // namespace redi

#endif  // REDI_INTEGER_SEQ_H

// vi: set ft=cpp sw=2:
