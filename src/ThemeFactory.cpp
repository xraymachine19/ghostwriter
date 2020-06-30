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

#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSettings>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "AppSettings.h"
#include "Color.h"
#include "ThemeFactory.h"

// Legacy theme settings (pre version 2 of ghostwriter)
#define GW_EDITOR_BACKGROUND_COLOR "Editor/backgroundColor"
#define GW_SPELLING_ERROR_COLOR "Editor/spellingErrorColor"
#define GW_DEFAULT_TEXT_COLOR "Markdown/defaultTextColor"
#define GW_MARKUP_COLOR "Markdown/markupColor"
#define GW_LINK_COLOR "Markdown/linkColor"
#define GW_HEADING_COLOR "Markdown/headingColor"
#define GW_EMPHASIS_COLOR "Markdown/emphasisColor"
#define GW_BLOCKQUOTE_COLOR "Markdown/blockquoteColor"
#define GW_CODE_COLOR "Markdown/codeColor"

const QString ThemeFactory::CLASSIC_LIGHT_THEME_NAME = QString("Classic Light");
const QString ThemeFactory::CLASSIC_DARK_THEME_NAME = QString("Classic Dark");
const QString ThemeFactory::PLAINSTRACTION_LIGHT_THEME_NAME = QString("Plainstraction Light");
const QString ThemeFactory::PLAINSTRACTION_DARK_THEME_NAME = QString("Plainstraction Dark");

ThemeFactory* ThemeFactory::instance = NULL;

ThemeFactory* ThemeFactory::getInstance()
{
    if (NULL == instance)
    {
        instance = new ThemeFactory();
    }

    return instance;
}

ThemeFactory::~ThemeFactory()
{
    ;
}

QStringList ThemeFactory::getAvailableThemes() const
{
    QStringList themeNames;

    foreach (Theme theme, builtInThemes)
    {
        themeNames.append(theme.getName());
    }

    themeNames << customThemeNames;
    return themeNames;
}

Theme ThemeFactory::getDefaultTheme() const
{
    QString err;
    return loadTheme(CLASSIC_LIGHT_THEME_NAME, err);
}

Theme ThemeFactory::loadTheme(const QString& name, QString& err) const
{
    err = QString();

    for (int i = 0; i < builtInThemes.size(); i++)
    {
        if (builtInThemes[i].getName() == name)
        {
            return builtInThemes[i];
        }
    }

    if (customThemeNames.contains(name))
    {
        QString themeFilePath = themeDirectoryPath + QString("/") +
            name + ".json";

        QFileInfo themeFileInfo(themeFilePath);

        if (!themeFileInfo.exists() && !themeFileInfo.isFile())
        {
            err = QObject::tr("The specified theme does not exist in the file system: ") +
                themeFilePath;
            return builtInThemes[0];
        }

        QFile themeFile(themeFilePath);

        if (!themeFile.open(QIODevice::ReadOnly))
        {
            err = QObject::tr("Could not open theme file for reading: ") +
                themeFilePath;
            return builtInThemes[0];
        }

        QJsonDocument json = QJsonDocument::fromJson(themeFile.readAll());

        themeFile.close();

        if (json.isNull() || !json.isObject() || json.isEmpty())
        {
            err = QObject::tr("Invalid theme format: ") +
                themeFilePath;
            return builtInThemes[0];
        }

        QJsonObject colorObject = json.object();
        Theme theme;
        theme.setName(name);
 
        foreach (QString key, Theme::getColorKeys())
        {
            QJsonValue value = colorObject.value(key);

            if (value.isUndefined() || !value.isString() || !Color::isValidColor(value.toString()))
            {
                err = QObject::tr("Invalid or missing value for %1 provided.").arg(key);
                return builtInThemes[0];
            }

            theme.setColorByKey(key, value.toString());
        }

        // Return success.
        err = QString();
        return theme;
    }
    else
    {
        err = QObject::tr("The specified theme is not available.  Try restarting the application.  "
            "If problem persists, please file a bug report.");
        return builtInThemes[0];
    }
}

void ThemeFactory::deleteTheme(const QString& name, QString& err)
{
    err = QString();

    QFile themeFile(this->getThemeFilePath(name));

    if (themeFile.exists() && !themeFile.remove())
    {
        err = QObject::tr("Could not delete theme at %1. Please try setting the file permissions to be writeable.")
            .arg(QFileInfo(themeFile).filePath());
        return;
    }

    // Finally, remove the theme from the available themes list.
    customThemeNames.removeOne(name);
}

void ThemeFactory::saveTheme(const QString& name, Theme& theme, QString& err)
{
    err = QString();

    // Check if theme was renamed.  If so, rename the theme's file.
    if (theme.getName() != name)
    {
        // Check for rename to default name.
        for (int i = 0; i < builtInThemes.size(); i++)
        {
            if (theme.getName() == builtInThemes[i].getName())
            {
                err = QObject::tr("'%1' already exists.  Please choose another name.").arg(theme.getName());
                return;
            }
        }

        QFile oldThemeFile(this->getThemeFilePath(name));
        QFile newThemeFile(this->getThemeFilePath(theme.getName()));

        if (newThemeFile.exists())
        {
            err = QObject::tr("'%1' theme already exists.  Please choose another name.").arg(theme.getName());
            return;
        }

        if (oldThemeFile.exists())
        {
            // Rename the theme file.
            if (!oldThemeFile.rename(this->getThemeFilePath(theme.getName())))
            {
                err = QObject::tr("Failed to rename theme.  Please check file permissions.");
                return;
            }

            // Now update the available themes list with the new theme name.
            for (int i = 0; i < customThemeNames.size(); i++)
            {
                if (name == customThemeNames[i])
                {
                    customThemeNames[i] = theme.getName();
                    std::sort(customThemeNames.begin(), customThemeNames.end());
                    break;
                }
            }
        }
    }

    QJsonDocument json;
    QJsonObject colorsObj;

    foreach (QString key, Theme::getColorKeys())
    {
        colorsObj.insert(key, theme.getColorByKey(key).name().toLower());
    }

    json.setObject(colorsObj);

    QFile themeFile(this->getThemeFilePath(theme.getName()));

    bool isNewTheme = !themeFile.exists();
    
    if (!themeFile.open(QIODevice::WriteOnly) || (themeFile.write(json.toJson()) < 0))
    {
        err = QObject::tr("Theme could not be saved to disk.  Please check file permissions or try renaming the theme.");
        return;
    }

    themeFile.close();

    if (isNewTheme)
    {
        customThemeNames.append(theme.getName());
        std::sort(customThemeNames.begin(), customThemeNames.end());
    }
}

