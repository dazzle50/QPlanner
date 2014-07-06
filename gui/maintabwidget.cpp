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
#include "model/day.h"
#include "model/calendarsmodel.h"
#include "model/calendar.h"
#include "model/resourcesmodel.h"
#include "model/resource.h"
#include "model/tasksmodel.h"
#include "model/task.h"

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

  // set initial column widths for tasks table view
  ui->tasksView->horizontalHeader()->setDefaultSectionSize( 140 );
  ui->tasksView->setColumnWidth( Task::SECTION_TITLE,    150 );
  ui->tasksView->setColumnWidth( Task::SECTION_DURATION,  60 );
  ui->tasksView->setColumnWidth( Task::SECTION_WORK,      50 );
  ui->tasksView->setColumnWidth( Task::SECTION_PREDS,     80 );
  ui->tasksView->setColumnWidth( Task::SECTION_RES,       80 );
  ui->tasksView->setColumnWidth( Task::SECTION_TYPE,     150 );
  ui->tasksView->setColumnWidth( Task::SECTION_PRIORITY,  50 );
  ui->tasksView->setColumnWidth( Task::SECTION_COST,      50 );
  ui->tasksView->setColumnWidth( Task::SECTION_COMMENT,  200 );

  // set initial column widths for reources table view
  ui->resourcesView->setColumnWidth( Resource::SECTION_INITIALS,  60 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_NAME,     150 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_ORG,      150 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_GROUP,    150 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_AVAIL,     65 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_COST,      65 );
  ui->resourcesView->setColumnWidth( Resource::SECTION_COMMENT,  250 );

  // set initial column widths for calendars table view
  ui->calendarsView->horizontalHeader()->setDefaultSectionSize( 150 );

  // set initial column widths for day types table view
  ui->daysView->horizontalHeader()->setDefaultSectionSize( 70 );
  ui->daysView->setColumnWidth( Day::SECTION_NAME, 150 );
  ui->daysView->setColumnWidth( Day::SECTION_WORK,  50 );
  ui->daysView->setColumnWidth( Day::SECTION_PARTS, 50 );
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
