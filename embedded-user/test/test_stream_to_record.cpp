/**
 * @file test_stream_to_record.cpp
 * @author UnnamedOrange
 * @brief 记录提取器相关单元测试。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <any>
#include <chrono>

#include <gtest/gtest.h>

#include <data_source/data_source_dummy.h>
#include <source_to_stream/source_to_stream_dummy.h>
#include <stream_to_record/record_receive_i.h>
#include <stream_to_record/stream_to_record_dummy.h>
#include <user_main.h>

using namespace std::literals;

/**
 * @brief 测试记录提取器能否配合前面的组件正常工作。
 */
TEST(stream_to_record, dummy)
{
    using namespace user;

    // 创建并绑定对象。
    auto dummy_source = std::make_shared<data_source_dummy>();
    auto dummy_source_to_stream = std::make_shared<source_to_stream_dummy>();
    auto dummy_stream_to_record = std::make_shared<stream_to_record_dummy>();
    dummy_stream_to_record->set_source_to_stream(dummy_source_to_stream);

    class dummy_receive_t : public record_receive_i
    {
    public:
        size_t n_int32_received{};

    public:
        void record_receive(const std::any&) override
        {
            // 已知类型一定为 record_dummy_t。

            // 令计数器自增即可。
            n_int32_received++;
        }
    };
    auto dummy_receive = std::make_shared<dummy_receive_t>();
    dummy_stream_to_record->register_interface(dummy_receive);

    // 定义相关常量。
    constexpr size_t expected_batch = 10;
    constexpr size_t expected_elements =
        expected_batch * source_to_stream_dummy::size_per_read / 4;

    // 启动核心模块并绑定，读取一定次数数据。
    {
        user_main main_module;
        main_module.bind(dummy_source, dummy_source_to_stream);

        // 延时相应次数对应的时间。
        std::this_thread::sleep_for((expected_batch - 0.5) *
                                    data_source_dummy::interval);
    }
    // 测试核心模块的正常析构。

    // 手动控制 stream_to_record 读取。
    dummy_stream_to_record->read_all();

    // 检查读入的字节数是否符合期望。
    EXPECT_EQ(dummy_receive->n_int32_received, expected_elements);
}
