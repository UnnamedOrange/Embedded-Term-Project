/**
 * @file config_manager.h
 * @author UnnamedOrange
 * @brief 配置管理器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "config_change.h"
#include "config_changed_i.h"
#include "config_struct.h"

namespace user
{
    /**
     * @brief 配置管理器。
     *
     * @todo 设计成员变量。
     * @todo 设计成员函数。
     */
    class config_manager
    {
    private:
        config_struct cfg; // 当前配置。
        std::vector<std::weak_ptr<config_changed_i>> registered_listeners;

    public:
        /**
         * @brief 注册监听配置修改的对象。
         */
        void register_listener(std::shared_ptr<config_changed_i> listener)
        {
            registered_listeners.push_back(listener);
        }
        /**
         * @brief 取消注册监听配置修改的对象。如果对象未注册，则不进行任何操作。
         */
        void unregister_receiver(std::shared_ptr<config_changed_i> listener)
        {
            for (auto it = registered_listeners.begin();
                 it != registered_listeners.end(); ++it)
                if ((*it).expired() || (*it).lock() == listener)
                {
                    registered_listeners.erase(it);
                    break;
                }
        }

    public:
        /**
         * @brief 获取当前配置的常值引用。
         */
        const config_struct& get_config() const { return cfg; }

    public:
        /**
         * @brief 根据配置修改方法修改配置。修改成功后，
         * 将会通知所有已注册的监听配置修改的对象。
         *
         * @todo 实现修改配置。
         */
        void change_config(const config_change& change)
        {
            // TODO: 实现修改配置。

            // 通知所有已注册的监听配置修改的对象。
            {
                for (auto& listener : registered_listeners)
                    if (!listener.expired())
                        listener.lock()->config_changed(cfg);
            }
        }

    public:
        /**
         * @brief 从指定文件加载配置。
         *
         * @todo 设计加载配置的异常处理。
         * @todo 实现加载配置。
         */
        void load_config(const std::filesystem::path& path) {}
        /**
         * @brief 将当前配置保存至指定文件。
         *
         * @todo 实现保存配置。
         */
        void save_config(const std::filesystem::path& path) const {}
    };

} // namespace user
