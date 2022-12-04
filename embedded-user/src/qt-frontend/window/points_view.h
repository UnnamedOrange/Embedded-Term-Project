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
#include <cstdint>
#include <deque>
#include <memory>

#include <QPaintEvent>
#include <QWidget>

#include <stream_to_record/record_receive_i.h>
#include <stream_to_record/stream_to_record_base.h>

class points_view : public QWidget
{
    Q_OBJECT

private:
    class record_receive_i : public user::record_receive_i
    {
    private:
        points_view& parent;

    public:
        record_receive_i(points_view& parent) : parent(parent) {}

    public:
        void record_receive(const std::any& record) override
        {
            // 将 record 看作 32 位有符号整数。
            // TODO: 支持自定义显示。
            const auto& value = std::any_cast<const int32_t&>(record);
            parent.points.push_back(value);
            while (parent.points.size() > parent.capacity())
                parent.points.pop_front();
            parent.repaint();
        }
    };
    friend record_receive_i;

private:
    Ui::points_view ui;

private:
    std::shared_ptr<record_receive_i> record_receive{
        std::make_shared<record_receive_i>(*this)};
    std::weak_ptr<user::stream_to_record_base> stream_to_record;

private:
    std::deque<int32_t> points;

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
