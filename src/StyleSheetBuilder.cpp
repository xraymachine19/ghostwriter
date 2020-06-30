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

    palette[BackgroundColor] = theme.getBackgroundColor();
    palette[ForegroundColor] = theme.getDefaultTextColor();
    palette[SelectedColor] = theme.getSelectionColor();
    palette[HighlightColor] = theme.getLinkColor();

    // If the background color is brighter than the foreground color...
    if (bgBrightness > fgBrightness)
    {
        // Create a UI chrome color based on a lightened editor text color,
        // such that the new color achieves a lower contrast ratio.
        //
        palette[InterfaceTextColor] = palette[ForegroundColor].lightenToMatchContrastRatio
            (
                theme.getBackgroundColor(),
                2.0
            );

        // Slightly blend the new UI chrome color with the editor background color
        // to help it match the theme better.
        //
        palette[InterfaceTextColor].setAlpha(220);
        palette[InterfaceTextColor] = palette[InterfaceTextColor].applyAlpha
            (
                theme.getBackgroundColor()
            );

        // Blend the UI chrome color with the background color even further for
        // the scroll bar color, as the scroll bar will otherwise tend to
        // stand out.
        //
        palette[FaintColor] = palette[InterfaceTextColor];
        palette[FaintColor].setAlpha(200);
        palette[FaintColor] = palette[FaintColor].applyAlpha
            (
                theme.getBackgroundColor()
            );
    }
    // Else if the foreground color is brighter than the background color...
    else
    {
        palette[InterfaceTextColor] = palette[ForegroundColor].darker(120);
        palette[FaintColor] = palette[InterfaceTextColor];
    }

    palette[PressedColor] = palette[InterfaceTextColor];
    palette[PressedColor].setAlpha(30);
    palette[PressedColor] = palette[PressedColor].applyAlpha(theme.getBackgroundColor());
    palette[HoverColor] = palette[PressedColor];

    palette[FaintColor] = theme.getDefaultTextColor();
    palette[FaintColor].setAlpha(30);
    palette[FaintColor] = palette[FaintColor].applyAlpha(theme.getBackgroundColor());

    buildScrollBarStyleSheet(roundedCorners);
    buildEditorStyleSheet();
    buildSplitterStyleSheet();
    buildStatusBarStyleSheet();
    buildStatusBarWidgetStyleSheet();
    buildLayoutStyleSheet();
    buildSidebarStyleSheet();
    buildSidebarWidgetStyleSheet(roundedCorners);
    buildStatusLabelStyleSheet();
}


