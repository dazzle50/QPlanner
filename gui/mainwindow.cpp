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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maintabwidget.h"

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
  //connect( plan, SIGNAL(signalPlanUpdated()), m_tabs, SLOT(slotUpdatePlanTab()),
  //         Qt::UniqueConnection );

  // ensure menus and plan tab are kept up-to-date when current tab changes
  //slotTabChange( m_tabs->currentIndex() );
  //connect( m_tabs, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)),
  //         Qt::UniqueConnection );

  // update edit menu with undostack undo & redo actions
  setModels();
}

/******************************************* setModels *******************************************/

void MainWindow::setModels()
{
  // set undostack for edit menu undo/redo

}
