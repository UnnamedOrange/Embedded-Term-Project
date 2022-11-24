/**
 * @file system_clock.hpp
 * @author UnnamedOrange
 * @brief Define the clocks used in the system.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <chrono>

// Enabling chrono literals casually is OK.
using namespace std::chrono_literals;

namespace utils
{
    /**
     * @brief Steady clock used in the system.
     */
    using system_steady_clock = std::chrono::steady_clock;
} // namespace utils
