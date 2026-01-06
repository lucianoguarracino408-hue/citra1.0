from robot_bridge import RobotClient

robot_client = RobotClient()

def send_command_to_robot(vx: float, vy: float):
    if vx != 0.0 or vy != 0.0:
        return robot_client.send_command(
            "move",
            {"vx": vx, "vy": vy}
        )
    else:
        return robot_client.send_command("stop", {})
