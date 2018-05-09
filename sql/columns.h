//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once
#include "sql/common.h"
#include "sql/stmt.h"

namespace neb {
namespace sql {

template <typename T>
struct column_base {
    typedef T type;
};
template <typename T>
struct column : public column_base<T> {
};

template <typename T>
class index : public column_base<T> {
};

template <typename T>
class key : public column_base<T> {
};

template <typename TT, typename CT>
struct table_column {
    typedef TT table_type;
    typedef CT column_type;
    typedef typename CT::type type;
};

template <typename TL>
struct get_key_column_type {
};

template <typename T, typename... TS>
struct get_key_column_type<neb::traits::type_list<T, TS...>> {
  typedef typename std::conditional<
      std::is_base_of<key<typename T::type>, T>::value, T,
      typename get_key_column_type<neb::traits::type_list<TS...>>::type>::type
      type;
};

template <> struct get_key_column_type<neb::traits::type_list<>> {
  typedef void type;
};
} // namespace sql
} // namespace neb

#define define_column(_name, _type, _dtype, _tname)                            \
  struct _name : public neb::sql::_type<_dtype> {                              \
    constexpr static const char *name = _tname;                                \
    static neb::sql::eq_cond_stmt<_name> eq(const _dtype &value) {             \
      return neb::sql::eq_cond_stmt<_name>(value);                             \
    }                                                                          \
    static neb::sql::ne_cond_stmt<_name> ne(const _dtype &value) {             \
      return neb::sql::ne_cond_stmt<_name>(value);                             \
    }                                                                          \
    static neb::sql::le_cond_stmt<_name> le(const _dtype &value) {             \
      return neb::sql::le_cond_stmt<_name>(value);                             \
    }                                                                          \
    static neb::sql::ge_cond_stmt<_name> ge(const _dtype &value) {             \
      return neb::sql::ge_cond_stmt<_name>(value);                             \
    }                                                                          \
  };

