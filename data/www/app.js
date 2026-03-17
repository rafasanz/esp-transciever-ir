const translations = {
  en: {
    heroEyebrow: "ESP IR Transceiver",
    heroTitle: "IR Bridge Control Deck",
    refresh: "Refresh",
    factoryReset: "Factory reset",
    tabDashboard: "Dashboard",
    tabCommands: "Commands",
    tabMappings: "Mappings",
    tabSequences: "Sequences",
    tabSettings: "Settings",
    summaryPanel: "Summary",
    connectionInfo: "Connection",
    activityInfo: "Bridge",
    memoryInfo: "Memory",
    liveStatus: "Live Status",
    recordCommand: "Record Command",
    quickSummary: "Quick Summary",
    deviceLog: "Device Log",
    auto: "Auto",
    refreshLogs: "Refresh logs",
    systemSnapshot: "System Snapshot",
    record: "Record",
    commandsTitle: "Commands",
    simpleMappings: "Simple Mappings",
    createMapping: "Create mapping",
    resolveMapping: "Resolve Mapping",
    test: "Test",
    commandSequences: "Command Sequences",
    createSequenceTitle: "Create Sequence",
    savedSequencesTitle: "Saved Sequences",
    addStep: "Add step",
    clear: "Clear",
    saveSequence: "Save sequence",
    appearance: "Appearance",
    lightMode: "Light mode",
    darkMode: "Dark mode",
    language: "Language",
    saveLanguage: "Save language",
    startupActions: "Startup Actions",
    startupNone: "Disabled",
    startupCommand: "Command",
    startupSequence: "Sequence",
    startupDelayPlaceholder: "Delay ms",
    saveStartup: "Save startup",
    startupHelp: "Run one saved command or replay one saved sequence after boot.",
    networking: "Networking",
    dhcpMode: "DHCP",
    staticMode: "Static",
    saveNetwork: "Save network",
    backup: "Backup",
    exportJson: "Export JSON",
    exportAll: "All data",
    exportCommands: "Commands only",
    exportMappings: "Mappings only",
    exportSequences: "Sequences only",
    importJson: "Import JSON",
    backupHelp: "You can export all data or only one section. Import expects a full backup.",
    commandNamePlaceholder: "Command name",
    sequenceNamePlaceholder: "Sequence name",
    timeoutPlaceholder: "Timeout ms",
    loading: "Loading...",
    idle: "Idle.",
    idleLastReceived: 'Idle. Last received: {value}',
    waitingForSignal: 'Waiting for signal for "{name}" ({seconds}s left)',
    none: "none",
    yes: "Yes",
    no: "No",
    ip: "IP",
    networkMode: "Address mode",
    heap: "Heap",
    maxBlock: "Max block",
    fragment: "Fragment",
    recording: "Recording",
    lastTarget: "Last target",
    lastSequence: "Last sequence",
    commandsCount: "Commands: {count}",
    mappingsCount: "Mappings: {count}",
    sequencesCount: "Sequences: {count}",
    lastMappedTarget: "Last mapped target: {value}",
    noCommandsStored: "No commands stored.",
    noMappingsStored: "No mappings stored.",
    noSequencesStored: "No sequences stored.",
    noStepsSelected: "No steps selected.",
    noTestYet: "No test yet.",
    from: "From",
    to: "To",
    sequenceSteps: "Sequence",
    targetCommand: "Target",
    timeoutMs: "Timeout: {value} ms",
    send: "Send",
    rename: "Rename",
    edit: "Edit",
    hideEditor: "Hide",
    delete: "Delete",
    saveJson: "Save JSON",
    inspect: "Inspect",
    commandJson: "Command JSON",
    protocol: "Protocol {value}",
    bits: "Bits {value}",
    repeats: "Repeats {value}",
    rawSamples: "Raw {value}",
    decoded: "Decoded",
    renameTitle: "Rename command",
    renameMessage: 'Choose a new name for "{name}".',
    renamePlaceholder: "New command name",
    deleteCommandTitle: "Delete command",
    deleteCommandMessage: 'Delete "{name}" and remove related mappings and sequences?',
    deleteSequenceTitle: "Delete sequence",
    deleteSequenceMessage: 'Delete sequence "{name}"?',
    factoryTitle: "Factory reset",
    factoryMessage: "Format device memory and erase commands, mappings, sequences, startup action and network config?",
    cancel: "Cancel",
    confirm: "Confirm",
    save: "Save",
    invalidStaticNetwork: "Invalid static network",
    networkStaticRedirect: "Saved. Rebooting. Opening {ip}...",
    networkDhcpRedirect: "Saved. Rebooting. The IP may change after reconnect.",
    commandNameRequired: "Command name required",
    sequenceNameRequired: "Sequence name required",
    addAtLeastTwoSteps: "Add at least two steps",
    targetRequired: "Target required",
    timeoutRange: "Timeout must be 100-5000 ms",
    maxSteps: "Max 8 steps",
    noSourceSelected: "No source selected",
    mappingTargetRequired: "No target selected",
    mappingDifferentCommands: "Source and target must be different",
    mappingCommandsUnavailable: "Commands are not loaded yet",
    startupConfigured: "Startup: {value}",
    startupDisabled: "Disabled",
    startupPending: "Pending",
    startupCommandSummary: "Command {target} after {delay} ms",
    startupSequenceSummary: "Sequence {target} after {delay} ms",
    startupTargetRequired: "Choose a startup target",
    loadingCommandJson: "Loading command JSON...",
    wifi: "WiFi",
    commandsLabel: "Commands",
    mappingsLabel: "Mappings",
    sequencesLabel: "Sequences",
    startupLabel: "Startup"
  },
  es: {
    heroEyebrow: "Transceptor IR ESP",
    heroTitle: "Panel de control de IR Bridge",
    refresh: "Refrescar",
    factoryReset: "Reset de fábrica",
    tabDashboard: "Resumen",
    tabCommands: "Comandos",
    tabMappings: "Mapeos",
    tabSequences: "Secuencias",
    tabSettings: "Ajustes",
    summaryPanel: "Resumen",
    connectionInfo: "Conexión",
    activityInfo: "Bridge",
    memoryInfo: "Memoria",
    liveStatus: "Estado en vivo",
    recordCommand: "Grabar comando",
    quickSummary: "Resumen rápido",
    deviceLog: "Log del dispositivo",
    auto: "Auto",
    refreshLogs: "Refrescar log",
    systemSnapshot: "Instantánea del sistema",
    record: "Grabar",
    commandsTitle: "Comandos",
    simpleMappings: "Mapeos simples",
    createMapping: "Crear mapeo",
    resolveMapping: "Resolver mapeo",
    test: "Probar",
    commandSequences: "Secuencias de comandos",
    createSequenceTitle: "Crear secuencia",
    savedSequencesTitle: "Secuencias guardadas",
    addStep: "Añadir paso",
    clear: "Limpiar",
    saveSequence: "Guardar secuencia",
    appearance: "Apariencia",
    lightMode: "Modo claro",
    darkMode: "Modo oscuro",
    language: "Idioma",
    saveLanguage: "Guardar idioma",
    startupActions: "Acciones al inicio",
    startupNone: "Desactivado",
    startupCommand: "Comando",
    startupSequence: "Secuencia",
    startupDelayPlaceholder: "Retardo ms",
    saveStartup: "Guardar arranque",
    startupHelp: "Ejecuta un comando guardado o reproduce una secuencia guardada al arrancar.",
    networking: "Red",
    dhcpMode: "DHCP",
    staticMode: "Estática",
    saveNetwork: "Guardar red",
    backup: "Copia",
    exportJson: "Exportar JSON",
    exportAll: "Todos los datos",
    exportCommands: "Solo comandos",
    exportMappings: "Solo mapeos",
    exportSequences: "Solo secuencias",
    importJson: "Importar JSON",
    backupHelp: "Puedes exportar todo o solo una sección. La importación espera una copia completa.",
    commandNamePlaceholder: "Nombre del comando",
    sequenceNamePlaceholder: "Nombre de la secuencia",
    timeoutPlaceholder: "Timeout ms",
    loading: "Cargando...",
    idle: "En espera.",
    idleLastReceived: "En espera. Último recibido: {value}",
    waitingForSignal: 'Esperando señal para "{name}" ({seconds}s restantes)',
    none: "ninguno",
    yes: "Sí",
    no: "No",
    ip: "IP",
    networkMode: "Modo de dirección",
    heap: "Heap",
    maxBlock: "Bloque máximo",
    fragment: "Fragmentación",
    recording: "Grabando",
    lastTarget: "Último destino",
    lastSequence: "Última secuencia",
    commandsCount: "Comandos: {count}",
    mappingsCount: "Mapeos: {count}",
    sequencesCount: "Secuencias: {count}",
    lastMappedTarget: "Último destino mapeado: {value}",
    noCommandsStored: "No hay comandos guardados.",
    noMappingsStored: "No hay mapeos guardados.",
    noSequencesStored: "No hay secuencias guardadas.",
    noStepsSelected: "No hay pasos seleccionados.",
    noTestYet: "Sin pruebas todavía.",
    from: "Origen",
    to: "Destino",
    sequenceSteps: "Secuencia",
    targetCommand: "Objetivo",
    timeoutMs: "Timeout: {value} ms",
    send: "Enviar",
    rename: "Renombrar",
    edit: "Editar",
    hideEditor: "Ocultar",
    delete: "Borrar",
    saveJson: "Guardar JSON",
    inspect: "Inspeccionar",
    commandJson: "JSON del comando",
    protocol: "Protocolo {value}",
    bits: "Bits {value}",
    repeats: "Repeticiones {value}",
    rawSamples: "Raw {value}",
    decoded: "Decodificado",
    renameTitle: "Renombrar comando",
    renameMessage: 'Elige un nuevo nombre para "{name}".',
    renamePlaceholder: "Nuevo nombre del comando",
    deleteCommandTitle: "Borrar comando",
    deleteCommandMessage: '¿Borrar "{name}" y eliminar sus mapeos y secuencias relacionadas?',
    deleteSequenceTitle: "Borrar secuencia",
    deleteSequenceMessage: '¿Borrar la secuencia "{name}"?',
    factoryTitle: "Reset de fábrica",
    factoryMessage: "¿Formatear la memoria del dispositivo y borrar comandos, mapeos, secuencias, acción de arranque y configuración de red?",
    cancel: "Cancelar",
    confirm: "Confirmar",
    save: "Guardar",
    invalidStaticNetwork: "Red estática inválida",
    networkStaticRedirect: "Guardado. Reiniciando. Abriendo {ip}...",
    networkDhcpRedirect: "Guardado. Reiniciando. La IP puede cambiar tras reconectar.",
    commandNameRequired: "Hace falta el nombre del comando",
    sequenceNameRequired: "Hace falta el nombre de la secuencia",
    addAtLeastTwoSteps: "Añade al menos dos pasos",
    targetRequired: "Hace falta el comando objetivo",
    timeoutRange: "El timeout debe estar entre 100 y 5000 ms",
    maxSteps: "Máximo 8 pasos",
    noSourceSelected: "No hay origen seleccionado",
    mappingTargetRequired: "No hay destino seleccionado",
    mappingDifferentCommands: "Origen y destino deben ser distintos",
    mappingCommandsUnavailable: "Los comandos todavía no están cargados",
    startupConfigured: "Arranque: {value}",
    startupDisabled: "Desactivado",
    startupPending: "Pendiente",
    startupCommandSummary: "Comando {target} tras {delay} ms",
    startupSequenceSummary: "Secuencia {target} tras {delay} ms",
    startupTargetRequired: "Elige un objetivo de arranque",
    loadingCommandJson: "Cargando JSON del comando...",
    wifi: "WiFi",
    commandsLabel: "Comandos",
    mappingsLabel: "Mapeos",
    sequencesLabel: "Secuencias",
    startupLabel: "Arranque"
  },
  pl: {
    heroEyebrow: "Transceiver IR ESP",
    heroTitle: "Panel Sterowania IR Bridge",
    refresh: "Odswiez",
    factoryReset: "Reset fabryczny",
    tabDashboard: "Panel",
    tabCommands: "Komendy",
    tabMappings: "Mapowania",
    tabSequences: "Sekwencje",
    tabSettings: "Ustawienia",
    summaryPanel: "Podsumowanie",
    connectionInfo: "Polaczenie",
    activityInfo: "Mostek",
    memoryInfo: "Pamiec",
    liveStatus: "Stan na zywo",
    recordCommand: "Nagraj komende",
    quickSummary: "Szybkie podsumowanie",
    deviceLog: "Log urzadzenia",
    auto: "Auto",
    refreshLogs: "Odswiez log",
    systemSnapshot: "Migawka systemu",
    record: "Nagraj",
    commandsTitle: "Komendy",
    simpleMappings: "Proste mapowania",
    createMapping: "Utworz mapowanie",
    resolveMapping: "Sprawdz mapowanie",
    test: "Test",
    commandSequences: "Sekwencje komend",
    createSequenceTitle: "Utworz sekwencje",
    savedSequencesTitle: "Zapisane sekwencje",
    addStep: "Dodaj krok",
    clear: "Wyczysc",
    saveSequence: "Zapisz sekwencje",
    appearance: "Wyglad",
    lightMode: "Tryb jasny",
    darkMode: "Tryb ciemny",
    language: "Jezyk",
    saveLanguage: "Zapisz jezyk",
    startupActions: "Akcje startowe",
    startupNone: "Wylaczone",
    startupCommand: "Komenda",
    startupSequence: "Sekwencja",
    startupDelayPlaceholder: "Opoznienie ms",
    saveStartup: "Zapisz start",
    startupHelp: "Uruchom jedna zapisana komende albo odtworz jedna zapisana sekwencje po starcie.",
    networking: "Siec",
    dhcpMode: "DHCP",
    staticMode: "Statyczny",
    saveNetwork: "Zapisz siec",
    backup: "Kopia",
    exportJson: "Eksportuj JSON",
    exportAll: "Wszystkie dane",
    exportCommands: "Tylko komendy",
    exportMappings: "Tylko mapowania",
    exportSequences: "Tylko sekwencje",
    importJson: "Importuj JSON",
    backupHelp: "Mozesz eksportowac wszystko albo tylko jedna sekcje. Import oczekuje pelnej kopii.",
    commandNamePlaceholder: "Nazwa komendy",
    sequenceNamePlaceholder: "Nazwa sekwencji",
    timeoutPlaceholder: "Timeout ms",
    loading: "Ladowanie...",
    idle: "Bezczynny.",
    idleLastReceived: "Bezczynny. Ostatnio odebrano: {value}",
    waitingForSignal: 'Oczekiwanie na sygnal dla "{name}" ({seconds}s pozostalo)',
    none: "brak",
    yes: "Tak",
    no: "Nie",
    ip: "IP",
    networkMode: "Tryb adresu",
    heap: "Heap",
    maxBlock: "Max blok",
    fragment: "Fragmentacja",
    recording: "Nagrywanie",
    lastTarget: "Ostatni cel",
    lastSequence: "Ostatnia sekwencja",
    commandsCount: "Komendy: {count}",
    mappingsCount: "Mapowania: {count}",
    sequencesCount: "Sekwencje: {count}",
    lastMappedTarget: "Ostatni zmapowany cel: {value}",
    noCommandsStored: "Brak zapisanych komend.",
    noMappingsStored: "Brak zapisanych mapowan.",
    noSequencesStored: "Brak zapisanych sekwencji.",
    noStepsSelected: "Nie wybrano krokow.",
    noTestYet: "Brak testu.",
    from: "Zrodlo",
    to: "Cel",
    sequenceSteps: "Sekwencja",
    targetCommand: "Cel",
    timeoutMs: "Timeout: {value} ms",
    send: "Wyslij",
    rename: "Zmien nazwe",
    edit: "Edytuj",
    hideEditor: "Ukryj",
    delete: "Usun",
    saveJson: "Zapisz JSON",
    inspect: "Sprawdz",
    commandJson: "JSON komendy",
    protocol: "Protokol {value}",
    bits: "Bity {value}",
    repeats: "Powtorzenia {value}",
    rawSamples: "Raw {value}",
    decoded: "Dekodowane",
    renameTitle: "Zmien nazwe komendy",
    renameMessage: 'Wybierz nowa nazwe dla "{name}".',
    renamePlaceholder: "Nowa nazwa komendy",
    deleteCommandTitle: "Usun komende",
    deleteCommandMessage: 'Usunac "{name}" oraz powiazane mapowania i sekwencje?',
    deleteSequenceTitle: "Usun sekwencje",
    deleteSequenceMessage: 'Usunac sekwencje "{name}"?',
    factoryTitle: "Reset fabryczny",
    factoryMessage: "Sformatowac pamiec urzadzenia i usunac komendy, mapowania, sekwencje, akcje startowa oraz konfiguracje sieci?",
    cancel: "Anuluj",
    confirm: "Potwierdz",
    save: "Zapisz",
    invalidStaticNetwork: "Nieprawidlowa siec statyczna",
    networkStaticRedirect: "Zapisano. Restart. Otwieram {ip}...",
    networkDhcpRedirect: "Zapisano. Restart. Adres IP moze sie zmienic po ponownym polaczeniu.",
    commandNameRequired: "Wymagana nazwa komendy",
    sequenceNameRequired: "Wymagana nazwa sekwencji",
    addAtLeastTwoSteps: "Dodaj co najmniej dwa kroki",
    targetRequired: "Wymagany cel",
    timeoutRange: "Timeout musi byc miedzy 100 a 5000 ms",
    maxSteps: "Maksymalnie 8 krokow",
    noSourceSelected: "Nie wybrano zrodla",
    mappingTargetRequired: "Nie wybrano celu",
    mappingDifferentCommands: "Zrodlo i cel musza byc rozne",
    mappingCommandsUnavailable: "Komendy nie sa jeszcze zaladowane",
    startupConfigured: "Start: {value}",
    startupDisabled: "Wylaczone",
    startupPending: "Oczekuje",
    startupCommandSummary: "Komenda {target} po {delay} ms",
    startupSequenceSummary: "Sekwencja {target} po {delay} ms",
    startupTargetRequired: "Wybierz cel startowy",
    loadingCommandJson: "Ladowanie JSON komendy...",
    wifi: "WiFi",
    commandsLabel: "Komendy",
    mappingsLabel: "Mapowania",
    sequencesLabel: "Sekwencje",
    startupLabel: "Start"
  }
};

