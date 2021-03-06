#include "camerawindow.h"
#include "cv.h"

using namespace cv;

//Constructor
CameraWindow::CameraWindow(CvCapture *cam, QWidget *parent)
    : QWidget(parent)
{
    m_camera = cam;
    m_photoCounter = 0;
    m_cvwidget = new CameraWidget(this);
    m_window = new QLabel(this);
    //horizontalGroupBox = new QGroupBox(tr("Horizontal layout"));
    QHBoxLayout *btnlayout = new QHBoxLayout();
    QPushButton *button = new QPushButton("Take picture");
    QPushButton *direct = new QPushButton("Direct Image");
    QPushButton *flow = new QPushButton("Flow Image");
    QPushButton *exit = new QPushButton("Restart");
    btnlayout->addWidget( button );
    btnlayout->addWidget( direct );
    btnlayout->addWidget( flow );
    btnlayout->addWidget( exit );

    QVBoxLayout *layout = new QVBoxLayout;
    //QHBoxLayout *layout = new QHBoxLayout;

    /*QPushButton *button = new QPushButton("Take picture");
    QPushButton *direct = new QPushButton("Direct Image");
    QPushButton *flow = new QPushButton("Flow Image");
    QPushButton *exit = new QPushButton("Restart");*/

    m_cvwidget->setFixedSize( 640, 480 );
    m_widget = new QWidget( m_cvwidget );
    m_widget->setFixedSize( 650 , 500 );
    //layout->addWidget(m_cvwidget,0,Qt::AlignHCenter);
    layout->addWidget( m_widget , 0 ,Qt::AlignHCenter );
    layout->addLayout( btnlayout );
    //layout->addWidget(horizontalGroupBox);
    /*layout->addWidget(button);
    layout->addWidget(direct);
    layout->addWidget(flow);
    layout->addWidget(exit);*/

    setLayout(layout);
    //setLayout(Hlayout);
    //resize(300, 200);

   /* for( int a = 0; a < 4; a = a + 1 )
       {
             connect(button, SIGNAL(pressed()), this, SLOT(savePicture()));
             startTimer(100);
             cvWaitKey(1000);
       }
*/
    connect(button, SIGNAL(pressed()), this, SLOT(savePicture()));
   // cvWaitKey(1000);
    connect(direct, SIGNAL(pressed()), this, SLOT(directImg()));
    connect(flow, SIGNAL(pressed()), this, SLOT(flowImg()));
    connect(exit, SIGNAL(pressed()), this, SLOT(restart()));

    startTimer(10);  // 0.01-second timer
 }

//Puts a new frame in the widget every 10msec
void CameraWindow::timerEvent(QTimerEvent*)
{

    IplImage *image = cvQueryFrame(m_camera);
    m_cvwidget->putFrame(image);

    /*QLabel *window = new QLabel(this); // sets parent of label to main window
    IplImage *image = cvQueryFrame(m_camera);
    IplImage* out = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3);
    window->setPixmap(QPixmap::fromImage(out));
    window->show();
    window->adjustSize();*/
}

// Display flow Image
void CameraWindow::flowImg()
{

}

//Saves a new picture
void CameraWindow::savePicture(void)
{
    IplImage *image = cvQueryFrame(m_camera);
    QPixmap photo = m_cvwidget->toPixmap(image);

    for(m_photoCounter=0;m_photoCounter<4;m_photoCounter++)
   {

        if (photo.save("/home/dthepok/Pictures/" + QString::number(m_photoCounter) + ".bmp"))
        {
            qDebug("Picture successfully saved!");
            //startTimer(1000);
        } else
        {
            qDebug("Error while saving the picture");
        }
    }

    //cvReleaseImage(&image);
}

// show directImage
void CameraWindow::directImg()
{


    Mat image1=imread("/home/bhakti/Pictures/0.bmp",0);
    Mat image2=imread("/home/bhakti/Pictures/1.bmp",0);
    Mat image3=imread("/home/bhakti/Pictures/2.bmp",0);
    Mat image4=imread("/home/bhakti/Pictures/3.bmp",0);

    // finding maximum from all images
    Mat max1 = cv::max(image1,image2);
    Mat max2 = cv::max(image3,image4);
    Mat max = cv::max(max1, max2);

    // finding minimum from all images
    Mat min1 = cv::min(image1,image2);
    Mat min2 = cv::min(image3,image4);
    Mat min = cv::min(min1, min2);

    // direct and global
    Mat direct = max-min;
    Mat global = min;

    imwrite( "/home/dthepok/Pictures/Direct_Image.bmp", direct);
    QImage image((uchar*)direct.data, direct.cols, direct.rows,QImage::Format_Indexed8);
    //QImage img=QImage("Direct_Imgae.bmp");  // QImage assignment
    //QLabel myLabel;
        //myLabel.setPixmap(QPixmap::fromImage(image));
    //QPixmap::fromImage(image);
        //myLabel.show();
    //QPixmap::fromImage(QImage((unsigned char*) direct.data, direct.cols, direct.rows, QImage::Format_RGB888));

    m_cvwidget->setVisible( false );
    // sets parent of label to main window
    m_window->setVisible( true );
    m_window->setFixedSize( 640, 480 );
    m_widget = new QWidget( m_window );
    m_widget->setFixedSize(650,500);
    m_window->setPixmap(QPixmap::fromImage(image));
    m_window->show();
    m_window->adjustSize();

    //QPixmap pixmap(QPixmap::fromImage(img));

    //widget.image_label->setPixmap(pixmap);


    /*IplImage* img = new IplImage(direct);
    IplImage* out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3);
    m_cvwidget->toPixmap(out);
    m_cvwidget->putFrame(out);
    cvWaitKey(1000000);*/
   // m_cvwidget->put

    //QPixmap photo = m_cvwidget->toPixmap(image);
    //imshow("image",image);
}

//restarts the program from inital
void CameraWindow::restart()
{
    m_window->setVisible( false );
     m_cvwidget->setVisible( true );
    IplImage *image = cvQueryFrame(m_camera);
    m_cvwidget->putFrame(image);

}
