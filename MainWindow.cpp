#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Application.hpp"
#include <QActionGroup>
#include <QKeyEvent>
#include <QUndoStack>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuEdit->addAction(qApp->undoStack()->createUndoAction(this));
    ui->menuEdit->addAction(qApp->undoStack()->createRedoAction(this));

    {
        QActionGroup *group = new QActionGroup(this);
        group->addAction(ui->actionCreate);
        group->addAction(ui->actionTransform);
    }

    ui->actionTransform->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCreateMode()
{
    ui->graphicsView->setCreateEditMode();
}

void MainWindow::setTransformMode()
{
    ui->graphicsView->setTransformEditMode();
}
