# Sistema IoT Resiliente para Farmacia 4.0

Este proyecto implementa una solución IoT de bajo costo y alta disponibilidad para la monitorización de la cadena de frío en farmacias. El sistema previene la pérdida de medicamentos termosensibles (vacunas/insulinas) mediante alertas en tiempo real y capacidad de respuesta remota.

---

## Características Principales

- **Monitorización Activa:** Lectura y transmisión de temperatura en tiempo real.  
- **Filtrado de Señal:** Algoritmo de oversampling y promedio para estabilizar lecturas del sensor analógico (LM35).  
- **Resiliencia:** Sistema de reconexión automática y redundancia de red (WiFi / 4G).  
- **Dashboard Industrial:** Visualización de datos, histórico de auditoría y alarmas visuales en ThingsBoard.  
- **Control Remoto:** Capacidad de activar sistemas de contingencia (coolers) desde la nube.

---

## Hardware

- **Microcontrolador:** ESP32 DevKit V1 (Dual Core).  
- **Sensor:** LM35 (Analógico), seleccionado por su linealidad y robustez frente a sensores digitales low-cost.  
- **Indicadores:** LEDs de estado (Normal, Alerta Calor, Alerta Frío).

---

## Arquitectura y Protocolos

El sistema utiliza una arquitectura estándar basada en **MQTT**:

- **Dispositivo:** El ESP32 publica telemetría JSON  
  `{"temp": 24.5}`  
  en el tópico:  
  `v1/devices/me/telemetry`
- **Broker/Cloud:** ThingsBoard procesa datos y ejecuta reglas de alarma.
- **Usuario:** Interfaz web para visualización y control mediante RPC (Remote Procedure Call).

---

## Roadmap y Futuro

- [ ] Implementación de sonda en glicol para “lectura limpia” (inercia térmica).  
- [ ] Sistema de respaldo energético (Batería Li-Ion / UPS).  
- [ ] Migración a stack 100% Open Source (Mosquitto + InfluxDB + Grafana).

---

**Materia:** Integración Tecnológica  
**Fecha:** Noviembre 2025