#include "xmlsetting.h"
#include <QTreeWidget>
#include <QFile>
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QDebug>
#include <QMessageBox>
#include <QChar>
#include <QtGui/QFileDialog>
#include <QtGui/QToolBar>

const QString XmlSetting::m_strComment("#Comment");
const QString XmlSetting::m_strText("#Text");

XmlSetting::XmlSetting(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
	, m_fileMenu(0)
	, m_editMenu(0)
	, m_bIsinit(false)
	, m_saveFileName("")
	, m_copyItem(0)
{
	ui.setupUi(this);

	initWidget();
	getMenu("file");
	getMenu("edit");
}

XmlSetting::~XmlSetting()
{

}

void XmlSetting::initWidget()
{
	m_tree = ui.m_tree;
	m_tree->setStyleSheet("QTreeWidget::item{height:30px}");	//ʹ�����п����ļ���������Ӧ
	QHeaderView *head = m_tree->header();
	head->setResizeMode(QHeaderView::ResizeToContents);			//���ñ�ͷ�������Ӧ�����峤�ȣ�
}

void XmlSetting::getMenu(const QString &strName, QMenu* pMenu, QToolBar* pToolBar)
{
	if (strName.toLower() == "file")
	{
		initFileMenu(pMenu);
		initFileToolBar(pToolBar);
	}
	else if (strName.toLower() == "edit")
	{
		initEditMenu(pMenu);
		initEditToolBar(pToolBar);
	}
}

void XmlSetting::initFileMenu(QMenu *pMenu)
{
	if (pMenu)
	{
		if (m_fileMenu)
			delete m_fileMenu;
		m_fileMenu = pMenu;
	}
	else
	{
		if (m_fileMenu)
			return ;
		m_fileMenu = new QMenu();
	}

	m_buildNewFileAct = m_fileMenu->addAction("New",
		this, SLOT(m_buildNewFileAct_triggered()), QKeySequence::New);
	m_openNewFileAct = m_fileMenu->addAction("Open",
		this, SLOT(m_openNewFileAct_triggered()), QKeySequence::Open);
	m_saveAsFileAct = m_fileMenu->addAction("Save As",
		this, SLOT(m_saveAsFileAct_triggered()), QKeySequence::SaveAs);
}

void XmlSetting::initFileToolBar(QToolBar *pToolBar)
{
	if (!pToolBar)
		return;

	pToolBar->addAction(m_buildNewFileAct);
	pToolBar->addAction(m_openNewFileAct);
	pToolBar->addAction(m_saveAsFileAct);
}

void XmlSetting::initEditMenu(QMenu *pMenu)
{
	if (pMenu)
	{
		if (m_editMenu)
			delete m_editMenu;
		m_editMenu = pMenu;
	}
	else
	{
		if (m_editMenu)
			return ;
		m_editMenu = new QMenu();
	}

	m_addNodeAct = m_editMenu->addAction("Insert Node",
		this, SLOT(m_addNodeAct_triggered()));
	m_addAttrAct = m_editMenu->addAction("Insert Attr",
		this, SLOT(m_addAttrAct_triggered()));
	m_addComment = m_editMenu->addAction("Insert Comment",
		this, SLOT(m_addComment_triggered()));
	m_addText = m_editMenu->addAction("Insert Text",
		this, SLOT(m_addText_triggered()));
	m_editMenu->addSeparator();

	m_deleteAct = m_editMenu->addAction("Delete", 
		this, SLOT(m_deleteAct_triggered()), QKeySequence::Delete);
	m_editMenu->addSeparator();

	m_upAct = m_editMenu->addAction("Node Up",
		this, SLOT(m_upAct_triggered()), QKeySequence("Ctrl+Up"));
	m_downAct = m_editMenu->addAction("Node Down",
		this, SLOT(m_downAct_triggered()), QKeySequence("Ctrl+Down"));
	m_editMenu->addSeparator();

	m_copyAct = m_editMenu->addAction("Node Copy",
		this, SLOT(m_copyAct_triggered()), QKeySequence::Copy);
	m_pasteAct = m_editMenu->addAction("Node Paste",
		this, SLOT(m_pasteAct_triggered()), QKeySequence::Paste);
}

void XmlSetting::initEditToolBar(QToolBar *pToolBar)
{
	if (!pToolBar)
		return;

	pToolBar->addAction(m_addNodeAct);
	pToolBar->addAction(m_addAttrAct);
	pToolBar->addAction(m_addComment);
	pToolBar->addAction(m_addText);
	pToolBar->addAction(m_deleteAct);
	pToolBar->addAction(m_upAct);
	pToolBar->addAction(m_downAct);
	pToolBar->addAction(m_copyAct);
	pToolBar->addAction(m_pasteAct);
}

void XmlSetting::readFromXmlFile(const QString &fileName)
{
	qDebug() << __FUNCTION__ << tr("Opening file %1......").arg(QFileInfo(fileName).fileName());
	m_bIsinit = false;
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString strErrorMsg;
		int nErrorLine = 0, nErrorColumn = 0;
		QDomDocument dom;
		if (dom.setContent(&file, false, &strErrorMsg, &nErrorLine, &nErrorColumn))
		{
			m_tree->clear();
			m_saveFileName = fileName;		//����򿪵��ļ���
			//�����в�����ڵ�
			QDomElement domElem = dom.documentElement();
			QTreeWidgetItem *pRootItem = addNodeItem(NULL, domElem.tagName());
			insertAttr(domElem, pRootItem);
			m_tree->addTopLevelItem(pRootItem);
			//���ڵ��ϵݹ�����ӽڵ�
			readChild(domElem, pRootItem);
		}
		else
			QMessageBox::warning(NULL, tr("Warning"), tr("XML File error : %1, Line : %2, Column : %3.")
			.arg(strErrorMsg).arg(nErrorLine).arg(nErrorColumn));
	}

	file.close();
	m_bIsinit = true;
	if (m_tree->topLevelItem(0))
		setEditActEnable(true);
	qDebug() << __FUNCTION__ << tr("Open file %1 to complete!").arg(QFileInfo(fileName).fileName());
}

