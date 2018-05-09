//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once
#include "sql/common.h"

namespace neb {
namespace sql {

template <typename... ARGS>
struct check_if_replicate {}; // end struct check_if_replicate

template <typename T, typename... ARGS> struct check_if_replicate<T, ARGS...> {
  const static bool value = neb::traits::is_type_in_type_list<
                                T, neb::traits::type_list<ARGS...>>::value ||
                            check_if_replicate<ARGS...>::value;
      };

    template<typename T> struct check_if_replicate<T>{
      const static bool value = false;
    };

    template<typename TC, typename TL>
      struct get_index_of_type_in_typelist{};
    template <typename TC, typename T1, typename... TS>
    struct get_index_of_type_in_typelist<TC,
                                         neb::traits::type_list<T1, TS...>> {
      const static int value = std::conditional<
          neb::traits::is_same_type<TC, T1>::value,
          neb::traits::int_number_type<0>,
          neb::traits::int_number_type<
              1 + get_index_of_type_in_typelist<
                      TC, neb::traits::type_list<TS...>>::value>>::type::value;
      };

      template <typename TC>
      struct get_index_of_type_in_typelist<TC, neb::traits::type_list<>> {
        const static int value  = -1;
      };

    template<typename TL, int index>
      struct get_type_at_index_in_typelist{
        const static int value = -1;
        typedef void type;
      };

      template <typename T, typename... TS, int index>
      struct get_type_at_index_in_typelist<neb::traits::type_list<T, TS...>,
                                           index> {
        typedef typename std::conditional<
            index == 0, T,
            typename get_type_at_index_in_typelist<
                neb::traits::type_list<TS...>, index - 1>::type>::type type;
      };

      template <typename TL>
      struct extract_content_type_list {
      };

      template <typename T1, typename T2, typename... TS>
      struct extract_content_type_list<neb::traits::type_list<T1, T2, TS...>> {
        typedef typename neb::traits::merge_type_list<
            neb::traits::type_list<typename T1::type>,
            typename extract_content_type_list<
                neb::traits::type_list<T2, TS...>>::type>::type type;
      };

      template <typename T1>
      struct extract_content_type_list<neb::traits::type_list<T1>> {
        typedef neb::traits::type_list<typename T1::type> type;
      };

      template <> struct extract_content_type_list<neb::traits::type_list<>> {
        typedef neb::traits::type_list<> type;
      };
  }
}
