#ifndef _H_DATABASE
#define _H_DATABASE

#include <memory>
#include <optional>
#include <string>

#include "sqlite3.h"

namespace invoice_maker::DB {

int connect();
int destroy();

struct BLOB {
    size_t n_bytes;
    const void *data;
};

class Statement {
   private:
    sqlite3_stmt *stmt = nullptr;
    int error = 0;
    bool is_executed = false;

   public:
    bool next();
    inline bool is_error() const { return this->error != SQLITE_DONE; }
    const char *error_message() const;

    std::string get_sql() const;

    int64_t get_int(int i_col) const;
    double get_float(int i_col) const;
    std::string get_string(int i_col) const;
    BLOB get_blob(int i_col) const;

    int bind(const std::string &param_name, sqlite3_int64 value) const;
    int bind(const std::string &param_name, double value) const;
    int bind(const std::string &param_name, const std::string &value) const;

    int bind(int i_param, sqlite3_int64 value) const;
    int bind(int i_param, double value) const;
    int bind(int i_param, const std::string &value) const;

    explicit Statement(int error_) : error(error_) {}
    explicit Statement(sqlite3_stmt *stmt_) : stmt(stmt_) {}

    ~Statement();
};

std::unique_ptr<Statement> query(const std::string &sql);
bool table_exists(const std::string &table_name);

}  // namespace invoice_maker::DB

#endif  // !_H_DATABASE
