# Guitar Amplifier Project

This repository contains the firmware and hardware design for a digital guitar amplifier platform built around an STM32-based embedded system and an Altium Designer PCB.

## Overview

The project combines:

- real-time audio DSP processing
- an on-device user interface with display output
- board-level amplifier control and safety handling
- a custom PCB design for audio, power, and control sections

The firmware is organized as a modular embedded application with separate blocks for DSP, effects, presets, UI rendering, and board support.

---

## What This Project Includes

### Firmware
The firmware is located in [Codes/Core](Codes/Core) and is structured around the following major components:

- [Codes/Core/Src/main.c](Codes/Core/Src/main.c) — system initialization, peripheral setup, and main control loop
- [Codes/Core/Src/audio_engine.c](Codes/Core/Src/audio_engine.c) — audio pipeline orchestration
- [Codes/Core/Src/dsp.c](Codes/Core/Src/dsp.c) — core DSP processing and level tracking
- [Codes/Core/Src/effects.c](Codes/Core/Src/effects.c) — effect processing modules
- [Codes/Core/Src/features.c](Codes/Core/Src/features.c) — presets and tone settings
- [Codes/Core/Src/ui.c](Codes/Core/Src/ui.c) — display rendering and user interface
- [Codes/Core/Src/board.c](Codes/Core/Src/board.c) — hardware control, display handling, and amplifier helpers

### Hardware
The PCB design is located in [PCB/Altium Designer](PCB/Altium%20Designer) and includes schematic sheets for:

- MCU and DSP control
- DAC and audio path
- power amplifier section
- power supply distribution
- SD card interface
- USB-TTL and debug connectivity

---

## Firmware Features

The embedded software currently supports:

- real-time sample and block processing
- high-pass filtering and noise gating
- distortion and soft clipping
- EQ, chorus, delay, and reverb effects
- preset-based tone shaping such as clean, crunch, lead, metal, and ambient
- live audio level metering on the display
- amplifier enable, mute, and fault monitoring

---

## PCB Description

The PCB is a mixed-signal hardware design intended for a DSP-based guitar amplifier. It was created in Altium Designer as a compact embedded audio platform that combines digital control, analog audio processing, and power amplification on one board.

### Main Hardware Blocks

The board is organized into several functional sections:

- MCU and DSP section
  - Built around an STM32-class controller and supporting DSP logic
  - Responsible for audio processing, UI control, and system coordination

- Audio DAC section
  - Includes DAC circuitry for high-quality audio output conversion
  - Designed to support the audio path between processing and amplification

- Power amplifier section
  - Intended for the final output stage and loudspeaker drive
  - Designed with careful power routing and protection in mind

- Power supply section
  - Includes the main power domains for the system, such as 24 V, 5 V, and 3.3 V rails
  - Designed to separate power-heavy paths from sensitive analog and digital circuitry

- Connectivity and interface section
  - Includes support for display control, SD card storage, USB-TTL communication, and programming/debug access

### PCB Design Intent

The PCB design focuses on several important goals:

- low-noise analog audio performance
- clean power distribution for the amplifier stage
- solid grounding and return paths for mixed digital/audio/power circuits
- short, controlled current loops in the power amplifier area
- practical routing for display, data, and debug interfaces

### Layer and Layout Strategy

The board is intended to use a multilayer stackup with continuous ground planes and controlled routing strategy. The planned approach is:

- top layer for critical components, short power paths, and local copper pours
- inner layers for solid ground planes and additional routing
- bottom layer for support routing and additional grounding coverage

This approach helps improve:

- signal integrity
- noise immunity
- power handling
- EMI behavior
- thermal and current distribution around the amplifier section

### Grounding and Noise Control

A key requirement in this design is careful grounding because the board combines digital logic, audio circuitry, and power electronics. The intended layout practice includes:

- continuous ground planes where possible
- short and direct return paths for high-current sections
- separation of noisy power/current loops from sensitive audio and control traces
- careful handling of the analog ground and power ground domains
- grounding stitches and copper pours where needed for return continuity

### Practical Layout Considerations

The PCB layout should keep the following priorities in mind:

- keep amplifier power loops short and wide
- place decoupling and bulk capacitors close to the relevant ICs
- avoid routing sensitive audio traces near high-current power paths
- keep the DAC and analog front-end routing compact and clean
- route display and control signals away from the strongest power amplifier currents where practical
- use wide copper pours for high-current nets and solid return paths

---

## Project Structure

- [Codes](Codes) — firmware source and headers
- [PCB](PCB) — Altium project files and generated outputs
- [Components](Components) — PCB component libraries and symbols
- [Datasheets](Datasheets) — reference documentation
- [Results](Results) — generated outputs and project results

---

## Getting Started

To work with the firmware:

1. Open the STM32 project in your preferred embedded development environment.
2. Build the firmware for the target MCU configuration.
3. Flash the device using the appropriate debug/programming interface.
4. Verify the audio path, display output, and amplifier control behavior on hardware.

For the hardware side, open the Altium project in [PCB/Altium Designer](PCB/Altium%20Designer) to review or modify the schematics and PCB layout.

---

## Notes

This project is a functional embedded audio and hardware design prototype. Before fabrication or production use, the PCB layout, power stage, grounding strategy, and firmware integration should be reviewed carefully and validated on hardware.