const state = {
  commands: {},
  commandDetails: {},
  mappings: {},
  sequences: {},
  counts: { commands: 0, mappings: 0, sequences: 0 },
  startup: { enabled: false, type: "none", target: "", delay_ms: 1200 },
  systemDevice: {},
  statusSnapshot: {},
  sequenceDraft: [],
  version: document.documentElement.dataset.appVersion || "",
  language: document.documentElement.dataset.cachedLang || localStorage.getItem("ir-bridge-language") || "en",
  theme: "light",
  networkMode: "dhcp",
  logsTimer: null,
  statusTimer: null,
  toastTimer: null,
  statusPolling: false,
  wasRecording: false,
  refreshInFlight: false,
  loaded: {
    ui: false,
    commands: false,
    mappings: false,
    sequences: false,
    logs: false
  },
  modalResolver: null
};

const cacheKeys = {
  uiState: "ir-bridge-cache-ui-state",
  commands: "ir-bridge-cache-commands",
  mappings: "ir-bridge-cache-mappings",
  sequences: "ir-bridge-cache-sequences"
};

function $(id) {
  return document.getElementById(id);
}

function t(key, params = {}) {
  const dict = translations[state.language] || translations.en;
  const source = dict[key] || translations.en[key] || key;
  return source.replace(/\{(\w+)\}/g, (_, name) => String(params[name] ?? ""));
}

