import zmq
import requests
import time
import json
import logging

STATUS_URL = "http://localhost:8080/status"
COMMAND_PUSH_URL = "tcp://127.0.0.1:5555"

log = logging.getLogger(__name__)

class RobotCommandClient:
    """
    Cliente seguro para envío de comandos al RobotCore.
    - ZMQ PUSH no bloqueante
    - Cola limitada
    - Timestamps para descarte de comandos viejos
    """

    def __init__(self):
        self.context = zmq.Context()
        self.pusher = self.context.socket(zmq.PUSH)

        # Protección ante caídas
        self.pusher.setsockopt(zmq.SNDHWM, 10)
        self.pusher.setsockopt(zmq.LINGER, 0)

        self.pusher.connect(COMMAND_PUSH_URL)
        log.info("ZMQ Command client conectado a %s", COMMAND_PUSH_URL)

    def send_command(self, cmd_type: str, payload: dict) -> bool:
        """
        Envía un comando al RobotCore.
        Retorna False si el comando fue descartado.
        """
        msg = {
            "type": cmd_type,
            "payload": payload,
            "ts": time.time()
        }

        try:
            self.pusher.send_json(msg, flags=zmq.NOBLOCK)
            log.debug("Comando enviado: %s", json.dumps(msg))
            return True

        except zmq.Again:
            log.warning(
                "Cola ZMQ llena. Comando descartado: %s", cmd_type
            )
            return False

        except zmq.ZMQError as e:
            log.error("Error ZMQ al enviar comando: %s", e)
            return False

    def is_core_alive(self) -> bool:
        """
        Heartbeat HTTP del RobotCore.
        """
        try:
            r = requests.get(STATUS_URL, timeout=0.5)
            return r.status_code == 200
        except requests.RequestException:
            return False

    def close(self):
        """
        Cierre seguro de recursos.
        """
        try:
            self.pusher.close(0)
        finally:
            self.context.term()
