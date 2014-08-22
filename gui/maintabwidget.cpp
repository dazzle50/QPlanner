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

#include <QMessageBox>
#include <QUndoStack>

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

#include "command/commandpropertieschange.h"

#include "delegate/daysdelegate.h"
#include "delegate/calendarsdelegate.h"
#include "delegate/resourcesdelegate.h"
#include "delegate/tasksdelegate.h"

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
  DaysDelegate*       dd = new DaysDelegate();
  CalendarsDelegate*  cd = new CalendarsDelegate();
  ResourcesDelegate*  rd = new ResourcesDelegate();
  TasksDelegate*      td = new TasksDelegate();
  ui->daysView->setItemDelegate( dd );
  ui->calendarsView->setItemDelegate( cd );
  ui->resourcesView->setItemDelegate( rd );
  ui->tasksView->setItemDelegate( td );

  // connect delegate edit cell to slot, queued so any earlier edit is finished and closed
  connect( dd, SIGNAL(editCell(QModelIndex,QString)),
           this, SLOT(slotEditDayCell(QModelIndex,QString)), Qt::QueuedConnection );
  connect( cd, SIGNAL(editCell(QModelIndex,QString)),
           this, SLOT(slotEditCalendarCell(QModelIndex,QString)), Qt::QueuedConnection );
  connect( rd, SIGNAL(editCell(QModelIndex,QString)),
           this, SLOT(slotEditResourceCell(QModelIndex,QString)), Qt::QueuedConnection );
  connect( td, SIGNAL(editCell(QModelIndex,QString)),
           this, SLOT(slotEditTaskCell(QModelIndex,QString)), Qt::QueuedConnection );

  // hide task 0 'plan summary' and resource 0 'unassigned'
  ui->tasksView->verticalHeader()->hideSection( 0 );
  ui->resourcesView->verticalHeader()->hideSection( 0 );

  // setup tasks gantt
  ui->ganttView->createGantt( ui->ganttWidget );
  ui->ganttView->setTable( ui->tasksView );
  ui->tasksView->setHeaderHeight( ui->ganttView->scaleHeight() );

  // create new palette for read-only edit widgets with different Base colour
  QPalette*     palette = new QPalette( ui->propertiesWidget->palette() );
  palette->setColor( QPalette::Base, palette->window().color() );

  // setup plan tab
  ui->planBeginning->setPalette( *palette );
  ui->planEnd->setPalette( *palette );
  ui->fileName->setPalette( *palette );
  ui->fileLocation->setPalette( *palette );
  ui->savedBy->setPalette( *palette );
  ui->savedWhen->setPalette( *palette );
  slotUpdatePlanTab();

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
  ui->daysView->horizontalHeader()->setDefaultSectionSize( 60 );
  ui->daysView->setColumnWidth( Day::SECTION_NAME, 150 );
  ui->daysView->setColumnWidth( Day::SECTION_WORK,  50 );
  ui->daysView->setColumnWidth( Day::SECTION_PERIODS, 50 );

  // set tasks view splitter behaviour & default position
  ui->tasksGanttSplitter->setStretchFactor( 1, 1 );
  QList<int> sizes = ui->tasksGanttSplitter->sizes();
  sizes[0] = ui->tasksView->horizontalHeader()->sectionSize( 0 ) +
             ui->tasksView->horizontalHeader()->sectionSize( 1 ) +
             ui->tasksView->horizontalHeader()->sectionSize( 2 ) +
             ui->tasksView->verticalHeader()->width();
  sizes[1] = sizes[0];
  ui->tasksGanttSplitter->setSizes( sizes );
}

/****************************************** destructor *******************************************/

MainTabWidget::~MainTabWidget()
{
  // free up memory used by the ui
  delete ui;
}

/****************************************** setModels ********************************************/

void MainTabWidget::setModels()
{
  // ensure table views are connected to correct models
  ui->tasksView->setModel( plan->tasks() );
  ui->resourcesView->setModel( plan->resources() );
  ui->calendarsView->setModel( plan->calendars() );
  ui->daysView->setModel( plan->days() );
  ui->ganttView->setTable( ui->tasksView );
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

/*************************************** slotEditDayCell *****************************************/

void MainTabWidget::slotEditDayCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable day type cell edit to be automatically re-started after validation failure
  setCurrentWidget( ui->daysTab );
  ui->daysView->setCurrentIndex( index );
  QMessageBox::warning( ui->daysView, "Project Planner", warning, QMessageBox::Retry );
  ui->daysView->edit( index );

  // clear override
  plan->days()->setOverride( QModelIndex(), QString() );
}

/************************************* slotEditCalendarCell **************************************/

void MainTabWidget::slotEditCalendarCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable calendar cell edit to be automatically re-started after validation failure
  setCurrentWidget( ui->calendarsTab );
  ui->calendarsView->setCurrentIndex( index );
  QMessageBox::warning( ui->calendarsView, "Project Planner", warning, QMessageBox::Retry );
  ui->calendarsView->edit( index );

  // clear override
  plan->calendars()->setOverride( QModelIndex(), QString() );
}

/************************************* slotEditResourceCell **************************************/

