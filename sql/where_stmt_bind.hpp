//
//  Created by Xuepeng Fan on 08/11/2017.
//

//#pragma once
//#include "sql/common.h"
//#include "sql/rows.h"
//#include <sstream>

//namespace neb{
  //namespace sql{

    template<typename TT, typename T>
      struct traverse_cond_for_bind{
        typedef typename TT::engine_type engine_type;
        typedef typename engine_type::native_statement_type native_statement_type;

        template<typename VT, sql_cond_type CT>
        static void run(engine_type * engine, native_statement_type stmt, const basic_cond_stmt<VT, CT>  & cst, int & index){
          static_assert(std::is_base_of<cond_stmt, T>::value, "T must be cond_stmt");
          engine->bind_to_native_statement(stmt, index + 1, cst.m_value);
          index ++;
        }
      };

    template<typename TT, typename T1, typename T2>
      struct traverse_cond_for_bind<TT, and_cond_stmt<T1, T2> >{
        typedef typename TT::engine_type engine_type;
        typedef typename engine_type::native_statement_type native_statement_type;

        static void run(engine_type * engine, native_statement_type stmt, const and_cond_stmt<T1, T2> & cst, int & index){
          traverse_cond_for_bind<TT, T1>::run(engine, stmt, cst.stmt1, index);
          traverse_cond_for_bind<TT, T2>::run(engine, stmt, cst.stmt2, index);
        }
      };

    template<typename TT, typename T1, typename T2>
      struct traverse_cond_for_bind<TT, or_cond_stmt<T1, T2> >{
        typedef typename TT::engine_type engine_type;
        typedef typename engine_type::native_statement_type native_statement_type;

        static void run(engine_type * engine, native_statement_type stmt, const or_cond_stmt<T1, T2> & cst, int & index){
          traverse_cond_for_bind<TT, T1>::run(engine, stmt, cst.stmt1, index);
          traverse_cond_for_bind<TT, T2>::run(engine, stmt, cst.stmt2, index);
        }
      };
  //}
//}
