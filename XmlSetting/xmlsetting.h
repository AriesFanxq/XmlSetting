#ifndef XMLSETTING_H
#define XMLSETTING_H

#include <QtGui/QWidget>
#include "ui_xmlsetting.h"
#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

class QToolBar;
class XmlSetting : public QWidget
{
	Q_OBJECT

public:
	XmlSetting(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XmlSetting();
	
	//strName = { "file", "edit" }
	void getMenu(const QString &strName, QMenu* pMenu=0, QToolBar* pToolBar=0);

protected:
	void contextMenuEvent(QContextMenuEvent *e);

private slots:
	void on_m_tree_itemDoubleClicked(QTreeWidgetItem *pItem, int column);
	void on_m_tree_itemChanged(QTreeWidgetItem *pItem, int column);
	void on_m_tree_itemEntered(QTreeWidgetItem *pItem, int column);
	void on_m_tree_itemPressed(QTreeWidgetItem *pItem, int column);

	//�ļ��˵������ۺ���
	void m_buildNewFileAct_triggered();
	void m_openNewFileAct_triggered();
	void m_saveAsFileAct_triggered();

	//�༭�˵������ۺ���
	void m_addNodeAct_triggered();
	void m_addAttrAct_triggered();
	void m_addComment_triggered();
	void m_addText_triggered();
	void m_deleteAct_triggered();
	void m_upAct_triggered();
	void m_downAct_triggered();
	void m_copyAct_triggered();
	void m_pasteAct_triggered();

private:
	Ui::XmlSettingClass ui;

	QTreeWidget *m_tree;
	//����������Ĳ˵�(�ļ�File�˵�)
	QMenu		*m_fileMenu;
	QAction		*m_buildNewFileAct;
	QAction		*m_openNewFileAct;
	QAction		*m_saveAsFileAct;
	//������еĽڵ㣬�����Ĳ˵�(�༭Edit�˵�)
	QMenu		*m_editMenu;
	QAction		*m_addNodeAct;
	QAction		*m_addAttrAct;
	QAction		*m_addComment;
	QAction		*m_addText;
	QAction		*m_deleteAct;
	QAction		*m_upAct;
	QAction		*m_downAct;
	QAction		*m_copyAct;
	QAction		*m_pasteAct;

	bool m_bIsinit;				//��ʼ���Ƿ����
	QString m_saveFileName;		//�����XML�ļ�����
	static const QString m_strComment;
	static const QString m_strText;

	QTreeWidgetItem	*m_copyItem;	//���ƽڵ㶯��ʱ������Ҫ���ƵĽڵ�

	//�������
	void initWidget();
	void initFileMenu(QMenu *pMenu);
	void initFileToolBar(QToolBar *pToolBar);
	void initEditMenu(QMenu *pMenu);
	void initEditToolBar(QToolBar *pToolBar);
	void setAddActEnable(bool bFlag);
	void setEditActEnable(bool bFlag);
	//��������ز���
	QTreeWidgetItem* getTreeCurrentItem();
	QTreeWidgetItem* addNodeItem(QTreeWidgetItem *pItem, QString strName, bool bExpand = false);
	void addAttrItem(QTreeWidgetItem *pItem, QString strName, QString strValue, bool bExpand = true);
	void addCommentItem(QTreeWidgetItem *pItem, QString strComment, bool bExpand = true);
	void addTextItem(QTreeWidgetItem *pItem, QString strText, bool bExpand = true);
	bool deleteItem(QTreeWidgetItem *pItem);
	bool moveItem(QTreeWidgetItem *pItem, bool bFlag);
	QTreeWidgetItem* copyItem(QTreeWidgetItem *pItem);
	bool isNodeItem(QTreeWidgetItem *pItem);
	//��ȡXML���
	void readFromXmlFile(const QString &fileName);
	void readChild(QDomElement& domElem, QTreeWidgetItem* pItem);
	void insertAttr(QDomElement& domElem, QTreeWidgetItem* pItem);
	//д��XML���
	void writeToXmlFIle(const QString &fileName);
	void writeChild(QXmlStreamWriter &xmlWriter, QTreeWidgetItem *pItem);
};

#endif // XMLSETTING_H
