/**
 * @file points_view.h
 * @author UnnamedOrange
 * @brief 逐点绘制的图。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#pragma once

#include "ui_points_view.h"

#include <any>
#include <array>
#include <chrono>
#include <cstdint>
#include <deque>
#include <memory>

#include <QPaintEvent>
#include <QWidget>

#include <stream_to_record/record_receive_i.h>
#include <stream_to_record/stream_to_record_base.h>
#include <utils/system_clock.hpp>

class points_view : public QWidget
{
    Q_OBJECT

private:
    class local_handler_i : virtual public user::record_receive_i
    {
    private:
        points_view& parent;

    private:
        std::array<utils::system_steady_clock::time_point, 10> time_points{};
        size_t crt_idx{};

    public:
        local_handler_i(points_view& parent) : parent(parent) {}

    public:
        void record_receive(const std::any& record) override
        {
            // 记录时间并计算速率。
            auto time_point = utils::system_steady_clock::now();
            auto time_span = time_point - time_points[crt_idx];
            time_points[crt_idx] = time_point;
            crt_idx++;
            if (crt_idx >= time_points.size())
                crt_idx = 0;
            using seconds = std::chrono::duration<double>;
            double per_second =
                time_points.size() /
                std::chrono::duration_cast<seconds>(time_span).count();
            parent.per_second = per_second;

            // 将 record 看作 32 位有符号整数。
            // TODO: 支持自定义显示。
            const auto& value = std::any_cast<const int32_t&>(record);
            parent.points.push_back(value);
            while (parent.points.size() > parent.capacity())
                parent.points.pop_front();
            parent.update();
        }
    };
    friend local_handler_i;

private:
    Ui::points_view ui;

private:
    std::shared_ptr<local_handler_i> local_handler{
        std::make_shared<local_handler_i>(*this)};
    std::weak_ptr<user::stream_to_record_base> stream_to_record;

private:
    std::deque<int32_t> points;
    double per_second{};

    // 绘图参数。
private:
    static constexpr qreal margin = 20.0;
    static constexpr qreal axis_line_width = 2.0;
    static constexpr qreal data_line_width = 1.0;
    static constexpr qreal point_interval = 4.0;

public:
    std::shared_ptr<user::record_receive_i> get_record_receive() const;

public:
    points_view(std::shared_ptr<user::stream_to_record_base> stream_to_record,
                QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    size_t capacity() const;
    bool expired() const noexcept;
};