function setBusy(el, busy) {
  if (el) el.disabled = !!busy;
}

function showToast(message) {
  const toast = $("toast");
  toast.textContent = message;
  toast.classList.add("show");
  clearTimeout(state.toastTimer);
  state.toastTimer = setTimeout(() => toast.classList.remove("show"), 2600);
}

function formBody(values) {
  return new URLSearchParams(values).toString();
}

function escapeHtml(value) {
  return String(value)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#39;");
}

function routeValueStyle(value, compact = false) {
  const length = String(value || "").length;
  let rem = 1.12;
  if (length > 12) rem = 1.03;
  if (length > 16) rem = 0.96;
  if (length > 22) rem = 0.9;
  if (length > 28) rem = 0.84;
  if (compact) rem = Math.min(rem, 0.94);
  return `style="--route-font-size:${rem}rem"`;
}

async function api(url, options = {}, format = "text") {
  const response = await fetch(url, {
    cache: "no-store",
    ...options
  });
  const text = await response.text();
  if (!response.ok) throw new Error(text || `HTTP ${response.status}`);
  return format === "json" ? (text ? JSON.parse(text) : {}) : text;
}

function isIPv4(value) {
  const parts = String(value || "").trim().split(".");
  return parts.length === 4 && parts.every(part => /^\d+$/.test(part) && Number(part) >= 0 && Number(part) <= 255);
}

