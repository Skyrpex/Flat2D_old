#include "ViewWidget.hpp"
#include "ui_ViewWidget.h"
#include <QToolButton>
#include <QSpacerItem>

ViewWidget::ViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewWidget)
{
    ui->setupUi(this);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(ui->createButton);
    group->addButton(ui->transformButton);
    group->addButton(ui->parentEditButton);

    transformButton->setChecked(true);
}

ViewWidget::~ViewWidget()
{
    delete ui;
}
