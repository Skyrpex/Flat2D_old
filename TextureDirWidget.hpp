#ifndef TEXTUREDIRWIDGET_H
#define TEXTUREDIRWIDGET_H

#include <QWidget>

class QFileSystemModel;

namespace Ui {
class TextureDirWidget;
}

class TextureDirWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TextureDirWidget(QWidget *parent = 0);
    ~TextureDirWidget();
    
private:
    QStringList nameFilters() const;

    Ui::TextureDirWidget *ui;
    QFileSystemModel *m_model;
};

#endif // TEXTUREDIRWIDGET_H
