#include <QAction>
#include <QDebug>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QString>
#include <QTextStream>

#include "Sidebar.h"

#include "FontAwesome.h"

Sidebar::Sidebar(QWidget* parent) : QFrame(parent)
{
    tabs = new QListWidget(this);
    stack = new QStackedWidget(this);

    connect(tabs, SIGNAL(currentRowChanged(int)), this, SLOT(changeTab(int)));

    if 
    (
        (QFontDatabase::addApplicationFont(":/resources/Font Awesome 5 Free-Solid-900.otf") < 0)
        || (QFontDatabase::addApplicationFont(":/resources/Font Awesome 5 Brands-Regular-400.otf") < 0)
    )
    {
        qWarning() << "Font Awesome could not be loaded!";
    }
    
    solidFont.setFamily(FONTAWESOME_SOLID_FONT);
    brandsFont.setFamily(FONTAWESOME_BRAND_FONT);
    solidFont.setPixelSize(18);
    brandsFont.setPixelSize(18);
    tabs->setFont(solidFont);
    tabs->setFixedWidth(40);
    tabs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabs->setObjectName("sidebarTabs");
    tabs->setStyleSheet("#sidebarTabs { margin: 0; padding: 0; border: 0 }");
    stack->setObjectName("sidebarWidgetArea");
    tabs->setStyleSheet("#sidebarWidgetArea { margin: 0; padding: 0; border: 0 }");
    this->setObjectName("sidebar");
    this->setStyleSheet("#sidebar { margin: 0; padding: 0; border: 0 }");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(tabs);
    layout->addWidget(stack);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
}

Sidebar::~Sidebar()
{
    ;
}

void Sidebar::addTab
(
    const QString& text,
    const QString& toolTip,
    QWidget* widget,
    FontAwesomeType fontType
)
{
    QListWidgetItem* item = createTab(text, toolTip, fontType);
    tabs->addItem(item);
    stack->addWidget(widget);
}

void Sidebar::insertTab
(
    int index,
    const QString& text,
    const QString& toolTip,
    QWidget* widget,
    FontAwesomeType fontType
)
{
    if ((index >= 0) && (index <= tabs->count()))
    {
        QListWidgetItem* item = createTab(text, toolTip, fontType);
        stack->insertWidget(index, widget);
        tabs->insertItem(index, item);
    }
}

void Sidebar::setCurrentTab(int index) 
{
    if ((index >= 0) && (index < tabs->count()))
    {
        tabs->setCurrentRow(index);
    }
}


int Sidebar::getCount() const
{
    return tabs->count();
}

void Sidebar::changeTab(int index)
{
    stack->setCurrentIndex(index);
}

QListWidgetItem* Sidebar::createTab(const QString& text,
            const QString& toolTip,
            FontAwesomeType fontType) 
{
    QListWidgetItem* item = new QListWidgetItem(text);
    item->setToolTip(toolTip);
    item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    switch (fontType)
    {
        case FontAwesomeSolid:
            item->setFont(solidFont);
            break;
        case FontAwesomeBrands:
            item->setFont(brandsFont);
            break;
        default:
            item->setFont(solidFont);
            break;
    }

    return item;
}