function downloadJson(filename, data) {
  const blob = new Blob([JSON.stringify(data, null, 2)], { type: "application/json" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = filename;
  a.click();
  URL.revokeObjectURL(url);
}

function readCache(key, fallback) {
  try {
    const raw = localStorage.getItem(key);
    return raw ? JSON.parse(raw) : fallback;
  } catch {
    return fallback;
  }
}

function writeCache(key, value) {
  try {
    localStorage.setItem(key, JSON.stringify(value));
  } catch {}
}

function clearDataCaches() {
  Object.values(cacheKeys).forEach(key => {
    try {
      localStorage.removeItem(key);
    } catch {}
  });
}

function getActiveTab() {
  return localStorage.getItem("ir-bridge-tab") || "dashboard";
}

function applyTranslations() {
  document.documentElement.lang = state.language;
  document.title = t("heroTitle");
  document.querySelectorAll("[data-i18n]").forEach(node => {
    if (!node.dataset.i18nTitle) {
      node.textContent = t(node.dataset.i18n);
    }
  });
  document.querySelectorAll("[data-i18n-placeholder]").forEach(node => {
    node.placeholder = t(node.dataset.i18nPlaceholder);
  });
  document.querySelectorAll("[data-i18n-title]").forEach(node => {
    const value = t(node.dataset.i18nTitle);
    node.title = value;
    node.setAttribute("aria-label", value);
  });
  $("netMode").options[0].text = t("dhcpMode");
  $("netMode").options[1].text = t("staticMode");
  $("startupType").options[0].text = t("startupNone");
  $("startupType").options[1].text = t("startupCommand");
  $("startupType").options[2].text = t("startupSequence");
  $("modalCancelBtn").textContent = t("cancel");
  $("modalConfirmBtn").textContent = t("confirm");
  $("resolveOutput").textContent = t("noTestYet");
  renderStartupTargets();
  renderSequenceDraft();
  renderCommands();
  renderMappings();
  renderSequences();
}

function applyTheme(theme) {
  state.theme = theme === "dark" ? "dark" : "light";
  document.documentElement.dataset.theme = state.theme;
  localStorage.setItem("ir-bridge-theme", state.theme);
  document.querySelectorAll(".theme-btn").forEach(button => {
    button.classList.toggle("active", button.dataset.theme === state.theme);
  });
}

function applyActiveTab(tab) {
  const target = tab || getActiveTab();
  document.querySelectorAll(".tab").forEach(item => item.classList.toggle("active", item.dataset.tab === target));
  document.querySelectorAll(".tab-panel").forEach(panel => {
    panel.classList.toggle("active", panel.dataset.panel === target);
  });
  localStorage.setItem("ir-bridge-tab", target);
}

function hydrateCachedUiState() {
  const cached = readCache(cacheKeys.uiState, null);
  if (!cached) return;

  state.version = cached.version || state.version;
  state.statusSnapshot = cached.statusSnapshot || state.statusSnapshot;
  state.systemDevice = cached.systemDevice || state.systemDevice;
  state.networkMode = cached.networkMode || state.networkMode;
  state.startup = cached.startup || state.startup;
  state.counts = cached.counts || state.counts;

  if (cached.language) {
    state.language = cached.language;
    localStorage.setItem("ir-bridge-language", state.language);
  }

  $("languageSelect").value = state.language;
  $("startupType").value = state.startup.type || "none";
  $("startupDelay").value = String(state.startup.delay_ms || 1200);
  $("netMode").value = cached.networkMode || "dhcp";
  $("netIP").value = cached.network?.ip || "";
  $("netGateway").value = cached.network?.gateway || "";
  $("netSubnet").value = cached.network?.subnet || "";
  toggleNetworkFields();

  renderStatus({
    ...state.statusSnapshot,
    counts: state.counts
  });
}

function hydrateCachedCommands() {
  const cached = readCache(cacheKeys.commands, null);
  if (!cached || !cached.commands) return;
  state.commands = cached.commands;
  state.counts.commands = Object.keys(state.commands).length;
  renderCommands();
}

function hydrateCachedMappings() {
  const cached = readCache(cacheKeys.mappings, null);
  if (!cached || !cached.mappings) return;
  state.mappings = cached.mappings;
  state.counts.mappings = Object.keys(state.mappings).length;
  renderMappings();
}

function hydrateCachedSequences() {
  const cached = readCache(cacheKeys.sequences, null);
  if (!cached || !cached.sequences) return;
  state.sequences = cached.sequences;
  state.counts.sequences = Object.keys(state.sequences).length;
  renderSequences();
}

function openModal({ title, message, confirmLabel, cancelLabel, mode = "confirm", value = "", placeholder = "" }) {
  $("modalTitle").textContent = title;
  $("modalMessage").textContent = message;
  $("modalCancelBtn").textContent = cancelLabel || t("cancel");
  $("modalConfirmBtn").textContent = confirmLabel || t("confirm");
  $("modalInput").hidden = mode !== "prompt";
  $("modalInput").value = value;
  $("modalInput").placeholder = placeholder;
  $("modalOverlay").hidden = false;

  if (mode === "prompt") {
    $("modalInput").focus();
    $("modalInput").select();
  } else {
    $("modalConfirmBtn").focus();
  }

  return new Promise(resolve => {
    state.modalResolver = resolve;
  });
}

function closeModal(result) {
  $("modalOverlay").hidden = true;
  if (state.modalResolver) {
    state.modalResolver(result);
    state.modalResolver = null;
  }
}

function modalConfirm(options) {
  return openModal({ ...options, mode: "confirm" });
}

function modalPrompt(options) {
  return openModal({ ...options, mode: "prompt" });
}

function renderStatus(data) {
  const bridge = data.bridge || {};
  state.statusSnapshot = data;
  state.version = data.version || state.version;
  if (data.counts) {
    state.counts.commands = Number(data.counts.commands || 0);
    state.counts.mappings = Number(data.counts.mappings || 0);
    state.counts.sequences = Number(data.counts.sequences || 0);
  }
  $("versionBadge").textContent = `v${state.version || "?"}`;

  const connectionRows = [
    [t("ip"), data.ip || "-"],
    [t("wifi"), state.systemDevice.ssid || "-"],
    [t("networkMode"), state.networkMode === "static" ? t("staticMode") : t("dhcpMode")]
  ];

  const activityRows = [
    [t("commandsLabel"), state.counts.commands],
    [t("mappingsLabel"), state.counts.mappings],
    [t("sequencesLabel"), state.counts.sequences],
    [t("startupLabel"), describeStartup(bridge)],
    [t("lastTarget"), bridge.lastMappedTarget || "-"],
    [t("lastSequence"), bridge.lastSequenceName || "-"]
  ];

  const memoryRows = [
    [t("heap"), `${data.heap || 0} B`],
    [t("maxBlock"), `${data.maxBlock || 0} B`],
    [t("fragment"), `${data.fragmentation || 0}%`]
  ];

  $("summaryConnection").innerHTML = renderSummaryRows(connectionRows);
  $("summaryActivity").innerHTML = renderSummaryRows(activityRows);
  $("summaryMemory").innerHTML = renderSummaryRows(memoryRows);

  $("recordState").textContent = bridge.recording
    ? t("waitingForSignal", { name: bridge.recordName || "", seconds: Math.ceil((bridge.recordRemainingMs || 0) / 1000) })
    : t("idleLastReceived", { value: bridge.lastReceivedName || bridge.lastReceivedSignature || t("none") });
  renderSystemWindow();
}

function commandMeta(command) {
  const items = [
    t("protocol", { value: command.protocol }),
    t("bits", { value: command.bits }),
    t("repeats", { value: command.repeats || 1 }),
    command.hasRaw ? t("rawSamples", { value: Number(command.rawLength || (Array.isArray(command.raw) ? command.raw.length : 0)) }) : t("decoded")
  ];
  return items.map(item => `<span class="chip">${item}</span>`).join("");
}

function commandEditorPayload(name) {
  const payload = state.commandDetails[name] || state.commands[name] || {};
  return escapeHtml(JSON.stringify(payload, null, 2));
}

function renderSummaryRows(rows) {
  return rows.map(([label, value]) => `
    <div class="summary-row">
      <span>${escapeHtml(label)}</span>
      <strong>${escapeHtml(value)}</strong>
    </div>
  `).join("");
}

function describeStartup(bridge = {}) {
  if (!state.startup.enabled || state.startup.type === "none") {
    return t("startupDisabled");
  }

  if (bridge.startupPending) {
    return `${startupSummaryText()} (${t("startupPending")})`;
  }

  if (bridge.lastStartupAction) {
    return bridge.lastStartupAction;
  }

  return startupSummaryText();
}

function startupSummaryText() {
  if (state.startup.type === "command") {
    return t("startupCommandSummary", {
      target: state.startup.target || "-",
      delay: state.startup.delay_ms || 0
    });
  }

  if (state.startup.type === "sequence") {
    return t("startupSequenceSummary", {
      target: state.startup.target || "-",
      delay: state.startup.delay_ms || 0
    });
  }

  return t("startupDisabled");
}

function renderStartupTargets() {
  if (!$("startupTarget")) return;

  const type = $("startupType").value || state.startup.type || "none";
  const select = $("startupTarget");
  const previous = state.startup.target || "";
  const items = type === "sequence"
    ? Object.keys(state.sequences).sort((a, b) => a.localeCompare(b))
    : Object.keys(state.commands).sort((a, b) => a.localeCompare(b));

  select.innerHTML = "";
  if (type === "none") {
    select.disabled = true;
    select.add(new Option(t("startupDisabled"), ""));
    select.value = "";
    return;
  }

  select.disabled = false;
  for (const item of items) {
    select.add(new Option(item, item));
  }

  if (items.includes(previous)) {
    select.value = previous;
  } else if (items.length) {
    select.value = items[0];
    state.startup.target = items[0];
  } else {
    select.value = "";
  }
}

function renderSystemWindow() {
  if (!$("systemWindow")) return;
  $("systemWindow").textContent = JSON.stringify({
    status: state.statusSnapshot,
    device: state.systemDevice
  }, null, 2);
}

function renderSequenceDraft() {
  if (!$("sequenceDraft")) return;
  if (!state.sequenceDraft.length) {
    $("sequenceDraft").textContent = t("noStepsSelected");
    return;
  }

  $("sequenceDraft").innerHTML = `
    <div class="chip-row">
      ${state.sequenceDraft.map(step => `<span class="chip">${escapeHtml(step)}</span>`).join("")}
    </div>
  `;
}

function updateMappingFormState() {
  if (!$("mapBtn") || !$("mapSource") || !$("mapTarget")) return;

  const source = $("mapSource").value;
  const target = $("mapTarget").value;
  const valid = !!source &&
    !!target &&
    source !== target &&
    !!state.commands[source] &&
    !!state.commands[target];

  $("mapBtn").disabled = !valid;
}

function renderCommands() {
  if (!$("commandsList")) return;

  const names = Object.keys(state.commands).sort((a, b) => a.localeCompare(b));
  state.counts.commands = names.length;
  $("commandCount").textContent = String(names.length);
  $("resolveSource").innerHTML = "";
  $("mapSource").innerHTML = "";
  $("mapTarget").innerHTML = "";
  $("sequenceStep").innerHTML = "";
  $("sequenceTarget").innerHTML = "";

  if (!names.length) {
    $("commandsList").innerHTML = `<div class="command-card empty-card">${escapeHtml(t("noCommandsStored"))}</div>`;
    updateMappingFormState();
    renderSequenceDraft();
    return;
  }

  for (const name of names) {
    $("resolveSource").add(new Option(name, name));
    $("mapSource").add(new Option(name, name));
    $("mapTarget").add(new Option(name, name));
    $("sequenceStep").add(new Option(name, name));
    $("sequenceTarget").add(new Option(name, name));
  }

  if (names.length > 1 && $("mapSource").value === $("mapTarget").value) {
    $("mapTarget").value = names[1];
  }

  renderStartupTargets();
  updateMappingFormState();

  $("commandsList").innerHTML = names.map(name => {
    const command = state.commands[name];
    const encoded = encodeURIComponent(name);
    return `
      <article class="command-card">
        <div class="command-head">
          <div>
            <h3>${escapeHtml(name)}</h3>
            <div class="meta">${commandMeta(command)}</div>
            <div class="subtle command-signature">${escapeHtml(command.signature || "-")}</div>
          </div>
          <div class="command-actions">
            <button class="primary icon-only" data-action="execute" data-name="${encoded}" title="${escapeHtml(t("send"))}" aria-label="${escapeHtml(t("send"))}"><span class="icon-svg icon-play" aria-hidden="true"></span></button>
            <button class="ghost icon-only" data-action="rename" data-name="${encoded}" title="${escapeHtml(t("rename"))}" aria-label="${escapeHtml(t("rename"))}"><span class="icon-svg icon-pencil-right" aria-hidden="true"></span></button>
            <button class="ghost icon-only" data-action="toggle" data-name="${encoded}" title="${escapeHtml(t("edit"))}" aria-label="${escapeHtml(t("edit"))}"><span class="icon-svg icon-menu" aria-hidden="true"></span></button>
            <button class="danger icon-only" data-action="delete" data-name="${encoded}" title="${escapeHtml(t("delete"))}" aria-label="${escapeHtml(t("delete"))}"><span class="icon-svg icon-close" aria-hidden="true"></span></button>
          </div>
        </div>
        <div id="editor-${encoded}" hidden>
          <textarea id="json-${encoded}" aria-label="${escapeHtml(t("commandJson"))}">${commandEditorPayload(name)}</textarea>
          <div class="row wrap">
            <button class="success icon-only" data-action="save" data-name="${encoded}" title="${escapeHtml(t("saveJson"))}" aria-label="${escapeHtml(t("saveJson"))}"><span class="icon-svg icon-check" aria-hidden="true"></span></button>
          </div>
        </div>
      </article>
    `;
  }).join("");

  Object.keys(state.commandDetails).forEach(name => {
    if (!state.commands[name]) {
      delete state.commandDetails[name];
    }
  });

  state.sequenceDraft = state.sequenceDraft.filter(step => !!state.commands[step]);
  renderSequenceDraft();
}

function renderMappings() {
  if (!$("mappingsList")) return;

  const entries = Object.entries(state.mappings).sort((a, b) => a[0].localeCompare(b[0]));
  state.counts.mappings = entries.length;
  $("mappingCount").textContent = String(entries.length);

  if (!entries.length) {
    $("mappingsList").innerHTML = `<div class="mapping-card empty-card">${escapeHtml(t("noMappingsStored"))}</div>`;
    return;
  }

  $("mappingsList").innerHTML = entries.map(([source, target]) => `
    <article class="mapping-card">
      <div class="flow-row">
        <div class="route-node">
          <span class="route-label">${escapeHtml(t("from"))}</span>
          <strong class="route-value" ${routeValueStyle(source)}>${escapeHtml(source)}</strong>
        </div>
        <div class="route-arrow" aria-hidden="true"><span class="icon-svg icon-arrow-right"></span></div>
        <div class="target-node">
          <span class="route-label">${escapeHtml(t("to"))}</span>
          <strong class="route-value" ${routeValueStyle(target)}>${escapeHtml(target)}</strong>
        </div>
        <div class="mapping-actions">
          <button class="ghost icon-only" data-action="inspect-map" data-name="${encodeURIComponent(source)}" title="${escapeHtml(t("inspect"))}" aria-label="${escapeHtml(t("inspect"))}"><span class="icon-svg icon-play" aria-hidden="true"></span></button>
          <button class="danger icon-only" data-action="delete-map" data-name="${encodeURIComponent(source)}" title="${escapeHtml(t("delete"))}" aria-label="${escapeHtml(t("delete"))}"><span class="icon-svg icon-close" aria-hidden="true"></span></button>
        </div>
      </div>
    </article>
  `).join("");
}

function renderSequences() {
  if (!$("sequencesList")) return;

  const names = Object.keys(state.sequences).sort((a, b) => a.localeCompare(b));
  state.counts.sequences = names.length;
  $("sequenceCount").textContent = String(names.length);

  if (!names.length) {
    $("sequencesList").innerHTML = `<div class="sequence-card empty-card">${escapeHtml(t("noSequencesStored"))}</div>`;
    return;
  }

  $("sequencesList").innerHTML = names.map(name => {
    const sequence = state.sequences[name];
    const steps = Array.isArray(sequence.steps) ? sequence.steps : [];
    return `
      <article class="sequence-card">
        <div class="flow-row">
          <div class="route-node">
            <span class="route-label">${escapeHtml(name)}</span>
            <div class="sequence-flow-steps">${steps.map(step => `<span class="chip">${escapeHtml(step)}</span>`).join("")}</div>
          </div>
          <div class="route-arrow" aria-hidden="true"><span class="icon-svg icon-arrow-right"></span></div>
          <div class="target-node">
            <span class="route-label">${escapeHtml(t("targetCommand"))}</span>
            <strong class="route-value" ${routeValueStyle(sequence.target || "-", true)}>${escapeHtml(sequence.target || "-")}</strong>
            <span class="route-label">${escapeHtml(t("timeoutMs", { value: sequence.timeout_ms || 800 }))}</span>
          </div>
          <div class="sequence-actions">
            <button class="danger icon-only" data-action="delete-sequence" data-name="${encodeURIComponent(name)}" title="${escapeHtml(t("delete"))}" aria-label="${escapeHtml(t("delete"))}"><span class="icon-svg icon-close" aria-hidden="true"></span></button>
          </div>
        </div>
      </article>
    `;
  }).join("");
}

function syncStartupTargetSelection() {
  if ($("startupTarget").options.length && state.startup.target) {
    $("startupTarget").value = state.startup.target;
  }
}

function invalidateData(...keys) {
  keys.forEach(key => {
    if (state.loaded[key] !== undefined) {
      state.loaded[key] = false;
    }
  });
}

async function loadUiState() {
  const data = await api("/api/ui-state", {}, "json");

  state.version = data.version || state.version;
  state.statusSnapshot = {
    version: data.version,
    ip: data.ip,
    heap: data.heap,
    maxBlock: data.maxBlock,
    fragmentation: data.fragmentation,
    uptimeMs: data.uptimeMs,
    counts: data.counts || state.counts,
    bridge: data.bridge || {}
  };
  state.counts = data.counts || state.counts;
  state.systemDevice = data.device || {};
  state.networkMode = data.network?.mode || "dhcp";

  if (data.settings) {
    state.language = data.settings.language || state.language;
    state.startup = data.settings.startup || state.startup;
    localStorage.setItem("ir-bridge-language", state.language);
  }

  $("languageSelect").value = state.language;
  $("startupType").value = state.startup.type || "none";
  $("startupDelay").value = String(state.startup.delay_ms || 1200);
  $("netMode").value = data.network?.mode || "dhcp";
  $("netIP").value = data.network?.ip || "";
  $("netGateway").value = data.network?.gateway || "";
  $("netSubnet").value = data.network?.subnet || "";

  toggleNetworkFields();
  writeCache(cacheKeys.uiState, {
    version: state.version,
    statusSnapshot: state.statusSnapshot,
    systemDevice: state.systemDevice,
    networkMode: state.networkMode,
    network: data.network || {},
    startup: state.startup,
    language: state.language,
    counts: state.counts
  });

  state.loaded.ui = true;
  applyTranslations();
  renderSystemWindow();
  renderStatus(state.statusSnapshot);
}

async function loadEssentialStateFallback() {
  const [settingsResult, networkResult, systemResult, statusResult] = await Promise.allSettled([
    loadSettings(),
    loadNetwork(),
    loadSystem(),
    loadStatus()
  ]);

  const firstError = [settingsResult, networkResult, systemResult, statusResult]
    .find(result => result.status === "rejected");
  if (firstError) {
    throw firstError.reason;
  }

  writeCache(cacheKeys.uiState, {
    version: state.version,
    statusSnapshot: state.statusSnapshot,
    systemDevice: state.systemDevice,
    networkMode: state.networkMode,
    network: {
      mode: $("netMode").value,
      ip: $("netIP").value,
      gateway: $("netGateway").value,
      subnet: $("netSubnet").value
    },
    startup: state.startup,
    language: state.language,
    counts: state.counts
  });
  state.loaded.ui = true;
}

async function loadCommandSummaries() {
  const data = await api("/api/commands", {}, "json");
  state.commands = data.commands || {};
  state.counts.commands = Object.keys(state.commands).length;
  writeCache(cacheKeys.commands, { commands: state.commands });
  state.loaded.commands = true;
  renderCommands();
}

async function loadCommandDetail(name, force = false) {
  if (!force && state.commandDetails[name]) {
    return state.commandDetails[name];
  }

  const data = await api(`/api/command?name=${encodeURIComponent(name)}`, {}, "json");
  state.commandDetails[name] = data.command || {};

  const editor = $(`json-${encodeURIComponent(name)}`);
  if (editor) {
    editor.value = JSON.stringify(state.commandDetails[name], null, 2);
  }

  return state.commandDetails[name];
}

async function loadMappings() {
  const data = await api("/api/mappings", {}, "json");
  state.mappings = data.mappings || {};
  state.counts.mappings = Object.keys(state.mappings).length;
  writeCache(cacheKeys.mappings, { mappings: state.mappings });
  state.loaded.mappings = true;
  renderMappings();
}

async function loadSequences() {
  const data = await api("/api/sequences", {}, "json");
  state.sequences = data.sequences || {};
  state.counts.sequences = Object.keys(state.sequences).length;
  writeCache(cacheKeys.sequences, { sequences: state.sequences });
  state.loaded.sequences = true;
  renderStartupTargets();
  renderSequences();
}

async function loadNetwork() {
  const data = await api("/api/network", {}, "json");
  const network = data.network || {};
  state.networkMode = network.mode || "dhcp";
  $("netMode").value = network.mode || "dhcp";
  $("netIP").value = network.ip || "";
  $("netGateway").value = network.gateway || "";
  $("netSubnet").value = network.subnet || "";
  toggleNetworkFields();
}

async function loadSystem() {
  const data = await api("/api/system", {}, "json");
  state.systemDevice = data.device || {};
  renderSystemWindow();
}

async function loadSettings() {
  const data = await api("/api/settings", {}, "json");
  const settings = data.settings || {};
  state.language = settings.language || "en";
  state.startup = settings.startup || state.startup;
  localStorage.setItem("ir-bridge-language", state.language);
  $("languageSelect").value = state.language;
  $("startupType").value = state.startup.type || "none";
  $("startupDelay").value = String(state.startup.delay_ms || 1200);
  renderStartupTargets();
  syncStartupTargetSelection();
  applyTranslations();
}

async function loadStatus() {
  const data = await api("/api/status", {}, "json");
  renderStatus(data);
  writeCache(cacheKeys.uiState, {
    version: state.version,
    statusSnapshot: state.statusSnapshot,
    systemDevice: state.systemDevice,
    networkMode: state.networkMode,
    network: {
      mode: $("netMode").value,
      ip: $("netIP").value,
      gateway: $("netGateway").value,
      subnet: $("netSubnet").value
    },
    startup: state.startup,
    language: state.language,
    counts: state.counts
  });

  const isRecording = !!(data.bridge && data.bridge.recording);
  if (state.wasRecording && !isRecording && !state.refreshInFlight) {
    state.refreshInFlight = true;
    try {
      state.commandDetails = {};
      invalidateData("commands");
      await ensureCommandsData(true);
      if (getActiveTab() === "dashboard") {
        await loadLogs(true);
      }
    } finally {
      state.refreshInFlight = false;
    }
  }
  state.wasRecording = isRecording;
}

async function loadLogs(force = false) {
  if (!force && state.loaded.logs) {
    return;
  }
  const text = await api("/api/logs");
  $("logWindow").textContent = text || "(empty)";
  state.loaded.logs = true;
}

async function ensureCommandsData(force = false) {
  if (force) {
    state.loaded.commands = false;
  }
  if (!state.loaded.commands) {
    await loadCommandSummaries();
  }
}

async function ensureMappingsData(force = false) {
  if (force) {
    state.loaded.commands = false;
    state.loaded.mappings = false;
  }

  const tasks = [];
  if (!state.loaded.commands) {
    tasks.push(loadCommandSummaries());
  }
  if (!state.loaded.mappings) {
    tasks.push(loadMappings());
  }

  if (!tasks.length) {
    updateMappingFormState();
    return;
  }

  const results = await Promise.allSettled(tasks);
  if (results.every(result => result.status === "rejected")) {
    throw results[0].reason;
  }

  updateMappingFormState();
}

async function ensureSequencesData(force = false) {
  if (force) {
    state.loaded.commands = false;
    state.loaded.sequences = false;
  }

  const tasks = [];
  if (!state.loaded.commands) {
    tasks.push(loadCommandSummaries());
  }
  if (!state.loaded.sequences) {
    tasks.push(loadSequences());
  }

  if (!tasks.length) {
    return;
  }

  const results = await Promise.allSettled(tasks);
  if (results.every(result => result.status === "rejected")) {
    throw results[0].reason;
  }
}

async function ensureSettingsData(force = false) {
  if (force) {
    state.loaded.commands = false;
    state.loaded.sequences = false;
  }
  await ensureCommandsData(false);
  if (!state.loaded.sequences) {
    await loadSequences();
  }
  syncStartupTargetSelection();
}

async function ensureDashboardData(force = false) {
  if (force) {
    state.loaded.logs = false;
  }
  if (!state.loaded.logs) {
    setTimeout(() => {
      loadLogs(force).catch(() => {});
    }, 120);
  }
}

async function refreshVisibleData(force = false) {
  let firstError = null;
  try {
    await loadUiState();
  } catch (error) {
    try {
      await loadEssentialStateFallback();
    } catch (fallbackError) {
      firstError = fallbackError || error;
    }
  }

  const activeTab = getActiveTab();
  try {
    if (activeTab === "commands") {
      await ensureCommandsData(force);
    } else if (activeTab === "mappings") {
      await ensureMappingsData(force);
    } else if (activeTab === "sequences") {
      await ensureSequencesData(force);
    } else if (activeTab === "settings") {
      await ensureSettingsData(force);
    } else {
      await ensureDashboardData(force);
    }
  } catch (error) {
    firstError = firstError || error;
  }

  if (firstError) {
    throw firstError;
  }
}

function scheduleStatusPoll(delay = document.hidden ? 5000 : 2500) {
  clearTimeout(state.statusTimer);
  state.statusTimer = setTimeout(async () => {
    if (state.statusPolling) {
      scheduleStatusPoll();
      return;
    }

    state.statusPolling = true;
    try {
      await loadStatus();
    } catch {}
    state.statusPolling = false;
    scheduleStatusPoll();
  }, delay);
}

function toggleNetworkFields() {
  const disabled = $("netMode").value === "dhcp";
  $("netIP").disabled = disabled;
  $("netGateway").disabled = disabled;
  $("netSubnet").disabled = disabled;
}

function handleNetworkReboot(mode, ip) {
  clearTimeout(state.statusTimer);
  clearTimeout(state.logsTimer);

  if (mode === "static" && isIPv4(ip)) {
    showToast(t("networkStaticRedirect", { ip }));
    setTimeout(() => {
      window.location.assign(`http://${ip}/`);
    }, 3200);
    return;
  }

  showToast(t("networkDhcpRedirect"));
}

async function saveCommand(name, button) {
  try {
    setBusy(button, true);
    const payload = JSON.parse($(`json-${encodeURIComponent(name)}`).value);
    const body = {
      name,
      protocol: String(payload.protocol ?? -1),
      value: String(payload.value ?? 0),
      bits: String(payload.bits ?? 0),
      frequency: String(payload.frequency ?? 38),
      repeats: String(payload.repeats ?? 1)
    };
    if (Array.isArray(payload.raw)) body.raw = JSON.stringify(payload.raw);
    const message = await api("/api/update", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody(body)
    });
    state.commandDetails[name] = payload;
    invalidateData("commands");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function executeCommand(name, button) {
  try {
    setBusy(button, true);
    const message = await api("/api/execute", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ name })
    });
    showToast(message);
  } finally {
    setBusy(button, false);
  }
}

