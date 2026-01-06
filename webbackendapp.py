from flask import Flask, request, jsonify
from flask_cors import CORS
import requests

app = Flask(__name__)
CORS(app)  # Permite que el frontend acceda al backend

CORE_URL = 'http://localhost:8080'

@app.route('/status', methods=['GET'])
def get_status():
    try:
        # Intentamos conectar con el Core en C++
        response = requests.get(f'{CORE_URL}/status', timeout=0.5)
        return jsonify(response.json()), 200
    except Exception as e:
        return jsonify({"status": "error", "msg": "Core C++ desconectado"}), 503

@app.route('/command', methods=['POST'])
def post_command():
    try:
        response = requests.post(f'{CORE_URL}/command', json=request.json, timeout=0.5)
        return jsonify(response.json()), 200
    except Exception as e:
        return jsonify({"status": "error", "msg": "No se pudo enviar el comando"}), 503

if __name__ == '__main__':
    print("CITRA Web Backend corriendo en http://localhost:5000")
    app.run(port=5000, debug=True)