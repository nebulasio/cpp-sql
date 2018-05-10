#include "sql/mysql.hpp"
#include "sql/table.h"

struct mymeta {
  constexpr static const char *table_name = "xxxyyy";
};

define_column(c1, column, uint64_t, "id");
define_column(c2, key, std::string, "event");
define_column(c3, index, uint64_t, "ts");
define_column(c4, column, int64_t, "a");
define_column(c5, column, uint32_t, "b");
define_column(c6, column, int32_t, "c");
define_column(c7, column, double, "d");
define_column(c8, column, float, "e");

typedef neb::sql::table<neb::sql::mysql<neb::sql::cppconn>, mymeta, c1, c2, c3,
                        c4, c5, c6, c7, c8>
    mytable;

int main(int argc, char *argv[]) {

  neb::sql::mysql<neb::sql::cppconn> engine("tcp://127.0.0.1:3306", "root", "",
                                            "test");
  mytable::create_table(&engine);

  mytable::row_collection_type rows;

  mytable::row_collection_type::row_type t1;
  t1.set<c1, c2, c3, c4, c5, c6, c7, c8>(1, "huobi", 123435, 1, 1, 1, 3.2, 2.0);
  rows.push_back(t1);

  mytable::insert_or_replace_rows(&engine, rows);

  auto ret1 = mytable::select<c1, c2, c3, c4, c5, c6, c7, c8>(&engine).eval();
  std::cout << "size: " << ret1.size() << std::endl;
  for (size_t i = 0; i < ret1.size(); ++i) {
    std::cout << ret1[i].get<c1>() << ", " << ret1[i].get<c2>() << ", "
              << ret1[i].get<c3>() << ", " << ret1[i].get<c4>() << ", "
              << ret1[i].get<c5>() << ", " << ret1[i].get<c6>() << ", "
              << ret1[i].get<c7>() << ", " << ret1[i].get<c8>() << ", "
              << std::endl;
  }
  std::cout << "---------------" << std::endl;
  mytable::select<c1, c2>(&engine).where(c1::eq(1)).eval();

  return 0;
}
