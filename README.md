# ESP Transceiver IR

ESP Transceiver IR es un transceptor infrarrojo con interfaz web para aprender comandos IR, guardarlos en memoria, crear mapeos simples, definir secuencias y ejecutar acciones automáticas al arrancar. El proyecto está orientado a `ESP8266` con `WeMos D1 mini` y usa `LittleFS` como almacenamiento persistente.

Versión actual del proyecto: `0.8.4`

## Qué hace

- Aprende comandos IR desde un receptor y los guarda con nombre.
- Reproduce comandos guardados usando un LED IR emisor.
- Crea mapeos simples `comando origen -> comando destino`.
- Define secuencias de varios comandos de entrada que disparan un comando guardado.
- Ejecuta un comando o una secuencia automáticamente al arrancar el dispositivo.
- Expone una interfaz web local para operar todo sin recompilar firmware.
- Soporta `English`, `Español` y `Polski` tanto en frontend como en mensajes del backend.
- Permite exportar e importar backups completos o parciales.
- Guarda configuración de red `DHCP` o IP estática.

## Hardware soportado

El repositorio está preparado actualmente solo para:

- `WeMos D1 mini`

Pines usados por defecto en `platformio.ini`:

- `GPIO4 / D2`: salida IR
- `GPIO5 / D1`: entrada IR

## Arquitectura

El firmware está dividido en estos módulos:

- `src/main.cpp`: arranque del sistema, loop principal y comandos serie.
- `src/ir/`: captura, envío, persistencia y lógica de secuencias/mapeos IR.
- `src/storage/`: acceso a `LittleFS` con guardado atómico del JSON principal.
- `src/wifi/`: conexión WiFi con `WiFiManager` y exportación del estado de red.
- `src/web/`: servidor HTTP y frontend embebido.
- `src/system/`: logging circular en memoria e internacionalización.
- `data/www/`: fuentes de la interfaz web.
- `src/web/embedded_ui.h`: copia embebida de la UI servida desde memoria de programa.

## Interfaz web

La UI está organizada por pestañas:

- `Resumen`
  - conexión, actividad del bridge, memoria, logs y snapshot del sistema
- `Comandos`
  - grabación, envío, edición JSON, renombrado y borrado
- `Mapeos`
  - mapeos simples y resolución de mapeos
- `Secuencias`
  - creación y gestión de secuencias de comandos
- `Ajustes`
  - tema, idioma, red, backup y acciones al inicio

## Persistencia

El proyecto guarda sus datos en:

- `LittleFS`
- archivo principal: `/ir_data.json`

Datos persistidos:

- comandos IR
- mapeos
- secuencias
- idioma
- configuración de red
- acción de arranque

El servicio de almacenamiento usa escritura temporal y `rename` para reducir el riesgo de corrupción.

## Acciones al inicio

Puedes configurar una acción automática al arrancar:

- desactivada
- ejecutar un comando guardado
- reproducir una secuencia guardada

La acción incluye retardo configurable en milisegundos.

## Backup e importación

La interfaz permite exportar:

- todo
- solo comandos
- solo mapeos
- solo secuencias

La importación repone el bloque funcional del bridge y valida:

- esquema JSON
- referencias entre comandos, mapeos y secuencias
- validez de la acción de arranque

## API HTTP

Base:

```text
http://<ip-del-dispositivo>/
```

Rutas principales:

- `GET /api/ui-state`
- `GET /api/bootstrap`
- `GET /api/status`
- `GET /api/system`
- `GET /api/network`
- `POST /api/network`
- `GET /api/settings`
- `POST /api/settings`
- `GET /api/startup`
- `POST /api/startup`
- `GET /api/commands`
- `GET /api/command?name=<nombre>`
- `GET /api/mappings`
- `GET /api/sequences`
- `POST /api/record`
- `POST /api/update`
- `POST /api/rename`
- `POST /api/delete`
- `POST /api/map`
- `POST /api/unmap`
- `GET /api/resolve?source=<nombre>`
- `POST /api/sequence`
- `POST /api/delete-sequence`
- `POST /api/execute`
- `GET /api/export`
- `POST /api/import`
- `POST /api/factory`
- `GET /api/logs`

## Comandos serie

Con el monitor serie a `115200`, el firmware acepta:

- `record NOMBRE`
- `list`
- `delete NOMBRE`
- `map ORIGEN DESTINO`
- `send NOMBRE`
- `reboot`

## Compilación y carga

Requisitos:

- `PlatformIO`
- `Python` instalado por PlatformIO
- `WeMos D1 mini` conectado por USB

Compilar:

```bash
pio run
```

Subir firmware:

```bash
pio run -t upload
```

Monitor serie:

```bash
pio device monitor -b 115200
```

## Dependencias

Dependencias principales definidas en `platformio.ini`:

- `IRremoteESP8266`
- `ArduinoJson`
- `WiFiManager`
- `ESPAsyncWebServer`
- `ESPAsyncTCP`

## Estructura de datos IR

Cada comando puede almacenar:

- protocolo decodificado
- valor
- número de bits
- frecuencia
- repeticiones
- timings raw, cuando procede
- firma estable para identificación

Esto permite reproducir tanto protocolos conocidos como capturas raw.

## Flujo de secuencias

Las secuencias están separadas de los mapeos simples:

- un mapeo simple resuelve `A -> B`
- una secuencia resuelve `A1 + A2 + A3 -> B`

Si un comando forma parte del prefijo de una secuencia activa, el motor espera el siguiente paso dentro de una ventana configurable. Si la secuencia no se completa, puede ejecutarse el mapeo simple diferido.

## Reset de fábrica

El reset de fábrica:

- borra completamente `LittleFS`
- elimina comandos, mapeos, secuencias, red y acción de arranque
- fuerza reinicio del dispositivo

## Desarrollo

Pautas útiles para mantener el proyecto:

- editar la UI fuente en `data/www/`
- mantener `src/web/embedded_ui.h` sincronizado con esos archivos
- usar `LittleFS` solo para persistencia de datos, no para servir la UI
- subir versión desde `platformio.ini` usando `MAJOR.MINOR.PATCH`

## Limitaciones conocidas

- el proyecto está afinado para `WeMos D1 mini`; no se mantiene soporte activo para `ESP-01M`
- el almacenamiento depende del tamaño de flash disponible en el `ESP8266`
- las emisiones IR deben validarse con el hardware real del equipo a controlar

## Autor

- GitHub: [@rafasanz](https://github.com/rafasanz)
