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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QGraphicsColorizeEffect>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QWidget>

#include "AppSettings.h"
#include "find_dialog.h"
#include "HtmlPreview.h"
#include "MarkdownEditor.h"
#include "ThemeFactory.h"
#include "TimeLabel.h"

#define MAX_RECENT_FILES 10

class DocumentManager;
class DocumentStatistics;
class DocumentStatisticsWidget;
class Outline;
class QListWidget;
class SessionStatistics;
class SessionStatisticsWidget;
class Sidebar;

/**
 * Main window for the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

	public:
        MainWindow(const QString& filePath = QString(), QWidget* parent = 0);
        virtual ~MainWindow();

    protected:
        QSize sizeHint() const;
        void resizeEvent(QResizeEvent* event);
        void keyPressEvent(QKeyEvent* e);
        bool eventFilter(QObject* obj, QEvent* event);
        void closeEvent(QCloseEvent* event);

    private slots:
        void quitApplication();
        void changeTheme();
        void showFindReplaceDialog();
        void openPreferencesDialog();
        void toggleHtmlPreview(bool checked);
        void toggleHemingwayMode(bool checked);
        void toggleFocusMode(bool checked);
        void toggleFullScreen(bool checked);
        void toggleHideMenuBarInFullScreen(bool checked);
        void toggleOutlineAlternateRowColors(bool checked);
        void toggleFileHistoryEnabled(bool checked);
        void toggleDisplayTimeInFullScreen(bool checked);
        void changeEditorWidth(EditorWidth editorWidth);
        void changeInterfaceStyle(InterfaceStyle style);
        void insertImage();
        void showStyleSheetManager();
        void showQuickReferenceGuide();
        void showWikiPage();
        void showAbout();
        void updateWordCount(int newWordCount);
        void updateWordsPerMinute(int wpm);
        void changeFocusMode(FocusMode focusMode);
        void applyTheme(const Theme& theme);
        void openRecentFile();
        void refreshRecentFiles();
        void clearRecentFileHistory();
        void changeDocumentDisplayName(const QString& displayName);
        void onOperationStarted(const QString& description);
        void onOperationFinished();
        void changeFont();
        void onFontSizeChanged(int size);
        void onSetLocale();
        void copyHtml();
        void showPreviewOptions();
        void onAboutToHideMenuBarMenu();
        void onAboutToShowMenuBarMenu();

	private:
        MarkdownEditor* editor;
        QWidget* statusBar;
        QSplitter* editorSplitter;
        QSplitter* sidebarSplitter;
        DocumentManager* documentManager;
        ThemeFactory* themeFactory;
        Theme theme;
        QString language;
        Sidebar* sidebar;
        QLabel* wordCountLabel;
        QLabel* wpmLabel;
        QLabel* statusLabel;
        TimeLabel* timeLabel;
        QPushButton* previewOptionsButton;
        QPushButton* exportButton;
        QPushButton* copyHtmlButton;
        QPushButton* hemingwayModeButton;
        QPushButton* focusModeButton;
        QPushButton* htmlPreviewButton;
        FindDialog* findReplaceDialog;
        HtmlPreview* htmlPreview;
        QWebEngineView* quickReferenceGuideViewer;
        QAction* htmlPreviewMenuAction;
        QAction* fullScreenMenuAction;
        QPushButton* fullScreenButton;
        Outline* outlineWidget;
        DocumentStatistics* documentStats;
        DocumentStatisticsWidget* documentStatsWidget;
        SessionStatistics* sessionStats;
        SessionStatisticsWidget* sessionStatsWidget;
        QListWidget* cheatSheetWidget;
        QAction* recentFilesActions[MAX_RECENT_FILES];
        QPoint lastMousePos;
        bool menuBarMenuActivated;
        int menuBarHeight;

        QList<QWidget*> statusBarButtons;
        QList<QWidget*> statusBarWidgets;

        AppSettings* appSettings;

		QAction* addMenuAction
        (
            QMenu* menu,
            const QString& name,
            const QString& shortcut = 0,
            bool checkable = false,
            bool checked = false,
            QActionGroup* actionGroup = 0
        );

        void buildMenuBar();
        QWidget* buildStatusBar();
        void buildSidebar();

        void adjustEditorWidth(int width);
        void applyTheme();
        void showMenuBar();
        void hideMenuBar();
        bool isMenuBarVisible() const;
};

#endif
