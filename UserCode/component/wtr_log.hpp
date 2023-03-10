/**
 * @file wtr_log.hpp
 * @author X. Y.
 * @brief WTR 日志库，可以方便地打印到标准输出流
 * @note 用法：
 *         打印信息：
 *              Log(1) << "Hello " << 666 << std::endl;
 *         打印带有当前代码行号的信息：
 *              LogWithLine(1) << "Hello " << 666 << std::endl;
 * @version 0.1
 * @date 2022-12-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <stdio.h>
#include <sstream>

namespace wtr
{
    enum LogLevel {
        Debug = 0,
        Info,
        Warning,
        Error
    };

    class LOG
    {
    private:
        int _min_output_loglevel = LogLevel::Debug;  // 打印输出的等级范围
        int _max_output_loglevel = LogLevel::Error; // 打印输出的等级范围

        LogLevel _log_level; // log 等级

        void PutString(const std::string &str)
        {
            if (_log_level >= _min_output_loglevel && _log_level <= _max_output_loglevel) {
                printf(str.c_str());
            }
        }

        void PutNewLine()
        {
            if (_log_level >= _min_output_loglevel && _log_level <= _max_output_loglevel) {
                putchar('\n');
            }
        }

    public:
        LOG(LogLevel log_level = LogLevel::Info)
            : _log_level(log_level){};

        LOG &operator<<(const std::stringstream &input)
        {
            PutString(input.str());
            return *this;
        }

        LOG &operator<<(const std::string &input)
        {
            PutString(input);
            return *this;
        }

        // // 为了可以调用 << endl,  重载 <<（endl类型的函数指针)
        LOG &operator<<(std::ostream &(*op)(std::ostream &))
        {
            (void)op;
            PutNewLine();
            return *this;
        }

        LOG &operator<<(const int &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const unsigned &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const long &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const unsigned long &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const long long &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const unsigned long long &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const float &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const double &input)
        {
            PutString(std::to_string(input));
            return *this;
        }

        LOG &operator<<(const long double &input)
        {
            PutString(std::to_string(input));
            return *this;
        }
    };
} // namespace wtr

static inline wtr::LOG wtrDebug()
{
    wtr::LOG wtrLog(wtr::LogLevel::Debug);
    return wtrLog;
}

static inline wtr::LOG wtrInfo()
{
    wtr::LOG wtrLog(wtr::LogLevel::Info);
    return wtrLog;
}

static inline wtr::LOG wtrWarning()
{
    wtr::LOG wtrLog(wtr::LogLevel::Warning);
    return wtrLog;
}

static inline wtr::LOG wtrError()
{
    wtr::LOG wtrLog(wtr::LogLevel::Error);
    return wtrLog;
}

#define wtrDebugLine()                      \
    printf("%s:%d:\t", __FILE__, __LINE__); \
    wtr::LOG wtrLog(wtr::LogLevel::Debug);  \
    wtrLog
#define wtrInfoLine()                       \
    printf("%s:%d:\t", __FILE__, __LINE__); \
    wtr::LOG wtrLog(wtr::LogLevel::Info);   \
    wtrLog
#define wtrWarningLine()                     \
    printf("%s:%d:\t", __FILE__, __LINE__);  \
    wtr::LOG wtrLog(wtr::LogLevel::Warning); \
    wtrLog

#define wtrErrorLine()                      \
    printf("%s:%d:\t", __FILE__, __LINE__); \
    wtr::LOG wtrLog(wtr::LogLevel::Error);  \
    wtrLog
