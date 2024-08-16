#ifndef UTILITY_UTIL_HPP
#define UTILITY_UTIL_HPP

#include <iostream>
#include <string>

#include <base_utils/Mem.h>
#include <tc/emh.h>

#define ITKCALL_S(func)                                                        \
{                                                                              \
    int status = func;                                                         \
    if (status != ITK_ok)                                                      \
    {                                                                          \
        char* error = nullptr;                                                 \
        EMH_ask_error_text(status, &error);                                    \
        std::string message = error;                                           \
        util::mem_free_s(error);                                               \
        throw ITKException(message, __FILE__, #func, __LINE__, status);        \
    }                                                                          \
}                                                                              \

class ITKException : public std::exception
{

private:
    std::string message;
    std::string file;
    std::string func;
    int line;
    int code;

public:
    ITKException(const std::string& message, const std::string& file,
        const std::string& func, const int line, const int code) :
        message(message), file(file), func(func), line(line), code(code)
    {

    }

    const char* what() const noexcept override
    {
        std::string full_message = "\nITK Exception: " + message + "\n"
            "at file: " + file + ", line: " + std::to_string(line) + ", call: "
            + func + ", return: " + std::to_string(code) + "\n";

        return full_message.c_str();
    }

    int getCode() const
    {
        return code;
    }

};

namespace util
{

    std::string get_parent_path(const std::string& path);

    int compare_dates(const date_t& date1, const date_t& date2);

    template <typename T>
    void mem_free_s(T*& ptr)
    {
        if (ptr)
        {
            MEM_free(ptr);
            ptr = nullptr;
        }
    }

} // namespace util

#endif // UTILITY_UTIL_HPP