async function deleteCommand(name, button) {
  const confirmed = await modalConfirm({
    title: t("deleteCommandTitle"),
    message: t("deleteCommandMessage", { name })
  });
  if (!confirmed) return;

  try {
    setBusy(button, true);
    const message = await api("/api/delete", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ name })
    });
    delete state.commandDetails[name];
    invalidateData("commands", "mappings", "sequences", "logs");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function renameCommand(name, button) {
  const nextName = await modalPrompt({
    title: t("renameTitle"),
    message: t("renameMessage", { name }),
    confirmLabel: t("save"),
    value: name,
    placeholder: t("renamePlaceholder")
  });
  if (!nextName || nextName.trim() === "" || nextName.trim() === name) return;

  try {
    setBusy(button, true);
    const message = await api("/api/rename", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ old_name: name, new_name: nextName.trim() })
    });
    state.commandDetails = {};
    invalidateData("commands", "mappings", "sequences", "logs");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function createMapping(button) {
  const source = $("mapSource").value;
  const target = $("mapTarget").value;

  if (!source) throw new Error(t("noSourceSelected"));
  if (!target) throw new Error(t("mappingTargetRequired"));
  if (source === target) throw new Error(t("mappingDifferentCommands"));
  if (!state.commands[source] || !state.commands[target]) {
    throw new Error(t("mappingCommandsUnavailable"));
  }

  try {
    setBusy(button, true);
    const message = await api("/api/map", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ source, target })
    });
    invalidateData("mappings");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function deleteMapping(source, button) {
  try {
    setBusy(button, true);
    const message = await api(`/api/unmap?source=${encodeURIComponent(source)}`, { method: "POST" });
    invalidateData("mappings");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function saveSequence(button) {
  const name = $("sequenceName").value.trim();
  const target = $("sequenceTarget").value;
  const timeout = Number($("sequenceTimeout").value || 800);

  if (!name) throw new Error(t("sequenceNameRequired"));
  if (state.sequenceDraft.length < 2) throw new Error(t("addAtLeastTwoSteps"));
  if (!target) throw new Error(t("targetRequired"));
  if (!Number.isFinite(timeout) || timeout < 100 || timeout > 5000) throw new Error(t("timeoutRange"));

  try {
    setBusy(button, true);
    const message = await api("/api/sequence", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({
        name,
        target,
        timeout_ms: String(timeout),
        enabled: "true",
        steps: JSON.stringify(state.sequenceDraft)
      })
    });
    state.sequenceDraft = [];
    $("sequenceName").value = "";
    $("sequenceTimeout").value = "800";
    renderSequenceDraft();
    invalidateData("sequences");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function deleteSequence(name, button) {
  const confirmed = await modalConfirm({
    title: t("deleteSequenceTitle"),
    message: t("deleteSequenceMessage", { name })
  });
  if (!confirmed) return;

  try {
    setBusy(button, true);
    const message = await api("/api/delete-sequence", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ name })
    });
    invalidateData("sequences");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function inspectMapping(source, button) {
  try {
    setBusy(button, true);
    const data = await api(`/api/resolve?source=${encodeURIComponent(source)}`, {}, "json");
    $("resolveSource").value = source;
    $("resolveOutput").textContent = JSON.stringify(data, null, 2);
  } finally {
    setBusy(button, false);
  }
}

async function saveNetwork(button) {
  const mode = $("netMode").value;
  const ip = $("netIP").value.trim();
  const gateway = $("netGateway").value.trim();
  const subnet = $("netSubnet").value.trim();

  if (mode === "static" && (!isIPv4(ip) || !isIPv4(gateway) || !isIPv4(subnet))) {
    throw new Error(t("invalidStaticNetwork"));
  }

  try {
    setBusy(button, true);
    const message = await api("/api/network", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ mode, ip, gateway, subnet })
    });

    state.networkMode = mode;
    $("netMode").value = mode;
    $("netIP").value = ip;
    $("netGateway").value = gateway;
    $("netSubnet").value = subnet;
    toggleNetworkFields();

    showToast(message);
    handleNetworkReboot(mode, ip);
  } finally {
    setBusy(button, false);
  }
}

