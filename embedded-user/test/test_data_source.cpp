/**
 * @file test_data_source.cpp
 * @author UnnamedOrange
 * @brief 数据源相关单元测试。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <chrono>
#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include <data_source/data_source_dummy.h>
#include <data_source/mux_controller.h>
#include <source_to_stream/source_to_stream_dummy.h>

using namespace std::literals;

/**
 * @brief 测试数据源多路复用器是否能够控制虚拟数据源。
 */
TEST(data_source, dummy_source)
{
    using namespace user;

    // 创建并绑定对象。
    auto dummy_source = std::make_shared<data_source_dummy>();
    auto dummy_source_to_stream = std::make_shared<source_to_stream_dummy>();

    // 定义相关常量。
    constexpr size_t expected_batch = 10;

    // 启动数据源多路复用控制器，读取一定次数数据。
    {
        mux_controller mux;
        mux.bind(dummy_source, dummy_source_to_stream);

        // 延时相应次数对应的时间。
        std::this_thread::sleep_for((0.5 + expected_batch) *
                                    data_source_dummy::interval);
    }
    // 测试数据源多路复用控制器的正常析构。

    // 检查读入的字节数是否符合期望。
    size_t bytes_read{};
    while (true)
    {
        auto byte_array = dummy_source_to_stream->next();
        if (!byte_array)
            break;
        bytes_read += byte_array->size();
    }
    EXPECT_EQ(bytes_read, expected_batch * data_source_dummy::size_per_read);
}