void XmlSetting::readChild(QDomElement& domElem, QTreeWidgetItem* pItem)
{
	QDomNode node = domElem.firstChild();

	while (!node.isNull())
	{
		//���ע�ͽڵ�
		if (node.isComment())
		{
			addCommentItem(pItem, node.nodeValue());
		}

		//��ӽڵ��ı�
		if (node.isText())
		{
			addTextItem(pItem, node.nodeValue());
		}

		//����ӽڵ�
		if(node.isElement())        
		{
			QDomElement element = node.toElement();
			QString strName = element.tagName();
			//����ǰ�ڵ�������ӵ��ؼ�
			QTreeWidgetItem *pChildItem = addNodeItem(pItem, strName);

			//����ǰ�ڵ��Ԫ�����Բ��뵽�ؼ�
			insertAttr(element, pChildItem);

			//�ݹ���뵱ǰ�ڵ���ӽڵ�
			readChild(element, pChildItem);            
		}
		node = node.nextSibling();
	}
}

void XmlSetting::insertAttr(QDomElement& domElem, QTreeWidgetItem* pItem)
{
	QDomNamedNodeMap attrMap = domElem.attributes();
	for (int i=0; i<attrMap.size(); ++i)
	{
		QDomNode attrNode = attrMap.item(i);
		QString strAttrName = attrNode.toAttr().name();	//Ԫ��������
		QString strAttrValue = attrNode.toAttr().value();
		if (strAttrValue.isEmpty())						//��ֵ������Ϊ0ռλ
			strAttrValue = "0";

		addAttrItem(pItem, strAttrName, strAttrValue);
	}
}

