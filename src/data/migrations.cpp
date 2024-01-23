#include "../data.hpp"
#include "../database.hpp"

namespace invoice_maker::data {

void migrate() {
    {
        if (!DB::table_exists("bank_accounts")) {
            auto bank_accounts = DB::query(R"(
                CREATE TABLE bank_accounts (
                    id              BIGINT UNSIGNED AUTO INCREMENT PRIMARY KEY,
                    number          VARCHAR(255) NOT NULL,
                    iban            VARCHAR(255) NULL,
                    bic             VARCHAR(31)  NULL,
                    bank_name       VARCHAR(255) NULL,
                    owner_name      VARCHAR(255) NOT NULL,
                    owner_surname   VARCHAR(255) NOT NULL,
                    owner_street    VARCHAR(255) NOT NULL,
                    owner_city      VARCHAR(255) NOT NULL,
                    owner_zip       VARCHAR(15)  NOT NULL,
                    owner_country   VARCHAR(63)  NOT NULL
                )
            )");

            bank_accounts->next();
        }
    }
}

}  // namespace invoice_maker::data
