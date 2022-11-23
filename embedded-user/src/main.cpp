/**
 * @file main.cpp
 * @author UnnamedOrange
 * @brief 嵌入式软件模块单独运行的入口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <thread>

#include <user_main.h>

int main()
{
    using namespace user;
    user_main user_main_module;
}
