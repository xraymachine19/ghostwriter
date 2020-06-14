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

#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>

#include "Color.h"
#include "ThemeEditorDialog.h"
#include "Theme.h"
#include "ThemeFactory.h"
#include "image_button.h"
#include "color_button.h"
#include "MessageBoxHelper.h"

ThemeEditorDialog::ThemeEditorDialog(const Theme& theme, QWidget* parent)
    : QDialog(parent), theme(theme)
{
    this->setWindowTitle(tr("Edit Theme"));
    oldThemeName = theme.getName();
    themeNameEdit = new QLineEdit(this);
    themeNameEdit->setText(theme.getName());

    textColorButton = new ColorButton(this);
    textColorButton->setColor(theme.getDefaultTextColor());

    markupColorButton = new ColorButton(this);
    markupColorButton->setColor(theme.getMarkupColor());

    linkColorButton = new ColorButton(this);
    linkColorButton->setColor(theme.getLinkColor());

    emphasisColorButton = new ColorButton(this);
    emphasisColorButton->setColor(theme.getEmphasisColor());

    blockTextColorButton = new ColorButton(this);
    blockTextColorButton->setColor(theme.getBlockquoteColor());

    spellcheckColorButton = new ColorButton(this);
    spellcheckColorButton->setColor(theme.getSpellingErrorColor());

    backgroundColorButton = new ColorButton(this);
    backgroundColorButton->setColor(theme.getBackgroundColor());

    QFormLayout* nameLayout = new QFormLayout();
    nameLayout->addRow(tr("Theme Name"), themeNameEdit);

    QWidget* nameSettingsWidget = new QWidget();
    nameSettingsWidget->setLayout(nameLayout);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(nameSettingsWidget);
    this->setLayout(layout);

    QFormLayout* editorColorsLayout = new QFormLayout();
    editorColorsLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

    editorColorsLayout->addRow(tr("Text Color"), textColorButton);
    editorColorsLayout->addRow(tr("Markup Color"), markupColorButton);
    editorColorsLayout->addRow(tr("Link Color"), linkColorButton);
    editorColorsLayout->addRow(tr("Emphasis Color"), emphasisColorButton);
    editorColorsLayout->addRow(tr("Block Text Color"), blockTextColorButton);
    editorColorsLayout->addRow(tr("Spelling Error Color"), spellcheckColorButton);
    editorColorsLayout->addRow(tr("Text Area Background Color"), backgroundColorButton);

    layout->addLayout(editorColorsLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Apply);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));

}

ThemeEditorDialog::~ThemeEditorDialog()
{
    ;
}

void ThemeEditorDialog::accept()
{
    if (saveTheme())
    {
        QDialog::accept();
    }
}

void ThemeEditorDialog::reject()
{
    emit themeUpdated(theme);
    QDialog::reject();
}

void ThemeEditorDialog::apply()
{
    if (saveTheme())
    {
        oldThemeName = theme.getName();
    }
}

bool ThemeEditorDialog::saveTheme()
{
    theme.setName(themeNameEdit->text());
    theme.setDefaultTextColor(textColorButton->color());
    theme.setMarkupColor(markupColorButton->color());
    theme.setLinkColor(linkColorButton->color());
    theme.setHeadingColor(emphasisColorButton->color());
    theme.setEmphasisColor(emphasisColorButton->color());
    theme.setBlockquoteColor(blockTextColorButton->color());
    theme.setCodeColor(blockTextColorButton->color());
    theme.setSpellingErrorColor(spellcheckColorButton->color());
    theme.setBackgroundColor(backgroundColorButton->color());

    QString error;

    ThemeFactory::getInstance()->saveTheme(oldThemeName, theme, error);

    if (!error.isNull())
    {
        MessageBoxHelper::critical
        (
            this,
            tr("Unable to save theme."),
            error
        );
        theme.setName(oldThemeName);
        return false;
    }

    emit themeUpdated(theme);

    return true;
}
