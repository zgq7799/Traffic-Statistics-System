#include <MyBgs.h>
#include <time.h>
#include <string>

using namespace std;

void print_all_algorithms();

int main() {
    /* 帧率（fps）：图形处理器每秒更新的次数,fps<30,人眼会感受到不流畅 */
    clock_t start_time;
    int fps=0,delay=50;
    Mat frame,img_background,img_foreground,canvas;

    /* 获取视频 */
    cout<<"Reading Video......"<<endl;
    string address = "/home/zgq/Desktop/MOTDemo-master/test_data/videos/videocut_4.mp4";
    VideoCapture *vc = new VideoCapture(address);



    return 0;
}

void print_all_algorithms() {
    cout<<"======背景抽离算法======\n \
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
