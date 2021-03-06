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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

class QUndoView;
class QItemSelection;
class MainTabWidget;
class QXmlStreamReader;
class QTableView;

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow( QWidget* parent = nullptr );         // constructor

  void setModels();                            // set models for views & undostack
  void message( QString = "" );                // show message on status bar and enure is top & active
  void setTitle( QString = "" );               // update main window title to include text
  bool savePlan( QString );                    // save plan to xml file
  bool loadPlan( QString );                    // load plan from xml file
  void loadDisplayData( QXmlStreamReader* );   // load display data from xml stream
  void loadTableColumnsRows( QList<QTableView*>, QXmlStreamReader*, QString );

  QList<QTableView*> listTasksTables();        // list task tables in all windows
  QList<QTableView*> listResourcesTables();    // list resource tables in all windows
  QList<QTableView*> listCalendarsTables();    // list calendar tables in all windows
  QList<QTableView*> listDaysTables();         // list day tables in all windows

public slots:
  void slotUndoStackView( bool );              // slot for actionUndoStackView triggered signal
  void slotUndoStackViewDestroyed();           // slot for undo stack view destroyed signal
  void slotCleanChanged( bool );               // slot for undostack clean state change
  void slotTabChange( int );                   // slot for mainTabWidget current changed signal
  void slotSchedulePlan();                     // slot for schedule plan action
  void slotIndent();                           // slot for indent task(s) action
  void slotOutdent();                          // slot for outdent task(s) action
  void slotStretchTasks( bool );               // slot for stretch tasks action
  void slotNewWindow();                        // slot for new window action

  void slotFileNew();                          // slot for file new plan action
  bool slotFileOpen();                         // slot for file open plan action
  bool slotFileSave();                         // slot for file save plan action
  bool slotFileSaveAs();                       // slot for file saveAs plan action
  void slotFilePrint();                        // slot for file print action
  void slotFilePrintPreview();                 // slot for file print preview action
  void slotAboutProjectPlanner();              // slot for about ProjectPlanner action

  void slotTaskSelectionChanged( const QItemSelection&,
                                 const QItemSelection& );   // slot for task selection change
  void slotTaskDataChanged( const QModelIndex&,
                            const QModelIndex& );           // slot for task data change

protected:
  void changeEvent( QEvent* );                 // reimplement to detect window loses active focus
  void closeEvent( QCloseEvent* );             // reimplement to check if user really wants to exit

private:
  Ui::MainWindow*         ui;                  // user interface created using qt designer
  QUndoView*              m_undoview;          // window to display contents of undostack
  MainTabWidget*          m_tabs;              // tabs for mainwindow central widget
  QList<QPointer<MainTabWidget>>  m_windows;   // list of other tabWidgets
};

#endif // MAINWINDOW_H
