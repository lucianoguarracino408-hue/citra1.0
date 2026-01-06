if not robot_client.is_core_alive():
    return {"status": "error", "reason": "RobotCore no disponible"}