async function saveLanguage(button) {
  try {
    setBusy(button, true);
    const language = $("languageSelect").value;
    const message = await api("/api/settings", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ language })
    });
    state.language = language;
    localStorage.setItem("ir-bridge-language", language);
    applyTranslations();
    invalidateData("ui");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function saveStartup(button) {
  const type = $("startupType").value;
  const target = $("startupTarget").value;
  const delayMs = Number($("startupDelay").value || 1200);

  if ((type === "command" || type === "sequence") && !target) {
    throw new Error(t("startupTargetRequired"));
  }
  if (!Number.isFinite(delayMs) || delayMs < 0 || delayMs > 30000) {
    throw new Error(t("startupDelayPlaceholder"));
  }

  try {
    setBusy(button, true);
    const message = await api("/api/startup", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({
        enabled: type === "none" ? "false" : "true",
        type,
        target: type === "none" ? "" : target,
        delay_ms: String(delayMs)
      })
    });
    state.startup = {
      enabled: type !== "none",
      type,
      target: type === "none" ? "" : target,
      delay_ms: delayMs
    };
    renderStartupTargets();
    invalidateData("ui");
    showToast(message);
    await refreshVisibleData(true);
  } finally {
    setBusy(button, false);
  }
}

async function startRecording(button) {
  const name = $("recordName").value.trim();
  if (!name) throw new Error(t("commandNameRequired"));

  try {
    setBusy(button, true);
    const message = await api("/api/record", {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: formBody({ name })
    });
    $("recordName").value = "";
    showToast(message);
    await loadStatus();
  } finally {
    setBusy(button, false);
  }
}

