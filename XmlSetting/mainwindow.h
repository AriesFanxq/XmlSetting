#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class XmlSetting;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:	
	//help
	void on_m_helpAct_triggered();

private:
	XmlSetting*	m_xmlSettingDlg;

	//file
	QMenu		*m_fileMenu;
	QToolBar	*m_fileToolBar;

	//edit
	QMenu		*m_editMenu;
	QToolBar	*m_editToolBar;

	//help
	QMenu		*m_helpMenu;
	QToolBar	*m_helpToolBar;
	QAction		*m_helpAct;

	void initWidget();
	void initMenu();
	void initToolBar();
};

#endif // MAINWINDOW_H
