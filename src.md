citra/
├── CMakeLists.txt
├── README.md
├── config/
│   └── citra.yaml / json
│
├── src/
│   ├── core/
│   │   ├── robot_core.cpp
│   │   ├── robot_core.h
│   │
│   ├── security/
│   │   ├── secure_guard.cpp
│   │   ├── integrity_chain.h
│   │   ├── honeypot_event.h
│   │   ├── backup_vault.h
│   │   └── validate_honeypot.cpp
│   │
│   ├── web/
│   │   ├── http_server.cpp
│   │   ├── json_api.cpp
│   │   └── placeholders.cpp
│   │
│   ├── watchdog/
│   │   └── watchdog.cpp
│   │
│   └── config/
│       ├── config_loader.cpp
│       └── config_loader.h
│
├── python/
│   ├── ai_interface.py
│   ├── robot_bridge.py
│   ├── zmq_http_bridge.py
│   └── services/
│       └── robot_bridge_service.py
│
├── include/
│   └── citra/
│       ├── robot_core.h
│       ├── security/
│       └── config/
│
└── scripts/
    └── run_dev.sh