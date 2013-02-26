#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setCreateMode();
    void setTransformMode();

protected:
    void keyReleaseEvent(QKeyEvent *event);
    
private:
    Ui::MainWindow *ui;
    QActionGroup *m_transformGroup;
};

#endif // MAINWINDOW_HPP
