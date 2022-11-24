/**
 * @file test_data_source.cpp
 * @author UnnamedOrange
 * @brief 数据源相关单元测试。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <memory>

#include <gtest/gtest.h>

#include <data_source/data_source_dummy.h>
#include <data_source/mux_controller.h>
#include <source_to_stream/source_to_stream_dummy.h>

/**
 * @brief 测试数据源多路复用器是否能够控制虚拟数据源。
 */
TEST(data_source, dummy_source)
{
    using namespace user;
    auto dummy_source = std::make_shared<data_source_dummy>();
    auto dummy_source_to_stream = std::make_shared<source_to_stream_dummy>();
    mux_controller mux;
    mux.bind(dummy_source, dummy_source_to_stream);
    // TODO: Read data source.
}
