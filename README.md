A C++ MySQL Wrapper
=======
This library provides an abstract of MySQL operations, with the power of static
checking of C++. Why another MySQL wrapper library? Well, traditional MySQL
wrappers directly use SQL string to do queries, and this is error-prone to us.

First of all, we think a SQL table is a data type with several meta info, like
columns, and table name. And each column has its name, data type and properties
(like index, key, or normal column). Thus, we define a table like this:

    struct mymeta {
      constexpr static const char *table_name = "yyy";
    };

    define_column(c1, column, uint64_t, "id");
    define_column(c2, key, std::string, "event");
    define_column(c3, index, uint64_t, "ts");
    typedef neb::sql::default_table<mymeta, c1, c2, c3> mytable;

As the code above shows, we defined three columns `c1`, `c2` and `c3`, and the
table name is defined in `mymeta`, which specifies the table name, `yyy`. The
type of the new table is called `mytable`. Now we may create this table

    neb::sql::default_engine engine("tcp::/127.0.0.1:3306", "username",
    "password", "sql_name");
    mytable::create_table(&engine);

Remember that a table is just a type, thus we need a mysql engine to do
operations on the new table. And also `create_table` is just a class method,
instead of an instance method.

Here is some example of inserting rows.

    mytable::row_collection_type rows;
    mytable::row_collection_type::row_type t1, t2;
    t1.set<c1, c2, c3>(1, "column1", 123435);
    rows.push_back(t1);
    t2.set<c1, c2, c3>(2, "column2", 1235);
    rows.push_back(t2);
    mytable::insert_or_replace_rows(&engine, rows);

And for updating rows,

    typedef neb::sql::row_collection<c2, c3> new_rows_t;
    typedef new_rows_t::row_type nr_t;
    new_rows_t nrs;
    nr_t t3;
    t3.set<c2, c3>("okex", 656);
    nrs.push_back(t3);
    mytable::update_rows(&engine, nrs);

Finally, here is how we do query,

    auto ret2 = mytable::select<c1, c2, c3>(&engine)
                  .where(c1::eq(2))
                  .order_by<c1, neb::sql::desc>()
                  .limit(1)
                  .eval();

*Note, we didn't support join tables yet. And we will support it soon. *




