/**
 * @file data_source_utility.h
 * @author UnnamedOrange
 * @brief 数据源类的辅助文件。定义类型。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <cstdint>
#include <vector>

namespace user
{
    /**
     * @brief 字节类型。
     * 
     * @todo 考虑时间戳的记录。
     */
    using byte_t = uint8_t;
    /**
     * @brief 字节流类型。
     */
    using byte_array_t = std::vector<byte_t>;
} // namespace user
