#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#endif // UTILS_H
#include "database.h"

namespace matthew{
    namespace utils{
        bool is_number(const std::string &s);
        std::optional<uint64_t> verifyId(std::string id);
        std::optional<varchar_32> verifyName(std::string name);
        std::optional<varchar_32> verifySurname(std::string surname);
        std::optional<int> verifyIsBudget(std::string isbudget);
        std::optional<uint32_t> verifyCourse(std::string course);
        std::optional<Student> verifyRecord(std::string str);
        bool strContainsChar(std::string str, char symbol);
        bool writeRecordsToXLSX(std::string records_string);
    }
}