void MainTabWidget::slotEditResourceCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable resource cell edit to be automatically re-started after validation failure
  setCurrentWidget( ui->resourcesTab );
  ui->resourcesView->setCurrentIndex( index );
  QMessageBox::warning( ui->resourcesView, "Project Planner", warning, QMessageBox::Retry );
  ui->resourcesView->edit( index );

  // clear override
  plan->resources()->setOverride( QModelIndex(), QString() );
}

/*************************************** slotEditTaskCell ****************************************/

void MainTabWidget::slotEditTaskCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable task cell edit to be automatically re-started after validation failure
  setCurrentWidget( ui->tasksGanttTab );
  ui->tasksView->setCurrentIndex( index );
  QMessageBox::warning( ui->tasksView, "Project Planner", warning, QMessageBox::Retry );
  ui->tasksView->edit( index );

  // clear override
  plan->tasks()->setOverride( QModelIndex(), QString() );
}

/****************************************** updateGantt ******************************************/

void MainTabWidget::updateGantt()
{
  // trigger gantt widget redraw
  ui->ganttWidget->update();
}

/**************************************** indexOfTasksTab ****************************************/

int MainTabWidget::indexOfTasksTab()
{
  // return index of tasks tab
  return indexOf( ui->tasksGanttTab );
}

/***************************************** updatePlan ********************************************/

void MainTabWidget::updatePlan()
{
  // check if we need to update plan from 'Plan' tab widgets
  if ( ui->title->text()                                != plan->title()  ||
       XDateTime::datetime( ui->planStart->dateTime() ) != plan->start()  ||
       ui->defaultCalendar->currentIndex()              != plan->index( plan->calendar() ) ||
       ui->dateTimeFormat->text()                       != plan->datetimeFormat() ||
       ui->notesEdit->toPlainText()                     != plan->notes() )
  {
    plan->undostack()->push( new CommandPropertiesChange(
      ui->title->text(),                   plan->title(),
      XDateTime::datetime( ui->planStart->dateTime() ), plan->start(),
      ui->defaultCalendar->currentIndex(), plan->index( plan->calendar() ),
      ui->dateTimeFormat->text(),          plan->datetimeFormat(),
      ui->notesEdit->toPlainText(),        plan->notes()) );
  }
}

/************************************** slotUpdatePlanTab ****************************************/

void MainTabWidget::slotUpdatePlanTab()
{
  // ensure 'Plan' tab widgets are up-to-date with what is in plan
  ui->title->setText( plan->title() );
  ui->title->setCursorPosition( 0 );

  ui->planBeginning->setText( XDateTime::toString( plan->beginning(), "dd/MM/yyyy hh:mm" ) );
  ui->planBeginning->setCursorPosition( 0 );
  ui->planBeginning->setToolTip( XDateTime::toString( plan->beginning(), plan->datetimeFormat() ) );

  ui->planStart->setTimeSpec( Qt::UTC );
  ui->planStart->setDateTimeRange( XDateTime::MIN_QDATETIME, XDateTime::MAX_QDATETIME );
  ui->planStart->setDateTime( XDateTime::qdatetime( plan->start() ) );
  ui->planStart->setToolTip( XDateTime::toString( plan->start(), plan->datetimeFormat() ) );

  ui->planEnd->setText( XDateTime::toString( plan->end(), "dd/MM/yyyy hh:mm") );
  ui->planEnd->setCursorPosition( 0 );
  ui->planEnd->setToolTip( XDateTime::toString( plan->end(), plan->datetimeFormat() ) );

  ui->defaultCalendar->clear();
  ui->defaultCalendar->addItems( plan->calendars()->namesList() );
  ui->defaultCalendar->setCurrentIndex( plan->index( plan->calendar() ) );

  ui->dateTimeFormat->setText( plan->datetimeFormat() );
  ui->dateTimeFormat->setCursorPosition( 0 );
  ui->dateTimeFormat->setToolTip( QDateTime::currentDateTime().toString( plan->datetimeFormat() ));

  ui->fileName->setText( plan->filename() );
  ui->fileName->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->filename() );

  ui->fileLocation->setText( plan->fileLocation() );
  ui->fileLocation->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->fileLocation() );

  ui->savedBy->setText( plan->savedBy() );
  ui->savedBy->setCursorPosition( 0 );
  ui->savedBy->setToolTip( plan->savedBy() );

  ui->savedWhen->setText( plan->savedWhen().toString("dd/MM/yyyy hh:mm") );
  ui->savedWhen->setCursorPosition( 0 );
  ui->savedWhen->setToolTip( plan->savedWhen().toString( plan->datetimeFormat() ) );

  ui->numTasks->setText( QString(": %1").arg( plan->numTasks() ) );
  ui->numResources->setText( QString(": %1").arg( plan->numResources() ) );
  ui->numCalendars->setText( QString(": %1").arg( plan->numCalendars() ) );
  ui->numDays->setText( QString(": %1").arg( plan->numDays() ) );

  ui->notesEdit->setPlainText( plan->notes() );
}

/************************************** tasksSelectionModel **************************************/

QItemSelectionModel* MainTabWidget::tasksSelectionModel()
{
  // return selection model for tasks table view
  return ui->tasksView->selectionModel();
}

/************************************* tasksSelectionIndexes *************************************/

QModelIndexList  MainTabWidget::tasksSelectionIndexes()
{
  // return selected indexes on tasks table view
  return ui->tasksView->selectionModel()->selection().indexes();
}
