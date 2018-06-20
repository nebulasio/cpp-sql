#include "sql/mysql.hpp"
#include "sql/table.h"
#include <thread>

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

  std::thread thrd([&engine]() {
    auto local_engine = engine.thread_copy();
    auto ret2 = mytable::select<c1, c2, c3>(local_engine.get())
                    // auto ret2 = mytable::select<c1, c2, c3>(&engine)
                    .where(c1::eq(2))
                    .order_by<c1, neb::sql::desc>()
                    .limit(1)
                    .eval();
    std::cout << ret2.size() << std::endl;
    std::cout << ret2[0].get<c1>() << ", " << ret2[0].get<c2>() << ", "
              << ret2[0].get<c3>() << std::endl;
  });

  auto ret1 = mytable::select<c1, c2, c3>(&engine).eval();
  std::cout << "size: " << ret1.size() << std::endl;
  for (size_t i = 0; i < ret1.size(); ++i) {
    std::cout << ret1[i].get<c1>() << ", " << ret1[i].get<c2>() << ", "
              << ret1[i].get<c3>() << std::endl;
  }

  thrd.join();

  return 0;
}
