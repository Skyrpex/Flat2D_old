#include "TextureDirWidget.hpp"
#include "ui_TextureDirWidget.h"
#include "TextureIconProvider.hpp"
#include <QFileSystemModel>
#include <QDebug>

TextureDirWidget::TextureDirWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextureDirWidget),
    m_model(new QFileSystemModel)
{
    ui->setupUi(this);

    m_model->setRootPath(QDir::currentPath());
    m_model->setNameFilterDisables(false);
    m_model->setNameFilters(nameFilters());
    m_model->setIconProvider(new TextureIconProvider);

    ui->treeView->setModel(m_model);
    ui->treeView->setRootIndex(m_model->index(QDir::currentPath()));
    for(int i = 1; i < m_model->columnCount(); ++i) {
        ui->treeView->setColumnHidden(i, true);
    }

}

TextureDirWidget::~TextureDirWidget()
{
    delete ui;
}

QStringList TextureDirWidget::nameFilters() const
{
    return QStringList() << "*.jpg"
                         << "*.png";
}
