//
//  Created by Xuepeng Fan on 08/30/2017.
//

#pragma once
#include "sql/common.h"
#include "sql/traits.h"
#include "sql/type_list.h"

namespace neb {

using namespace sql;

/*
 * ntobject is used to define a struct without writng your own class.
 * To use it, you need to declare a bunch of types first, like this
 *
 * define_nt(email, std::string, "email");
 * define_nt(uid, uint64_t, "uid");
 * define_nt(name, std::string, "name");
 *
 * typedef ntobject<email, uid, name> myobject_t;
 *
 * Now you have your own structure -- myobject_t, you can set and get field in it like
 *
 * myobject_t obj;
 * obj.set<name>("xuepeng");
 * obj.set<email>("xp@example.com");
 * obj.set<uid>(128);
 *
 * std::string val_name = obj.get<name>();
 *
 * Further more, you can set multiple values with any order if you want,
 *
 * obj.set<name, email>("xuepeng", "xp@example.com");
 * obj.set<email, uid, name> ("xp@example.com", 128, "xuepeng");
 *
 *
 * */
template <typename... ARGS>
class ntobject {
public:
  typedef typename neb::traits::type_list<ARGS...> type_list;

  ntobject() : m_content(new content_type()) {}

  template <typename CT> void set(const typename CT::type &val) {
    static_assert(neb::traits::is_type_in_type_list<
                      CT, neb::traits::type_list<ARGS...>>::value,
                  "Cannot set a value that's not in the ntobject/row!");
    const static int index =
        get_index_of_type_in_typelist<CT,
                                      neb::traits::type_list<ARGS...>>::value;
    std::get<index>(*m_content) = val;
    }

    template <typename CT, typename CT1, typename... CARGS, typename... PARGS>
    void set(const typename CT::type &val,
             const typename CT1::type &val1,
             PARGS... params)
    {
      static_assert(neb::traits::is_type_in_type_list<
                        CT, neb::traits::type_list<ARGS...>>::value,
                    "Cannot set a value that's not in the row!");
      static_assert(neb::traits::is_type_in_type_list<
                        CT1, neb::traits::type_list<ARGS...>>::value,
                    "Cannot set a value that's not in the row!");
      const static int index =
          get_index_of_type_in_typelist<CT,
                                        neb::traits::type_list<ARGS...>>::value;
      std::get<index>(*m_content) = val;

      set<CT1, CARGS...>(val1, params...);
    }

    template <typename CT>
    typename CT::type get() const
    {
      static_assert(neb::traits::is_type_in_type_list<
                        CT, neb::traits::type_list<ARGS...>>::value,
                    "Cannot get a value that's not in the ntobject/row!");
      const static int index =
          get_index_of_type_in_typelist<CT,
                                        neb::traits::type_list<ARGS...>>::value;
      return std::get<index>(*m_content);
    }

protected:
  typedef typename neb::traits::convert_type_list_to_tuple<
      typename extract_content_type_list<
          neb::traits::type_list<ARGS...>>::type>::type content_type;
  std::shared_ptr<content_type> m_content;
};

template <typename... ARGS>
class ntarray {
public:
    typedef ntobject<ARGS...> row_type;

    void push_back(const row_type &row) { m_collection.push_back(row); }

    void clear() { m_collection.clear(); }

    size_t size() const { return m_collection.size(); }

    row_type &operator[](size_t index) { return m_collection[index]; }

    const row_type &operator[](size_t index) const
    {
        return m_collection[index];
    }

protected:
    std::vector<row_type> m_collection;
};

template <typename T>
struct is_ntobject {
    const static bool value = false;
};

template <typename... ARGS>
struct is_ntobject<ntobject<ARGS...>> {
    const static bool value = true;
};

template <typename T>
struct is_ntarray {
    const static bool value = false;
};

template <typename... ARGS>
struct is_ntarray<ntarray<ARGS...>> {
    const static bool value = true;
};

} // namespace neb

#define define_nt(_name, _dtype, _tname)                                       \
    struct _name {                                                             \
        constexpr static const char *name = _tname;                            \
        typedef _dtype type;                                                   \
    };
