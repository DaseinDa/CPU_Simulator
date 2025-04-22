# CPU Simulator

This project is a lightweight **instruction pipeline simulator** written in **C++17**, designed to model the internal behavior of a modern CPU according to RISC V ISA. It simulates the execution of instructions through a pipeline and outputs the state of the **Reorder Buffer (ROB)** and **Reservation Stations** at each clock **cycle**.

---

## Features

- Simulates instruction execution in a pipelined CPU architecture
- Models out-of-order execution using ROB and Reservation Stations
- Prints internal CPU state for each cycle
- Easy to modify, extend, and integrate into research or teaching tools

---

## Build & Run

Make sure you have a C++17-compatible compiler (e.g., `g++` 7.0 or higher) and `make` installed.

```bash
# In the project root directory
make
./myapp
