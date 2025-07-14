#ifndef SQL_QUERY_BUILDER_H
#define SQL_QUERY_BUILDER_H

#include <string>
#include <vector>
#include <map>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept;
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept;
    SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept;
    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept;
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept;
    virtual std::string BuildQuery() const;
    virtual void Clear() noexcept;

protected:
    std::vector<std::string> columns_;
    std::string from_table_;
    std::vector<std::pair<std::string, std::string>> where_conditions_;
};

#endif
