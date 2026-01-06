#pragma once

#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/crypto.h> // para OPENSSL_cleanse
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include <cstring> // para memset (fallback)

// ---- SHA-256 en hexadecimal ----
std::string sha256_hex(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.data()), data.size(), hash);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned char b : hash) {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

// ---- HMAC-SHA256 en binario ----
std::vector<unsigned char> hmac_sha256_bin(const std::vector<unsigned char>& key, const std::string& data) {
    unsigned char out[EVP_MAX_MD_SIZE];
    unsigned int len = 0;

    if (!HMAC(EVP_sha256(),
              key.data(), static_cast<int>(key.size()),
              reinterpret_cast<const unsigned char*>(data.data()), data.size(),
              out, &len)) {
        // Opcional: lanzar excepción o loguear error
        return {};
    }

    return std::vector<unsigned char>(out, out + len);
}

// ---- SecureString seguro y portable ----
class SecureString {
    std::vector<unsigned char> data_;

public:
    explicit SecureString(const std::string& s) : data_(s.begin(), s.end()) {}

    ~SecureString() {
        // Usar OPENSSL_cleanse si está disponible (más seguro que memset)
        if (!data_.empty()) {
            OPENSSL_cleanse(data_.data(), data_.size());
            // Alternativa portable si no usas OpenSSL: explicit_bzero o volatile loop
        }
    }

    // Evita c_str() si es posible. Si se necesita, haz una copia explícita.
    std::string str() const {
        return std::string(data_.begin(), data_.end());
    }

    size_t size() const noexcept { return data_.size(); }

    // Eliminado: c_str() con static thread_local (riesgoso)
};

// ---- Validación de email (mejorada mínimamente) ----
bool is_valid_email(const std::string& email) {
    static const std::regex pattern(
        R"(^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$)",
        std::regex::optimize
    );
    return !email.empty() && email.size() <= 254 && std::regex_match(email, pattern);
}

// ---- Configuración segura de cURL (como función utilitaria) ----
void curl_setup_secure(CURL* curl) {
    if (!curl) return;
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    // Opcional: añadir CA bundle si es necesario
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/cacert.pem");
}

// ---- Validación segura de rutas (evita directory traversal) ----
bool is_path_within(const std::string& base, std::string path) {
    // Nota: una implementación robusta requiere canonicalización (eliminar '..', '.', etc.)
    // Esta es una versión defensiva simple, útil solo si 'path' ya está normalizado.

    if (base.empty() || path.empty()) return false;

    // Asegurar que ambas rutas terminen con '/'
    std::string base_norm = base.back() == '/' ? base : base + '/';
    if (path.back() != '/') path += '/';

    // Verificar prefijo y que no haya escape vía ".."
    if (path.substr(0, base_norm.size()) != base_norm) return false;

    // Protección adicional: si hay ".." en la parte relativa, rechazar
    std::string relative = path.substr(base_norm.size());
    if (relative.find("..") != std::string::npos) return false;

    return true;
}