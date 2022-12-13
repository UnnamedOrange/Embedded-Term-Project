/**
 * @file data_source_dummy.h
 * @author UnnamedOrange
 * @brief 测试用虚拟数据源。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <random>
#include <string>

#include "data_source_base.h"
#include "data_source_type.h"
#include "data_source_utility.h"
#include <utils/system_clock.hpp>

namespace user
{
    /**
     * @brief 测试用虚拟数据源。
     * 每过 100ms 生成 4 字节数据。
     */
    class data_source_dummy : public data_source_base
    {
    public:
        using clock_t = utils::system_steady_clock;
        /**
         * @brief 每次可读数据的间隔时间。
         */
        static constexpr clock_t::duration interval = 100ms;

    private:
        std::random_device rd;
        std::default_random_engine engine{rd()};
        std::uniform_int_distribution<int32_t> int32_dist{-64, 63};
        std::array<clock_t::time_point, 256> last_time_point{};

    public:
        data_source_type type() const override
        {
            return data_source_type::dummy;
        }
        std::string name() const override { return "Dummy"; }

        byte_array_t read(size_t size, int mux_address) override
        {
            // 每经过 interval 可读取到一次新的数据。
            {
                mux_address = mux_address % last_time_point.size();
                if (clock_t::now() - last_time_point[mux_address] < interval)
                    return {};
                last_time_point[mux_address] = clock_t::now();
            }

            assert(size == 4);
            byte_array_t ret(size);
            *reinterpret_cast<int32_t*>(ret.data()) = int32_dist(engine);
            return ret;
        }

        void write(const byte_array_t&) override {}
    };
} // namespace user
