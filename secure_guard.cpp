#include "secure_guard.h"
#include "integrity_chain.h"
#include "audit_log.h"
#include "backup_vault.h"

#include <openssl/crypto.h>

static constexpr uint64_t MAX_SKEW_SECONDS = 300;

SecureGuard::SecureGuard(const std::vector<unsigned char>& shared_key)
    : key_(shared_key) {}

bool SecureGuard::verify_timestamp(uint64_t ts) const {
    uint64_t now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    return (ts <= now + MAX_SKEW_SECONDS) && (ts + MAX_SKEW_SECONDS >= now);
}

bool SecureGuard::verify_hmac(const SecureEvent& event) const {
    auto expected = hmac_sha256_bin(key_, event.payload);
    if (expected.size() != event.hmac.size()) return false;

    return CRYPTO_memcmp(expected.data(),
                          event.hmac.data(),
                          expected.size()) == 0;
}

bool SecureGuard::process_event(const SecureEvent& event) {
    if (!verify_timestamp(event.timestamp)) {
        AuditLog::log("INVALID_TIMESTAMP", "high");
        return false;
    }

    if (!verify_hmac(event)) {
        AuditLog::log("HMAC_VERIFICATION_FAILED", "critical");
        return false;
    }

    IntegrityChain::append(event.payload);
    BackupVault::store(event.payload);

    AuditLog::log("EVENT_ACCEPTED", "info");
    return true;
}
