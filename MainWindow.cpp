#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QImage>
#include <QImageReader>
#include <QColorSpace>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listModel_(new ColorItemModel(this))
    , itemsModel_(new ColorDotsItemsModel(this))
{
    ui->setupUi(this);
    ui->checkBoxFitImage->setHidden(true);

    QItemSelectionModel *selModel = new QItemSelectionModel(listModel_);

    ui->listView->setModel(listModel_);
    ui->listView->setSelectionModel(selModel);

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(listModel_);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionModel(selModel);
    ui->tableView->setSortingEnabled(true);

    ui->treeView->setModel(itemsModel_);
    ui->treeView->setSelectionModel(selModel);

    on_pushButtonOpen_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoadImage_triggered()
{
    on_pushButtonOpen_clicked();
}

bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    image_ = newImage;
    if (image_.colorSpace().isValid())
        image_.convertToColorSpace(QColorSpace::SRgb);
    qDebug() << ui->labelImage->geometry();
    ui->labelImage->setPixmap(QPixmap::fromImage(image_));

    ui->scrollArea->setVisible(true);


    if (ui->checkBoxFitImage->isChecked()) {
        qDebug() << ui->labelImage->geometry();
        ui->labelImage->setGeometry(ui->scrollArea->geometry());
        qDebug() << ui->labelImage->geometry();
    } else {
        ui->labelImage->adjustSize();
    }

    setWindowFilePath(fileName);

    qDebug() << "Model loading started";
    statusBar()->showMessage("Model loading started");
    foreach (QObject *child, children()) {
        QWidget *w = dynamic_cast<QWidget *>(child);
        if (w) {
            w->setEnabled(false);
        }
    }
    listModel_->setImage(image_);
    itemsModel_->setImage(image_);
    foreach (QObject *child, children()) {
        QWidget *w = dynamic_cast<QWidget *>(child);
        if (w) {
            w->setEnabled(true);
        }
    }
    statusBar()->showMessage("Model loading finished");
    qDebug() << "Model loading finished";

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
                                .arg(QDir::toNativeSeparators(fileName)).arg(image_.width()).arg(image_.height()).arg(image_.depth());
    statusBar()->showMessage(message);
    return true;
}




void MainWindow::on_pushButtonOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть изображение");
    if (fileName.isEmpty()) {
        return;
    }
    loadFile(fileName);
}

