#include <TrackerManager.h>
#include <BgsManager.h>
#include <time.h>
#include <string>
#include <sstream>
#include <StatisticsManager.h>

using namespace std;

TrackerManager tkm;
BgsManager bgsm;
StatisticsManager sm;

void mark_contours(Mat frame,Mat src,vector<Rect2f> &contour_rects);

int main() {
    /* 当前视频帧,后景,前景,当前帧画布 */
    Mat frame,img_background,img_foreground,canvas;

    /* 获取视频 */
    string address;
    cout<<"Input the video path : "<<endl;
    cin>>address;
    cout<<"Reading Video......"<<endl;
    VideoCapture* vc = new VideoCapture(address);

    if(!vc->isOpened()) {
        cout<<"Video Error!"<<endl;
        return -1;
    }

    /* 帧：视频中一副静止的图像,人眼规律24帧/秒 */
    long total_frame = vc->get(CV_CAP_PROP_FRAME_COUNT);
    cout<<"Total Frame is : "<<total_frame<<endl;

    /* 视频帧率（fps）：图形处理器每秒更新的次数,fps<30,人眼会感受到不流畅  */
    double fps1 = vc->get(CV_CAP_PROP_FPS);
    cout<<"fps is :"<<fps1<<endl;

    /* 显示视频 */
    for(int i=0;i<10;i++) {
        *vc >> frame; /* VideoCapture 重载运算符读出帧 */
        cv::imshow("Source Video",frame);
        waitKey(1); /* 给予显示图像的时间 */
    }

    /* 设置计数窗口ROI（Rectangle 矩形框） */
    Rect count_window = selectROI("Source Video",frame);

    /* 定义方向 */
    int in_direct=-1,out_direct=-1;
    while(true) {
        cout<<"Please input the in direction:";
        scanf("%d",&in_direct);
        if(in_direct>0 && in_direct<5) {
            break;
        } else {
            cout<<"Input is : "<<in_direct<<endl;
            cout<<"Input Error! ";
            getchar();
        }
    }

    while(true) {
        cout<<"Please input the out direction:";
        scanf("%d",&out_direct);
        if(out_direct>0 && out_direct<5) {
            break;
        } else {
            cout<<"Input Error! ";
            getchar();
        }
    }

    /* 前景提取算法初始化 */
    bgsm.create_bgs(bgsm.get_algorithm_name());

    /* 流量分析对象创建 */
    sm.create(&tkm,count_window,(DIRECTION)in_direct,(DIRECTION)out_direct);
    sm.on_begin_count();

    long frames = 0;
    double t,fps,total_fps=0.0;
    /* 开始执行 */
    while(true) {
        frames++;
        t = (double)cv::getTickCount();
        /*if (cv::waitKey(50) == 30){
            break;
        }*/
        *vc >> frame; /* 读入当前视频帧frame */
        canvas = frame.clone();
        if(!frame.empty()) {
            /* 预处理：均值滤波 */
            blur(frame,frame,Size(3,3));
            /* 背景提取算法调用，分离前景目标 */
            bgsm.process(frame,img_foreground,img_background);
            /* 将前景图(foreground)转换为灰度图 */
            if(img_foreground.channels()==3) {
                cvtColor(img_foreground,img_foreground,CV_BGR2GRAY);
            }
            /* 组合开闭运算，对前景二值图像去噪声(锚点为中心点) */
            Mat kernel_1 = getStructuringElement(MORPH_RECT,Size(5,5));
            Mat kernel_2 = getStructuringElement(MORPH_RECT,Size(5,5));
            //imshow("Before filtering",img_foreground);
            morphologyEx(img_foreground,img_foreground,MORPH_OPEN,kernel_1);
            morphologyEx(img_foreground,img_foreground,MORPH_CLOSE,kernel_2);
            /* 通过前景提取轮廓 */
            vector<Rect2f> contour_rects;
            mark_contours(img_foreground,canvas,contour_rects);
            /* 追踪目标 */
            vector<int> ids;
            tkm.update_tracker(contour_rects,ids,canvas);
            /* 包围矩形的绘制 */
            for(int i=0;i<contour_rects.size();i++) {
                Rect2f rect = contour_rects[i];
                rectangle(canvas,rect,Scalar(255,255,255));
                /*int id = ids[i];
                if(id >= 0) {
                    stringstream ss;
                    string id_str;
                    ss << id;
                    ss >> id_str;
                    putText(canvas,id_str,Point(rect.x,rect.y),CV_FONT_VECTOR0,0.5,Scalar(255,255,255),2);
                }*/
            }

            /* 处理当前帧的统计信息 */
            sm.on_count();
            STATISTICS_INFO info = sm.get_statistics_inf();
            sm.display_statistics(info,canvas,count_window);

            /* 获取并显示FPS */
            t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
            fps = 1.0 / t;
            total_fps += fps;
            char fps_str[10];
            sprintf(fps_str, "FPS:%.2f", fps);
            putText(canvas,fps_str,Point(5,20),CV_FONT_VECTOR0,0.5,Scalar(255,255,255),2);

            /* 播放视频 */
            imshow("Source Video",canvas);
            waitKey(1);
            imshow("Foreground",img_foreground);
            waitKey(1);
            if(frames == 1194) {
                break;
            }
        }
    }
    vc->release();
    destroyAllWindows();
    //cout<<"Average FPS : "<<total_fps/frames<<endl;
    return 0;
}

