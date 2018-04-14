#include <MyBgs.h>
#include <bgslibrary.h>

MyBgs::MyBgs() {
    bgs = NULL;
}

MyBgs::~MyBgs() {

}

void MyBgs::create_bgs(BgsNameEnum bgs_name) {
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
         bgs = new DPAdaptiveMedian();
         break;
     }
}

void MyBgs::process(Mat input, Mat &img_foreground, Mat &img_background) {
    bgs->process(input,img_foreground,img_background);
}
