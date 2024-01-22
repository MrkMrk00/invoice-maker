#include "database.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

#include "sqlite3.h"

namespace invoice_maker::DB {
static sqlite3 *CONNECTION = NULL;

int connect() {
    int flags =
        SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;

    int result_code = sqlite3_open_v2(u8"Database", &CONNECTION, flags, NULL);

    if (result_code) {
        fprintf(stderr, "DB Error: %s\n", sqlite3_errmsg(CONNECTION));

        return result_code;
    }

    return 0;
}

int destroy() {
    if (!CONNECTION) {
        return 0;
    }

    return sqlite3_close(CONNECTION);
}

bool Statement::next() {
    if (this->error != 0) {
        return false;
    }

    int code = sqlite3_step(this->stmt);
    this->is_executed = true;

    if (code != SQLITE_ROW) {
        this->error = code;

        return false;
    }

    return true;
}

const char *Statement::error_message() const {
    if (this->is_error()) {
        return sqlite3_errstr(this->error);
    }

    return "";
}

std::unique_ptr<Statement> query(const std::string &sql) {
    sqlite3_stmt *stmt = NULL;
    const char *tail_ptr = NULL;

    int code = sqlite3_prepare_v3(CONNECTION, sql.c_str(), sql.size(), 0, &stmt,
                                  &tail_ptr);

    if (code != SQLITE_OK) {
        fprintf(stderr, "SQLite Error %d (%s): %s\n", code,
                sqlite3_errstr(code), sqlite3_errmsg(CONNECTION));

        return std::make_unique<Statement>(code);
    }

    return std::make_unique<Statement>(stmt);
}

int64_t Statement::get_int(unsigned int i_col) const {
    return sqlite3_column_int64(this->stmt, i_col);
}

double Statement::get_float(unsigned int i_col) const {
    return sqlite3_column_double(this->stmt, i_col);
}

std::string Statement::get_string(unsigned int i_col) const {
    return std::string{(const char *)sqlite3_column_text(this->stmt, i_col)};
}

BLOB Statement::get_blob(unsigned int i_col) const {
    return {
        .n_bytes = static_cast<size_t>(sqlite3_column_bytes(this->stmt, i_col)),
        .data = sqlite3_column_blob(this->stmt, i_col),
    };
}

int Statement::bind(unsigned int i_param, sqlite3_int64 value) const {
    assert(i_param > 0);

    return sqlite3_bind_int64(this->stmt, i_param, value);
}

int Statement::bind(unsigned int i_param, double value) const {
    assert(i_param > 0);

    return sqlite3_bind_double(this->stmt, i_param, value);
}

int Statement::bind(unsigned int i_param, const std::string &value) const {
    assert(i_param > 0);

    return sqlite3_bind_text(this->stmt, i_param, value.c_str(), value.size(),
                             SQLITE_STATIC);
}

int Statement::bind(const std::string &param_name, sqlite3_int64 value) const {
    int i_param = sqlite3_bind_parameter_index(this->stmt, param_name.c_str());

    return this->bind(i_param, value);
}

int Statement::bind(const std::string &param_name, double value) const {
    int i_param = sqlite3_bind_parameter_index(this->stmt, param_name.c_str());

    return this->bind(i_param, value);
}

int Statement::bind(const std::string &param_name,
                    const std::string &value) const {
    int i_param = sqlite3_bind_parameter_index(this->stmt, param_name.c_str());

    return this->bind(i_param, value);
}

Statement::~Statement() {
    std::cout << "Deleting statement (" << this->get_sql() << ")\n"
              << std::endl;

    int code = sqlite3_finalize(this->stmt);
    if (code != SQLITE_OK) {
        fprintf(stderr, "Finalize statement error %d: %s", code,
                sqlite3_errstr(code));
    }
}

std::string Statement::get_sql() const {
    return sqlite3_expanded_sql(this->stmt);
}

bool table_exists(const std::string &table_name) {
    auto stmt = query("SELECT name FROM pragma_table_list WHERE name = ?;");
    stmt->bind(1, table_name);

    return stmt->next();
}

}  // namespace invoice_maker::DB
