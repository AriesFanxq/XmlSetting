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
	m_tree->setStyleSheet("QTreeWidget::item{height:30px}");	//使树的列宽与文件长度自适应
	QHeaderView *head = m_tree->header();
	head->setResizeMode(QHeaderView::ResizeToContents);			//设置表头宽度自适应（字体长度）
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
			m_saveFileName = fileName;		//保存打开的文件名
			//在树中插入根节点
			QDomElement domElem = dom.documentElement();
			QTreeWidgetItem *pRootItem = addNodeItem(NULL, domElem.tagName());
			insertAttr(domElem, pRootItem);
			m_tree->addTopLevelItem(pRootItem);
			//根节点上递归插入子节点
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
		//添加注释节点
		if (node.isComment())
		{
			addCommentItem(pItem, node.nodeValue());
		}

		//添加节点文本
		if (node.isText())
		{
			addTextItem(pItem, node.nodeValue());
		}

		//添加子节点
		if(node.isElement())        
		{
			QDomElement element = node.toElement();
			QString strName = element.tagName();
			//将当前节点内容添加到控件
			QTreeWidgetItem *pChildItem = addNodeItem(pItem, strName);

			//将当前节点的元素属性插入到控件
			insertAttr(element, pChildItem);

			//递归插入当前节点的子节点
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
		QString strAttrName = attrNode.toAttr().name();	//元素属性名
		QString strAttrValue = attrNode.toAttr().value();
		if (strAttrValue.isEmpty())						//空值的项设为0占位
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
	xmlWriter.writeStartDocument("1.0", true);		//开始写入文档
	
	QTreeWidgetItem *pRootItem = m_tree->topLevelItem(0);	//获取树中的根节点
	if (pRootItem)
	{
		xmlWriter.writeStartElement(pRootItem->text(0));	//开始写入根节点

		//如果存在子节点，则执行
		if (pRootItem->childCount())
			writeChild(xmlWriter, pRootItem);				//递归写入子节点

		xmlWriter.writeEndElement();						//结束写入根节点
	}
	xmlWriter.writeEndDocument();					//结束写入文档

	file.close();									//关闭文件
}

void XmlSetting::writeChild(QXmlStreamWriter &xmlWriter, QTreeWidgetItem *pItem)
{
	if (!pItem)
		return ;

	int nChildNum = pItem->childCount();
	//写入属性
	for (int i=0; i<nChildNum; ++i)
	{
		QTreeWidgetItem *pChildItem = pItem->child(i);
		QString strName = pChildItem->text(0);
		QString strValue = pChildItem->text(1);
		if (!strValue.isEmpty() && strName != m_strComment && strName != m_strText)
		{
			xmlWriter.writeAttribute(strName, strValue);	//属性
		}
	}
	//写入子节点
	for (int i=0; i<nChildNum; ++i)
	{
		QTreeWidgetItem *pChildItem = pItem->child(i);
		QString strName = pChildItem->text(0);
		QString strValue = pChildItem->text(1);
		if (strValue.isEmpty())
		{
			xmlWriter.writeStartElement(strName);	//开始子节点
			writeChild(xmlWriter, pChildItem);
			xmlWriter.writeEndElement();			//结束子节点
		}
		else if (!strValue.isEmpty() && strName == m_strComment)
		{
			xmlWriter.writeComment(strValue);		//注释
		}
		else if (!strValue.isEmpty() && strName == m_strText)
		{
			xmlWriter.writeCharacters(strValue);	//节点内容
		}
	}
}

void XmlSetting::on_m_tree_itemDoubleClicked(QTreeWidgetItem *pItem, int column)
{
	//节点的Value列不可编辑修改
	if (column == 1 && pItem->text(1).isEmpty())
		return ;

	//注释、文本的name列不可编辑修改
	if (column == 0 && (pItem->text(0) == m_strComment || pItem->text(0) == m_strText))
		return ;

	m_tree->openPersistentEditor(pItem, column);		//使该位置的节点可编辑
}

void XmlSetting::on_m_tree_itemChanged(QTreeWidgetItem *pItem, int column)
{
	if (!m_bIsinit)
		return ;

	//节点的value列不能为空
	if (pItem->text(column).isEmpty())
	{
		QMessageBox::warning(NULL, "Warning", "Content can't be empty, Set to the default value of NULL!", QMessageBox::Yes);
		pItem->setText(column, "NULL");
		return ;
	}

	//节点、属性不能以数字开头
	QChar c = pItem->text(0).at(0);
	if (c.isDigit())
	{
		QMessageBox::warning(NULL, "Warning", "Name can't start with a number!", QMessageBox::Yes);
		pItem->setText(0, "Name");
		return ;
	}

	writeToXmlFIle(m_saveFileName);
	m_tree->closePersistentEditor(pItem, column);	//使该位置的节点不可编辑
}

void XmlSetting::on_m_tree_itemPressed(QTreeWidgetItem *pItem, int column)
{
	if (!(QApplication::mouseButtons() & Qt::RightButton)) 
		return;

	//弹出菜单栏
	if (m_editMenu)
		m_editMenu->exec(QCursor::pos());
}

void XmlSetting::on_m_tree_itemEntered(QTreeWidgetItem *pItem, int column)
{
	// 	m_tree->closePersistentEditor(pItem, column);	//使该位置的节点不可编辑
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
	//如果不是节点，则不能插入
	if (!isNodeItem(pItem))
		return ;

	addNodeItem(pItem, "nodeName", true);
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addAttrAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//如果不是节点，则不能插入
	if (!isNodeItem(pItem))
		return ;

	addAttrItem(pItem, "attrName", "0");
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addComment_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//如果不是节点，则不能插入
	if (!isNodeItem(pItem))
		return ;

	addCommentItem(pItem, "comment");
	writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_addText_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	//如果不是节点，则不能插入
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
	bool ret = moveItem(pItem, false);	//节点上移
	if (ret)
		writeToXmlFIle(m_saveFileName);
}

void XmlSetting::m_downAct_triggered()
{
	QTreeWidgetItem* pItem = getTreeCurrentItem();
	bool ret = moveItem(pItem, true);	//节点上移
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

	//如果当前节点是属性、注释、文本节点，则不能粘贴子节点
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

//节点名字不能以数字开头
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

//属性名字不能以数字开头
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

//添加注释节点
void XmlSetting::addCommentItem(QTreeWidgetItem *pItem, QString strComment, bool bExpand)
{
	if (pItem)
		pItem->setExpanded(bExpand);
	else
		return ;

	QTreeWidgetItem* pChildItem = new QTreeWidgetItem(pItem);
	pChildItem->setText(0, m_strComment);	//注释节点name默认为#comment
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

//bFlag = true 下移，bFlag = flase 上移
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

//重载的右键创建菜单事件
void XmlSetting::contextMenuEvent(QContextMenuEvent *e)
{
	Q_UNUSED(e)
	//弹出菜单
	if (m_fileMenu)
		m_fileMenu->exec(QCursor::pos());
}
