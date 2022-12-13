/**
 * @file points_view.cpp
 * @author UnnamedOrange
 * @brief 逐点绘制的图。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#include "points_view.h"

#include <QPainter>

points_view::points_view(
    std::shared_ptr<user::stream_to_record_base> stream_to_record,
    QWidget* parent)
    : QWidget(parent), stream_to_record(stream_to_record)
{
    ui.setupUi(this);
}

std::shared_ptr<user::record_receive_i> points_view::get_record_receive() const
{
    return local_handler;
}
std::shared_ptr<user::record_receive_i> points_view::get_record_receive_2()
    const
{
    return remote_handler;
}

void points_view::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制辅助线。
    painter.setClipRect(margin, margin, width() - margin * 2,
                        height() - margin * 2);
    painter.setPen(QPen(expired() ? Qt::lightGray : Qt::gray, 1));
    painter.drawLine(0, height() / 2, width(), height() / 2);
    // 绘制图像。
    constexpr auto transform_x = [&](size_t x) -> qreal {
        return margin + x * point_interval;
    };
    auto transform_y = [&](int32_t y) -> qreal {
        return height() / 2 - y * point_interval / 4.0;
    };
    painter.setClipRect(margin, margin, width() - margin * 2,
                        height() - margin * 2);
    painter.setPen(QPen(expired() ? Qt::lightGray : Qt::red, data_line_width));
    for (size_t i = 1; i < points.size(); i++)
    {
        painter.drawLine(QPoint(transform_x(i - 1), transform_y(points[i - 1])),
                         QPoint(transform_x(i), transform_y(points[i])));
    }
    // 绘制坐标轴。
    painter.setClipRect(event->rect());
    painter.setPen(
        QPen(expired() ? Qt::lightGray : Qt::black, axis_line_width));
    painter.drawRect(margin, margin, width() - 2 * margin,
                     height() - 2 * margin);
    // 绘制文字。
    painter.setClipRect(event->rect());
    painter.setPen(QPen(expired() ? Qt::lightGray : Qt::black, 1));
    painter.drawText(event->rect(), QString::number(per_second));
}

size_t points_view::capacity() const
{
    return (width() - margin * 2) / point_interval;
}
bool points_view::expired() const noexcept
{
    return stream_to_record.expired();
}
