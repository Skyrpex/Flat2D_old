#include "ViewWidget.hpp"
#include "ui_ViewWidget.h"
#include <QToolButton>
#include <QSpacerItem>

ViewWidget::ViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewWidget)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(ui->toolBar);
    layout->setMargin(0);
    layout->setSpacing(4);
    layout->setContentsMargins(5, 0, 0, 0);

    QToolButton *createButton = new QToolButton;
    createButton->setText("Create");
    createButton->setCheckable(true);
    createButton->setAutoRaise(true);
    connect(createButton, SIGNAL(pressed()), ui->graphicsView, SLOT(setCreateEditMode()));
    layout->addWidget(createButton);

    QToolButton *transformButton = new QToolButton;
    transformButton->setText("Transform");
    transformButton->setCheckable(true);
    transformButton->setAutoRaise(true);
    connect(transformButton, SIGNAL(pressed()), ui->graphicsView, SLOT(setTransformEditMode()));
    layout->addWidget(transformButton);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(createButton);
    group->addButton(transformButton);

    layout->addStretch();

    transformButton->setChecked(true);
}

ViewWidget::~ViewWidget()
{
    delete ui;
}
