#pragma once

/**
 *  37种前景抽取算法选项(基于BGSLibrary)
 *
 * @brief The BgsNameEnum enum
 */
enum BgsNameEnum{
  ZERONULL,
  FrameDifferenceBGS,
  StaticFrameDifferenceBGS,
  WeightedMovingMeanBGS,
  WeightedMovingVarianceBGS,
  MixtureOfGaussianV1BGS,
  MixtureOfGaussianV2BGS,
  AdaptiveBackgroundLearning,
  AdaptiveSelectiveBackgroundLearning,
  GMG,
  DPAdaptiveMedianBGS,
  DPGrimsonGMMBGS,
  DPZivkovicAGMMBGS,
  DPMeanBGS,
  DPWrenGABGS,
  DPPratiMediodBGS,
  DPEigenbackgroundBGS,
  DPTextureBGS,
  T2FGMM_UM,
  T2FGMM_UV,
  T2FMRF_UM,
  T2FMRF_UV,
  FuzzySugenoIntegral,
  FuzzyChoquetIntegral,
  LBSimpleGaussian,
  LBFuzzyGaussian,
  LBMixtureOfGaussians,
  LBAdaptiveSOM,
  LBFuzzyAdaptiveSOM,
  MultiLayerBGS,
  //PBAS,
  VuMeter,
  KDE,
  IndependentMultimodalBGS,
  SJN_MultiCueBGS,
  SigmaDeltaBGS,
  SuBSENSEBGS,
  LOBSTERBGS,
};
