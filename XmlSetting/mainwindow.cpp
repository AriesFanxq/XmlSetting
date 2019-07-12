#include "mainwindow.h"
#include "xmlsetting.h"
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	initMenu();
	initToolBar();
	initWidget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initMenu()
{
	m_fileMenu = menuBar()->addMenu("File(&F)");

	m_editMenu = menuBar()->addMenu("Edit(&E)");

	m_helpMenu = menuBar()->addMenu("Help(&H)");
	m_helpAct = m_helpMenu->addAction("&Help", 
		this, SLOT(on_m_helpAct_triggered()), QKeySequence("Ctrl+H"));
}

void MainWindow::initToolBar()
{
	m_fileToolBar = addToolBar("File ToolBar");

	m_editToolBar = addToolBar("Edit ToolBar");

	m_helpToolBar = addToolBar("Help ToolBar");
	m_helpToolBar->addAction(m_helpAct);
}

void MainWindow::initWidget()
{
	m_xmlSettingDlg = new XmlSetting();
	m_xmlSettingDlg->getMenu("file", m_fileMenu, m_fileToolBar);
	m_xmlSettingDlg->getMenu("edit", m_editMenu, m_editToolBar);

	setCentralWidget(m_xmlSettingDlg);
	setWindowTitle(tr("Parameter Maker"));
}

void MainWindow::on_m_helpAct_triggered()
{
	QMessageBox::information(NULL, tr("Help"), tr("Company: JSPT\nAuthor: Fanxq"));
}