StyleSheetBuilder::~StyleSheetBuilder() 
{
    ;
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

void StyleSheetBuilder::buildScrollBarStyleSheet(const bool roundedCorners) 
{
    scrollBarStyleSheet = "";

    QTextStream stream(&scrollBarStyleSheet);
    QString scrollBarRadius = "0px";
    QString scrollAreaPadding = "3px 3px 0px 3px";

    if (roundedCorners)
    {
        scrollBarRadius = "3px";
    }

    stream
        << "QAbstractScrollArea::corner { background: transparent } "
        << "QAbstractScrollArea { padding: "
        << scrollAreaPadding
        << "; margin: 0 } "
        << "QScrollBar::horizontal { border: 0; background: transparent; height: 6px; margin: 0 } "
        << "QScrollBar::handle:horizontal { border: 0; background: "
        << palette[FaintColor]
        << "; min-width: 50px; border-radius: "
        << scrollBarRadius
        << "; } "
        << "QScrollBar::vertical { border: 0; background: transparent; width: 6px; margin: 0 } "
        << "QScrollBar::handle:vertical { border: 0; background: "
        << palette[FaintColor]
        << "; min-height: 50px; border-radius: "
        << scrollBarRadius
        << "; } "
        << "QScrollBar::vertical:hover { background: "
        << palette[FaintColor]
        << "; border-radius: "
        << scrollBarRadius
        << " } "
        << "QScrollBar::horizontal:hover { background: "
        << palette[FaintColor]
        << "; border-radius: "
        << scrollBarRadius
        << " } "
        << "QScrollBar::handle:vertical { border: 0; background: "
        << palette[FaintColor]
        << "; min-height: 50px; border-radius: "
        << scrollBarRadius
        << "; } "
        << "QScrollBar::handle:vertical:hover { background: "
        << palette[HighlightColor]
        << " } "
        << "QScrollBar::handle:horizontal:hover { background: "
        << palette[HighlightColor]
        << " } "
        << "QScrollBar::add-line { background: transparent; border: 0 } "
        << "QScrollBar::sub-line { background: transparent; border: 0 } "
        ;
}

void StyleSheetBuilder::buildLayoutStyleSheet() 
{
    QString styleSheet = "";
    QTextStream stream(&styleSheet);

    stream
        << "#editorLayoutArea { background-color: "
        << palette[BackgroundColor]
        << "; border: 0; margin: 0 }"
        ;
        
    this->layoutStyleSheet = styleSheet;
}

void StyleSheetBuilder::buildSplitterStyleSheet() 
{
    this->splitterStyleSheet = 
        "QSplitter::handle:vertical { height: 0px; } "
        "QSplitter::handle:horizontal { width: 0px; } ";
}

void StyleSheetBuilder::buildEditorStyleSheet() 
{
    editorStyleSheet = "";

    QTextStream stream(&editorStyleSheet);
    
    stream
        << "QPlainTextEdit { border: 0; "
        << "margin: 0; padding: 5px; background-color: "
        << palette[BackgroundColor]
        << "; color: "
        << palette[ForegroundColor]
        << "; selection-color: "
        << palette[ForegroundColor]
        << "; selection-background-color: "
        << palette[SelectedColor]
        << " } "
        << scrollBarStyleSheet
        ;
}

void StyleSheetBuilder::buildStatusBarStyleSheet() 
{
    statusBarStyleSheet = "";

    QTextStream stream(&statusBarStyleSheet);

    stream
        << "#statusBar { margin: 0; padding: 0; border-top: 1px solid "
        << palette[FaintColor]
        << "; border-left: 0; border-right: 0; border-bottom: 0; background: "
        << palette[BackgroundColor]
        << "; color: "
        << palette[InterfaceTextColor] 
        << " } "
        ;
}

void StyleSheetBuilder::buildStatusBarWidgetStyleSheet() 
{
    statusBarWidgetStyleSheet = "";

    QTextStream stream(&statusBarWidgetStyleSheet);

    // Set the label and button font size to a fixed point size,
    // since on Windows using the default QLabel and QPushButton
    // font sizes results in tiny font sizes for these.  We need
    // them to stand out a little bit more than a typical label
    // or button.
    //
    int fontSize = 11;

    stream
        << "QLabel { font-size: "
        << fontSize
        << "pt; margin: 0px; padding: 5px; border: 0; background: transparent; color: "
        << palette[InterfaceTextColor]
        << " } "
        << "QPushButton { padding: 5 5 5 5; margin: 0; border: 0; border-radius: 5px; background: transparent"
        << "; color: "
        << palette[InterfaceTextColor]
        << "; font-size: 16px; width: 32px } "
        << "QPushButton:pressed, QPushButton:flat, QPushButton:checked, QPushButton:hover { padding: 5 5 5 5; margin: 0; color: "
        << palette[InterfaceTextColor]
        << "; background-color: "
        << palette[PressedColor]
        << " } "
        ;
}

void StyleSheetBuilder::buildStatusLabelStyleSheet() 
{
    statusLabelStyleSheet = "";

    QTextStream stream(&statusLabelStyleSheet);

    stream
        << "color: "
        << palette[InterfaceTextColor]
        << "; background-color: "
        << palette[HoverColor]
        << "; border-radius: 5px; padding: 3px"
        ;
}

void StyleSheetBuilder::buildSidebarStyleSheet() 
{
    sidebarStyleSheet = "";

    QTextStream stream(&sidebarStyleSheet);

    stream << "QStackedWidget { border: 0; padding: 1; margin: 0"
           << "; background-color: "
           << palette[BackgroundColor]
           << "; border-width: 0px; border-right: 1px solid "
           << palette[FaintColor]
           << "} QListWidget { outline: none; margin: 0; padding: 0; background-color: "
           << palette[BackgroundColor]
           << "; color: "
           << palette[InterfaceTextColor]
           << "; border-width: 0px; border-right: 0; border-style: solid; border-color: "
           << palette[FaintColor]
           << "; } "
           << "QListWidget::item { padding: 5 5 5 5; margin: 0; background-color: "
           << palette[BackgroundColor]
           << "; color: "
           << palette[InterfaceTextColor]
           << "; border-width: 0px; border-left-width: 3px; border-style: solid; border-color: "
           << palette[BackgroundColor]
           << " } QListWidget::item:selected { border-color: "
           << palette[HighlightColor]
           << "; color: "
           << palette[InterfaceTextColor]
           << "; background-color: "
           << palette[SelectedColor]
           << " } "
        ;
}

void StyleSheetBuilder::buildSidebarWidgetStyleSheet(const bool roundedCorners) 
{
    sidebarWidgetStyleSheet = "";

    QTextStream stream(&sidebarWidgetStyleSheet);
    int sidebarFontSize = 11;

    QString listSelectionBorderRadius = "0px";

    if (roundedCorners)
    {
        listSelectionBorderRadius = "3px";
    }

    // Important!  For QListWidget (used in sidebar), set
    // QListWidget { outline: none } for the style sheet to get rid of the
    // focus rectangle without losing keyboard focus capability.
    // Unforntunately, this property isn't in the Qt documentation, so
    // it's being documented here for posterity's sake.
    //

    stream 
        << "QListWidget { outline: none; border: 0; padding: 1; background-color: "
        << palette[BackgroundColor]
        << "; color: "
        << palette[InterfaceTextColor]
        << "; font-size: "
        << sidebarFontSize
        << "pt } QListWidget::item { border: 0; padding: 1 0 1 0; margin: 0; background-color: "
        << palette[BackgroundColor]
        << "; color: "
        << palette[InterfaceTextColor]
        << " } "
        << "QListWidget::item:selected { border-radius: "
        << listSelectionBorderRadius
        << "; color: "
        << palette[InterfaceTextColor]
        << "; background-color: "
        << palette[SelectedColor]
        << " } "
        << "QLabel { border: 0; padding: 0; margin: 0; background-color: transparent; "
        << "font-size: "
        << sidebarFontSize
        << "pt; color: "
        << palette[InterfaceTextColor]
        << " } "
        << scrollBarStyleSheet
        ;
}