#include <StatisticsManager.h>
#include <time.h>

StatisticsManager::StatisticsManager() {

}

StatisticsManager::~StatisticsManager() {

}

void StatisticsManager::create(TrackerManager *ptm, Rect count_window, DIRECTION in, DIRECTION out) {
    if(ptm==NULL) {
        return;
    }
    /* 初始化各个成员 */
    this->ptm = ptm;
    this->count_status = false;
    this->count_window = count_window;
    this->in_direct = in;
    this->out_direct = out;
}

void StatisticsManager::on_begin_count() {
    if(count_status==true || ptm==NULL) {
        cout<<"Flow Statistics Manager Error!"<<endl;
        cout<<"Count_Status:"<<count_status<<endl;
        return;
    }
    /* 为统计窗口的全局信息数结构分配内存 */
    memset(&this->st_info,0,sizeof(STATISTICS_INFO));
    /* 将计数状态改为开始 */
    count_status = true;
    /* 设置统计开始时间 */
    time_t start_time;
    time(&start_time);
    this->st_info.start_time = start_time;
    /* 初始化统计数据 */
    this->st_info.total_in_count = 0;
    this->st_info.total_out_count = 0;
}

void StatisticsManager::on_count() {
    if(count_status==false || ptm == NULL) {
        return;
    }
    for(int i=0;i<this->ptm->trackers.size();i++) {
        TRACKER_INFO tracker = this->ptm->trackers[i];
        /* 计算追踪器物体与in边界和out边界的中心距离 */
        int in_dist = -1,out_dist = -1;
        int tracker_center_x = tracker.contour_rect.x + tracker.contour_rect.width / 2;
        int tracker_center_y = tracker.contour_rect.y + tracker.contour_rect.height / 2;
        switch(in_direct) {
        case DIRECTION_BOTTOM:
            in_dist = abs(tracker_center_y - (count_window.y + count_window.height));
            break;
        case DIRECTION_TOP:
            in_dist = abs(tracker_center_y - count_window.y);
            break;
        case DIRECTION_LEFT:
            in_dist = abs(tracker_center_x - count_window.x);
            break;
        case DIRECTION_RIGHT:
            in_dist = abs(tracker_center_x - (count_window.x + count_window.width));
            break;
        };
        switch(out_direct) {
        case DIRECTION_BOTTOM:
            out_dist = abs(tracker_center_y - (count_window.y + count_window.height));
            break;
        case DIRECTION_TOP:
            out_dist = abs(tracker_center_y - count_window.y);
            break;
        case DIRECTION_LEFT:
            out_dist = abs(tracker_center_x - count_window.x);
            break;
        case DIRECTION_RIGHT:
            out_dist = abs(tracker_center_x - (count_window.x + count_window.width));
            break;
        };
        /* 判断当前跟踪器是否在计数窗口当中 */
        bool is_in_window = false;
        if(tracker_center_x >= count_window.x &&
                tracker_center_x <= count_window.x + count_window.width &&
                tracker_center_y >= count_window.y &&
                tracker_center_y <= count_window.y + count_window.height) {
            is_in_window = true;
        }
        /* 查看此跟踪器是否有对应的跟踪器统计信息 */
        int index = -1;
        for(int j=0;j<tracker_all_inf.size();j++) {
            if(tracker_all_inf[j].tracker_id == tracker.tracker_id) {
                index = j;
                break;
            }
        }
        /* 若index=-1,说明该跟踪器没有对应的统计信息,当跟踪器进入统计窗口再计数 */
        ST_TRACKER_INFO new_tracker_info;
        if(index == -1) {
            if(!is_in_window) {
                continue;
            }
            new_tracker_info.tracker_id = tracker.tracker_id;
            /* 若进入统计窗口,判断从哪一端进入 */
            if(in_dist < out_dist) { /* in端进入 */
                new_tracker_info.direct = DIRECTION_FLOW_OUT; /* OUT方向为计数方向 */
            } else {
                new_tracker_info.direct = DIRECTION_FLOW_IN;
            }
            tracker_all_inf.push_back(new_tracker_info);
        } else {
            /* 该跟踪器已经有对应的统计信息了(已经进入过窗口),若此时跟踪器离开窗口,则计数 */
            if(is_in_window) {
                continue;
            }
            new_tracker_info = tracker_all_inf[index];
            /* 已经离开窗口,进行计数 */
            if(in_dist < out_dist) { /* 从in端离开 */
                if(tracker_all_inf[index].direct == DIRECTION_FLOW_IN) {
                    st_info.total_in_count++;
                }
            } else {
                if(tracker_all_inf[index].direct == DIRECTION_FLOW_OUT) {
                    st_info.total_out_count++;
                }
            }
            /* 计数完成后,删掉对应的统计信息 */
            vector<ST_TRACKER_INFO>::iterator it = tracker_all_inf.begin();
            while(it != tracker_all_inf.end()) {
                if((*it).tracker_id == new_tracker_info.tracker_id) {
                    tracker_all_inf.erase(it);
                } else {
                    it++;
                }
            }
        }
    }
}

void StatisticsManager::on_end_count() {
    time_t end_time;
    time(&end_time);
    if(!count_status) {
        return;
    }
    /* 设置统计结束时间 */
    this->st_info.end_time = end_time;
    /* 修改状态 */
    count_status = false;
    /* 清除所有跟踪器 */
    tracker_all_inf.clear();
}

void StatisticsManager::display_statistics(STATISTICS_INFO info,Mat &canvas,Rect count_window) {
    if(info.start_time == 0) {
        return;
    } else {
        time_t tim;
        time(&tim);
        if(tim - info.start_time <= 0) {
            return;
        }
    }
    char str[100];
    sprintf(str,"in:%d/out:%d ",info.total_in_count,info.total_out_count);
    rectangle(canvas,count_window,Scalar(255,255,255));
    putText(canvas,str,
            Point(count_window.x,count_window.y+count_window.height/2),
            CV_FONT_HERSHEY_SIMPLEX,
            1,
            Scalar(255,255,255),
            2);
}

STATISTICS_INFO StatisticsManager::get_statistics_inf() {
    STATISTICS_INFO info;
    info.start_time = this->st_info.start_time;
    info.end_time = this->st_info.end_time;
    info.total_in_count = this->st_info.total_in_count;
    info.total_out_count = this->st_info.total_out_count;
    return info;
}
