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

    {
        m_transformGroup = new QActionGroup(this);
        m_transformGroup->addAction(ui->actionSelect);
        m_transformGroup->addAction(ui->actionRotate);
        m_transformGroup->addAction(ui->actionTranslate);
        m_transformGroup->addAction(ui->actionScale);
    }

    ui->actionTransform->setChecked(true);
    ui->actionSelect->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCreateMode()
{
    m_transformGroup->setDisabled(true);

    ui->graphicsView->setCreateEditMode();
}

void MainWindow::setTransformMode()
{
    m_transformGroup->setDisabled(false);

    ui->graphicsView->setTransformEditMode();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_R:
    case Qt::Key_T:
    case Qt::Key_S:
        ui->graphicsView->setSelectTransformMode();
        ui->actionSelect->setChecked(true);
        break;

    default:
        QMainWindow::keyReleaseEvent(event);
    }
}
