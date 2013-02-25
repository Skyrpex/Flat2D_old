#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->actionCreate);
    group->addAction(ui->actionTransform);

    group->addAction(ui->actionRotate);
    group->addAction(ui->actionTranslate);
    group->addAction(ui->actionScale);

    ui->actionTransform->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
