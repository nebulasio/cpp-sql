#include "sql/mysql.hpp"
#include "sql/table.h"

struct mymeta {
  constexpr static const char *table_name = "xxxyyy";
};

struct md {
  int a;
  int b;
  int c;
};

define_column(c1, column, uint64_t, "id");
define_column(c2, key, std::string, "event");
define_column(c3, index, uint64_t, "ts");
define_column(c4, index, int64_t, "a");
define_column(c5, column, uint32_t, "b");
define_column(c6, column, int32_t, "c");
define_column(c7, column, double, "d");
define_column(c8, column, float, "e");
define_column(c9, column, uint16_t, "u16");
define_column(c10, column, int16_t, "i16");
define_column(c11, column, uint8_t, "u8");
define_column(c12, column, int8_t, "i8");
define_column(c13, column, md, "md");

typedef neb::sql::table<neb::sql::mysql<neb::sql::cppconn>, mymeta, c1, c2, c3,
                        c4, c5, c6, c7, c8, c9, c10, c11, c12>
    mytable;

int main(int argc, char *argv[]) {

  neb::sql::mysql<neb::sql::cppconn> engine("tcp://127.0.0.1:3306", "root", "",
                                            "test");
  mytable::drop_table(&engine);
  mytable::create_table(&engine);

  mytable::row_collection_type rows;

  mytable::row_collection_type::row_type t1;
  t1.set<c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12>(
      1, "huobi", 123435, 1, 1, 1, 3.2, 2.0, 16, 16.0, 8, 8.0);
  rows.push_back(t1);

  mytable::insert_or_replace_rows(&engine, rows);

  auto ret1 =
      mytable::select<c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12>(
          &engine)
          .eval();
  std::cout << "size: " << ret1.size() << std::endl;
  for (size_t i = 0; i < ret1.size(); ++i) {
    std::cout << ret1[i].get<c1>() << ", " << ret1[i].get<c2>() << ", "
              << ret1[i].get<c3>() << ", " << ret1[i].get<c4>() << ", "
              << ret1[i].get<c5>() << ", " << ret1[i].get<c6>() << ", "
              << ret1[i].get<c7>() << ", " << ret1[i].get<c8>() << ", "
              << ret1[i].get<c9>() << ", " << ret1[i].get<c10>() << ", "
              << static_cast<int>(ret1[i].get<c11>()) << ", "
              << static_cast<int>(ret1[i].get<c12>()) << ", " << std::endl;
  }
  std::cout << "---------------" << std::endl;
  mytable::select<c1, c2>(&engine).where(c1::eq(1)).eval();

  return 0;
}
