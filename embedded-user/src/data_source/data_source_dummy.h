/**
 * @file data_source_dummy.h
 * @author UnnamedOrange
 * @brief 测试用虚拟数据源。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <cstdint>
#include <random>

#include "data_source_base.h"
#include "data_source_type.h"
#include "data_source_utility.h"

namespace user
{
    /**
     * @brief 测试用虚拟数据源。
     */
    class data_source_dummy : public data_source_base
    {
    private:
        std::random_device rd;
        std::default_random_engine engine{rd()};
        std::uniform_int_distribution<size_t> size_dist{0, 40};
        std::uniform_int_distribution<unsigned> byte_dist{0, 255};

    public:
        data_source_type type() const override
        {
            return data_source_type::dummy;
        }

        byte_array_t read() override
        {
            // 生成随机长度（0 到 40）、随机内容的字节流。
            byte_array_t ret(size_dist(engine));
            for (auto& byte : ret)
                byte = static_cast<byte_t>(engine());
            return ret;
        }
    };
} // namespace user
