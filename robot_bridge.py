import zmq
import json
import logging
import requests

COMMAND_ADDRESS = "tcp://localhost:5555"
STATUS_URL = "http://localhost:8080/status"


class RobotClient:
    """
    Cliente CITRA Robotics
    - Comandos: ZMQ PUSH
    - Estado: HTTP GET
    """

    def __init__(self):
        self.context = zmq.Context()
        self.pusher = self.context.socket(zmq.PUSH)

        try:
            self.pusher.connect(COMMAND_ADDRESS)
            logging.info(f"[ZMQ] Conectado a {COMMAND_ADDRESS}")
        except zmq.ZMQError as e:
            logging.error(f"[ZMQ] Error de conexión: {e}")

    def send_command(self, cmd_type: str, payload: dict) -> dict:
        command = {
            "type": cmd_type,
            "payload": payload
        }

        try:
            self.pusher.send_string(json.dumps(command))
            return {"status": "success", "command": cmd_type}
        except zmq.ZMQError as e:
            logging.error(f"Error ZMQ: {e}")
            return {"status": "error", "message": str(e)}

    def get_status_sync(self) -> dict:
        try:
            r = requests.get(STATUS_URL, timeout=1)
            r.raise_for_status()
            return r.json()
        except requests.RequestException as e:
            logging.warning(f"Error HTTP status: {e}")
            return {"status": "error"}
if __name__ == "__main__":
    print("Robot Bridge iniciado")
    client = RobotClient()
    while True:
        pass

    import time
if __name__ == "__main__":
    print("Robot Bridge iniciado")
    while True:
        print("Bridge vivo")
        time.sleep(2)
if __name__ == "__main__":
    print("✅ Robot Bridge iniciado")
    client = RobotClient()
    while True:
        print("Bridge vivo")
        time.sleep(2)