//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace std;
//using namespace cv;
//
//int main() {
//    VideoCapture capture;
//    Mat frame;
//    frame = capture.open("../data/video-02.mp4");
//    if(!capture.isOpened())
//    {
//        printf("can not open ...\n");
//        return -1;
//    }
//    namedWindow("output",WINDOW_AUTOSIZE);
//
//    while (capture.read(frame))
//    {
//        imshow("output", frame);
//        waitKey(10);
//    }
//    capture.release();
//    return 0;
//
//}
//
//void test() {
//    Mat src = imread("../assets/girl.jpg");
//    imshow("src", src);
//    waitKey(0);
//}

/******************************************************
文件名   :main.cpp
描  述   :车辆识别
语  言   :c++
作  者   :范泽华
修  改   :
日  期   :2018-05-13
说  明   :opencv的支持
******************************************************/
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/videoio/videoio_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

class MyClass {
public:
    MyClass();

    MyClass(char **argv);

    ~MyClass();

    void play();//帧差
    void bplay();//背景差
    Mat getGray(Mat frame);//灰度
    Mat getSmooth(Mat frame);//高斯平滑
    Mat getDiff(Mat preframe, Mat frame);//帧差
    Mat getEz(Mat frame);//二值化
    Mat getPz(Mat frame);//膨胀
    Mat getFs(Mat frame);//腐蚀
    Mat Deal(Mat preframe, Mat frame);//处理
private:
    VideoCapture capture;
    double FPS;
};

/******************************************************
函数名称： MyClass
函数功能： 初始化
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
MyClass::MyClass() {
    capture.open("../data/交大七公里交通监控.avi");
//    capture.open("../data/video-01.avi");
//    capture.open("../data/video-02.mp4");
//    capture.open("../data/video-03.avi");
    //capture = 0;
    if (!capture.isOpened())//判断是否打开视频文件
    {
        exit(1);
    }
    FPS = capture.get(CV_CAP_PROP_FPS);
}

/******************************************************
函数名称： MyClass
函数功能： 初始化
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
MyClass::MyClass(char **argv) {
    capture.open(argv[1]);
    //capture = 0;
    if (!capture.isOpened())//判断是否打开视频文件
    {
        exit(1);
    }
    FPS = capture.get(CV_CAP_PROP_FPS);
}

/******************************************************
函数名称： ~MyClass
函数功能： 释放空间
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
MyClass::~MyClass() {
    capture.release();
}

/******************************************************
函数名称： play
函数功能： 播放帧差法
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
void MyClass::play() {
    Mat frame, preframe, curframe, result;
    namedWindow("播放界面按Esc退出", 0);
    //cvResizeWindow("播放界面按Esc退出", 600, 500);
    while (true) {
        capture >> frame;
        if (frame.empty())break;
        if (preframe.empty())preframe = frame.clone();//首帧处理
        curframe = frame.clone();
        result = Deal(preframe, curframe);
        imshow("播放界面按Esc退出", frame);
        imshow("识别界面", result);

        if (waitKey(1000.0 / FPS) == 27)//按原FPS显示
        {
            cout << "ESC退出!" << endl;
            break;
        }
        preframe = frame.clone();//记录当前帧为下一帧的前帧
    }
}

/******************************************************
函数名称： play
函数功能： 播放背景差法
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
void MyClass::bplay() {
    Mat frame, bframe, curframe, result;
    namedWindow("播放界面按Esc退出", 0);
    cvResizeWindow("播放界面按Esc退出", 600, 500);
    while (true) {
        capture >> frame;
        if (frame.empty())break;
        if (bframe.empty())bframe = frame.clone();
        curframe = frame.clone();
        result = Deal(bframe, curframe);
        imshow("播放界面按Esc退出", frame);
        imshow("识别界面", result);
        imshow("背景", bframe);
        if (waitKey(1000.0 / FPS) == 27)//按原FPS显示
        {
            cout << "ESC退出!" << endl;
            break;
        }
    }
}

/******************************************************
函数名称： getGray
函数功能： 灰度处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getGray(Mat frame) {
    Mat img;
    cvtColor(frame, img, CV_RGB2GRAY);
    return img;
}

/******************************************************
函数名称： getSmooth
函数功能： 平滑处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getSmooth(Mat frame) {
    Mat img;
    GaussianBlur(frame, img, Size(13, 13), 2, 2);
    return img;
}

/******************************************************
函数名称： getDiff
函数功能： 帧差化处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getDiff(Mat preframe, Mat frame) {
    Mat img;
    absdiff(preframe, frame, img);
    return img;
}

/******************************************************
函数名称： getEz
函数功能： 二值化处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getEz(Mat frame) {
    Mat img;
    threshold(frame, img, 30, 255, CV_THRESH_BINARY);
    return img;
}

/******************************************************
函数名称： getPz
函数功能： 膨胀处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getPz(Mat frame) {
    Mat img;
    Mat element = getStructuringElement(MORPH_RECT, Size(11, 30));
    dilate(frame, img, element);
    return img;
}

/******************************************************
函数名称： getFs
函数功能： 腐蚀处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::getFs(Mat frame) {
    Mat img;
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 16));
    erode(frame, img, element);
    return img;
}

/******************************************************
函数名称： Deal
函数功能： ROI区域处理
传入参数：
返 回 值：
建立时间： 2018-05-17
修改时间：
建 立 人： 范泽华
修 改 人：
其它说明：
******************************************************/
Mat MyClass::Deal(Mat preframe, Mat frame) {
    Mat result = frame.clone();
    Mat curimageROI = preframe(Rect(preframe.cols / 5, preframe.rows / 5, preframe.cols / 1.5, preframe.rows / 1.5));
    Mat preimageROI = frame(Rect(frame.cols / 5, frame.rows / 5, frame.cols / 1.5, frame.rows / 1.5));

    Mat sm_pre, sm_cur;
    sm_pre = getSmooth(preimageROI);
    sm_cur = getSmooth(curimageROI);
    imshow("平滑处理", sm_cur);

    Mat gray_pre, gray_cur;
    gray_pre = getGray(sm_pre);
    gray_cur = getGray(sm_cur);
    imshow("灰度处理", gray_cur);

    Mat diff;
    diff = getDiff(gray_pre, gray_cur);
    imshow("帧差处理", diff);

    Mat ez;
    ez = getEz(diff);
    imshow("二值化处理", ez);

    Mat pz;
    pz = getPz(ez);
    imshow("膨胀处理", pz);

    Mat fs;
    fs = getFs(pz);
    imshow("腐蚀处理", fs);

    vector<vector<Point> > contours;
    findContours(fs, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector<Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        boundRect[i] = boundingRect(contours[i]);
        //Rect(boundRect[i].x + frame.cols, boundRect[i].y + frame.rows, boundRect[i].width, boundRect[i].height)
        rectangle(result, Rect(boundRect[i].x + frame.cols / 5, boundRect[i].y + frame.rows / 5, boundRect[i].width,
                               boundRect[i].height), Scalar(0, 255, 0), 1);//在result上绘制正外接矩形
    }
    return result;
}

int main() {
    //外部输入
//    if (argc >= 2){
//        MyClass *myclass = new MyClass(argv);
//        myclass->play();
//        delete myclass;
//    }
    //默认输入
//    else{
    MyClass *myclass = new MyClass();
    myclass->play();
    delete myclass;
//    }
    return 0;
}