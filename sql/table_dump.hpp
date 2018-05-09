//#pragma once
//#include "sql/common.h"
//#include "sql/table_create.h"
//#include <sstream>

// namespace neb{
// namespace sql{
template<typename T, typename T1, typename... TS>
static void recursive_dump_col_name(std::stringstream & ss){
  ss<<T::name<<", ";
  recursive_dump_col_name<T1, TS...>(ss);
}
template<typename T>
static void recursive_dump_col_name(std::stringstream & ss){
  ss<<T::name;
}

template<typename T, typename T1, typename... TS>
static void recursive_dump_col_creation(std::stringstream & ss){
  dump_col_creation<T>::dump(ss);
  ss<<", ";
  recursive_dump_col_creation<T1, TS...>(ss);
}
template<typename T>
static void recursive_dump_col_creation(std::stringstream & ss){
  dump_col_creation<T>::dump(ss);
  ss<<")";
}
template<typename T, typename T1, typename... TS>
static void recursive_dump_for_index(std::stringstream & ss){
  if(std::is_base_of<index<typename T::type>, T>::value){
    ss<<"create "<<T::name<<"_index on "<<TM::table_name<<" ("<<T::name<<");";
  }else{
  }
  recursive_dump_for_index<T1, TS...>(ss);
}
template<typename T>
static void recursive_dump_for_index(std::stringstream & ss){
  if(std::is_base_of<index<typename T::type>, T>::value){
    ss<<"create "<<T::name<<"_index on "<<TM::table_name<<" ("<<T::name<<");";
  }else{
  }
}

template<typename T, typename T1, typename... TS>
static void recurseive_dump_update_item_and_ignore_key(std::stringstream & ss){
  if(std::is_base_of<key<typename T::type>, T>::value){
  }else{
    ss<<T::name<<"=?, ";
  }
  recurseive_dump_update_item_and_ignore_key<T1, TS...>(ss);
}

template<typename T>
static void recurseive_dump_update_item_and_ignore_key(std::stringstream & ss){
  if(std::is_base_of<key<typename T::type>, T>::value){
    return ;
  }
  ss<<T::name<<"=?";
}

template<typename T, typename T1, typename... TS>
static void recurseive_dump_update_item_for_only_where(std::stringstream & ss){
  if(std::is_base_of<key<typename T::type>, T>::value){
    ss<<T::name<<"=?";
  }else{
    recurseive_dump_update_item_for_only_where<T1, TS...>(ss);
  }
}

template<typename T>
static void recurseive_dump_update_item_for_only_where(std::stringstream & ss){
  if(std::is_base_of<key<typename T::type>, T>::value){
    ss<<T::name<<"=?";
  }
}


//}
//}
