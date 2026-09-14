# Digital-PLL-Frequency-Synthesizer---Phase-Synchronization-Discrete-Time-Control-TI-F28027

## Introduction

A Phase-Locked Loop (PLL) is a feedback control system used to synchronize the phase and frequency of a generated signal with a reference signal.

PLLs are fundamental building blocks in modern electronic systems because they allow a system to generate, track and synchronize precise frequencies starting from a reference clock or signal.

They are widely used in applications such as:

Frequency synthesizers: generation of stable and precise frequencies for wireless communication systems, RF transceivers, mobile devices, routers and other digital communication equipment.
Signal demodulation: recovery of frequency and phase information in communication receivers.
Clock generation and synchronization: generation and synchronization of system clocks in digital electronics and high-speed communication systems.
Motor speed control: synchronization and regulation of rotational speed using a precise frequency reference.
Power systems: synchronization of electrical generators and power electronic converters with the grid phase.
Data communication systems: clock and carrier synchronization in digital communication architectures.

This project focuses on the implementation of a digital multiplier PLL, where phase error is obtained from the multiplication of the reference signal and the generated signal. The resulting signal is filtered, compensated and used to correct the phase evolution of the synthesized waveform.

The design was implemented in C on a Texas Instruments LaunchPad XL F28027, transforming the mathematical PLL model into a real-time embedded implementation.

Hardware setup:

![Digital Architecture](images/hardware_setup.png)

Block diagram:

![Digital Architecture](images/Block_diagram.png)
