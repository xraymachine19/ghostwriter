/***********************************************************************
 *
 * Copyright (C) 2014-2020 wereturtle
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

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QTextEdit>

const QString ThemePreviewer::loremIpsum = 
    "<p><h2><strong>"
    "<font color='@markup'># </font>"
    "<font color='@strong'>Lorem ipsum</font></strong></h2></p>"
    "<p>Lorem ipsum "
    "<em>"
    "<font color='@markup'>*</font>"
    "<font color='@emph'>dolor</font>"
    "<font color='@markup'>*</font>"
    "</em> "
    "sit amet, "
    "<strong>"
    "<font color='@markup'>**</font>"
    "<font color='@strong'>consectetur</font>"
    "<font color='@markup'>**</font>"
    "</strong> "
    "adipiscing "
    "<font color='@link'>[elit][^1]</font>. "
    "Etiam "
    "<font color='@markup'>`</font>"
    "<font color='@code'>aliquam</font>"
    "<font color='@markup'>`</font>, "
    "diam.</p>";

ThemePreviewer::ThemePreviewer(const Theme& theme, int width, int height, qreal dpr)
{
    QString text = loremIpsum;

    text.replace("@markup", theme.getMarkupColor().name());
    text.replace("@emph", theme.getEmphasisColor().name());
    text.replace("@strong", theme.getEmphasisColor().name());
    text.replace("@link", theme.getLinkColor().name());
    text.replace("@code", theme.getCodeColor().name());
 
    QTextEdit textEdit;
    textEdit.setHtml(text);
    textEdit.setFrameStyle(QFrame::NoFrame);
	textEdit.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textEdit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit.setFixedSize(width, height);
    textEdit.setStyleSheet
    (
        "QTextEdit { color: "
        + theme.getDefaultTextColor().toRgbString() 
        + "; background-color: " 
        + theme.getBackgroundColor().toRgbString() 
        + "; font-family: monospace; font-size: 14pt }"
    );

    QPixmap thumbnailPixmap(width * dpr, height * dpr);
    thumbnailPixmap.setDevicePixelRatio(dpr);
    QPainter painter(&thumbnailPixmap);
    painter.setRenderHints(QPainter::Antialiasing);
    textEdit.render(&painter);
    
    // Draw icon to indicate if theme is built-in
    if (theme.isBuiltIn())
    {
        int w = width / 10;
        int h = w;
        int x = width - w - 2;
        int y = height - h - 2;

        QSvgRenderer renderer(QString(":/resources/images/ghostwriter.svg"));
        QImage builtInIcon(w * dpr, h * dpr, QImage::Format_ARGB32);
        builtInIcon.setDevicePixelRatio(dpr);
        builtInIcon.fill(Qt::transparent);
        renderer.render(&painter, QRectF(x, y, w, h));
        
    }

    painter.end();
    thumbnailPreviewIcon = thumbnailPixmap;
}

ThemePreviewer::~ThemePreviewer()
{
    ;
}

QIcon ThemePreviewer::getIcon()
{
    return thumbnailPreviewIcon;
}