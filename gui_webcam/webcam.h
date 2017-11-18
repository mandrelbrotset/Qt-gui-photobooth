#ifndef WEBCAM_H
#define WEBCAM_H

#include <QMainWindow>
////////////////////////////////////////////////////////////////////////
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;
////////////////////////////////////////////////////////////////////////


namespace Ui {
class Webcam;
}

class Webcam : public QMainWindow
{
    Q_OBJECT
public slots:
    void streamer();    // function that streams frames from the  webcam

public:
    explicit Webcam(QWidget *parent = 0);
    ~Webcam();

private slots:
    void on_takePicture_clicked();

private:
    Ui::Webcam *ui;
    ////////////////////////////////////////////////////////////////////////
    time_t now;
    struct tm nowLocal;
    Mat flippedFrame;             // create opencv Mat type to store images
    QTimer* qtimer;
    VideoCapture capture;   // opencv video capture object
    string takePicture();   // returns the name of the picture taken
    QImage cvt_Mat_To_Qt_Image(Mat);    // function to convert opencv mat image format to qt image format
};

#endif // WEBCAM_H
