#ifndef ADVANCED_SQL_QUERY_BUILDER_H
#define ADVANCED_SQL_QUERY_BUILDER_H

#include "sql_query_builder.h"
#include <string>
#include <vector>
#include <tuple>

class AdvancedSqlSelectQueryBuilder : public SqlSelectQueryBuilder {
public:
    AdvancedSqlSelectQueryBuilder& AddWhereGreaterThan(const std::string& key, const std::string& value) noexcept;
    AdvancedSqlSelectQueryBuilder& AddWhereLessThan(const std::string& key, const std::string& value) noexcept;
    std::string BuildQuery() const override;
    void Clear() noexcept override;

private:
    std::vector<std::tuple<std::string, std::string, std::string>> advanced_where_conditions_;
};

#endif
