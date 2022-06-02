#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void get_process();
    void stop();
    void kill_process();
    void cont();
    void filtro();
    void cpu();
    void prioridade();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
