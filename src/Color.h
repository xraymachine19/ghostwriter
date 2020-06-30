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

#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include <QTextStream>
#include <QString>

class Color;

QTextStream& operator<<(QTextStream& output, const Color& color);

/**
 * Helper class for dealing with colors that inherits QColor
 * to add additional functionality.
 */
class Color : public QColor
{
    public:
        using QColor::QColor;

        /**
         * Constructor taking a QColor as a parameter.
         */
        Color(const QColor& color);

        friend QTextStream& operator<<(QTextStream& output, const Color& color);

        /**
         * Applies an alpha effect to this foreground color against the
         * given background color, returning a blend of the two colors.  In
         * other words, new color returned will have this color appearing
         * to be transparent or semi-transparent as though it were colored
         * on top of the background color.  The alpha value used is from
         * this color's alpha() channel.
         */
        Color applyAlpha
        (
            const Color& background
        ) const;

        /**
         * Applies an alpha effect to this foreground color against the
         * given background color, returning a blend of the two colors.  In
         * other words, new color returned will have this color appearing
         * to be transparent or semi-transparent as though it were colored
         * on top of the background color.  The alpha value used is from
         * the alpha parameter passed in, and will be applied to all channels.
         *
         * alpha parameter must be within the range of 0 to 255.
         */
        Color applyAlpha
        (
            const Color& background,
            int alpha
        ) const;

        /**
         * Gets string representation of this color in RGB format for
         * use in style sheets.
         */
         QString toRgbString() const;

        /**
         * Gets string representation of this color in RGBA format for
         * use in style sheets.
         */
         QString toRgbaString() const;

        /**
         * Returns the luminance of this color on a scale of 0.0 (dark) to
         * 1.0 (light).  Luminance is based on how light or dark a color
         * appears to the human eye.
         */
        double getLuminance() const;

        /**
         * Returns a new color based on this foreground color,
         * such that the new color is lightened to achieve the desired
         * contrast ratio against the given background color.
         *
         * Note:
         *
         *   This method assumes that this foreground color is darker
         *   than the background color.  Passing in a background color
         *   that is darker than this color results in
         *   this original color being returned.
         */
        Color lightenToMatchContrastRatio
        (
            const Color& background,
            double contrastRatio
        ) const;

    private:
        /**
         * Helper method for applying alpha value to a single color channel.
         * It will return the new color channel value that results from
         * applying the given alpha value to the given foreground channel
         * value against the given background channel value.
         */
        static inline int applyAlphaToChannel
        (
            const int foreground,
            const int background,
            const qreal alpha
        )
        {
            return (int) ((foreground * alpha) + (background * (1.0 - alpha)));
        }
};

#endif // COLOR_H
