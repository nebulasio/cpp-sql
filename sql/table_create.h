//
//  Created by Xuepeng Fan on 08/11/2017.
//
#pragma once
#include <sstream>
#include "sql/columns.h"
#include "sql/traits.h"

namespace neb {
namespace sql {
namespace internal {

template <typename T> struct dump_col_type_creation {
  static void dump(std::stringstream &ss) { ss << " BLOB "; }
        };

#define impl_table_dump_types(cpptype, sqltype)                                \
  template <> struct dump_col_type_creation<cpptype> {                         \
    static void dump(std::stringstream &ss) { ss << " " << sqltype << " "; }   \
  }
        impl_table_dump_types(uint64_t, "BIGINT UNSIGNED");
        impl_table_dump_types(int64_t, "BIGINT");
        impl_table_dump_types(uint32_t, "INT UNSIGNED");
        impl_table_dump_types(int32_t, "INT");
        impl_table_dump_types(int16_t, "SMALLINT");
        impl_table_dump_types(uint16_t, "SMALLINT UNSIGNED");
        impl_table_dump_types(int8_t, "TINYINT");
        impl_table_dump_types(uint8_t, "TINYINT UNSIGNED");
        impl_table_dump_types(float, "FLOAT");
        impl_table_dump_types(double, "DOUBLE");
        impl_table_dump_types(std::string, "VARCHAR(20)");

        //////////////////////////
    }

    enum{
      key_type,
      index_type,
      column_type,
    };
    template<typename T>
      struct extract_col_type{
        typedef typename T::type ct;
        const static int value = std::conditional<std::is_base_of<key<ct>, T>::value,
              traits::int_number_type<key_type>, typename std::conditional<std::is_base_of<index<ct>, T>::value,
              traits::int_number_type<index_type>, traits::int_number_type<column_type> >::type >::type::value;
      };
    template<typename T, int V = extract_col_type<T>::value>
      struct dump_col_creation{};

    template<typename T>
      struct dump_col_creation<T, key_type>{
        static void dump(std::stringstream & ss){
          ss<<T::name;
          internal::dump_col_type_creation<typename T::type>::dump(ss);
          ss<<" primary key";
        }
      };

    template<typename T>
      struct dump_col_creation<T, index_type>{
        static void dump(std::stringstream & ss){
          ss<<T::name;
          internal::dump_col_type_creation<typename T::type>::dump(ss);
        }
      };
    template<typename T>
      struct dump_col_creation<T, column_type>{
        static void dump(std::stringstream & ss){
          ss<<T::name;
          internal::dump_col_type_creation<typename T::type>::dump(ss);
        }
      };
  }
}
