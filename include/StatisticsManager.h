#pragma once

#include <vector>
#include <TrackerManager.h>

/**
 * 物体方向
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
    DIRECTION_FLOW_NONE,
    DIRECTION_FLOW_IN,
    DIRECTION_FLOW_OUT
};

/**
 * 跟踪器统计信息
 * @brief The ST_TRACKER_INFO struct
 */
struct ST_TRACKER_INFO {
    int tracker_id; /* 跟踪器ID */
    DIRECTION_FLOW direct; /* 跟踪器方向 */
    bool is_in_counted; /* 跟踪器进入是否统计 */
    bool is_out_counted; /* 跟踪器出去是否统计 */
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

    /* 初始化 */
    void create(TrackManager* ptm,Rect count_window,DIRECTION in=DIRECTION_NONE,DIRECTION out=DIRECTION_NONE);
    void on_begin_count();
    void on_count();
    void on_end_count();
    void get_statistics_inf(STATISTICS_INFO &info);

private:
    bool count_status; /* 是否处于计数状态 */
    Rect count_window; /* 计数ROI */
    DIRECTION in_diect; /* 计数窗口的进入方向 */
    DIRECTION out_direct; /* 计数窗口的出去方向 */
    STATISTICS_INFO st_info; /* 窗口统计 */
    TrackManager* ptm;      /* 跟踪器管理 */
    vector<ST_TRACKER_INFO> tracker_all_inf; /* 跟踪器集合 */
};