async function exportConfig(button) {
  try {
    setBusy(button, true);
    const scope = $("exportScope").value || "all";
    const payload = {};

    if (scope === "all" || scope === "commands") {
      await ensureCommandsData(true);
      const commands = {};
      for (const name of Object.keys(state.commands).sort((a, b) => a.localeCompare(b))) {
        const detail = await loadCommandDetail(name, true);
        commands[name] = detail;
      }
      payload.commands = commands;
    }

    if (scope === "all" || scope === "mappings") {
      if (!state.loaded.mappings || scope !== "all") {
        await loadMappings();
      }
      payload.mappings = state.mappings;
    }

    if (scope === "all" || scope === "sequences") {
      if (!state.loaded.sequences || scope !== "all") {
        await loadSequences();
      }
      payload.sequences = state.sequences;
    }

    if (scope === "all") {
      payload.startup = state.startup;
    }

    const filenames = {
      all: "ir-bridge-backup-all.json",
      commands: "ir-bridge-commands.json",
      mappings: "ir-bridge-mappings.json",
      sequences: "ir-bridge-sequences.json"
    };

    downloadJson(filenames[scope] || "ir-bridge-backup.json", payload);
    showToast(t("exportJson"));
  } finally {
    setBusy(button, false);
  }
}

async function importConfig(file) {
  if (!file) return;
  const text = await file.text();
  JSON.parse(text);
  const message = await api("/api/import", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: text
  });
  clearDataCaches();
  state.commandDetails = {};
  invalidateData("ui", "commands", "mappings", "sequences", "logs");
  showToast(message);
  await refreshVisibleData(true);
  $("importFile").value = "";
}

