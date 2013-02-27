#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QActionGroup>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
