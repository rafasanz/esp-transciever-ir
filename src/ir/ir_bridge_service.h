/*
 * Module: IR bridge runtime
 * Responsibilities:
 * - coordinate recording, mapping, sequence matching, and startup playback
 * - expose the live bridge state used by the web interface
 */

#pragma once

#include <Arduino.h>
#include "command_manager.h"
#include "ir_receiver.h"
#include "ir_sender.h"

#ifndef IR_SEQUENCE_MAX_STEPS
#define IR_SEQUENCE_MAX_STEPS 8
#endif

class IRBridgeService {
public:
    IRBridgeService(CommandManager& commandManager, IRReceiver& receiver, IRSender& sender);

    void begin();
    void process();
    bool startRecording(const String& name);
    bool isRecording() const;
    String getRecordingName() const;
    uint32_t getRecordingRemainingMs() const;
    void exportStatus(JsonObject obj) const;
    bool executeCommand(const String& name);
    bool executeSequence(const String& name);
    void refreshStartupAction();

private:
    CommandManager& _commandManager;
    IRReceiver& _receiver;
    IRSender& _sender;

    bool _recordMode = false;
    String _recordName;
    uint32_t _recordStartedAt = 0;
    uint32_t _lastTriggerAt = 0;
    String _lastTriggerSignature;
    String _lastReceivedName;
    String _lastReceivedSignature;
    String _lastMappedTarget;
    String _lastSequenceName;
    uint32_t _lastReceivedAt = 0;
    String _pendingSteps[IR_SEQUENCE_MAX_STEPS];
    uint8_t _pendingStepCount = 0;
    uint32_t _pendingDeadlineAt = 0;
    bool _pendingFallbackReady = false;
    String _pendingFallbackSource;
    bool _pendingExactReady = false;
    String _pendingExactTarget;
    String _pendingExactSequenceName;
    bool _startupActionScheduled = false;
    bool _startupActionTriggered = false;
    String _startupActionType;
    String _startupActionTarget;
    uint32_t _startupActionDueAt = 0;
    String _startupPendingSequence[IR_SEQUENCE_MAX_STEPS];
    uint8_t _startupPendingSequenceCount = 0;
    uint8_t _startupPendingSequenceIndex = 0;
    uint32_t _startupPendingSequenceNextAt = 0;
    String _lastStartupAction;

    bool isRecordingExpired() const;
    bool shouldIgnoreTrigger(const String& signature, uint32_t now) const;
    void finishRecording();
    void handleReceive(decode_results& results);
    void clearPendingSequence();
    void flushPendingSequence(uint32_t now);
    bool processSequenceAwareCommand(const String& commandName, uint32_t now);
    void clearStartupPlayback();
    void processStartupAction(uint32_t now);
    bool triggerStartupAction();
};
