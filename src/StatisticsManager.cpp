#include <StatisticsManager.h>
#include <time.h>

StatisticsManager::StatisticsManager() {

}

StatisticsManager::~StatisticsManager() {

}

void StatisticsManager::create(TrackManager *ptm, Rect count_window, DIRECTION in, DIRECTION out) {
    if(ptm==NULL) {
        return;
    }
    this->count_window = count_window;
    this->in_diect = in;
    this->out_direct = out;
}

void StatisticsManager::on_begin_count() {
    if(count_status==true || ptm==NULL) {
        cout<<"Flow Statistics Manager Error!"<<endl;
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

void StatisticsManager::get_statistics_inf(STATISTICS_INFO &info) {
    info.start_time = this->st_info.start_time;
    info.end_time = this->st_info.end_time;
    info.total_in_count = this->st_info.total_in_count;
    info.total_out_count = this->st_info.total_out_count;
}
