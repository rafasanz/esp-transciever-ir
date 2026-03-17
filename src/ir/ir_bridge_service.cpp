/*
 * Module: IR bridge runtime
 * Responsibilities:
 * - process incoming IR frames and dispatch mapped or sequenced actions
 * - manage recording windows, startup actions, and runtime status snapshots
 */

#include "ir_bridge_service.h"
#include "system/logger.h"
#include "system/i18n.h"

#ifndef IR_RECORD_TIMEOUT_MS
#define IR_RECORD_TIMEOUT_MS 15000UL
#endif

#ifndef IR_RETRIGGER_GUARD_MS
#define IR_RETRIGGER_GUARD_MS 350UL
#endif

#ifndef IR_STARTUP_STEP_GAP_MS
#define IR_STARTUP_STEP_GAP_MS 220UL
#endif

IRBridgeService::IRBridgeService(CommandManager& commandManager, IRReceiver& receiver, IRSender& sender)
    : _commandManager(commandManager), _receiver(receiver), _sender(sender) {}

void IRBridgeService::begin() {
    _recordMode = false;
    _recordName = "";
    _lastTriggerAt = 0;
    _lastTriggerSignature = "";
    clearPendingSequence();
    clearStartupPlayback();
    _startupActionTriggered = false;
    _lastStartupAction = "";
    refreshStartupAction();
}

bool IRBridgeService::startRecording(const String& name) {
    String cleanName = name;
    cleanName.trim();
    if (cleanName.length() == 0) {
        return false;
    }

    _recordMode = true;
    _recordName = cleanName;
    _recordStartedAt = millis();
    Logger::log(String(I18n::pick("Recording started: ", "Grabación iniciada: ", "Rozpoczeto nagrywanie: ")) + _recordName);
    return true;
}

bool IRBridgeService::isRecording() const {
    return _recordMode;
}

String IRBridgeService::getRecordingName() const {
    return _recordName;
}

uint32_t IRBridgeService::getRecordingRemainingMs() const {
    if (!_recordMode) {
        return 0;
    }

    const uint32_t elapsed = millis() - _recordStartedAt;
    return (elapsed >= IR_RECORD_TIMEOUT_MS) ? 0 : (IR_RECORD_TIMEOUT_MS - elapsed);
}

bool IRBridgeService::isRecordingExpired() const {
    return _recordMode && (millis() - _recordStartedAt >= IR_RECORD_TIMEOUT_MS);
}

bool IRBridgeService::shouldIgnoreTrigger(const String& signature, uint32_t now) const {
    return signature.length() > 0 &&
           signature == _lastTriggerSignature &&
           now - _lastTriggerAt < IR_RETRIGGER_GUARD_MS;
}

void IRBridgeService::finishRecording() {
    _recordMode = false;
    _recordName = "";
    _recordStartedAt = 0;
}

void IRBridgeService::clearPendingSequence() {
    _pendingStepCount = 0;
    _pendingDeadlineAt = 0;
    _pendingFallbackReady = false;
    _pendingFallbackSource = "";
    _pendingExactReady = false;
    _pendingExactTarget = "";
    _pendingExactSequenceName = "";
    for (uint8_t i = 0; i < IR_SEQUENCE_MAX_STEPS; ++i) {
        _pendingSteps[i] = "";
    }
}

void IRBridgeService::clearStartupPlayback() {
    _startupPendingSequenceCount = 0;
    _startupPendingSequenceIndex = 0;
    _startupPendingSequenceNextAt = 0;
    for (uint8_t i = 0; i < IR_SEQUENCE_MAX_STEPS; ++i) {
        _startupPendingSequence[i] = "";
    }
}

void IRBridgeService::flushPendingSequence(uint32_t now) {
    if (_pendingStepCount == 0) {
        return;
    }

    if (_pendingExactReady && _pendingExactTarget.length() > 0) {
        if (executeCommand(_pendingExactTarget)) {
            _lastMappedTarget = _pendingExactTarget;
            _lastSequenceName = _pendingExactSequenceName;
            Logger::log(String(I18n::pick("Sequence executed: ", "Secuencia ejecutada: ", "Wykonano sekwencje: ")) + _pendingExactSequenceName + " -> " + _pendingExactTarget);
        }
    } else if (_pendingFallbackReady && _pendingFallbackSource.length() > 0) {
        IRCommandData targetCommand;
        String targetName;
        if (_commandManager.resolveMappedCommand(_pendingFallbackSource, targetCommand, targetName)) {
            _sender.send(targetCommand);
            _lastMappedTarget = targetName;
            _lastSequenceName = "";
            Logger::log(String(I18n::pick("Deferred mapping executed: ", "Mapeo diferido ejecutado: ", "Wykonano opoznione mapowanie: ")) + _pendingFallbackSource + " -> " + targetName);
        }
    }

    _lastTriggerAt = now;
    clearPendingSequence();
}

