#include "advanced_sql_query_builder.h"
#include <sstream>

AdvancedSqlSelectQueryBuilder& AdvancedSqlSelectQueryBuilder::AddWhereGreaterThan(const std::string& key, const std::string& value) noexcept {
    advanced_where_conditions_.emplace_back(key, value, ">");
    return *this;
}

AdvancedSqlSelectQueryBuilder& AdvancedSqlSelectQueryBuilder::AddWhereLessThan(const std::string& key, const std::string& value) noexcept {
    advanced_where_conditions_.emplace_back(key, value, "<");
    return *this;
}

std::string AdvancedSqlSelectQueryBuilder::BuildQuery() const {
    std::string base_query = SqlSelectQueryBuilder::BuildQuery();
    std::string where_clause;

    if (!advanced_where_conditions_.empty()) {
        if (base_query.find("WHERE") != std::string::npos) {
            where_clause += " AND ";
        }
        else {
            where_clause += " WHERE ";
        }

        for (size_t i = 0; i < advanced_where_conditions_.size(); ++i) {
            where_clause += std::get<0>(advanced_where_conditions_[i]) + std::get<2>(advanced_where_conditions_[i]) + std::get<1>(advanced_where_conditions_[i]);

            if (i < advanced_where_conditions_.size() - 1) {
                where_clause += " AND ";
            }
        }
    }

    base_query.pop_back();
    return base_query + where_clause + ";";
}

void AdvancedSqlSelectQueryBuilder::Clear() noexcept {
    SqlSelectQueryBuilder::Clear();
    advanced_where_conditions_.clear();
}