void mark_contours(Mat frame,Mat src,vector<Rect2f> &contour_rects) {
    Mat img = frame; /* 前景 */
    vector<vector<Point> > contours; /* 轮廓集合 */
    vector<Vec4i> topo; /* 轮廓拓扑 */
    /* 寻找前景图像的轮廓,获取最外部轮廓和拓扑结构,压缩水平,垂直和对角直线段,仅保留它们的端点 */
    findContours(frame,contours,topo,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    if(contours.size()==0) {
        return;
    }
    /* 遍历外部轮廓:仅有同一层级的轮廓,向量0位置为同层级下一轮廓 */
    for(int index=0;index>=0;index=topo[index][0]) {
        if(index < contours.size() && contours[index].size() < 20) {
            continue;
        }
        /* 画轮廓 */
        drawContours(src,contours,index,Scalar(255,255,255),1,8,topo);
        /* 计算包围轮廓最小矩形 */
        RotatedRect rect = minAreaRect(contours[index]);
        contour_rects.push_back(rect.boundingRect2f());
    }
}

int main2() {
    Mat m = imread("/home/zgq/Desktop/Lighthouse.jpg");
    if(m.channels()==3) {
        cvtColor(m,m,CV_BGR2GRAY);
        threshold(m,m,0,255,THRESH_BINARY | THRESH_OTSU);
    }
    //imshow("Bicolor",m);
    cout<<m;
    waitKey();
    Mat element1 = getStructuringElement(MORPH_RECT,Size(5,5));
    Mat element2 = getStructuringElement(MORPH_RECT,Size(5,5));
    morphologyEx(m,m,MORPH_OPEN,element1);
    imshow("Next1",m);
    morphologyEx(m,m,MORPH_CLOSE,element2);
    imshow("Next2",m);
    waitKey();
    return 0;
}

int main3() {
    Mat m = imread("/home/zgq/Desktop/face.jpg");
    cvtColor(m,m,CV_BGR2GRAY);
    Mat g;
    threshold(m, g, 100, 255, cv::THRESH_BINARY);
    imshow("Source",g);
    waitKey(1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    findContours(
        g,
        contours,
        hierachy,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
        );
    g = cv::Scalar::all(0);
    cv::drawContours(g, contours, -1, cv::Scalar::all(255));
    imshow("Contour",g);
    RotatedRect rect = minAreaRect(contours[0]);
    cout<<rect.boundingRect2f()<<endl;
    waitKey();
    return 0;
}

int main4() {
    KalmanFilter kf;
    /* 状态矩阵Xk(4*1) (ak,bk,Δak,Δbk) */
    int state_num = 4;
    /* 观测矩阵Zk(2*1) (ak,bk) */
    int measure_num = 2;
    kf.init(state_num,measure_num);
    /* 转移矩阵A */
    kf.transitionMatrix = (Mat_<float>(state_num, state_num) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);
    /* 测量矩阵H */
    kf.measurementMatrix =  (Mat_<float>( measure_num,state_num) <<1,0,0,0,0,1,0,0);
    /* 后验错误估计协方差矩阵P */
    setIdentity(kf.errorCovPost);
    /* 系统噪声方差矩阵Q(默认值) */
    setIdentity(kf.processNoiseCov, Scalar::all(1e-5));
    /* 测量噪声方差矩阵R(默认值) */
    setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1));
    cout<<kf.measurementNoiseCov<<endl;
    return 0;
}
