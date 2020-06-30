/***********************************************************************
 *
 * Copyright (C) 2014-2020 wereturtle
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

#include <QColor>
#include <QString>

#include "Color.h"

Color::Color(const QColor& color) : QColor(color)
{
    ;
}

QTextStream& operator<<(QTextStream& output, const Color& color) 
{ 
    output << color.toRgbString();
    return output;
}

Color Color::applyAlpha(const Color& background) const
{
    Color blendedColor(0, 0, 0);

    blendedColor.setRed
    (
        applyAlphaToChannel
        (
            this->red(),
            background.red(),
            this->alphaF()
        )
    );

    blendedColor.setGreen
    (
        applyAlphaToChannel
        (
            this->green(),
            background.green(),
            this->alphaF()
        )
    );

    blendedColor.setBlue
    (
        applyAlphaToChannel
        (
            this->blue(),
            background.blue(),
            this->alphaF()
        )
    );

    return blendedColor;
}

Color Color::applyAlpha
(
    const Color& background,
    int alpha
) const
{
    if ((alpha < 0) || alpha > 255)
    {
        qCritical("ColorHelper::applyAlpha: alpha value must be "
            "between 0 and 255. Value provided = %d",
            alpha);
    }

    Color blendedColor(0, 0, 0);
    qreal normalizedAlpha = alpha / 255.0;

    blendedColor.setRed
    (
        applyAlphaToChannel
        (
            this->red(),
            background.red(),
            normalizedAlpha
        )
    );

    blendedColor.setGreen
    (
        applyAlphaToChannel
        (
            this->green(),
            background.green(),
            normalizedAlpha
        )
    );

    blendedColor.setBlue
    (
        applyAlphaToChannel
        (
            this->blue(),
            background.blue(),
            normalizedAlpha
        )
    );

    return blendedColor;
}


QString Color::toRgbString() const
{
    return QString("rgb(%1, %2, %3)")
        .arg(this->red())
        .arg(this->green())
        .arg(this->blue());
}

QString Color::toRgbaString() const
{
    return QString("rgba(%1, %2, %3, %4)")
        .arg(this->red())
        .arg(this->green())
        .arg(this->blue())
        .arg(this->alpha());
}

// Algorithm taken from *Grokking the GIMP* by Carey Bunks,
// section 5.3.
//
// Grokking the GIMP
// by Carey Bunks
// Copyright (c) 2000 by New Riders Publishing, www.newriders.com
// ISBN 0-7357-0924-6.
//
double Color::getLuminance() const
{
    Color c = *this;

    // Ensure color is non-zero.
    if (c == Color(Qt::black))
    {
        c.setRgb(1, 1, 1);
    }

    return (0.30 * c.redF()) + (0.59 * c.greenF()) + (0.11 * c.blueF());
}

Color Color::lightenToMatchContrastRatio
(
    const Color& background,
    double contrastRatio
) const
{
    double fgBrightness = this->getLuminance();
    double bgBrightness = background.getLuminance();

    // If the background color is brighter than the foreground color...
    if (bgBrightness > fgBrightness)
    {
        double actualContrastRatio = bgBrightness / fgBrightness;
        double colorFactor = contrastRatio / actualContrastRatio;

        Color result = *this;

        // Ensure color is non-zero.
        if (result == QColor(Qt::black))
        {
            result.setRgb(1, 1, 1);
        }

        qreal r = result.redF() / colorFactor;
        qreal g = result.greenF() / colorFactor;
        qreal b = result.blueF() / colorFactor;

        if (r > 1.0)
        {
            r = 1.0;
        }

        if (g > 1.0)
        {
            g = 1.0;
        }

        if (b > 1.0)
        {
            b = 1.0;
        }

        result.setRgbF(r, g, b);

        return result;
    }
    else
    {
        // This algorithm cannot change the foreground color when it is
        // lighter than the background color, so return this color's
        // original value.
        //
        return *this;
    }
}
