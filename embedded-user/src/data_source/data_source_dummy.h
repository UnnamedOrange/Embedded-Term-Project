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
#include <utils/system_clock.hpp>

namespace user
{
    /**
     * @brief 测试用虚拟数据源。
     */
    class data_source_dummy : public data_source_base
    {
    public:
        using clock_t = utils::system_steady_clock;
        /**
         * @brief 每次读取的数据流的大小。
         */
        static constexpr size_t size_per_read = 20;
        /**
         * @brief 每次可读数据的间隔时间。
         */
        static constexpr clock_t::duration interval = 100ms;

    private:
        std::random_device rd;
        std::default_random_engine engine{rd()};
        std::uniform_int_distribution<unsigned> byte_dist{0, 255};
        clock_t::time_point last_time_point{clock_t::now()};

    public:
        data_source_type type() const override
        {
            return data_source_type::dummy;
        }

        byte_array_t read() override
        {
            // 每经过 interval 可读取到一次新的数据。
            {
                if (clock_t::now() - last_time_point < interval)
                    return {};
                last_time_point = clock_t::now();
            }

            byte_array_t ret(size_per_read);
            for (auto& byte : ret)
                byte = static_cast<byte_t>(engine());
            return ret;
        }
    };
} // namespace user
