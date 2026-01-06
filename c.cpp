#pragma once
#include <string>
#include <vector>
#include <chrono>

struct SecureEvent {
    std::string payload;
    std::vector<unsigned char> hmac;
    uint64_t timestamp;
};

class SecureGuard {
public:
    explicit SecureGuard(const std::vector<unsigned char>& shared_key);

    bool process_event(const SecureEvent& event);

private:
    bool verify_timestamp(uint64_t ts) const;
    bool verify_hmac(const SecureEvent& event) const;

    std::vector<unsigned char> key_;
};
