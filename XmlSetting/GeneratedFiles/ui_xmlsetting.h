/********************************************************************************
** Form generated from reading UI file 'xmlsetting.ui'
**
** Created: Fri Apr 26 09:01:21 2019
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XMLSETTING_H
#define UI_XMLSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XmlSettingClass
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *m_tree;

    void setupUi(QWidget *XmlSettingClass)
    {
        if (XmlSettingClass->objectName().isEmpty())
            XmlSettingClass->setObjectName(QString::fromUtf8("XmlSettingClass"));
        XmlSettingClass->resize(600, 400);
        horizontalLayout = new QHBoxLayout(XmlSettingClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_tree = new QTreeWidget(XmlSettingClass);
        m_tree->setObjectName(QString::fromUtf8("m_tree"));

        horizontalLayout->addWidget(m_tree);


        retranslateUi(XmlSettingClass);

        QMetaObject::connectSlotsByName(XmlSettingClass);
    } // setupUi

    void retranslateUi(QWidget *XmlSettingClass)
    {
        XmlSettingClass->setWindowTitle(QApplication::translate("XmlSettingClass", "XmlSetting", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_tree->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("XmlSettingClass", "Value", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("XmlSettingClass", "Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class XmlSettingClass: public Ui_XmlSettingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XMLSETTING_H
