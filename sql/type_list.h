//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once

#include <tuple>
namespace neb {
namespace traits {
template <typename... ARGS> struct type_list {};
template <typename A1, typename... ARGS> struct type_list<A1, ARGS...> {
  typedef A1 first_type;
  typedef type_list<ARGS...> sub_type_list;
  const static int len = 1 + type_list<ARGS...>::len;
      };

    template<>
      struct type_list<>{
        const static int len = 0;
      };


    template<typename T1, typename T2>
      struct is_same_type{
        const static bool value = false;
      };

    template<typename T1>
      struct is_same_type<T1, T1>{
        const static bool value = true;
      };

    template <int V>
      struct int_number_type{
        const static int value = V;
      };

    template <typename T, typename TL>
      struct is_type_in_type_list{
        const static bool value = is_same_type<T, typename TL::first_type>::value ||
          is_type_in_type_list<T, typename TL::sub_type_list>::value;
      };

    template<typename T>
      struct is_type_in_type_list<T, type_list<> >{
        const static bool value = false;
      };

    template<typename TL1, typename TL2>
      struct is_contain_types{};

    template<typename... ARGS1, typename T, typename... ARGS2>
      struct is_contain_types<type_list<ARGS1...>, type_list<T, ARGS2...> >{
        const static bool value = is_type_in_type_list<T, type_list<ARGS1...> >::value &&
          is_contain_types<type_list<ARGS1...>, type_list<ARGS2...> > ::value;
      };

    template<typename... ARGS1>
      struct is_contain_types<type_list<ARGS1...>, type_list<> >{
        const static bool value = true;
      };

    template<typename T1, typename T2>
      struct merge_type_list{};

    template<typename... ARGS1, typename... ARGS2>
      struct merge_type_list<type_list<ARGS1...>, type_list<ARGS2...> >{
        typedef type_list<ARGS1..., ARGS2...> type;
      };

    template<typename T1, typename T2> struct merge_tuple{};

    template<typename... ARGS1, typename... ARGS2>
      struct merge_tuple<std::tuple<ARGS1...>, std::tuple<ARGS2...> >{
        typedef std::tuple<ARGS1..., ARGS2...> type;
      };

    template<typename TL>
      struct convert_type_list_to_tuple{};

    template <typename T, typename T1, typename... TS>
    struct convert_type_list_to_tuple<neb::traits::type_list<T, T1, TS...>> {
      typedef typename merge_tuple<
          std::tuple<T>, typename convert_type_list_to_tuple<
                             neb::traits::type_list<T1, TS...>>::type>::type
          type;
      };
      template <typename T>
      struct convert_type_list_to_tuple<neb::traits::type_list<T>> {
        typedef std::tuple<T> type;
      };
      template <> struct convert_type_list_to_tuple<neb::traits::type_list<>> {
        typedef std::tuple<> type;
      };
  }
}
