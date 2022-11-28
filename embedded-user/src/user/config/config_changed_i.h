/**
 * @file config_changed_i.h
 * @author UnnamedOrange
 * @brief 配置被更改接口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "config_struct.h"

namespace user
{
    /**
     * @brief 配置被更改接口。
     *
     * @note 接口需要虚继承。
     */
    class config_changed_i
    {
    public:
        /**
         * @brief 当配置被修改时，该函数将会被配置管理器所在线程调用。
         *
         * @param new_config 新的配置。
         */
        virtual void config_changed(const config_struct& new_config) = 0;
    };
} // namespace user
