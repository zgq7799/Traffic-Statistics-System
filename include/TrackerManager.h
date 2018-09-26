#pragma once

#include <global.h>
#include <opencv2/tracking.hpp>

/**
 * 跟踪器基本信息
 * @brief The TRACKER_INFO struct
 */
struct TRACKER_INFO {
    int tracker_id;    /* 跟踪器ID */
    KalmanFilter kf;   /* 卡尔曼滤波器 */
    bool is_find;      /* tracker是否跟踪到目标 */
    Rect2f contour_rect;   /* 包围轮廓的矩形框(轮廓实际位置,匹配到tracker后手动更新) */
    int life;          /* Tracker存在时长(单位：帧) */
    int track_frame_count; /* 跟踪到目标的帧数 */
    int consist_fail_count; /* 连续未跟踪到目标的帧数，跟踪到则置0 */
};

class TrackerManager {
public:
    TrackerManager();
    ~TrackerManager();

    vector<TRACKER_INFO> trackers; /* 跟踪器基本信息集合 */
    /**
     * @brief update_tracker 更新跟踪器，找到更加贴近目标的包围框
     * @param objects 跟踪对象集合
     * @param ids     跟踪对象id集合
     * @param frame   当前帧
     */

    /* 下一帧更新(多个tracker的管理)：
       1.将下一帧中的每一个包围轮廓的矩形框与所有tracker实际跟踪到的矩形框中心做对比,在一定误差内,还是原来的id；
       (特别的，如果初始没有tracker，那么id即为-1,注：每一帧更新trackerIds都是重新初始化的)
       2.如果包围轮廓的矩形框与所有实际tracker跟踪到的矩形框都不尽相同，返回-1；
       3.将对比后的所有tracker_id，放入trackerIds中；
       4.如果trackerIds中不是-1，且trackers集合中能够匹配到trackerIds的id，则代表分配成功；
       5.对于没有分配成功的id，更新trackers失效信息；
       6.删除无效跟踪器；
       7.-1情形的处理：包围轮廓的矩形框没有与任何一个tracker匹配到，那么说明这是一个新物体，要分配一个新tracker；
    */
    void update_tracker(vector<Rect2f> contour_rects,vector<int> &ids,Mat frame);

private:
    Mat frame;
    int next_tracker_id = 0;

    void init_kalman_filter(KalmanFilter &kf);
    TRACKER_INFO create_tracker(Rect2f contour_rect);
    void delete_invaild_trackers();

    /* 匹配目标和跟踪器 */
    int dispatch_obj_to_tracker_kalman(Rect2f contour_rect);
};
