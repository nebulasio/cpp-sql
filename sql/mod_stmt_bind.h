//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once
#include "sql/common.h"
#include "sql/rows.h"
#include "sql/traits.h"
#include "sql/columns.h"

namespace neb {
namespace sql {

template <typename TT, int index> struct traverse_row_for_bind {
  typedef typename TT::engine_type engine_type;
  typedef typename engine_type::native_statement_type native_statement_type;

  template <typename VT>
  static auto run(engine_type *engine, native_statement_type stmt,
                  const VT &val) ->
      typename std::enable_if<(VT::type_list::len > index), void>::type {
    typedef typename get_type_at_index_in_typelist<typename VT::type_list,
                                                   index>::type cur_type;
    engine->bind_to_native_statement(stmt, index + 1,
                                     val.template get<cur_type>());
    traverse_row_for_bind<TT, index + 1>::run(engine, stmt, val);
  }

  template <typename VT>
  static auto run(engine_type *engine, native_statement_type stmt,
                  const VT &val) ->
      typename std::enable_if<(VT::type_list::len <= index), void>::type {}
};
//////////////////////////
template <typename TT, int index, typename TL>
struct traverse_row_for_bind_and_put_key_to_last {
  typedef typename TT::engine_type engine_type;
  typedef typename engine_type::native_statement_type native_statement_type;

  template <typename VT>
  static auto run(engine_type *engine, native_statement_type stmt,
                  const VT &val, int &next_index) ->
      typename std::enable_if<(VT::type_list::len > index), void>::type {
    typedef typename get_type_at_index_in_typelist<typename VT::type_list,
                                                   index>::type current_type;

    if (std::is_base_of<key<typename current_type::type>,
                        current_type>::value) {
      traverse_row_for_bind_and_put_key_to_last<TT, index + 1, TL>::run(
          engine, stmt, val, next_index);
      engine->bind_to_native_statement(stmt, next_index + 1,
                                       val.template get<current_type>());
      next_index++;
    } else {
      engine->bind_to_native_statement(stmt, next_index + 1,
                                       val.template get<current_type>());
      next_index++;
      traverse_row_for_bind_and_put_key_to_last<TT, index + 1, TL>::run(
          engine, stmt, val, next_index);
    }
  }

  template <typename VT>
  static auto run(engine_type *engine, native_statement_type stmt,
                  const VT &val, int &next_index) ->
      typename std::enable_if<(VT::type_list::len <= index), void>::type {}
      };

  }
}
