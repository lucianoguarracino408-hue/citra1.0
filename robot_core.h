robot_core.h

cat > $BASE/cpp/src/robot_core.h << 'EOF'
#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map> 


struct RobotState {
    double x=0.0, y=0.0, theta=0.0, vx=0.0, vy=0.0;
    bool moving=false;
    int battery=100;
};

struct RobotCommand {
    std::string type;
    nlohmann::json payload;
};

class RobotCore {
public:
    using TelemetryCb = std::function<void(const nlohmann::json&)>;
    RobotCore();
    ~RobotCore();
    void start();
    void stop();
    void push_command(const RobotCommand& cmd);
    RobotState get_state() const;
    void set_telemetry_callback(TelemetryCb cb);
    nlohmann::json state_to_json() const;

private:
    void thread_loop();
    void handle_command(const RobotCommand& cmd);
    std::thread worker_;
    std::atomic<bool> running_{false};
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<RobotCommand> q_;
    RobotState state_;
    TelemetryCb telemetry_cb_{nullptr};
    const std::chrono::milliseconds tick_{100};
};
EOF