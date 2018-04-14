#include <MyBgs.h>
#include <time.h>
#include <string>
#include <StatisticsManager.h>
#include <TrackerManager.h>

using namespace std;

TrackManager tkm;

void print_all_algorithms();

int main() {
    clock_t start_time;
    int fps=0,delay=50;
    Mat frame,img_background,img_foreground,canvas;

    /* 获取视频 */
    cout<<"Reading Video......"<<endl;
    string address = "/home/zgq/Desktop/MOTDemo-master/test_data/videos/videocut_4.mp4";
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
    for(;;) {
        cout<<"Please input the in direction:";
        cin>>in_direct;
        if(in_direct>0 && in_direct<5) {
            break;
        } else {
            cout<<"Input Error! ";
        }
    }

    for(;;) {
        cout<<"Please input the out direction:";
        cin>>out_direct;
        if(out_direct>0 && out_direct<5) {
            break;
        } else {
            cout<<"Input Error! ";
        }
    }

    /* 流量分析对象创建 */
    StatisticsManager sm;
    sm.create(&tkm,count_window,(DIRECTION)in_direct,(DIRECTION)out_direct);

    return 0;
}

void print_all_algorithms() {
    cout<<"======BGSLibrary Algorithms List(Have deleted some errors)======\n \
      [0]FrameDifferenceBGS\n \
      [1]StaticFrameDifferenceBGS\n \
      [2]WeightedMovingMeanBGS\n \
      [3]WeightedMovingVarianceBGS\n \
      [4]MixtureOfGaussianV1BGS\n \
      [5]MixtureOfGaussianV2BGS\n \
      [6]AdaptiveBackgroundLearning\n \
      [7]AdaptiveSelectiveBackgroundLearning\n \
      [8]GMG\n \
      [9]DPAdaptiveMedianBGS\n \
      [10]DPGrimsonGMMBGS\n \
      [11]DPZivkovicAGMMBGS\n \
      [12]DPMeanBGS\n \
      [13]DPWrenGABGS\n \
      [14]DPPratiMediodBGS\n \
      [15]DPEigenbackgroundBGS\n \
      [16]DPTextureBGS\n \
      [17]T2FGMM_UM\n \
      [18]T2FGMM_UV\n \
      [19]T2FMRF_UM\n \
      [20]T2FMRF_UV\n \
      [21]FuzzySugenoIntegral\n \
      [22]FuzzyChoquetIntegral\n \
      [23]LBSimpleGaussian\n \
      [24]LBFuzzyGaussian\n \
      [25]LBMixtureOfGaussians\n \
      [26]LBAdaptiveSOM\n \
      [27]LBFuzzyAdaptiveSOM\n \
      [28]MultiLayerBGS\n \
      [29]VuMeter\n \
      [30]KDE\n \
      [31]IndependentMultimodalBGS\n \
      [32]SJN_MultiCueBGS\n \
      [33]SigmaDeltaBGS\n \
      [34]SuBSENSEBGS\n \
      [35]LOBSTERBGS" <<endl;
}
