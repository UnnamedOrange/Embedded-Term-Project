/**
 * @file def_reflection.h
 * @author UnnamedOrange
 * @brief 数据流裁剪器-记录提取器的反射表。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

{
    user::data_source_type::dummy,
    {
        {"dummy", make_generator<user::source_to_stream_dummy, user::stream_to_record_dummy>()},
    }
},
