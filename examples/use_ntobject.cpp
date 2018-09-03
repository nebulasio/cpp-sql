#include "sql/ntobject.h"
#include "sql/table.h"

define_nt(email, std::string, "email");
define_nt(uid, uint64_t, "ui");
define_nt(uname, std::string, "uname");

int main(int argc, char *argv[]) {

  typedef neb::ntobject<email, uid, uname> myobj_t;
  myobj_t obj;
  obj.set<uname>("xuepeng");
  obj.set<uid>(122);
  obj.set<email>("xp@example.com");

  typedef neb::ntarray<email, uid, uname> theobjects_t;
  typedef typename theobjects_t::row_type theobject_t;
  theobject_t t;
  t.set<email, uid, uname>(obj.get<email>(), obj.get<uid>(), obj.get<uname>());
  theobjects_t obs;

  obs.push_back(t);

  std::cout << obs.size() << std::endl;

  typedef neb::ntobject<uid, email, uname> tt_t;
  typedef neb::traits::type_list<uid, email, uname> t1_t;
  typedef neb::traits::type_list<email, uid, uname> t2_t;
  std::cout << "type compatiable: "
            << neb::traits::is_two_type_list_compatible<t1_t, t2_t>::value
            << std::endl;

  tt_t tt = obj;

  if (tt.get<email>() == obj.get<email>() && tt.get<uid>() == obj.get<uid>() &&
      tt.get<uname>() == obj.get<uname>()) {
    std::cout << "type convert ok" << std::endl;
  } else {
    std::cout << "type convert wrong" << std::endl;
  }
  return 0;
}
