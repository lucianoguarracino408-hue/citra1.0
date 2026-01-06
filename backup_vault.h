#pragma once
#include <string>

class BackupVault {
public:
    static void store(const std::string& data);
};
#include "backup_vault.h"
#include <fstream>

void BackupVault::store(const std::string& data) {
    std::ofstream f("backup.vault", std::ios::app | std::ios::binary);
    f.write(data.data(), data.size());
    f.put('\n');
}
