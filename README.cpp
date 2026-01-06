README
#############################################
cat > $BASE/README.md << 'EOF'
# CITRA Robotics (auto-generated)
Compilar:
  mkdir -p build
  cd build
  cmake ../cpp
  make -j4
  ./robot_server
EOF

echo "=== Proyecto generado correctamente ==="
echo "Ubicación: $BASE"
// En security.h
inline std::string calculate_hmac_sha256(...) { ... }
long long now = std::time(nullptr);
long long ts_val = std::stoll(ts);
if (std::abs(now - ts_val) > 300) { // 5 minutos
    res.status = 403;
    res.set_content(R"({"error":"timestamp expired"})", "application/json");
    return;
}// En main.cpp
#include <csignal>
std::atomic<bool> server_running{true};

void signal_handler(int) { server_running = false; }

int main() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // ... luego, en el loop:
    svr.listen("0.0.0.0", 8080);
    while (server_running) std::this_thread::sleep_for(1s);
    core.stop();
}