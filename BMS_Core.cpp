#include <iostream>
#include <vector>

// Abstract Base Class demonstrating ECE System Interface Architecture
class PowerSystem {
protected:
    float terminalVoltage;
    bool systemSafetyStatus;

public:
    PowerSystem() : terminalVoltage(0.0), systemSafetyStatus(true) {}
    virtual ~PowerSystem() {} // Virtual destructor to prevent memory leaks

    virtual void readTelemetry() = 0;       // Pure virtual function
    virtual void evaluateSafetyFlags() = 0; // Pure virtual function
};

// Derived Class simulating an EV Battery Pack Monitoring System
class BatteryManagementSystem : public PowerSystem {
private:
    std::vector<float> cellVoltages;
    float upperVoltageLimit;
    float lowerVoltageLimit;

public:
    // Constructor utilizing Member Initializer List for fast execution
    BatteryManagementSystem(int totalCells, float maxV, float minV) 
        : upperVoltageLimit(maxV), lowerVoltageLimit(minV) {
        cellVoltages.assign(totalCells, 3.7f); // Default safe lithium-ion cell state
    }

    // Overriding system abstraction with concrete telemetry simulations
    void readTelemetry() override {
        // Simulating voltage drop under standard load conditions
        for(size_t i = 0; i < cellVoltages.size(); ++i) {
            cellVoltages[i] -= 0.05f; 
        }
        terminalVoltage = 0.0f;
        for(float v : cellVoltages) terminalVoltage += v;
    }

    void evaluateSafetyFlags() override {
        for(size_t i = 0; i < cellVoltages.size(); ++i) {
            if(cellVoltages[i] < lowerVoltageLimit) {
                systemSafetyStatus = false;
                std::cout << "[CRITICAL ALERT] Cell " << i << " Under-Voltage Detected: " 
                          << cellVoltages[i] << "V. Engaging Disconnect Circuit!" << std::endl;
                return;
            }
        }
        std::cout << "[SYSTEM NORMAL] Pack Voltage: " << terminalVoltage << "V. All cells within safe operating window." << std::endl;
    }
};

int main() {
    // Instantiate a 4-Cell EV Pack (Max 4.2V, Min 3.2V)
    BatteryManagementSystem standardEVBMS(4, 4.2f, 3.2f);

    std::cout << "--- Initializing HBTU ECE Cyber-Physical BMS Simulation ---" << std::endl;
    for(int cycle = 0; cycle < 12; ++cycle) {
        standardEVBMS.readTelemetry();
        standardEVBMS.evaluateSafetyFlags();
    }
    return 0;
}

