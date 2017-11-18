#include "webcam.h"
#include "ui_webcam.h"

#include <QtCore>
#include <cstdlib>

Webcam::Webcam(QWidget *parent) : QMainWindow(parent), ui(new Ui::Webcam)
{
    ui->setupUi(this);

    now = time(NULL);
    nowLocal = *localtime(&now);
    srand(time(NULL));

    capture.open(0);
    if(!capture.isOpened())
    {
        std::cout << "error opening camera" << std::endl;
    }

    qtimer = new QTimer(this);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(streamer()));
    qtimer->start(20);
}

Webcam::~Webcam()
{
    delete ui;
}

void Webcam::on_takePicture_clicked()
{
    string imageName = "Snapshot saved as: " + takePicture();
    QString name = QString::fromStdString(imageName);
    ui->status->clear();           // clear all text currently in the text area
    ui->status->setText(name);     // show the name of the video saved
}

void Webcam::streamer()
{
    Mat copy;
    Mat frame;
    capture.read(frame);
    if (frame.empty())
    {
        return;
    }

    flip(frame, flippedFrame, 1);
    flippedFrame.copyTo(copy);
    QImage image_to_show = cvt_Mat_To_Qt_Image(copy);
    ui->videoArea->setPixmap(QPixmap::fromImage(image_to_show));
}

string Webcam::takePicture()
{
    std::string imageName = to_string(nowLocal.tm_hour) + ":" + to_string(nowLocal.tm_min) + ":" + to_string(nowLocal.tm_sec) + ":" + to_string(rand() % 500) + ".jpg";
    imwrite(imageName, flippedFrame);
    return imageName;
}

QImage Webcam::cvt_Mat_To_Qt_Image(Mat mat)
{
    // the whole code in this function was copied from the file in the link below
    // https://github.com/MicrocontrollersAndMore/OpenCV_3_Windows_10_Installation_Tutorial/blob/master/CannyWebcamWithQtGUI.cpp

    if(mat.channels() == 1) {                   // if grayscale image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // declare and return a QImage
    } else if(mat.channels() == 3) {            // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);     // invert BGR to RGB
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // declare and return a QImage
    } else {
        qDebug() << "in convertOpenCVMatToQtQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}
