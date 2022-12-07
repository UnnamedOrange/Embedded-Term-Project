/**
 * @file record_writer_base.h
 * @author UnnamedOrange
 * @brief 数据写入器基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>

#include "record_receive_i.h"
#include <utils/system_clock.hpp>

namespace user
{
    /**
     * @brief 数据写入器基类。
     *
     * @todo 设计成员变量。
     * @todo 设计成员函数。
     */
    class record_writer_base : virtual public record_receive_i
    {
    private:
        std::ofstream ofs;

    public:
        /**
         * @brief 生成当前时间的字符串，用于文件名。
         */
        static std::string generate_time_string()
        {
            auto now = utils::system_clock::now();
            auto now_c = utils::system_clock::to_time_t(now);
            std::string time_str = std::ctime(&now_c);
            time_str.pop_back();
            for (auto it = time_str.begin(); it != time_str.end(); it++)
                if (*it == ':')
                    it = time_str.erase(it);
            return time_str;
        }

    public:
        record_writer_base()
        {
            std::filesystem::create_directory("data");
            auto file_name = "data/" + generate_time_string() + ".txt";
            ofs.open(file_name);
        }
        virtual ~record_writer_base() = default;

    protected:
        /**
         * @brief 将数据写入文件。
         */
        void write(const std::string& data) { ofs << data; }
    };
} // namespace user
