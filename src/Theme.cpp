/***********************************************************************
 *
 * Copyright (C) 2015-2020 wereturtle
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

#include "Theme.h"

const QStringList Theme::keyArray
(
    QStringList()
        << "foreground"
        << "background"
        << "markup"
        << "link"
        << "heading"
        << "emphasis"
        << "blockquote"
        << "code"
        << "spelling"
);

Theme::Theme()
    : builtInFlag(false)
{

}

Theme::Theme(const Theme& other) 
{
    this->name = other.name;
    this->builtInFlag = other.builtInFlag;
    this->colors = other.colors;
}

Theme::Theme(const QString& name, bool builtIn)
    : name(name), builtInFlag(builtIn)
{
    ;
}

Theme::~Theme()
{
    ;
}

Theme& Theme::operator=(const Theme& other) 
{
    if (this != &other)
    {
        this->name = other.name;
        this->builtInFlag = other.builtInFlag;
        this->colors = other.colors;
    }

    return *this;
}

QString Theme::getName() const
{
    return name;
}

void Theme::setName(const QString& value)
{
    name = value;
}

bool Theme::isBuiltIn() const
{
    return builtInFlag;
}

void Theme::setBuiltIn(const bool builtIn)
{
    builtInFlag = builtIn;
}

QStringList Theme::getColorKeys()
{
    return keyArray;
}

Color Theme::getColorByKey(const QString& key) 
{
    return colors.value(key, Color(QColor::Invalid));
}

void Theme::setColorByKey(const QString& key, const Color& value) 
{
    colors.insert(key, value);
}

Color Theme::getDefaultTextColor() const
{
    return colors.value("foreground");
}

void Theme::setDefaultTextColor(const Color& value)
{
    colors.insert("foreground", value);
}

Color Theme::getBackgroundColor() const
{
    return colors.value("background");
}

void Theme::setBackgroundColor(const Color& value)
{
    colors.insert("background", value);
}

Color Theme::getMarkupColor() const
{
    return colors.value("markup");
}

void Theme::setMarkupColor(const Color& value)
{
    colors.insert("markup", value);
}

Color Theme::getLinkColor() const
{
    return colors.value("link");
}

void Theme::setLinkColor(const Color& value)
{
    colors.insert("link", value);
}

Color Theme::getHeadingColor() const
{
    return colors.value("heading");
}

void Theme::setHeadingColor(const Color& value)
{
    colors.insert("heading", value);
}

Color Theme::getEmphasisColor() const
{
    return colors.value("emphasis");
}

void Theme::setEmphasisColor(const Color& value)
{
    colors.insert("emphasis", value);
}

Color Theme::getBlockquoteColor() const
{
    return colors.value("blockquote");
}

void Theme::setBlockquoteColor(const Color& value)
{
    colors.insert("blockquote", value);
}

Color Theme::getCodeColor() const
{
    return colors.value("code");
}

void Theme::setCodeColor(const Color& value)
{
    colors.insert("code", value);
}

Color Theme::getSpellingErrorColor() const
{
    return colors.value("spelling");
}

void Theme::setSpellingErrorColor(const Color& value)
{
    colors.insert("spelling", value);
}
