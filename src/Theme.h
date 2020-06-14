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

#ifndef THEME_H
#define THEME_H

#include <QMap>
#include <QString>

#include "Color.h"

enum TextWidth
{
    TextWidthNarrow,
    TextWidthMedium,
    TextWidthWide
};

/**
 * Encapsulates a theme for customizing the look and feel of the application.
 */
class Theme
{
    public:
        Theme();
        Theme(const Theme& other);
        Theme(const QString& name, bool builtIn = true);
        ~Theme();

        Theme& operator=(const Theme& other);

        QString getName() const;
        void setName(const QString& value);

        bool isBuiltIn() const;
        void setBuiltIn(const bool builtIn);

        static QStringList getColorKeys();

        Color getColorByKey(const QString& key);
        void setColorByKey(const QString& key, const Color& value);

        Color getDefaultTextColor() const;
        void setDefaultTextColor(const Color& value);

        Color getBackgroundColor() const;
        void setBackgroundColor(const Color& value);

        Color getMarkupColor() const;
        void setMarkupColor(const Color& value);

        Color getLinkColor() const;
        void setLinkColor(const Color& value);

        Color getHeadingColor() const;
        void setHeadingColor(const Color& value);

        Color getEmphasisColor() const;
        void setEmphasisColor(const Color& value);

        Color getBlockquoteColor() const;
        void setBlockquoteColor(const Color& value);

        Color getCodeColor() const;
        void setCodeColor(const Color& value);

        Color getSpellingErrorColor() const;
        void setSpellingErrorColor(const Color& value);

    private:
        const static QStringList keyArray;
        QString name;

        // true if theme is built-in, false if custom (i.e., user-created).
        bool builtInFlag;

        QMap<QString, Color> colors;
};

#endif
