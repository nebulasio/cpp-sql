#include "sql/mysql.hpp"
#include "sql/table.h"

struct mymeta {
  constexpr static const char *table_name = "yyy";
};

define_column(c1, column, uint64_t, "id");
define_column(c2, key, std::string, "event");
define_column(c3, index, uint64_t, "ts");

typedef neb::sql::table<neb::sql::mysql<neb::sql::cppconn>, mymeta, c1, c2, c3>
    mytable;

int main(int argc, char *argv[]) {

  neb::sql::mysql<neb::sql::cppconn> engine("tcp://127.0.0.1:3306", "root", "",
                                            "test");
  mytable::create_table(&engine);

  mytable::row_collection_type rows;

  mytable::row_collection_type::row_type t1, t2;
  t1.set<c1, c2, c3>(1, "huobi", 123435);
  rows.push_back(t1);
  t2.set<c1, c2, c3>(2, "okex", 1235);
  rows.push_back(t2);

  mytable::insert_or_replace_rows(&engine, rows);

  auto ret1 = mytable::select<c1, c2, c3>(&engine).eval();
  std::cout << "size: " << ret1.size() << std::endl;
  for (size_t i = 0; i < ret1.size(); ++i) {
    std::cout << ret1[i].get<c1>() << ", " << ret1[i].get<c2>() << ", "
              << ret1[i].get<c3>() << std::endl;
  }
  std::cout << "---------------" << std::endl;

  typedef neb::sql::row_collection<c2, c3> new_rows_t;
  typedef new_rows_t::row_type nr_t;
  new_rows_t nrs;
  nr_t t3;
  t3.set<c2, c3>("okex", 656);
  nrs.push_back(t3);

  mytable::update_rows(&engine, nrs);
  auto ret2 = mytable::select<c1, c2, c3>(&engine)
                  .where(c1::eq(2))
                  .order_by<c1, neb::sql::desc>()
                  .limit(1)
                  .eval();
  std::cout << ret2.size() << std::endl;
  std::cout << ret2[0].get<c1>() << ", " << ret2[0].get<c2>() << ", "
            << ret2[0].get<c3>() << std::endl;

  return 0;
}