void XmlSetting::writeToXmlFIle(const QString &fileName)
{
	qDebug() << __FUNCTION__;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return ;
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument("1.0", true);		//��ʼд���ĵ�
	
	QTreeWidgetItem *pRootItem = m_tree->topLevelItem(0);	//��ȡ���еĸ��ڵ�
	if (pRootItem)
	{
		xmlWriter.writeStartElement(pRootItem->text(0));	//��ʼд����ڵ�

		//��������ӽڵ㣬��ִ��
		if (pRootItem->childCount())
			writeChild(xmlWriter, pRootItem);				//�ݹ�д���ӽڵ�

		xmlWriter.writeEndElement();						//����д����ڵ�
	}
	xmlWriter.writeEndDocument();					//����д���ĵ�

	file.close();									//�ر��ļ�
}

void XmlSetting::writeChild(QXmlStreamWriter &xmlWriter, QTreeWidgetItem *pItem)
{
	if (!pItem)
		return ;

	int nChildNum = pItem->childCount();
	//д������
	for (int i=0; i<nChildNum; ++i)
	{
		QTreeWidgetItem *pChildItem = pItem->child(i);
		QString strName = pChildItem->text(0);
		QString strValue = pChildItem->text(1);
		if (!strValue.isEmpty() && strName != m_strComment && strName != m_strText)
		{
			xmlWriter.writeAttribute(strName, strValue);	//����
		}
	}
	//д���ӽڵ�
	for (int i=0; i<nChildNum; ++i)
	{
		QTreeWidgetItem *pChildItem = pItem->child(i);
		QString strName = pChildItem->text(0);
		QString strValue = pChildItem->text(1);
		if (strValue.isEmpty())
		{
			xmlWriter.writeStartElement(strName);	//��ʼ�ӽڵ�
			writeChild(xmlWriter, pChildItem);
			xmlWriter.writeEndElement();			//�����ӽڵ�
		}
		else if (!strValue.isEmpty() && strName == m_strComment)
		{
			xmlWriter.writeComment(strValue);		//ע��
		}
		else if (!strValue.isEmpty() && strName == m_strText)
		{
			xmlWriter.writeCharacters(strValue);	//�ڵ�����
		}
	}
}

void XmlSetting::on_m_tree_itemDoubleClicked(QTreeWidgetItem *pItem, int column)
{
	//�ڵ��Value�в��ɱ༭�޸�
	if (column == 1 && pItem->text(1).isEmpty())
		return ;

	//ע�͡��ı���name�в��ɱ༭�޸�
	if (column == 0 && (pItem->text(0) == m_strComment || pItem->text(0) == m_strText))
		return ;

	m_tree->openPersistentEditor(pItem, column);		//ʹ��λ�õĽڵ�ɱ༭
}

void XmlSetting::on_m_tree_itemChanged(QTreeWidgetItem *pItem, int column)
{
	if (!m_bIsinit)
		return ;

	//�ڵ��value�в���Ϊ��
	if (pItem->text(column).isEmpty())
	{
		QMessageBox::warning(NULL, "Warning", "Content can't be empty, Set to the default value of NULL!", QMessageBox::Yes);
		pItem->setText(column, "NULL");
		return ;
	}

	//�ڵ㡢���Բ��������ֿ�ͷ
	QChar c = pItem->text(0).at(0);
	if (c.isDigit())
	{
		QMessageBox::warning(NULL, "Warning", "Name can't start with a number!", QMessageBox::Yes);
		pItem->setText(0, "Name");
		return ;
	}

	writeToXmlFIle(m_saveFileName);
	m_tree->closePersistentEditor(pItem, column);	//ʹ��λ�õĽڵ㲻�ɱ༭
}

void XmlSetting::on_m_tree_itemPressed(QTreeWidgetItem *pItem, int column)
{
	if (!(QApplication::mouseButtons() & Qt::RightButton)) 
		return;

	//�����˵���
	if (m_editMenu)
		m_editMenu->exec(QCursor::pos());
}

void XmlSetting::on_m_tree_itemEntered(QTreeWidgetItem *pItem, int column)
{
	// 	m_tree->closePersistentEditor(pItem, column);	//ʹ��λ�õĽڵ㲻�ɱ༭
}

