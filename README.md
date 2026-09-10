# Real-Time Cyber-Physical Battery Management System (BMS) 🔋

A high-reliability, firmware-driven Battery Management System (BMS) architecture designed to bridge low-level physical telemetry with robust object-oriented control software. This project documents the complete engineering lifecycle of safety-critical automotive firmware: transitioning from a decoupled **Desktop Architectural Proof-of-Concept** to an **In-Hardware Deployed Subsystem** running on a 32-bit microcontroller.

---

## 🕹️ Live Hardware Emulation Sandbox
Interact with the live firmware loop, physical ADC routing, and safety isolation controls directly via the hardware emulation layer:
👉 **[Launch Interactive Wokwi Hardware Emulation Workspace](PASTE_YOUR_WOKWI_URL_HERE)**

> **Hardware Operation Instructions:** Rotate the physical potentiometer knob to inject dynamic analog cell voltages. Observe how the firmware captures the voltage drop, executes safety filters, and activates the isolation state machine in real-time.

---

## 🏗️ System Architecture & Abstraction Model

The system implements strict **Hardware Abstraction Layer (HAL)** paradigms, ensuring the core safety state machine remains completely decoupled from physical sensor dependencies.

```text
       [ Physical Telemetry Input: 0V - 3.3V ]
                         │
                         ▼
        [ 12-Bit Analog-to-Digital Converter ]
                         │
                         ▼
┌────────────────────────────────────────────────────────┐
│  Hardware Abstraction Layer (HAL Interface Class)     │
│  - virtual void sampleSensors() = 0;                   │
│  - virtual void evaluateSafetyLimits() = 0;            │
└────────────────────────┬───────────────────────────────┘
                         │ (Runtime Polymorphism)
                         ▼
┌────────────────────────────────────────────────────────┐
│  Concrete Subsystem Implementation                    │
│  - Hysteresis Logic & Bounds Analysis                 │
│  - Virtual Destructor Dynamic Allocation Protection   │
└────────────────────────┬───────────────────────────────┘
                         │
                         ▼
       [ Critical Fault Isolation State Machine ]
```

---

## 📂 Implementation Topography

The system topology is compartmentalized into two distinct verification spaces:

### 💼 1. Desktop Prototyping Space (`/desktop-prototype`)
* **Execution Environment:** Host Computer (`g++` toolchain / standard POSIX target).
* **Engineering Objective:** Validates the abstract type hierarchy, memory footprint predictability, and inheritance polymorphism under automated, deterministic degradation arrays.
* **Memory Management Strategy:** Implements explicit virtual destructors to prevent heap fragmentation and memory leaks during multi-level interface cleanup.

### ⚡ 2. Bare-Metal Target Firmware (`/esp32-firmware`)
* **Execution Environment:** 32-bit Dual-Core Tensilica Xtensa LX6 (ESP32 SoC Architecture).
* **Engineering Objective:** Compiles the abstract safety parameters into time-critical, hardware-mapped logic loops.
* **Signal Conditioning:** Interlaces raw 12-bit register reads with the standard Lithium-Ion operational envelope ($3.20\text{V} - 4.25\text{V}$) via low-overhead arithmetic scaling.

---

## 📊 Technical Control Matrix

| Parameter / Metric | Baseline Operational Specification |
| :--- | :--- |
| **Core Programming Baseline** | ISO/IEC C++17 (`-std=c++17`) |
| **Microcontroller Architecture** | 32-bit ESP32 RISC SoC |
| **ADC Native Resolution** | 12-Bit ($0 - 4095$ quantization levels) |
| **Lower Safety Threshold (UV)** | $< 3.20\text{V}$ $\rightarrow$ Immediate Cell Isolation Trip |
| **Upper Safety Threshold (OV)** | $> 4.25\text{V}$ $\rightarrow$ Disengage Charging Loop Relay |
| **Hardware Execution Topology** | Event-driven loop tracking asynchronous sampling cycles |

---

## 🚀 Technical Achievements Highlighted
* **Zero Memory Leak Profile:** Uses explicit modern memory management structures avoiding unmanaged object pointer leaks.
* **Deterministic Execution:** Avoids heavy dynamic containers inside the hardware-deployed loop to minimize unexpected processing latency.
* **High Interface Modularity:** The system interface layout allows swapping the ESP32 chip for an STM32 or automotive-grade chip without modifying the core logic.
