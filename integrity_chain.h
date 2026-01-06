#pragma once
#include <string>

class IntegrityChain {
public:
    static void append(const std::string& data);
};
#include "integrity_chain.h"
#include <fstream>

static std::string last_hash = "GENESIS";

void IntegrityChain::append(const std::string& data) {
    std::string combined = last_hash + data;
    last_hash = sha256_hex(combined);

    std::ofstream f("integrity.chain", std::ios::app);
    f << last_hash << "\n";
}