bool IRBridgeService::processSequenceAwareCommand(const String& commandName, uint32_t now) {
    if (_pendingStepCount > 0 && now > _pendingDeadlineAt) {
        flushPendingSequence(now);
    }

    if (_pendingStepCount > 0) {
        String candidateSteps[IR_SEQUENCE_MAX_STEPS];
        for (uint8_t i = 0; i < _pendingStepCount; ++i) {
            candidateSteps[i] = _pendingSteps[i];
        }
        if (_pendingStepCount < IR_SEQUENCE_MAX_STEPS) {
            candidateSteps[_pendingStepCount] = commandName;
            SequenceMatchResult extended;
            if (_commandManager.resolveSequence(candidateSteps, _pendingStepCount + 1, extended)) {
                _pendingSteps[_pendingStepCount] = commandName;
                ++_pendingStepCount;
                _pendingDeadlineAt = now + extended.timeoutMs;
                _pendingExactReady = extended.hasExact;
                _pendingExactTarget = extended.targetName;
                _pendingExactSequenceName = extended.sequenceName;
                _lastSequenceName = extended.hasExact ? extended.sequenceName : "";

                if (extended.hasExact && !extended.hasPrefix) {
                    flushPendingSequence(now);
                }
                return true;
            }
        }

        flushPendingSequence(now);
    }

    SequenceMatchResult single;
    const String singleStep[1] = {commandName};
    if (_commandManager.resolveSequence(singleStep, 1, single)) {
        _pendingSteps[0] = commandName;
        _pendingStepCount = 1;
        _pendingDeadlineAt = now + single.timeoutMs;
        _pendingExactReady = single.hasExact;
        _pendingExactTarget = single.targetName;
        _pendingExactSequenceName = single.sequenceName;
        _pendingFallbackReady = _commandManager.resolveMapping(commandName, _pendingFallbackSource);
        _pendingFallbackSource = commandName;

        if (single.hasExact && !single.hasPrefix) {
            flushPendingSequence(now);
        }
        return true;
    }

    IRCommandData targetCommand;
    String targetName;
    if (_commandManager.resolveMappedCommand(commandName, targetCommand, targetName)) {
        _sender.send(targetCommand);
        _lastMappedTarget = targetName;
        _lastSequenceName = "";
        _lastTriggerAt = now;
        Logger::log(String(I18n::pick("Mapped and sent: ", "Mapeado y enviado: ", "Zmapowano i wyslano: ")) + targetName);
        return true;
    }

    return false;
}

bool IRBridgeService::executeCommand(const String& name) {
    IRCommandData command;
    if (!_commandManager.getCommand(name, command)) {
        return false;
    }

    _sender.send(command);
    return true;
}

bool IRBridgeService::executeSequence(const String& name) {
    JsonDocument sequenceDoc;
    if (!_commandManager.getSequenceDefinition(name, sequenceDoc)) {
        return false;
    }

    JsonArrayConst steps = sequenceDoc["sequence"]["steps"].as<JsonArrayConst>();
    if (steps.isNull() || steps.size() < 2 || steps.size() > IR_SEQUENCE_MAX_STEPS) {
        return false;
    }

    clearStartupPlayback();
    for (JsonVariantConst step : steps) {
        _startupPendingSequence[_startupPendingSequenceCount++] = step.as<String>();
    }
    _startupPendingSequenceNextAt = millis();
    return true;
}

void IRBridgeService::refreshStartupAction() {
    const StartupActionConfig config = _commandManager.getStartupAction();
    _startupActionType = config.enabled ? config.type : "none";
    _startupActionTarget = config.enabled ? config.target : "";
    _startupActionDueAt = millis() + config.delayMs;
    _startupActionScheduled = config.enabled &&
        _startupActionType != "none" &&
        _startupActionTarget.length() > 0;
    _startupActionTriggered = false;
    clearStartupPlayback();
}

