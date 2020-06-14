/***********************************************************************
 *
 * Copyright (C) 2014-2018 wereturtle
 * Copyright (C) 2009, 2010, 2011, 2012, 2013, 2014 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "ThemePreviewer.h"

#include <QPainter>
#include <QPixmap>
#include <QImage>

ThemePreviewer::ThemePreviewer(const Theme& theme, int width, int height)
{
    this->width = width;
    this->height = height;

    renderPreview(theme);
}

ThemePreviewer::~ThemePreviewer()
{
    ;
}

QIcon ThemePreviewer::getIcon()
{
    return thumbnailPreviewIcon;
}

void ThemePreviewer::renderPreview(const Theme& newSettings)
{
    this->theme = newSettings;

    QPixmap thumbnailPixmap(this->width, this->height);
    QPainter painter(&thumbnailPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // First, draw the editor "background".
    painter.fillRect
    (
        thumbnailPixmap.rect(),
        theme.getBackgroundColor()
    );

    // Now draw a circle in the background for each of the text colors.
    int w = 2 * width / 3;
    int h = height / 4;
    int x = (width / 3) - 3;
    int y = ((3 * height) / 4) - 3;
    int radius = (h / 4);
    int xoffset = w / 3;
    int cx1 = x + (xoffset / 2);
    int cx2 = cx1 + xoffset;
    int cx3 = cx2 + xoffset;
    int cy = y + (h / 2);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(theme.getDefaultTextColor()));
    painter.drawEllipse(QPoint(cx1, cy), radius, radius);
    painter.setBrush(QBrush(theme.getMarkupColor()));
    painter.drawEllipse(QPoint(cx2, cy), radius, radius);
    painter.setBrush(QBrush(theme.getLinkColor()));
    painter.drawEllipse(QPoint(cx3, cy), radius, radius);

    // Draw icon to indicate if theme is built-in
    if (theme.isBuiltIn())
    {
        w = width / 10;
        h = w;
        x = w / 4;
        y = x;

        painter.drawImage
        (
            x,
            y,
            QImage(":/resources/images/ghostwriter.svg").scaled
            (
                QSize(w, h),
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation
            )
        );
    }
    painter.end();
    thumbnailPreviewIcon = QIcon(thumbnailPixmap);
}
