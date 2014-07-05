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

#include "maintabwidget.h"
#include "ui_maintabwidget.h"

#include "model/plan.h"
#include "model/daysmodel.h"
#include "model/calendarsmodel.h"
#include "model/resourcesmodel.h"
#include "model/tasksmodel.h"

/*************************************************************************************************/
/***************************** Tabbed widget containing main screens *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainTabWidget::MainTabWidget( QWidget* parent ) : QTabWidget( parent ), ui( new Ui::MainTabWidget )
{
  // setup palette & ui for main tab widget
  QPalette  pal = palette();
  pal.setBrush( QPalette::Inactive, QPalette::Highlight, QColor("#E0E0E0") );
  setPalette( pal );
  ui->setupUi( this );

  // set models & delegates for table views
  setModels();

  // set initial column widths for tables views
  //plan->tasks()->setColumnWidths( ui->tasksView );
  plan->resources()->setColumnWidths( ui->resourcesView );
  ui->calendarsView->horizontalHeader()->setDefaultSectionSize( 150 );
  plan->days()->setColumnWidths( ui->daysView );

}

/****************************************** destructor *******************************************/

MainTabWidget::~MainTabWidget()
{
  // free up memory used by the ui
  delete ui;
}

/****************************************** endEdits *********************************************/

void MainTabWidget::endEdits()
{
  // end any task/resource/calendar/day edits in progress
  ui->tasksView->endEdit();
  ui->resourcesView->endEdit();
  ui->calendarsView->endEdit();
  ui->daysView->endEdit();
}

/****************************************** setModels ********************************************/

void MainTabWidget::setModels()
{
  // ensure table views are connected to correct models
  ui->tasksView->setModel( plan->tasks() );
  ui->resourcesView->setModel( plan->resources() );
  ui->calendarsView->setModel( plan->calendars() );
  ui->daysView->setModel( plan->days() );
  //ui->ganttView->setTable( ui->tasksView );
}
