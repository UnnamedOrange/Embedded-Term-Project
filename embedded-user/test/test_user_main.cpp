/**
 * @file test_user_main.cpp
 * @author UnnamedOrange
 * @brief `user_main` 相关单元测试。
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
 * @brief 测试 `user_main` 能否独立工作。
 */
TEST(user_main, dummy)
{
    using namespace user;

    // 创建对象。
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

    // 定义相关常量。
    constexpr size_t expected_batch = 10;
    constexpr size_t expected_elements =
        expected_batch * source_to_stream_dummy::size_per_read / 4;

    // 启动核心模块并绑定，读取一定次数数据。
    {
        user_main main_module;
        {
            auto dummy_source = std::make_shared<data_source_dummy>();
            main_module.register_object(dummy_source);
            auto dummy_source_to_stream =
                std::make_shared<source_to_stream_dummy>();
            main_module.register_object(dummy_source_to_stream);
            auto dummy_stream_to_record =
                std::make_shared<stream_to_record_dummy>();
            main_module.register_object(dummy_stream_to_record);

            dummy_stream_to_record->set_source_to_stream(
                dummy_source_to_stream);
            dummy_stream_to_record->register_interface(dummy_receive);
            main_module.bind(dummy_source, dummy_source_to_stream);
        }

        // 延时相应次数对应的时间。
        std::this_thread::sleep_for((expected_batch - 0.5) *
                                    data_source_dummy::interval);

        main_module.read_all();
    }

    // 检查读入的字节数是否符合期望。
    EXPECT_EQ(dummy_receive->n_int32_received, expected_elements);
}
