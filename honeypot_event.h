#pragma once
#include <nlohmann/json.hpp>

bool validate_honeypot_event(const nlohmann::json& event);
#include "honeypot_event.h"

bool validate_honeypot_event(const nlohmann::json& e) {
    if (!e.contains("type") || !e["type"].is_string()) return false;
    if (!e.contains("timestamp") || !e["timestamp"].is_number_integer())
        return false;

    return true;
}
