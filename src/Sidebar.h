#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QListWidget>
#include <QStackedWidget>

#include "Color.h"
#include "FontAwesome.h"

class Sidebar : public QFrame
{
    Q_OBJECT

    public:
        Sidebar(QWidget* parent = NULL);
        ~Sidebar();

        void addTab
        (
            const QString& text,
            const QString& toolTip,
            QWidget* widget,
            FontAwesomeType fontType = FontAwesomeSolid
        );

        void insertTab
        (
            int index, 
            const QString& text,
            const QString& toolTip,
            QWidget* widget,
            FontAwesomeType fontType = FontAwesomeSolid
        );

        void setCurrentTab(int index);

        int getCount() const;

    private slots:
        void changeTab(int index);

    private:
        QListWidget* tabs;
        QStackedWidget* stack;

        QFont brandsFont;
        QFont solidFont;

        QListWidgetItem* createTab
        (
            const QString& text,
            const QString& tooltip,
            FontAwesomeType fontType = FontAwesomeSolid
        );
};

#endif // SIDEBAR_H