void XmlSetting::m_buildNewFileAct_triggered()
{
	QString strFileName = QFileDialog::getSaveFileName(NULL, NULL, ".", "config(*.xml)");
	if (strFileName.isEmpty())
		return ;

	m_saveFileName = strFileName;
	m_tree->clear();
	QTreeWidgetItem *pRootItem = addNodeItem(NULL, "Root", true);
	m_tree->addTopLevelItem(pRootItem);
	setEditActEnable(true);
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_openNewFileAct_triggered()
{
	QString strFileName = QFileDialog::getOpenFileName(NULL, NULL, ".", "config(*.xml)");
	if (strFileName.isEmpty())
		return ;

	readFromXmlFile(strFileName);
}

void XmlSetting::m_saveAsFileAct_triggered()
{
	QString strFileName = QFileDialog::getSaveFileName(NULL, NULL, ".", "config(*.xml)");
	if (strFileName.isEmpty())
		return ;

	m_saveFileName = strFileName;
	writeToXmlFIle(m_saveFileName);
}

QTreeWidgetItem* XmlSetting::getTreeCurrentItem()
{
	if (m_tree)
		return m_tree->currentItem();
	else
		return NULL;
}
void XmlSetting::m_addNodeAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//������ǽڵ㣬���ܲ���
	if (!isNodeItem(pItem))
		return ;

	addNodeItem(pItem, "nodeName", true);
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addAttrAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//������ǽڵ㣬���ܲ���
	if (!isNodeItem(pItem))
		return ;

	addAttrItem(pItem, "attrName", "0");
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addComment_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//������ǽڵ㣬���ܲ���
	if (!isNodeItem(pItem))
		return ;

	addCommentItem(pItem, "comment");
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addText_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//������ǽڵ㣬���ܲ���
	if (!isNodeItem(pItem))
		return ;

	addTextItem(pItem, "text");
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_deleteAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	bool ret = deleteItem(pItem);
	if (ret)
		writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_upAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	bool ret = moveItem(pItem, false);	//�ڵ�����
	if (ret)
		writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_downAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	bool ret = moveItem(pItem, true);	//�ڵ�����
	if (ret)
		writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_copyAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	if (pItem)
	{
		if (m_copyItem)
			deleteItem(m_copyItem);
		m_copyItem = copyItem(pItem);
	}
}

void XmlSetting::m_pasteAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();

	if (!pItem)
		return ;

	//�����ǰ�ڵ������ԡ�ע�͡��ı��ڵ㣬����ճ���ӽڵ�
	if (!isNodeItem(pItem))
		return ;

	if (m_copyItem)
	{
		QTreeWidgetItem *pCopyItem = copyItem(m_copyItem);
		pItem->addChild(pCopyItem);
		pItem->setExpanded(true);
		writeToXmlFIle(m_saveFileName);
	}
}

void XmlSetting::setAddActEnable(bool bFlag)
{
	m_addNodeAct->setEnabled(bFlag);
	m_addAttrAct->setEnabled(bFlag);
	m_addComment->setEnabled(bFlag);
	m_addText->setEnabled(bFlag);
}

void XmlSetting::setEditActEnable(bool bFlag)
{
	setAddActEnable(bFlag);
	m_deleteAct->setEnabled(bFlag);
	m_upAct->setEnabled(bFlag);
	m_downAct->setEnabled(bFlag);
	m_copyAct->setEnabled(bFlag);
	m_pasteAct->setEnabled(bFlag);
}

//�ڵ����ֲ��������ֿ�ͷ
QTreeWidgetItem* XmlSetting::addNodeItem(QTreeWidgetItem *pItem, QString strName, bool bExpand)
{
	QTreeWidgetItem* pChildItem = NULL;
	if (pItem)
	{
		pItem->setExpanded(bExpand);
		pChildItem = new QTreeWidgetItem(pItem);
	}
	else
		pChildItem = new QTreeWidgetItem();

	pChildItem->setText(0, strName);
	pChildItem->setTextColor(0, Qt::blue);

	return pChildItem;
}

