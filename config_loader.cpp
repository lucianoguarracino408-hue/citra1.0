robot_core.cpp

cat > $BASE/cpp/src/robot_core.cpp << 'EOF'
#include "robot_core.h"
#include <algorithm>

RobotCore::RobotCore() = default;

RobotCore::~RobotCore(){
    stop();
}

void RobotCore::start(){
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true))
        return;
    worker_ = std::thread(&RobotCore::thread_loop, this);
}

void RobotCore::stop(){
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false))
        return;
    cv_.notify_all();
    if (worker_.joinable())
        worker_.join();
}

void RobotCore::push_command(const RobotCommand& cmd){
    {
        std::lock_guard<std::mutex> lk(mtx_);
        q_.push(cmd);
    }
    cv_.notify_one();
}

RobotState RobotCore::get_state() const{
    std::lock_guard<std::mutex> lk(mtx_);
    return state_;
}

void RobotCore::set_telemetry_callback(TelemetryCb cb){
    telemetry_cb_ = std::move(cb);
}

nlohmann::json RobotCore::state_to_json() const{
    std::lock_guard<std::mutex> lk(mtx_);
    nlohmann::json j;
    j["x"]=state_.x;
    j["y"]=state_.y;
    j["theta"]=state_.theta;
    j["vx"]=state_.vx;
    j["vy"]=state_.vy;
    j["moving"]=state_.moving;
    j["battery"]=state_.battery;
    j["timestamp_ms"] = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return j;
}

void RobotCore::handle_command(const RobotCommand& cmd){
    std::lock_guard<std::mutex> lk(mtx_);

    if(cmd.type=="move"){
        double vx = cmd.payload.value("vx", 0.0);
        double vy = cmd.payload.value("vy", 0.0);
        vx = std::clamp(vx, -2.0, 2.0);
        vy = std::clamp(vy, -2.0, 2.0);
        state_.vx = vx;
        state_.vy = vy;
        state_.moving = true;
    }
    else if(cmd.type=="stop"){
        state_.moving = false;
        state_.vx = 0;
        state_.vy = 0;
    }
}

void RobotCore::thread_loop(){
    while(running_){
        {
            std::unique_lock<std::mutex> lk(mtx_);
            if(q_.empty())
                cv_.wait_for(lk, tick_);

            while(!q_.empty()){
                RobotCommand cmd = q_.front();
                q_.pop();
                lk.unlock();
                handle_command(cmd);
                lk.lock();
            }
        }

        {
            std::lock_guard<std::mutex> lk(mtx_);
            double dt = tick_.count()/1000.0;
            if(state_.moving){
                state_.x += state_.vx * dt;
                state_.y += state_.vy * dt;
                state_.battery = std::max(0, state_.battery - 1);
            } else {
                state_.battery = std::min(100, state_.battery + 1);
            }
        }

        if(telemetry_cb_){
            telemetry_cb_(state_to_json());
        }

        std::this_thread::sleep_for(tick_);
    }
}
EOF