QDir ThemeFactory::getThemeDirectory() const
{
    return themeDirectory;
}

QString ThemeFactory::getThemeFilePath(const QString& themeName) const
{
    return this->themeDirectoryPath + "/" + themeName + ".json";
}

QString ThemeFactory::generateUntitledThemeName() const
{
    QString name = QObject::tr("Untitled 1");
    int count = 1;
    bool availableNameFound = false;

    while (!availableNameFound)
    {
        bool tryAgain = false;

        for (int i = 0; i < this->customThemeNames.size(); i++)
        {
            if (name == this->customThemeNames[i])
            {
                count++;
                name = QObject::tr("Untitled %1").arg(count);
                tryAgain = true;
                break;
            }
        }

        if (!tryAgain)
        {
            availableNameFound = true;
        }
    }

    return name;
}

ThemeFactory::ThemeFactory()
{
    themeDirectoryPath = AppSettings::getInstance()->getThemeDirectoryPath();
    themeDirectory = QDir(themeDirectoryPath);

    if (!themeDirectory.exists())
    {
        themeDirectory.mkpath(themeDirectory.path());
    }

    themeDirectory.setFilter(QDir::Files | QDir::Readable | QDir::Writable);
    themeDirectory.setSorting(QDir::Name);
    themeDirectory.setNameFilters(QStringList("*.json"));

    QFileInfoList themeFiles = themeDirectory.entryInfoList();
    QMap<QString, QString> sortedThemes;

    for (int i = 0; i < themeFiles.size(); i++)
    {
        QString baseName = themeFiles[i].baseName();
        sortedThemes.insertMulti(baseName.toLower(), baseName);
    }

    customThemeNames = sortedThemes.values();

    // Set up built-in themes.
    loadClassicLightTheme();
    loadClassicDarkTheme();
    loadPlainstractionLightTheme();
    loadPlainstractionDarkTheme();
}

void ThemeFactory::loadClassicLightTheme()
{
    Theme theme(CLASSIC_LIGHT_THEME_NAME);
    theme.setBackgroundColor("#eff0f1");
    theme.setDefaultTextColor("#31363b");
    theme.setSelectionColor("#E6E7E8");
    theme.setMarkupColor("#bdc3c7");
    theme.setLinkColor("#2980b9");
    theme.setHeadingColor("#31363b");
    theme.setEmphasisColor("#31363b");
    theme.setBlockquoteColor("#31363b");
    theme.setCodeColor("#31363b");
    theme.setSpellingErrorColor("#da4453");
    theme.setBuiltIn(true);

    builtInThemes.append(theme);
}

void ThemeFactory::loadClassicDarkTheme()
{
    Theme theme(CLASSIC_DARK_THEME_NAME);
    theme.setBackgroundColor("#151719");
    theme.setDefaultTextColor("#bdc3c7");
    theme.setSelectionColor("#26282a");
    theme.setMarkupColor("#575b5f");
    theme.setLinkColor("#5f8eb1");
    theme.setHeadingColor("#bdc3c7");
    theme.setEmphasisColor("#bdc3c7");
    theme.setBlockquoteColor("#bdc3c7");
    theme.setCodeColor("#bdc3c7");
    theme.setSpellingErrorColor("#da4453");
    theme.setBuiltIn(true);

    builtInThemes.append(theme);
}

void ThemeFactory::loadPlainstractionLightTheme()
{
    Theme theme(PLAINSTRACTION_LIGHT_THEME_NAME);
    theme.setBackgroundColor("#ffffff");
    theme.setDefaultTextColor("#1d2023");
    theme.setSelectionColor("#e8e9ea");
    theme.setMarkupColor("#c4e4f1");
    theme.setLinkColor("#c23184");
    theme.setHeadingColor("#009bc8");
    theme.setEmphasisColor("#009bc8");
    theme.setBlockquoteColor("#009bc8");
    theme.setCodeColor("#009bc8");
    theme.setSpellingErrorColor("#c23184");
    theme.setBuiltIn(true);

    builtInThemes.append(theme);
}

void ThemeFactory::loadPlainstractionDarkTheme()
{
    Theme theme(PLAINSTRACTION_DARK_THEME_NAME);
    theme.setBackgroundColor("#141414");
    theme.setDefaultTextColor("#c7cfd0");
    theme.setSelectionColor("#1c242a");
    theme.setMarkupColor("#074051");
    theme.setLinkColor("#b31771");
    theme.setHeadingColor("#009bc8");
    theme.setEmphasisColor("#009bc8");
    theme.setBlockquoteColor("#009bc8");
    theme.setCodeColor("#009bc8");
    theme.setSpellingErrorColor("#b31771");
    theme.setBuiltIn(true);

    builtInThemes.append(theme);
}