async function resolveSelected(button) {
  const source = $("resolveSource").value;
  if (!source) throw new Error(t("noSourceSelected"));
  await inspectMapping(source, button);
}

async function requestFactoryReset() {
  const confirmed = await modalConfirm({
    title: t("factoryTitle"),
    message: t("factoryMessage")
  });
  if (!confirmed) return;
  clearDataCaches();
  const message = await api("/api/factory", { method: "POST" });
  showToast(message);
}

function scheduleAutoLogs(delay = 3000) {
  clearTimeout(state.logsTimer);
  if (!$("logsAuto").checked) {
    return;
  }

  state.logsTimer = setTimeout(async () => {
    try {
      await loadLogs(true);
    } catch {}
    scheduleAutoLogs();
  }, delay);
}

function bindEvents() {
  document.querySelectorAll(".tab").forEach(button => {
    button.addEventListener("click", () => {
      applyActiveTab(button.dataset.tab);
      refreshVisibleData(false).catch(err => showToast(err.message));
    });
  });

  $("refreshBtn").addEventListener("click", () => refreshVisibleData(true).catch(err => showToast(err.message)));
  $("recordBtn").addEventListener("click", event => startRecording(event.currentTarget).catch(err => showToast(err.message)));
  $("mapBtn").addEventListener("click", event => createMapping(event.currentTarget).catch(err => showToast(err.message)));
  $("mapSource").addEventListener("change", updateMappingFormState);
  $("mapTarget").addEventListener("change", updateMappingFormState);
  $("netMode").addEventListener("change", toggleNetworkFields);
  $("netSaveBtn").addEventListener("click", event => saveNetwork(event.currentTarget).catch(err => showToast(err.message)));
  $("languageSaveBtn").addEventListener("click", event => saveLanguage(event.currentTarget).catch(err => showToast(err.message)));
  $("startupSaveBtn").addEventListener("click", event => saveStartup(event.currentTarget).catch(err => showToast(err.message)));
  $("startupType").addEventListener("change", event => {
    state.startup.type = event.target.value;
    if (event.target.value === "none") {
      state.startup.target = "";
    }
    renderStartupTargets();
  });
  $("startupTarget").addEventListener("change", event => {
    state.startup.target = event.target.value;
  });
  $("logsRefreshBtn").addEventListener("click", () => loadLogs(true).catch(err => showToast(err.message)));
  $("resolveBtn").addEventListener("click", event => resolveSelected(event.currentTarget).catch(err => showToast(err.message)));
  $("exportBtn").addEventListener("click", event => exportConfig(event.currentTarget).catch(err => showToast(err.message)));
  $("importFile").addEventListener("change", event => importConfig(event.target.files[0]).catch(err => showToast(err.message)));
  $("sequenceAddStepBtn").addEventListener("click", () => {
    const step = $("sequenceStep").value;
    if (!step) return;
    if (state.sequenceDraft.length >= 8) {
      showToast(t("maxSteps"));
      return;
    }
    state.sequenceDraft.push(step);
    renderSequenceDraft();
  });
  $("sequenceClearBtn").addEventListener("click", () => {
    state.sequenceDraft = [];
    renderSequenceDraft();
  });
  $("sequenceSaveBtn").addEventListener("click", event => saveSequence(event.currentTarget).catch(err => showToast(err.message)));
  $("factoryBtn").addEventListener("click", () => requestFactoryReset().catch(err => showToast(err.message)));
  $("logsAuto").addEventListener("change", event => {
    if (event.target.checked) {
      scheduleAutoLogs();
    } else {
      clearTimeout(state.logsTimer);
    }
  });
  $("themeSwitch").addEventListener("click", event => {
    const button = event.target.closest(".theme-btn");
    if (!button) return;
    applyTheme(button.dataset.theme);
  });

  $("commandsList").addEventListener("click", async event => {
    const button = event.target.closest("button[data-action]");
    if (!button) return;

    const name = decodeURIComponent(button.dataset.name);
    const action = button.dataset.action;

    try {
      if (action === "toggle") {
        const editor = $(`editor-${encodeURIComponent(name)}`);
        editor.hidden = !editor.hidden;
        const title = editor.hidden ? t("edit") : t("hideEditor");
        button.title = title;
        button.setAttribute("aria-label", title);
        if (!editor.hidden) {
          const textarea = $(`json-${encodeURIComponent(name)}`);
          textarea.value = t("loadingCommandJson");
          await loadCommandDetail(name);
        }
        return;
      }
      if (action === "execute") return await executeCommand(name, button);
      if (action === "rename") return await renameCommand(name, button);
      if (action === "delete") return await deleteCommand(name, button);
      if (action === "save") return await saveCommand(name, button);
    } catch (err) {
      showToast(err.message);
    }
  });

  $("mappingsList").addEventListener("click", async event => {
    const button = event.target.closest("button[data-action]");
    if (!button) return;
    const source = decodeURIComponent(button.dataset.name);
    try {
      if (button.dataset.action === "delete-map") return await deleteMapping(source, button);
      if (button.dataset.action === "inspect-map") return await inspectMapping(source, button);
    } catch (err) {
      showToast(err.message);
    }
  });

  $("sequencesList").addEventListener("click", async event => {
    const button = event.target.closest("button[data-action]");
    if (!button) return;
    const name = decodeURIComponent(button.dataset.name);
    try {
      if (button.dataset.action === "delete-sequence") return await deleteSequence(name, button);
    } catch (err) {
      showToast(err.message);
    }
  });

  $("modalOverlay").addEventListener("click", event => {
    if (event.target === $("modalOverlay")) closeModal(false);
  });
  $("modalCloseBtn").addEventListener("click", () => closeModal(false));
  $("modalCancelBtn").addEventListener("click", () => closeModal(false));
  $("modalConfirmBtn").addEventListener("click", () => {
    closeModal($("modalInput").hidden ? true : $("modalInput").value);
  });
  $("modalInput").addEventListener("keydown", event => {
    if (event.key === "Enter") closeModal($("modalInput").value);
  });
  document.addEventListener("keydown", event => {
    if (event.key === "Escape" && !$("modalOverlay").hidden) {
      closeModal(false);
    }
  });
  document.addEventListener("visibilitychange", () => {
    scheduleStatusPoll(200);
  });
}

async function init() {
  bindEvents();
  applyTheme(localStorage.getItem("ir-bridge-theme") || "light");
  hydrateCachedUiState();
  hydrateCachedCommands();
  hydrateCachedMappings();
  hydrateCachedSequences();
  applyTranslations();
  applyActiveTab();
  await refreshVisibleData(false);
  scheduleStatusPoll(1200);
}

init().catch(err => {
  showToast(err.message);
});
