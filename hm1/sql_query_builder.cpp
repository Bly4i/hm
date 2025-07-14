#include "sql_query_builder.h"
#include <sstream>

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(const std::string& column) noexcept {
    columns_.push_back(column);
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumns(const std::vector<std::string>& columns) noexcept {
    columns_.insert(columns_.end(), columns.begin(), columns.end());
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(const std::string& table) noexcept {
    from_table_ = table;
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::string& key, const std::string& value) noexcept {
    where_conditions_.push_back({ key, value });
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::map<std::string, std::string>& kv) noexcept {
    for (const auto& pair : kv) {
        where_conditions_.push_back({ pair.first, pair.second });
    }
    return *this;
}

std::string SqlSelectQueryBuilder::BuildQuery() const {
    std::stringstream query;
    query << "SELECT ";
    if (columns_.empty()) {
        query << "*";
    }
    else {
        for (size_t i = 0; i < columns_.size(); ++i) {
            query << columns_[i];
            if (i < columns_.size() - 1) {
                query << ", ";
            }
        }
    }
    query << " FROM " << from_table_;

    if (!where_conditions_.empty()) {
        query << " WHERE ";
        for (size_t i = 0; i < where_conditions_.size(); ++i) {
            query << where_conditions_[i].first << "=" << where_conditions_[i].second;
            if (i < where_conditions_.size() - 1) {
                query << " AND ";
            }
        }
    }

    query << ";";
    return query.str();
}

void SqlSelectQueryBuilder::Clear() noexcept {
    columns_.clear();
    from_table_.clear();
    where_conditions_.clear();
}
