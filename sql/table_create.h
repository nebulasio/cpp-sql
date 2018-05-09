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
      template<>
        struct dump_col_type_creation<uint64_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<int64_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<uint32_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<int32_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<uint16_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<int16_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<uint8_t>{
          static void dump(std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<int8_t>{
          static void dump (std::stringstream & ss){
            ss<< " INTEGER ";
          }
        };
      template<>
        struct dump_col_type_creation<std::string>{
        static void dump(std::stringstream &ss) { ss << " VARCHAR(20) "; }
        };
      template<>
        struct dump_col_type_creation<double>{
          static void dump(std::stringstream & ss){
            ss<< " REAL ";
          }
        };
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
