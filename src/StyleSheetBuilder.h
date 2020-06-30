/***********************************************************************
 *
 * Copyright (C) 2020 wereturtle
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

#ifndef STYLESHEETBUILDER_H
#define STYLESHEETBUILDER_H

#include <QString>

#include "Color.h"
#include "Theme.h"

class StyleSheetBuilder
{
    public:
        StyleSheetBuilder(const Theme& theme, const bool roundedCorners);
        ~StyleSheetBuilder();

        Color getChromeForegroundColor();
        QString getLayoutStyleSheet();
        QString getSplitterStyleSheet();
        QString getEditorStyleSheet();
        QString getStatusBarStyleSheet();
        QString getStatusBarWidgetsStyleSheet();
        QString getStatusLabelStyleSheet();
        QString getSidebarStyleSheet();
        QString getSidebarWidgetStyleSheet();
        
    private:
        enum PaletteType
        {
            BackgroundColor,
            ForegroundColor,
            FaintColor,
            PressedColor,
            HoverColor,
            SelectedColor,
            HighlightColor,
            InterfaceTextColor,
            LastPaletteColor = InterfaceTextColor
        };

        Color palette[LastPaletteColor + 1];

        QString scrollBarStyleSheet;
        QString layoutStyleSheet;
        QString splitterStyleSheet;
        QString editorStyleSheet;
        QString statusBarStyleSheet;
        QString statusBarWidgetStyleSheet;
        QString statusLabelStyleSheet;
        QString sidebarStyleSheet;
        QString sidebarWidgetStyleSheet;

        void buildScrollBarStyleSheet(const bool roundedCorners);
        void buildLayoutStyleSheet();
        void buildSplitterStyleSheet();
        void buildEditorStyleSheet();
        void buildStatusBarStyleSheet();
        void buildStatusBarWidgetStyleSheet();
        void buildStatusLabelStyleSheet();
        void buildSidebarStyleSheet();
        void buildSidebarWidgetStyleSheet(const bool roundedCorners);
};

#endif // STYLESHEETBUILDER_H