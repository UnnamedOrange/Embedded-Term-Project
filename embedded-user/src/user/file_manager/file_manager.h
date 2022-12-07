/**
 * @file file_manager.h
 * @author UnnamedOrange
 * @brief 数据文件管理器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace user
{
    /**
     * @brief 数据文件管理器。
     */
    class file_manager
    {
    public:
        /**
         * @brief 设定的工作目录。
         */
        static constexpr std::string_view working_dir = "data";

    public:
        /**
         * @brief 获取文件列表。文件名之间用换行符分隔，保证结尾是空行。
         */
        std::string file_list() const
        {
            using namespace std::filesystem;
            std::string ret;
            for (auto& p : directory_iterator(working_dir))
            {
                if (p.is_regular_file())
                {
                    ret += p.path().filename().string();
                    ret += '\n';
                }
            }
            return ret;
        }
        /**
         * @brief 获取文件内容。文件不存在则返回空字符串。
         *
         * @param file_name @ref working_dir "工作目录" 下的文件名。
         */
        std::string file_content(const std::string& file_name) const
        {
            using namespace std::filesystem;
            std::string ret;
            path file_path(working_dir);
            file_path /= file_name;
            if (exists(file_path))
            {
                std::ifstream file(file_path);
                std::getline(file, ret, '\0');
            }
            return ret;
        }
    };
} // namespace user
