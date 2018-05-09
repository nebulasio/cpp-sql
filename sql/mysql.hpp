#pragma once

#include "sql/engine.h"
#include "sql/rows.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace neb{
  namespace sql{

    template<class STMT, class T> struct mysql_bind_setter{
      static void bind(STMT , int , const T& ){
        throw std::runtime_error("No support yet");
      }
    };

    template<class STMT>
      struct mysql_bind_setter<STMT, std::string>{
      static void bind(STMT stmt, int index, const std::string& value){
        stmt->setString(index, value);
      }
      };
    template<class STMT>
      struct mysql_bind_setter<STMT, int32_t>{
      static void bind(STMT stmt, int index, int32_t value){
        stmt->setInt(index, value);
      }
      };
    template<class STMT>
      struct mysql_bind_setter<STMT, uint32_t>{
      static void bind(STMT stmt, int index, uint32_t value){
        stmt->setUInt(index, value);
      }
      };
    template<class STMT>
      struct mysql_bind_setter<STMT, int64_t>{
      static void bind(STMT stmt, int index, int64_t value){
        stmt->setInt64(index, value);
      }
      };
    template<class STMT>
      struct mysql_bind_setter<STMT, uint64_t>{
      static void bind(STMT stmt, int index, uint64_t value){
        stmt->setUInt64(index, value);
      }
      };
    template<class STMT>
      struct mysql_bind_setter<STMT, double>{
      static void bind(STMT stmt, int index, double value){
        stmt->setDouble(index, value);
      }
      };

    template<class T>
      struct mysql_rs_getter{};
#define impl_mysql_rs_getter(type, method) \
    template<> \
      struct mysql_rs_getter<type>{ \
        template <typename RST> \
          static type get(RST * r, const std::string & name){ \
            return r->method(name); \
          } \
      }; \

    impl_mysql_rs_getter(std::string, getString);
    impl_mysql_rs_getter(double, getDouble);
    impl_mysql_rs_getter(int64_t, getInt64);
    impl_mysql_rs_getter(uint64_t, getUInt64);
    impl_mysql_rs_getter(int32_t, getInt);
    impl_mysql_rs_getter(uint32_t, getUInt);
#undef impl_mysql_rs_getter

    template<typename... ARGS>
      struct mysql_record_setter{};

    template<>
      struct mysql_record_setter<>{
        template<typename RT, typename RST>
          static void set(RT & , RST * ){
          }
      };
    template<typename T, typename... ARGS>
      struct mysql_record_setter<T, ARGS...>{
        template<typename RT, typename RST>
          static void set(RT & row, RST * r){
            row.template set<T>(mysql_rs_getter<typename T::type>::get(r, T::name));
            mysql_record_setter<ARGS...>::set(row, r);
          }
      };

template <> class mysql<cppconn> {
  public:
    typedef std::shared_ptr<::sql::PreparedStatement > native_statement_type;
    typedef ::sql::ResultSet * query_result_type;

    mysql(const std::string & url, const std::string & usrname, const std::string & passwd, const std::string &dbname)
    : m_url(url), m_usrname(usrname), m_passwd(passwd), m_dbname(dbname){

    m_sql_driver = get_driver_instance();
    m_sql_conn= m_sql_driver->connect(m_url, m_usrname, m_passwd);
    m_sql_conn->setSchema(m_dbname);

  }
  void eval_sql_string(const std::string &sql) {
    std::shared_ptr<::sql::Statement> stmt(m_sql_conn->createStatement());
    stmt->execute(sql);
  }

  query_result_type eval_sql_query_string(const std::string &sql) {
    std::shared_ptr<::sql::Statement> stmt(m_sql_conn->createStatement());
    return stmt->executeQuery(sql);
  }

  template<typename... ARGS>
    row_collection<ARGS...> parse_records(query_result_type query_result){
      row_collection<ARGS...> rows;

      while(query_result->next()){
        typename row_collection<ARGS...>::row_type r;
        mysql_record_setter<ARGS...>::set(r, query_result);
        rows.push_back(r);
      }
      return rows;

    }



  native_statement_type prepare_sql_with_string(const std::string &sql) {
    native_statement_type stmt;
    stmt.reset(m_sql_conn->prepareStatement(sql.c_str()));
    return stmt;
  }

  query_result_type eval_native_sql_stmt(native_statement_type stmt) {
    return stmt->executeQuery();
  }
  template <typename T>
  void bind_to_native_statement(native_statement_type stmt, int index,
                                const T &value) {
    mysql_bind_setter<native_statement_type, T>::bind(stmt, index, value);
  }

  void begin_transaction() {  }
  void end_transaction() {  }

protected:
  ::sql::Driver *m_sql_driver;
  ::sql::Connection * m_sql_conn;

  const std::string m_url;
  const std::string m_usrname;
  const std::string m_passwd;
  const std::string m_dbname;
};

  }
}

