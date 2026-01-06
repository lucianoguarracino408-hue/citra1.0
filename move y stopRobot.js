async function move(dir) {
    if (!connected || shuttingDown) return;

    // Traducimos dirección a coordenadas para el Core C++
    let coords = { vx: 0, vy: 0 };
    if (dir === 'Adelante') coords.vx = 1.0;
    if (dir === 'Atrás')    coords.vx = -1.0;
    if (dir === 'Izquierda') coords.vy = -1.0;
    if (dir === 'Derecha')   coords.vy = 1.0;

    updateTelemetry(dir, 50);

    // Envío real al servidor
    await fetch('http://localhost:5000/command', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ type: 'move', payload: coords })
    });
}

async function stopRobot() {
    if (!connected || shuttingDown) return;
    updateTelemetry("Detenido", 0);
    
    await fetch('http://localhost:5000/command', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ type: 'stop', payload: {} })
    });
}