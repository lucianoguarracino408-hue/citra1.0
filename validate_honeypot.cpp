if (std::abs(now - ts) > 300)
std::string expected_hmac = calculate_hmac_sha256_bin(...)

bool validate_honeypot_event(const nlohmann::json& j) {
    try {
        if (!j.contains("type") || !j.contains("timestamp") ||
            !j.contains("file") || !j.contains("hmac") ||
            !j.contains("nonce"))
            return false;

        if (j["type"] != "HONEYPOT_TRIGGER")
            return false;

        auto now = std::time(nullptr);
        auto ts = j["timestamp"].get<std::time_t>();
        if (std::llabs(now - ts) > 300)
            return false;

        if (!is_safe_filename(j["file"].get<std::string>()))
            return false;

        return true;
    } catch (...) {
        return false;
    }
}
bool verify_hmac(const SecureEvent& ev,
                 const std::vector<unsigned char>& key) {

    auto expected = hmac_sha256_bin(key, ev.payload);

    if (expected.size() != ev.hmac.size())
        return false;

    return CRYPTO_memcmp(expected.data(),
                          ev.hmac.data(),
                          expected.size()) == 0;
}
bool is_rate_limited(const std::string& source_id) {
    return RateLimiter::check(source_id, 60); // 60/min
}
void Guard::process_event(const SecureEvent& ev) {

    if (!verify_hmac(ev, secret_key_)) {
        AuditLog::log("HMAC_MISMATCH", "critical");
        return;
    }

    if (is_replay(ev.nonce)) {
        AuditLog::log("REPLAY_ATTACK", "critical");
        return;
    }

    if (is_rate_limited(ev.source_id)) {
        AuditLog::log("RATE_LIMIT", "info");
        return;
    }

    AlertSystem::send(ev.payload);
}
