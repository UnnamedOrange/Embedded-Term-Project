/**
 * @file owner_compare.hpp
 * @author UnnamedOrange
 * @brief Compare the owners of two smart pointers.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

namespace utils
{
    /**
     * @brief Whether the owners of two smart pointers equal.
     */
    template <typename T1, typename T2>
    inline bool owner_equal(const T1& a, const T2& b)
    {
        return !a.owner_before(b) && !b.owner_before(a);
    }
} // namespace utils
