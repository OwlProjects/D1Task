#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "ColorItemModel.h"
#include "ColorDotsItemsModel.h"


#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLoadImage_triggered();

    void on_pushButtonOpen_clicked();

private: // fields
    Ui::MainWindow *ui;

private: // functions
    bool loadFile(const QString &fileName);
    QImage image_;
    ColorItemModel *listModel_;
    ColorDotsItemsModel *itemsModel_;

};
#endif // MAINWINDOW_H