//�������ֲ��������ֿ�ͷ
void XmlSetting::addAttrItem(QTreeWidgetItem *pItem, QString strName, QString strValue, bool bExpand)
{
	if (pItem)
		pItem->setExpanded(bExpand);
	else
		return ;

	QTreeWidgetItem* pChildItem = new QTreeWidgetItem(pItem);
	pChildItem->setText(0, strName);
	pChildItem->setText(1, strValue);
	pChildItem->setTextColor(0, Qt::red);
}

//���ע�ͽڵ�
void XmlSetting::addCommentItem(QTreeWidgetItem *pItem, QString strComment, bool bExpand)
{
	if (pItem)
		pItem->setExpanded(bExpand);
	else
		return ;

	QTreeWidgetItem* pChildItem = new QTreeWidgetItem(pItem);
	pChildItem->setText(0, m_strComment);	//ע�ͽڵ�nameĬ��Ϊ#comment
	pChildItem->setText(1, strComment);
	QFont font;
	font.setBold(true);
	font.setItalic(true);
	pChildItem->setFont(0, font);
	pChildItem->setFont(1, font);
	pChildItem->setTextColor(0, Qt::green);
	pChildItem->setTextColor(1, Qt::green);
}

void XmlSetting::addTextItem(QTreeWidgetItem *pItem, QString strText, bool bExpand)
{
	if (pItem)
		pItem->setExpanded(bExpand);
	else
		return ;

	QTreeWidgetItem* pChildItem = new QTreeWidgetItem(pItem);
	pChildItem->setText(0, m_strText);
	pChildItem->setText(1, strText);
	QFont font;
	font.setBold(true);
	font.setItalic(true);
	pChildItem->setFont(0, font);
	pChildItem->setTextColor(0, Qt::darkGray);		
}

bool XmlSetting::deleteItem(QTreeWidgetItem *pItem)
{
	if (!pItem)
		return false;

	int nChildCount = pItem->childCount();
	if (nChildCount)
	{
		for (int i=0; i<nChildCount; ++i)
		{
			deleteItem(pItem->child(nChildCount - 1 - i));
		}
	}
	if (pItem)
	{
		delete pItem;
		pItem = NULL;
	}

	return true;
}

//bFlag = true ���ƣ�bFlag = flase ����
bool XmlSetting::moveItem(QTreeWidgetItem *pItem, bool bFlag)
{
	if (!pItem)
		return false;

	QTreeWidgetItem *pParentItem = pItem->parent();
	if (!pParentItem)
		return false;

	int nIndex = pParentItem->indexOfChild(pItem);
	if (bFlag)
	{
		if (nIndex == pParentItem->childCount() - 1)
			return false;
		nIndex = (nIndex == pParentItem->childCount() - 1) ? nIndex :  ++nIndex;
	}
	else
	{
		if (nIndex == 0)
			return false;
		nIndex = nIndex ? --nIndex :  nIndex;
	}

	pParentItem->removeChild(pItem);
	pParentItem->insertChild(nIndex, pItem);

	m_tree->setCurrentItem(pItem);
	
	return true;
}

QTreeWidgetItem* XmlSetting::copyItem(QTreeWidgetItem *pItem)
{
	if (!pItem)
		return NULL;

	QTreeWidgetItem* pParentItem = new QTreeWidgetItem(*pItem);

	int nChildCount = pItem->childCount();
	for (int i=0; i<nChildCount; ++i)
	{
		QTreeWidgetItem* pChildItem = copyItem(pItem->child(i));
		pParentItem->addChild(pChildItem);
	}
	
	return pParentItem;
}

bool XmlSetting::isNodeItem(QTreeWidgetItem *pItem)
{
	if (!pItem)
		return false;

	bool bRet = pItem->text(1).isEmpty();

	return bRet;
}

//���ص��Ҽ������˵��¼�
void XmlSetting::contextMenuEvent(QContextMenuEvent *e)
{
	Q_UNUSED(e)
	//�����˵�
	if (m_fileMenu)
		m_fileMenu->exec(QCursor::pos());
}
