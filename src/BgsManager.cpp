#include <BgsManager.h>
#include <bgslibrary.h>

BgsManager::BgsManager() {
    bgs = NULL;
}

BgsManager::~BgsManager() {

}

void BgsManager::create_bgs(BgsNameEnum bgs_name) {
    switch (bgs_name) {
     case FrameDifferenceBGS:
         bgs = new FrameDifference();
         break;
     case StaticFrameDifferenceBGS:
         bgs = new StaticFrameDifference();
         break;
     case WeightedMovingMeanBGS:
         bgs = new WeightedMovingMean();
         break;
     case WeightedMovingVarianceBGS:
         bgs = new WeightedMovingVariance();
         break;
     case MixtureOfGaussianV1BGS:
         //
         break;
     case MixtureOfGaussianV2BGS:
         bgs = new MixtureOfGaussianV2();
         break;
     case BgsNameEnum::AdaptiveBackgroundLearning:
         bgs = new bgslibrary::algorithms::AdaptiveBackgroundLearning();
         break;
     case BgsNameEnum::AdaptiveSelectiveBackgroundLearning:
         bgs = new bgslibrary::algorithms::AdaptiveSelectiveBackgroundLearning();
         break;
     case GMG:
         //bgs = new GMG();
         break;
     case DPAdaptiveMedianBGS:
         bgs = new DPAdaptiveMedian();
         break;
     case DPGrimsonGMMBGS:
         bgs = new DPGrimsonGMM();
         break;
     case DPZivkovicAGMMBGS:
         bgs = new DPZivkovicAGMM();
         break;
     case DPMeanBGS:
         bgs = new DPMean();
         break;
     case DPWrenGABGS:
         bgs = new DPWrenGA();
         break;
     case DPPratiMediodBGS:
         bgs = new DPPratiMediod();
         break;
     case DPEigenbackgroundBGS:
         bgs = new DPEigenbackground();
         break;
     case DPTextureBGS:
         bgs = new DPTexture();
         break;
     case BgsNameEnum::T2FGMM_UM:
         bgs = new bgslibrary::algorithms::T2FGMM_UM();
         break;
     case BgsNameEnum::T2FGMM_UV:
         bgs = new bgslibrary::algorithms::T2FGMM_UV();
         break;
     case BgsNameEnum::T2FMRF_UM:
         bgs = new bgslibrary::algorithms::T2FMRF_UM();
         break;
     case BgsNameEnum::T2FMRF_UV:
         bgs = new bgslibrary::algorithms::T2FMRF_UV();
         break;
     case BgsNameEnum::FuzzySugenoIntegral:
         bgs = new bgslibrary::algorithms::FuzzySugenoIntegral();
         break;
     case BgsNameEnum::FuzzyChoquetIntegral:
         bgs = new bgslibrary::algorithms::FuzzyChoquetIntegral();
         break;
     case BgsNameEnum::LBSimpleGaussian:
         bgs = new bgslibrary::algorithms::LBSimpleGaussian();
         break;
     case BgsNameEnum::LBFuzzyGaussian:
         bgs = new bgslibrary::algorithms::LBFuzzyGaussian();
         break;
     case BgsNameEnum::LBMixtureOfGaussians:
         bgs = new bgslibrary::algorithms::LBMixtureOfGaussians();
         break;
     case BgsNameEnum::LBAdaptiveSOM:
         bgs = new bgslibrary::algorithms::LBAdaptiveSOM();
         break;
     case BgsNameEnum::LBFuzzyAdaptiveSOM:
         bgs = new bgslibrary::algorithms::LBFuzzyAdaptiveSOM();
         break;
     case MultiLayerBGS:
         bgs = new MultiLayer();
         break;
     case BgsNameEnum::VuMeter:
         bgs = new bgslibrary::algorithms::VuMeter();
         break;
     case BgsNameEnum::KDE:
         bgs = new bgslibrary::algorithms::KDE();
         break;
     case IndependentMultimodalBGS:
         bgs = new IndependentMultimodal();
         break;
     case SJN_MultiCueBGS:
         //bgs = new SJN_MultiCue();
         break;
     case SigmaDeltaBGS:
         bgs = new SigmaDelta();
         break;
     case SuBSENSEBGS:
         bgs = new SuBSENSE();
         break;
     case LOBSTERBGS:
         bgs = new LOBSTER();
         break;
     default:
        bgs = NULL;
     }
}

void BgsManager::process(Mat input, Mat &img_foreground, Mat &img_background) {
    bgs->process(input,img_foreground,img_background);
}

BgsNameEnum BgsManager::get_algorithm_name() {
    int num = 0;
    BgsNameEnum name;
    print_all_algorithms();
    while(true) {
        cin>>num;
        if(num >=1 && num <=36) {
            name = (BgsNameEnum)num;
            break;
        } else {
            cout<<"Input Error!"<<endl;
            cout<<"Your Choice : ";
        }
    }
    return name;
}

void BgsManager::print_all_algorithms() {
    cout<<"======BGSLibrary Algorithms List(Have deleted some errors)======\n \
      [1]FrameDifferenceBGS\n \
      [2]StaticFrameDifferenceBGS\n \
      [3]WeightedMovingMeanBGS\n \
      [4]WeightedMovingVarianceBGS\n \
      [5]MixtureOfGaussianV1BGS\n \
      [6]MixtureOfGaussianV2BGS\n \
      [7]AdaptiveBackgroundLearning\n \
      [8]AdaptiveSelectiveBackgroundLearning\n \
      [9]GMG\n \
      [10]DPAdaptiveMedianBGS\n \
      [11]DPGrimsonGMMBGS\n \
      [12]DPZivkovicAGMMBGS\n \
      [13]DPMeanBGS\n \
      [14]DPWrenGABGS\n \
      [15]DPPratiMediodBGS\n \
      [16]DPEigenbackgroundBGS\n \
      [17]DPTextureBGS\n \
      [18]T2FGMM_UM\n \
      [19]T2FGMM_UV\n \
      [20]T2FMRF_UM\n \
      [21]T2FMRF_UV\n \
      [22]FuzzySugenoIntegral\n \
      [23]FuzzyChoquetIntegral\n \
      [24]LBSimpleGaussian\n \
      [25]LBFuzzyGaussian\n \
      [26]LBMixtureOfGaussians\n \
      [27]LBAdaptiveSOM\n \
      [28]LBFuzzyAdaptiveSOM\n \
      [29]MultiLayerBGS\n \
      [30]VuMeter\n \
      [31]KDE\n \
      [32]IndependentMultimodalBGS\n \
      [33]SJN_MultiCueBGS\n \
      [34]SigmaDeltaBGS\n \
      [35]SuBSENSEBGS\n \
      [36]LOBSTERBGS" <<endl;
      cout<<"Your Choice : ";
}
