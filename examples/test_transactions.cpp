#include "sql/mysql.hpp"
#include "sql/table.h"
#include <chrono>
#include <cstdlib>
#include <iostream>

struct mymeta {
  constexpr static const char *table_name = "test_transaction";
};

define_column(c1, key, uint64_t, "id");
define_column(c2, column, std::string, "event");
define_column(c3, index, uint64_t, "ts");

typedef neb::sql::table<neb::sql::mysql<neb::sql::cppconn>, mymeta, c1, c2, c3>
    mytable;

int main(int argc, char *argv[]) {

  neb::sql::mysql<neb::sql::cppconn> engine("tcp://127.0.0.1:3306", "root", "",
                                            "test");
  mytable::delete_rows(&engine);
  mytable::create_table(&engine);

  mytable::row_collection_type rows;

  mytable::row_collection_type::row_type t1;
  int64_t max = 10000;
  for (int i = 0; i < max; ++i) {
    t1.set<c1, c2, c3>(i, std::to_string(i), i);
    rows.push_back(t1);
  }

  auto start = std::chrono::system_clock::now();
  mytable::insert_or_replace_rows(&engine, rows);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "items: " << max << ", time: " << elapsed_seconds.count()
            << " seconds" << std::endl;
  ;

  return 0;
}
