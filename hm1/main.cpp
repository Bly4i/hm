#include "sql_query_builder.h"
#include "advanced_sql_query_builder.h"
#include <iostream>
#include <cassert>

int main() {
    SqlSelectQueryBuilder query_builder_base;
    query_builder_base.AddColumn("name").AddColumn("phone");
    query_builder_base.AddFrom("students");
    query_builder_base.AddWhere("id", "42").AddWhere("name", "John");
    assert(query_builder_base.BuildQuery() == "SELECT name, phone FROM students WHERE id=42 AND name=John;");

    AdvancedSqlSelectQueryBuilder advanced_query_builder_1;
    advanced_query_builder_1.AddColumns({ "item_name", "quantity" });
    advanced_query_builder_1.AddFrom("inventory");
    advanced_query_builder_1.AddWhereGreaterThan("stock_count", "100");
    assert(advanced_query_builder_1.BuildQuery() == "SELECT item_name, quantity FROM inventory WHERE stock_count>100;");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
