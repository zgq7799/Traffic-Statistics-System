#pragma once

#include <bgslibrary.h>
#include <BgsNameEnum.h>

/**
 * 前景抽取算法类
 *
 * @brief The BgsManager class
 */

class BgsManager {
public:
    BgsManager();
    ~BgsManager();
    BgsNameEnum get_algorithm_name();
    /**
     * 根据前景抽取算法的名称创建对应实例
     *
     * @brief create_bgs
     * @param bgs_name 名称
     */
    void create_bgs(BgsNameEnum bgs_name);

    /**
     * 调用对应算法进行处理
     * @brief process
     * @param input      输入图像
     * @param foreground 前景二值化图像
     * @param background 背景二值化图像
     */
    void process(Mat input,Mat &img_foreground,Mat &img_background);



private:
    /* IBGS是各前景抽取算法类的抽象类 */
    IBGS *bgs;
    void print_all_algorithms();
};
