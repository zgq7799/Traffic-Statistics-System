#include <TrackerManager.h>

TrackerManager::TrackerManager() {
    next_tracker_id = 1;
}

TrackerManager::~TrackerManager() {

}

void TrackerManager::init_kalman_filter(KalmanFilter &kf) {
    /* 状态矩阵Xk(4*1) (ak,bk,Δak,Δbk) */
    int state_num = 4;
    /* 观测矩阵Zk(2*1) (ak,bk) */
    int measure_num = 2;
    kf.init(state_num,measure_num);
    /* 转移矩阵A */
    kf.transitionMatrix = (Mat_<float>(state_num, state_num) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);
    /* 测量矩阵H */
    kf.measurementMatrix =  (Mat_<float>(measure_num,state_num) <<1,0,0,0,0,1,0,0);
    /* 后验错误估计协方差矩阵P(4*4单位矩阵) */
    setIdentity(kf.errorCovPost);
    /* 系统噪声方差矩阵Q(默认值4*4) */
    setIdentity(kf.processNoiseCov, Scalar::all(1e-5));
    /* 测量噪声方差矩阵R(默认值2*2) */
    setIdentity(kf.measurementNoiseCov, Scalar::all(1e-1));
}

TRACKER_INFO TrackerManager::create_tracker(Rect2f contour_rect){
    /* 初始化tracker信息 */
    TRACKER_INFO tracker;
    init_kalman_filter(tracker.kf);
    tracker.tracker_id           = next_tracker_id++;
    tracker.life                 = 1;
    tracker.contour_rect         = contour_rect;
    tracker.consist_fail_count   = 0;
    tracker.track_frame_count    = 1;

    float contour_rect_x = contour_rect.x + contour_rect.width/2;
    float contour_rect_y = contour_rect.y + contour_rect.height/2;
    Mat measurement = Mat::zeros(4, 1, CV_32F);
    measurement.at<float>(0) = contour_rect_x;
    measurement.at<float>(1) = contour_rect_y;
    tracker.kf.statePost = measurement;  /* statePost正确数据,statePre预测数据 */

    trackers.push_back(tracker);
    return tracker;
}


void TrackerManager::update_tracker(vector<Rect2f> contour_rects, vector<int> &ids, Mat canvas) {
    if(contour_rects.size() == 0) {
        return;
    }
    this->frame = canvas; /* 原画 */
    /* 遍历每一个Tracker */
    for(int i=0;i<trackers.size();i++) {
        TRACKER_INFO *tracker = &trackers[i]; /* 注意:tracker_rect是跟踪器跟踪到的矩形(引用更新) */
        tracker->kf.predict();
    }
    /* 遍历每一个包围轮廓的矩形，匹配目标和跟踪器,注意：矩形和对应的tracker_id是一一对应的 */
    for(int i=0;i<contour_rects.size();i++) {
        Rect2f rect = contour_rects[i];
        int tracker_id = TrackerManager::dispatch_obj_to_tracker_kalman(rect);
        if(tracker_id >= 0) {
            ids.push_back(tracker_id);
        } else {
            ids.push_back(-1);
        }
    }
    /* 查看跟踪器是否跟踪到目标，如果没跟踪到更新相关信息 */
    for(int i=0;i<trackers.size();i++) {
        bool assaigned = false;
        for(int j=0;j<ids.size();j++) {
            if(trackers[i].tracker_id == ids[j] && ids[j]!=-1) {
                assaigned = true;
                break;
            }
        }
        if(!assaigned) {
            trackers[i].life++;
            trackers[i].consist_fail_count++;
        }
    }
    /* 删除无效跟踪器 */
    delete_invaild_trackers();
    /* 给没有跟踪器的对象分配跟踪器,id=-1的分配新的跟踪器 */
    for(int i=0;i<ids.size();i++) {
        if(ids[i] == -1) {
            TRACKER_INFO trk = create_tracker(contour_rects[i]); /* 目标实际矩形位置 */
            ids[i] = trk.tracker_id;
        }
    }
}

int TrackerManager::dispatch_obj_to_tracker_kalman(Rect2f contour_rect){
    /* 物体包围轮廓矩形的中心点 */
    float contour_rect_x = contour_rect.x + contour_rect.width/2;
    float contour_rect_y = contour_rect.y + contour_rect.height/2;
    float min_dist = -1;
    int id = -1,index = -1;
    /* 找到最能够匹配物体的追踪器 */
    for(int i=0;i<trackers.size();i++){
        TRACKER_INFO tracker = trackers[i];
        KalmanFilter kf = tracker.kf;
        /* 卡尔曼滤波预测位置 */
        float predict_x = kf.statePre.at<float>(0);
        float predict_y = kf.statePre.at<float>(1);
        /* 跟踪器跟踪矩形与物体实际包围矩形中心点的距离 */
        float dist = sqrt(pow(contour_rect_x - predict_x,2) +
                          pow(contour_rect_y - predict_y,2));
        if((min_dist == -1 || min_dist > dist) && dist < 50 ){
            min_dist = dist;
            id = tracker.tracker_id;
            index = i;
        }
    }
    if(id != -1) {

        Mat measurement = Mat::zeros(2, 1, CV_32F);
        measurement.at<float>(0) = contour_rect_x;
        measurement.at<float>(1) = contour_rect_y;
        trackers[index].kf.correct(measurement);
        trackers[index].life++;
        trackers[index].consist_fail_count   = 0;
        trackers[index].track_frame_count++;
        trackers[index].contour_rect = contour_rect;
    }
    return id;
}

void TrackerManager::delete_invaild_trackers() {
    vector<TRACKER_INFO>::iterator it = trackers.begin();
    while(it != trackers.end()) {
        /* 持续超过50帧没匹配就删掉tracker */
        if((*it).consist_fail_count >= 50) {
            it = trackers.erase(it);
        } else {
            it++;
        }
    }
}

