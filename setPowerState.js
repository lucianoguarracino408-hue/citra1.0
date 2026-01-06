async function setPowerState(on) {
    if (on) {
        try {
            // Intentamos saludar al backend para ver si está vivo
            const response = await fetch('http://localhost:5000/status');
            if (!response.ok) throw new Error();
            
            connected = true;
            document.body.classList.remove("power-off");
            powerButton.textContent = "⏻ APAGAR SISTEMA";
            powerButton.classList.add("on");
            statusIndicator.textContent = "Conectado";
            statusIndicator.classList.add("connected");
        } catch (error) {
            alert("Error: No se pudo establecer conexión con el servidor de CITRA.");
            return; // No enciende si no hay servidor
        }
    } else {
        connected = false;
        document.body.classList.add("power-off");
        powerButton.textContent = "⏻ ENCENDER SISTEMA";
        powerButton.classList.remove("on");
        statusIndicator.textContent = "Desconectado";
        statusIndicator.classList.remove("connected");
    }
}