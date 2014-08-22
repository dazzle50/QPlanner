/***************************************************************************
 *   Copyright (C) 2014 by Richard Crook                                   *
 *   http://code.google.com/p/projectplanner                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QFileDialog>
#include <QUndoView>
#include <QUndoStack>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maintabwidget.h"
#include "model/plan.h"
#include "model/tasksmodel.h"
#include "model/task.h"

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
  // initialise private variables
  m_undoview = nullptr;
  m_tabs     = new MainTabWidget();

  // setup ui for main window including central widget of tabs
  ui->setupUi( this );
  setCentralWidget( m_tabs );
  resize( 900, 450 );

  // ensure plan tab and is kept up-to-date when plan signals changes
  connect( plan, SIGNAL(signalPlanUpdated()), m_tabs, SLOT(slotUpdatePlanTab()),
           Qt::UniqueConnection );

  // ensure menus and plan tab are kept up-to-date when current tab changes
  slotTabChange( m_tabs->currentIndex() );
  connect( m_tabs, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)),
           Qt::UniqueConnection );

  // update edit menu with undostack undo & redo actions
  setModels();
}

/******************************************* setModels *******************************************/

void MainWindow::setModels()
{
  // set undostack for edit menu undo/redo
  QAction* undoAction = plan->undostack()->createUndoAction( this );
  undoAction->setShortcut( QKeySequence::Undo );
  undoAction->setStatusTip( "Undo the last operation" );
  ui->menuEdit->insertAction( ui->actionUndo, undoAction );
  QAction* redoAction = plan->undostack()->createRedoAction( this );
  redoAction->setShortcut( QKeySequence::Redo );
  redoAction->setStatusTip( "Redo the last operation" );
  ui->menuEdit->insertAction( ui->actionUndo, redoAction );
  ui->menuEdit->removeAction( ui->actionUndo );
  ui->menuEdit->removeAction( ui->actionRedo );
  ui->actionUndo = undoAction;
  ui->actionRedo = redoAction;

  // set undostack for undoview
  if ( m_undoview != nullptr ) m_undoview->setStack( plan->undostack() );

  // set models for each main tab widget
  m_tabs->setModels();
  foreach( MainTabWidget* tabs, m_windows )
    if (tabs)
    {
      tabs->setModels();
      tabs->updateGantt();
    }

  // ensure is task data changed, indent & outdent are enabled correctly
  connect( plan->tasks(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
           this, SLOT(slotTaskDataChanged(QModelIndex,QModelIndex)),
           Qt::UniqueConnection );

  // ensure when select changes on tasks view, indent & outdent are enabled correctly
  connect( m_tabs->tasksSelectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT(slotTaskSelectionChanged(QItemSelection,QItemSelection)),
           Qt::UniqueConnection );
}

/******************************************* message *********************************************/

void MainWindow::message( QString msg )
{
  // raise other windows
  foreach( MainTabWidget* tabs, m_windows )
    if (tabs) tabs->raise();

  // show message on status bar and enure is top & active
  raise();
  activateWindow();
  ui->statusBar->showMessage( msg );
}

/************************************** slotTaskDataChanged **************************************/

void MainWindow::slotTaskDataChanged( const QModelIndex& index, const QModelIndex& )
{
  // ensure if task becomes not-null (because title added) check if indent/outdent affected
  if ( index.column() == Task::SECTION_TITLE )
    slotTaskSelectionChanged( QItemSelection(), QItemSelection() );
}

/*********************************** slotTaskSelectionChanged ************************************/

void MainWindow::slotTaskSelectionChanged( const QItemSelection&, const QItemSelection& )
{
  // task selection has changed, determine unique rows selected
  QSet<int>  rows;
  foreach( QModelIndex index, m_tabs->tasksSelectionIndexes() )
    rows.insert( index.row() );

  // ensure indent & outdent actions only enabled is action possible
  bool inEnable  = false;
  bool outEnable = false;

  foreach( int row, rows )
  {
    if ( plan->tasks()->canIndent( row ) )  inEnable = true;
    if ( plan->tasks()->canOutdent( row ) ) outEnable = true;
  }

  ui->actionIndent->setEnabled( inEnable );
  ui->actionOutdent->setEnabled( outEnable );
}

/******************************************* slotIndent ******************************************/

void MainWindow::slotIndent()
{
  // indent task(s) - determine unique rows selected
  m_tabs->endEdits();
  QSet<int>  rows;
  foreach( QModelIndex index, m_tabs->tasksSelectionIndexes() )
    rows.insert( index.row() );

  plan->tasks()->indentRows( rows );
}

/****************************************** slotOutdent ******************************************/

void MainWindow::slotOutdent()
{
  // outdent task(s) - determine unique rows selected
  m_tabs->endEdits();
  QSet<int>  rows;
  foreach( QModelIndex index, m_tabs->tasksSelectionIndexes() )
    rows.insert( index.row() );

  plan->tasks()->outdentRows( rows );
}

/****************************************** slotFileNew ******************************************/

void MainWindow::slotFileNew()
{
  // slot for file new plan action

  qDebug("MainWindow::slotFileNew() - TODO !!!!");
}

/****************************************** slotFileOpen *****************************************/

bool MainWindow::slotFileOpen()
{
  // slot for file open plan action - get user to select filename and location
  m_tabs->endEdits();
  QString filename = QFileDialog::getOpenFileName();
  if ( filename.isEmpty() )
  {
    message();
    return false;
  }

  return false; // loadPlan( filename );
}

/****************************************** slotFileSave *****************************************/

bool MainWindow::slotFileSave()
{
  // slot for file save plan action - if plan has filename, save to same file and location
  m_tabs->endEdits();

  return false;
}

/***************************************** slotFileSaveAs ****************************************/

bool MainWindow::slotFileSaveAs()
{
  // slot for file saveAs plan action - get user to select filename and location
  m_tabs->endEdits();

  return false;
}

/***************************************** slotFilePrint *****************************************/

void MainWindow::slotFilePrint()
{
  // slot for file saveAs plan action
  m_tabs->endEdits();
  qDebug("MainWindow::slotFilePrint() - TODO !!!!");
}

/************************************** slotFilePrintPreview *************************************/

void MainWindow::slotFilePrintPreview()
{
  // slot for file saveAs plan action
  m_tabs->endEdits();
  qDebug("MainWindow::slotFilePrintPreview() - TODO !!!!");
}

/****************************************** slotFileExit *****************************************/

void MainWindow::slotFileExit()
{
  // slot for file saveAs plan action
  m_tabs->endEdits();
  qDebug("MainWindow::slotFileExit() - TODO !!!!");
}

/*************************************** slotAboutQPlanner ***************************************/

void MainWindow::slotAboutQPlanner()
{
  // slot for file saveAs plan action
  m_tabs->endEdits();
  qDebug("MainWindow::slotAboutQPlanner() - TODO !!!!");
}

/**************************************** slotSchedulePlan ***************************************/

void MainWindow::slotSchedulePlan()
{
  // get plan to reschedule all the tasks
  m_tabs->endEdits();
  //plan->schedule();
}

/*************************************** slotStretchTasks ****************************************/

void MainWindow::slotStretchTasks( bool checked )
{
  // if stretch tasks flag is changed, trigger redraw of gantt
  m_tabs->endEdits();

  // TODO
  Q_UNUSED( checked );
}

/***************************************** slotNewWindow *****************************************/

void MainWindow::slotNewWindow()
{
  // open new window
  MainTabWidget*  tabWidget = new MainTabWidget();
  tabWidget->setAttribute( Qt::WA_QuitOnClose, false );
  tabWidget->setAttribute( Qt::WA_DeleteOnClose, true );
  //tabWidget->removePlanTab();
  tabWidget->setWindowTitle( windowTitle() );
  tabWidget->show();
  m_windows.append( tabWidget );
}

/*************************************** slotViewUndoStack ***************************************/

void MainWindow::slotUndoStackView( bool checked )
{
  // show undo stack view window if checked, otherwise hide
  m_tabs->endEdits();
  if ( checked )
  {
    if ( m_undoview == nullptr )
    {
      m_undoview = new QUndoView( plan->undostack() );
      m_undoview->setWindowTitle( "Undo stack" );
      m_undoview->setAttribute( Qt::WA_QuitOnClose, false );
      m_undoview->setAttribute( Qt::WA_DeleteOnClose, true );
      connect( m_undoview, SIGNAL(destroyed()), this, SLOT(slotUndoStackViewDestroyed()),
               Qt::UniqueConnection );
    }
    m_undoview->show();
  }
  else
  {
    if ( m_undoview ) m_undoview->hide();
  }
}

/*********************************** slotUndoStackViewDestroyed **********************************/

void MainWindow::slotUndoStackViewDestroyed()
{
  // undo stack view window closed and destroyed so reset pointer and uncheck action
  m_undoview = nullptr;
  ui->actionUndoStackView->setChecked( false );
}

/***************************************** slotTabChange *****************************************/

void MainWindow::slotTabChange( int index )
{
  // check if switched to tasks/gantt tab, enable its menu & actions, otherwise hide them
  if ( index == m_tabs->indexOfTasksTab() )
  {
    ui->menuTask->menuAction()->setVisible( true );
    ui->actionIndent->setVisible( true );
    ui->actionOutdent->setVisible( true );
  }
  else
  {
    ui->menuTask->menuAction()->setVisible( false );
    ui->actionIndent->setVisible( false );
    ui->actionOutdent->setVisible( false );
  }

  // ensure plan reflects 'Plan' tab widgets and vice versa
  m_tabs->updatePlan();
  m_tabs->slotUpdatePlanTab();
}
