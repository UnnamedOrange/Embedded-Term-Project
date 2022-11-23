/**
 * @file source_to_stream_base.cpp
 * @author UnnamedOrange
 * @brief 数据流裁剪基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "source_to_stream_base.h"

using namespace user;

void source_to_stream_base::async_push(const byte_array_t& byte_array)
{
    for (const auto& byte : byte_array)
        queue.enqueue(byte);
}

std::optional<byte_t> source_to_stream_base::next_byte()
{
    byte_t ret;
    // 先返回回退的字节。
    if (!unput_bytes.empty())
    {
        ret = unput_bytes.back();
        unput_bytes.pop_back();
        return ret;
    }
    // 再尝试从队列中取出字节。
    else if (queue.try_dequeue(ret))
        return ret;
    // 下一个字节尚未到来。
    return std::nullopt;
}
void source_to_stream_base::unput(byte_t byte) { unput_bytes.push_back(byte); }
