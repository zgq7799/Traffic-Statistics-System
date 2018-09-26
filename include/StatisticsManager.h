#pragma once

#include <vector>
#include <TrackerManager.h>

/**
 * 全局物体流向
 * @brief The DIRECTION enum
 */
enum DIRECTION {
    DIRECTION_NONE=0,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_TOP,
    DIRECTION_BOTTOM
};

/**
 * 跟踪器方向(按进出划分或者无效)
 * @brief The DIRECTION_FLOW enum
 */
enum DIRECTION_FLOW {
    DIRECTION_FLOW_NONE=0,
    DIRECTION_FLOW_IN,
    DIRECTION_FLOW_OUT
};

/**
 * 跟踪器统计信息
 * @brief The ST_TRACKER_INFO struct
 */
struct ST_TRACKER_INFO {
    int tracker_id; /* 跟踪器ID */
    DIRECTION_FLOW direct; /* 跟踪器方向标记 */
};

/**
 * 全局统计信息
 * @brief The STATISTICS_INFO struct
 */
struct STATISTICS_INFO {
    int total_in_count; /* 总流入数 */
    int total_out_count; /* 总流出数 */
    time_t start_time;  /* 开始时间 */
    time_t end_time;    /* 结束时间 */
};

class StatisticsManager {
public:
    StatisticsManager();
    ~StatisticsManager();

    DIRECTION in_direct; /* 计数窗口的进入方向 */
    DIRECTION out_direct; /* 计数窗口的出去方向 */

    /* 初始化 */
    void create(TrackerManager* ptm,
                Rect count_window,
                DIRECTION in=DIRECTION_NONE,
                DIRECTION out=DIRECTION_NONE);
    void on_begin_count();
    /*
       基于一个假设：跟踪器刚进入窗口时，一定离它进入的那个边界更近；
       例如：
           1.从左(in)至右(out)进入：分配给tracker统计信息时，一定in_dist < out_dist,故标记OUT；
                             离开：in_dist > out_dist,故out++;
           2.从右(out)至左(in)进入：分配给tracker统计信息时，一定in_dist > out_dist,故标记IN；
                             离开：in_dist < out_dist,故in++;
    */
    void on_count();
    void on_end_count();
    void display_statistics(STATISTICS_INFO info,Mat &canvas,Rect count_window);
    STATISTICS_INFO get_statistics_inf();

private:
    TrackerManager* ptm; /* 跟踪器管理 */
    bool count_status; /* 是否处于计数状态 */
    Rect count_window; /* 计数ROI */
    STATISTICS_INFO st_info; /* 窗口统计 */
    vector<ST_TRACKER_INFO> tracker_all_inf; /* 跟踪器集合 */
};


