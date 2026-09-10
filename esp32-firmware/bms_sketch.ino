#include <Arduino.h>

// Abstract Base Class representing the System Architecture Interface
class VehiclePowerSystem {
protected:
    int analogPin;
    float simulatedVoltage;
    bool safetyRelayState;

public:
    VehiclePowerSystem(int pin) : analogPin(pin), simulatedVoltage(0.0), safetyRelayState(true) {}
    virtual ~VehiclePowerSystem() {} // Virtual destructor to ensure zero memory leaks

    virtual void sampleSensors() = 0;       // Pure virtual interface method
    virtual void evaluateSafetyLimits() = 0; // Pure virtual interface method
};

// Concrete implementation simulating an EV Battery Management Subsystem
class BatteryMonitor : public VehiclePowerSystem {
private:
    const float lowVoltageThreshold = 3.20;
    const float highVoltageThreshold = 4.25;

public:
    BatteryMonitor(int pin) : VehiclePowerSystem(pin) {}

    void sampleSensors() override {
        int rawADC = analogRead(analogPin);
        // Map standard 12-bit ESP32 ADC values (0-4095) to typical Lithium-Ion voltage curve (0.0V - 4.5V)
        simulatedVoltage = (rawADC / 4095.0) * 4.50;
    }

    void evaluateSafetyLimits() override {
        Serial.print("[TELEMETRY] Current Monitored Cell Voltage: ");
        Serial.print(simulatedVoltage);
        Serial.println(" V");

        if (simulatedVoltage < lowVoltageThreshold) {
            safetyRelayState = false;
            Serial.println("[⚠️ CRITICAL ALERT] Under-Voltage State! Tripping Main Battery Disconnect Circuit!");
        } 
        else if (simulatedVoltage > highVoltageThreshold) {
            safetyRelayState = false;
            Serial.println("[⚠️ CRITICAL ALERT] Over-Voltage/Overcharge State! Disengaging Charger Relay!");
        } 
        else {
            safetyRelayState = true;
            Serial.println("[✔️ STATUS NORMAL] Voltage levels within baseline operating limits.");
        }
        Serial.println("-----------------------------------------------------------------");
    }
};

// Instantiate the Object globally mapped to Analog Input Pin 34
BatteryMonitor bmsPack(34);

void setup() {
    Serial.begin(115200);
    pinMode(34, INPUT);
    Serial.println("=== HBTU ECE Cyber-Physical Systems Lab Initialized ===");
    Serial.println("Adjust the hardware potentiometer knob to change simulated cell voltages dynamically.");
    Serial.println("-----------------------------------------------------------------");
}

void loop() {
    bmsPack.sampleSensors();
    bmsPack.evaluateSafetyLimits();
    delay(1500); // Sample telemetry loop every 1.5 seconds
}

