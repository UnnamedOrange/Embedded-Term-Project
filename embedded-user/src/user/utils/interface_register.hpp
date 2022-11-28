/**
 * @file interface_register.hpp
 * @author UnnamedOrange
 * @brief Holding registered interfaces.
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <memory>
#include <vector>

#include "owner_compare.hpp"

namespace utils
{
    /**
     * @brief Holding registered interfaces.
     */
    template <typename interface_t,
              typename _weak_t = std::weak_ptr<interface_t>,
              typename _shared_t = std::shared_ptr<interface_t>>
    class interface_register : public std::vector<_weak_t>
    {
    public:
        /**
         * @brief Weak pointer of the interface type.
         */
        using weak_t = _weak_t;
        /**
         * @brief Shared pointer to the interface type.
         */
        using shared_t = _shared_t;

    public:
        /**
         * @brief Unregister a specific interface once.
         * If the interface is not registered, do nothing.
         */
        void unregister_once(const shared_t& interface)
        {
            for (auto it = this->begin(); it != this->end(); ++it)
                if (owner_equal(interface, *it))
                {
                    this->erase(it);
                    return;
                }
        }
        /**
         * @brief Unregister all of a specific interface.
         * If the interface is not registered, do nothing.
         */
        void unregister_all(const shared_t& interface)
        {
            this->erase_if([&interface](const weak_t& weak_interface) {
                return owner_equal(interface, weak_interface);
            });
        }
        /**
         * @brief Unregister all expired interfaces.
         */
        void clear_expired()
        {
            this->erase_if([](const weak_t& weak_interface) {
                return weak_interface.expired();
            });
        }
    };
} // namespace utils
