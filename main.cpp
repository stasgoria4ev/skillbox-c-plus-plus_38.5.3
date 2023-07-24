#include <QtGui/QtGui>
#include <QApplication>

#include <QGraphicsscene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
//
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>

QImage blurImage(const QImage& source, int blurRadius) {
    if(source.isNull()) return {};
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    //
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),
                 QRectF(0, 0, source.width(), source.height()));
    return result;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget pictureViewWindow;
    QVBoxLayout vbox(&pictureViewWindow);
    //QHBoxLayout buttons;
    auto *pictureWidget = new QLabel(&pictureViewWindow);
    vbox.addWidget(pictureWidget);
    auto *songSlider = new QSlider(&pictureViewWindow);
    songSlider->setOrientation(Qt::Horizontal);
    songSlider->setMinimum(0);
    songSlider->setMaximum(10);
    vbox.addWidget(songSlider);

    auto *openFileButton = new QPushButton("Open", &pictureViewWindow);
    /*buttons.addWidget(openFileButton);
    vbox.addLayout(&buttons);*/

    vbox.addWidget(openFileButton);

    //
    QString filePath;

    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, &pictureWidget] {
        filePath = QFileDialog::getOpenFileName(nullptr,
                                     "Open picture",
                                     "..\\",
                                     "picture files (*.jpg , *.png)");
        //при выборе файла картинка не отображалась в окне, пока не начнёт двигаться слайдер, поэтому добавил setPixmap ещё и здесь.
        pictureWidget->setPixmap(QPixmap::fromImage(QImage(filePath).scaled(
                pictureWidget->width(),
                pictureWidget->height(), Qt::KeepAspectRatio)));
    });

    QObject::connect(songSlider, &QSlider::valueChanged, [&pictureWidget, &filePath](int newValue){
        pictureWidget->setPixmap(QPixmap::fromImage(blurImage(QImage(filePath), newValue).scaled(
                pictureWidget->width(),
                pictureWidget->height(), Qt::KeepAspectRatio)));
    });

    pictureViewWindow.resize(640, 554);
    pictureViewWindow.show();

    return QApplication::exec();
}

//#include "main.moc"