bool IRBridgeService::triggerStartupAction() {
    if (_startupActionType == "command") {
        if (!executeCommand(_startupActionTarget)) {
            return false;
        }
        _lastStartupAction = String(I18n::pick("Startup command: ", "Comando de arranque: ", "Komenda startowa: ")) + _startupActionTarget;
        Logger::log(_lastStartupAction);
        return true;
    }

    if (_startupActionType == "sequence") {
        if (!executeSequence(_startupActionTarget)) {
            return false;
        }
        _lastStartupAction = String(I18n::pick("Startup sequence: ", "Secuencia de arranque: ", "Sekwencja startowa: ")) + _startupActionTarget;
        Logger::log(_lastStartupAction);
        return true;
    }

    return false;
}

void IRBridgeService::processStartupAction(uint32_t now) {
    if (_startupPendingSequenceCount > 0 && now >= _startupPendingSequenceNextAt) {
        const String stepName = _startupPendingSequence[_startupPendingSequenceIndex];
        if (stepName.length() > 0) {
            IRCommandData command;
            if (_commandManager.getCommand(stepName, command)) {
                _sender.send(command);
            }
        }

        ++_startupPendingSequenceIndex;
        if (_startupPendingSequenceIndex >= _startupPendingSequenceCount) {
            clearStartupPlayback();
        } else {
            _startupPendingSequenceNextAt = now + IR_STARTUP_STEP_GAP_MS;
        }
    }

    if (_startupActionTriggered || !_startupActionScheduled || now < _startupActionDueAt) {
        return;
    }

    _startupActionTriggered = triggerStartupAction();
    _startupActionScheduled = false;
}

void IRBridgeService::process() {
    const uint32_t now = millis();

    if (isRecordingExpired()) {
        Logger::log(I18n::pick("Recording timeout.", "Tiempo de grabación agotado.", "Przekroczono czas nagrywania."));
        finishRecording();
    }

    processStartupAction(now);

    if (_pendingStepCount > 0 && now > _pendingDeadlineAt) {
        flushPendingSequence(now);
    }

    if (!_receiver.available()) {
        return;
    }

    decode_results* results = _receiver.getResults();
    handleReceive(*results);
    _receiver.resume();
}

void IRBridgeService::handleReceive(decode_results& results) {
    if (results.repeat) {
        return;
    }

    IRCommandData received;
    if (!commandFromResults(_recordMode ? _recordName : "", results, received)) {
        Logger::log(I18n::pick("Ignored IR frame: unsupported payload.", "Trama IR ignorada: payload no soportado.", "Pominieto ramke IR: nieobslugiwany ladunek."));
        return;
    }

    const String signature = commandSignature(received);
    const uint32_t now = millis();
    if (shouldIgnoreTrigger(signature, now)) {
        return;
    }

    _lastReceivedAt = now;
    _lastReceivedSignature = signature;
    _lastReceivedName = _commandManager.getNameBySignature(signature);
    _lastMappedTarget = "";
    _lastSequenceName = "";

    Logger::log(String(I18n::pick("IR recv sig=", "IR recibido firma=", "Odebrano IR sygnatura=")) + signature);
    if (_lastReceivedName.length() > 0) {
        Logger::log(String(I18n::pick("Matched command: ", "Comando reconocido: ", "Dopasowana komenda: ")) + _lastReceivedName);
    }

    if (_recordMode) {
        received.name = _recordName;
        if (_commandManager.saveCommand(received)) {
            Logger::log(String(I18n::pick("Command saved: ", "Comando guardado: ", "Zapisano komende: ")) + _recordName);
        } else {
            Logger::log(I18n::pick("Command save failed.", "Fallo al guardar el comando.", "Nie udalo sie zapisac komendy."));
        }
        finishRecording();
        _lastTriggerSignature = signature;
        _lastTriggerAt = now;
        return;
    }

    if (_lastReceivedName.length() == 0) {
        return;
    }

    if (processSequenceAwareCommand(_lastReceivedName, now)) {
        _lastTriggerSignature = signature;
    }
}

void IRBridgeService::exportStatus(JsonObject obj) const {
    obj["recording"] = _recordMode;
    obj["recordName"] = _recordName;
    obj["recordRemainingMs"] = getRecordingRemainingMs();
    obj["lastReceivedName"] = _lastReceivedName;
    obj["lastReceivedSignature"] = _lastReceivedSignature;
    obj["lastMappedTarget"] = _lastMappedTarget;
    obj["lastSequenceName"] = _lastSequenceName;
    obj["lastReceivedAt"] = _lastReceivedAt;
    obj["startupType"] = _startupActionType;
    obj["startupTarget"] = _startupActionTarget;
    obj["startupPending"] = _startupActionScheduled || _startupPendingSequenceCount > 0;
    obj["lastStartupAction"] = _lastStartupAction;
}
