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

#include <QTextStream>

#include "Color.h"
#include "StyleSheetBuilder.h"

StyleSheetBuilder::StyleSheetBuilder(const Theme& theme, const bool roundedCorners) 
{
    QString styleSheet;
    QTextStream stream(&styleSheet);

    double fgBrightness = theme.getDefaultTextColor().getLuminance();
    double bgBrightness = theme.getBackgroundColor().getLuminance();

    Color scrollBarColor;
    this->chromeFgColor = theme.getDefaultTextColor();

    // If the background color is brighter than the foreground color...
    if (bgBrightness > fgBrightness)
    {
        // Create a UI chrome color based on a lightened editor text color,
        // such that the new color achieves a lower contrast ratio.
        //
        this->chromeFgColor = this->chromeFgColor.lightenToMatchContrastRatio
            (
                theme.getBackgroundColor(),
                2.1
            );

        // Slightly blend the new UI chrome color with the editor background color
        // to help it match the theme better.
        //
        this->chromeFgColor.setAlpha(220);
        this->chromeFgColor = this->chromeFgColor.applyAlpha
            (
                theme.getBackgroundColor()
            );

        // Blend the UI chrome color with the background color even further for
        // the scroll bar color, as the scroll bar will otherwise tend to
        // stand out.
        //
        scrollBarColor = this->chromeFgColor;
        scrollBarColor.setAlpha(200);
        scrollBarColor = scrollBarColor.applyAlpha
            (
                theme.getBackgroundColor()
            );

    }
    // Else if the foreground color is brighter than the background color...
    else
    {
        this->chromeFgColor = this->chromeFgColor.darker(120);
        scrollBarColor = this->chromeFgColor;
    }

    QString scrollbarColorRGB = scrollBarColor.toRgbString();
    Color scrollBarHoverColor = theme.getLinkColor();
    QString scrollBarHoverRGB = scrollBarHoverColor.toRgbString();

    QString backgroundColorRGBA =
        theme.getBackgroundColor().toRgbaString();

    QString editorSelectionFgColorRGB =
        theme.getBackgroundColor().toRgbString();

    QString editorSelectionBgColorRGB =
        theme.getDefaultTextColor().toRgbString();

    QString statusBarItemFgColorRGB;
    QString statusBarButtonFgPressHoverColorRGB;
    QString statusBarButtonBgPressHoverColorRGBA;

    Color buttonPressColor(this->chromeFgColor);
    buttonPressColor.setAlpha(30);

    statusBarItemFgColorRGB = this->chromeFgColor.toRgbString();
    statusBarButtonFgPressHoverColorRGB = statusBarItemFgColorRGB;
    statusBarButtonBgPressHoverColorRGBA = buttonPressColor.toRgbaString();

    styleSheet = "";

    stream
        << "#editorLayoutArea { background-color: transparent; border: 0; margin: 0 }"
        ;
        
    this->layoutStyleSheet = styleSheet;
    styleSheet = "";

    QString scrollBarRadius = "0px";
    QString scrollAreaPadding = "3px 3px 0px 3px";

    if (roundedCorners)
    {
        scrollBarRadius = "4px";
    }

    QString defaultTextColorRGB =
        theme.getDefaultTextColor().toRgbString();

    stream
        << "QPlainTextEdit { border: 0; "
        << "margin: 0; padding: 5px; background-color: "
        << backgroundColorRGBA
        << "; color: "
        << defaultTextColorRGB
        << "; selection-color: "
        << editorSelectionFgColorRGB
        << "; selection-background-color: "
        << editorSelectionBgColorRGB
        << " } "
        << "QAbstractScrollArea::corner { background: transparent } "
        << "QAbstractScrollArea { padding: "
        << scrollAreaPadding
        << "; margin: 0 } "
        << "QScrollBar::horizontal { border: 0; background: transparent; height: 8px; margin: 0 } "
        << "QScrollBar::handle:horizontal { border: 0; background: "
        << scrollbarColorRGB
        << "; min-width: 50px; border-radius: "
        << scrollBarRadius
        << "; } "
        << "QScrollBar::vertical { border: 0; background: transparent; width: 8px; margin: 0 } "
        << "QScrollBar::handle:vertical { border: 0; background: "
        << scrollbarColorRGB
        << "; min-height: 50px; border-radius: "
        << scrollBarRadius
        << "; } "
        << "QScrollBar::handle:vertical:hover { background: "
        << scrollBarHoverRGB
        << " } "
        << "QScrollBar::handle:horizontal:hover { background: "
        << scrollBarHoverRGB
        << " } "
        << "QScrollBar::add-line { background: transparent; border: 0 } "
        << "QScrollBar::sub-line { background: transparent; border: 0 } "
        ;

    this->editorStyleSheet = styleSheet;
    styleSheet = "";

    stream
        << "QSplitter::handle:vertical { height: 0px; } "
        << "QSplitter::handle:horizontal { width: 0px; } ";

    this->splitterStyleSheet = styleSheet;

    styleSheet = "";
    
    int statusBarBorder = 1;

    Color statusBarBorderColor = theme.getDefaultTextColor();
    statusBarBorderColor.setAlpha(30);
    statusBarBorderColor = statusBarBorderColor.applyAlpha(theme.getBackgroundColor());

    stream
        << "QStatusBar { margin: 0; padding: 0; border-top: "
        << statusBarBorder
        << "px solid "
        << statusBarBorderColor.toRgbString()
        << "; border-left: 0; border-right: 0; border-bottom: 0; background: "
        << backgroundColorRGBA
        << "; color: "
        << statusBarItemFgColorRGB 
        << " } "
        << "QStatusBar::item { border: 0; padding: 0; margin: 0 } ";

    this->statusBarStyleSheet = styleSheet;

    // Make the word count and focus mode button font size
    // match the menu bar's font size, since on Windows using
    // the default QLabel and QPushButton font sizes results in
    // tiny font sizes for these.  We need them to stand out
    // a little bit more than a typical label or button.
    //
    int menuBarFontSize = 11;//this->menuBar()->fontInfo().pointSize();

    styleSheet = "";

    stream
        << "QLabel { font-size: "
        << menuBarFontSize
        << "pt; margin: 0px; padding: 5px; border: 0; background: transparent; color: "
        << statusBarItemFgColorRGB
        << " } "
        << "QPushButton { padding: 2 5 2 5; margin: 0; border: 0; border-radius: 5px; background: transparent"
        << "; color: "
        << statusBarItemFgColorRGB
        << " } "
        << "QPushButton:pressed, QPushButton:flat, QPushButton:checked, QPushButton:hover { padding: 2 5 2 5; margin: 0; color: "
        << statusBarButtonFgPressHoverColorRGB
        << "; background-color: "
        << statusBarButtonBgPressHoverColorRGBA
        << " } "
        ;

    this->statusBarWidgetStyleSheet = styleSheet;
    styleSheet = "";

    stream
        << "color: "
        << statusBarButtonFgPressHoverColorRGB
        << "; background-color: "
        << statusBarButtonBgPressHoverColorRGBA
        << "; border-radius: 5px; padding: 3px"
        ;

    this->statusLabelStyleSheet = styleSheet;
    styleSheet = "";

    // Style the sidebar

    QString sidebarFgString = theme.getDefaultTextColor().toRgbString();

    Color alphaSidebarSelectionColor = theme.getDefaultTextColor();
    alphaSidebarSelectionColor.setAlpha(200);
    QString sidebarSelectionFgString = theme.getBackgroundColor().toRgbString();
    QString sidebarSelectionBgString = alphaSidebarSelectionColor.toRgbaString();

    int sidebarFontSize = 11; //cheatSheetWidget->font().pointSize();

    QString listSelectionBorderRadius = "3px";

    if (roundedCorners)
    {
        listSelectionBorderRadius = "0px";
    }

    // Important!  For QListWidget (used in sidebar), set
    // QListWidget { outline: none } for the style sheet to get rid of the
    // focus rectangle without losing keyboard focus capability.
    // Unforntunately, this property isn't in the Qt documentation, so
    // it's being documented here for posterity's sake.
    //
    int primaryRowAlpha = 0;
    int alternateRowAlpha = 20;

    double sidebarFgBrightness = theme.getDefaultTextColor().getLuminance();
    double sidebarBgBrightness = theme.getBackgroundColor().getLuminance();

    // If the sidebar background color is brighter than the foreground color...
    if (sidebarBgBrightness > sidebarFgBrightness)
    {
        primaryRowAlpha = 10;
        alternateRowAlpha = 50;
    }

    stream << "QListWidget { outline: none; border: 0; padding: 1; background-color: transparent; color: "
            << sidebarFgString
            << "; alternate-background-color: rgba(255, 255, 255, "
            << alternateRowAlpha
            << "); font-size: "
            << sidebarFontSize
            << "pt } QListWidget::item { padding: 1 0 1 0; margin: 0; background-color: "
            << "rgba(0, 0, 0, "
            << primaryRowAlpha
            << ") } QListWidget::item:alternate { padding: 1; margin: 0; background-color: "
            << "rgba(255, 255, 255, 10)"
            << " } "
            << "QListWidget::item:selected { border-radius: "
            << listSelectionBorderRadius
            << "; color: "
            << sidebarSelectionFgString
            << "; background-color: "
            << sidebarSelectionBgString
            << " } "
        ;

    QString sidebarScrollBarRadius = "4px";

    if (roundedCorners)
    {
        sidebarScrollBarRadius = "0px";
    }

    stream << "QLabel { border: 0; padding: 0; margin: 0; background-color: transparent; "
           << "font-size: "
           << sidebarFontSize
           << "pt; color: "
           << sidebarFgString
           << " } "
           << "QScrollBar::horizontal { border: 0; background: transparent; height: 8px; margin: 0 } "
           << "QScrollBar::handle:horizontal { border: 0; background: "
           << sidebarFgString
           << "; min-width: 20px; border-radius: "
           << sidebarScrollBarRadius
           << "; } "
           << "QScrollBar::vertical { border: 0; background: transparent; width: 8px; margin: 0 } "
           << "QScrollBar::handle:vertical { border: 0; background: "
           << sidebarFgString
           << "; min-height: 20px; border-radius: "
           << sidebarScrollBarRadius
           << "; } "
           << "QScrollBar::add-line { background: transparent; border: 0 } "
           << "QScrollBar::sub-line { background: transparent; border: 0 } "
           << "QAbstractScrollArea::corner { background: transparent } "
        ;

    this->sidebarStyleSheet = styleSheet;
    this->sidebarWidgetStyleSheet = styleSheet;
}


StyleSheetBuilder::~StyleSheetBuilder() 
{
    ;
}

Color StyleSheetBuilder::getChromeForegroundColor() 
{
    return this->chromeFgColor;
}

QString StyleSheetBuilder::getLayoutStyleSheet() 
{
    return layoutStyleSheet;
}

QString StyleSheetBuilder::getSplitterStyleSheet() 
{
    return splitterStyleSheet;
}

QString StyleSheetBuilder::getEditorStyleSheet() 
{
    return editorStyleSheet;
}

QString StyleSheetBuilder::getStatusBarStyleSheet() 
{
    return statusBarStyleSheet;
}

QString StyleSheetBuilder::getStatusBarWidgetsStyleSheet() 
{
    return statusBarWidgetStyleSheet;
}

QString StyleSheetBuilder::getStatusLabelStyleSheet() 
{
    return statusLabelStyleSheet;
}

QString StyleSheetBuilder::getSidebarStyleSheet() 
{
    return sidebarStyleSheet;
}

QString StyleSheetBuilder::getSidebarWidgetStyleSheet() 
{
    return sidebarWidgetStyleSheet;
}