//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once
#include "sql/common.h"
#include <iostream>

namespace neb {
namespace sql {

struct sqlite3 {}; // TODO

struct cppconn {};

template <class FS> class mysql {};


struct sql_debug {
  typedef void *native_statement_type;
  void eval_sql_string(const std::string &sql) {
    std::cout << sql << std::endl;
  }

  native_statement_type prepare_sql_with_string(const std::string &sql) {
    std::cout << sql << std::endl;
    return NULL;
  }
  void eval_native_sql_stmt(native_statement_type stmt) {}
  template <typename T>
  void bind_to_native_statement(native_statement_type stmt, int index,
                                const T &value) {
    std::cout << "bind value: " << value << " to " << index << std::endl;
  }

  void begin_transaction() { std::cout << "begin transaction" << std::endl; }
  void end_transaction() { std::cout << "end transaction" << std::endl; }
    };
  }
}
