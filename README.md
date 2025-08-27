# Out-of-Order CPU Pipeline Simulator
<img width="500" height="500" alt="image" src="https://github.com/user-attachments/assets/eedbb709-321b-42e9-8c4f-0bac970397fb" />

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

bash
# In the project root directory
make
./myapp
# Out-of-Order CPU Pipeline Simulator

An advanced **cycle-accurate CPU simulator** written in **C++17**, modeling an out-of-order, speculative, superscalar pipeline.  
This project implements key concepts in modern processor design, including **Tomasulo’s algorithm**, **register renaming**, **branch prediction**, and **dynamic scheduling**, allowing detailed cycle-by-cycle performance analysis under customizable architectural parameters.

---

## 🚀 Features

- **6-Stage Out-of-Order Pipeline**
  - **Fetch** – Up to `NF` instructions per cycle, with branch target buffer (BTB) lookup and prediction.  
  - **Decode & Rename** – Decoding and physical register renaming for eliminating WAW/WAR hazards.  
  - **Issue** – Allocation into reservation stations with dependency tracking.  
  - **Execute** – Parallel execution via multiple functional units (INT, FPADD, FPMUL, LOAD, STORE, BRANCH).  
  - **Write Back** – Results broadcast to the register file and waiting reservation stations.  
  - **Commit** – In-order commit using a circular reorder buffer (ROB).

- **Dynamic Branch Prediction**
  - Local **2-bit saturating counters** with a **16-entry BTB**.  
  - Early and conservative recovery mechanisms for misprediction rollback.

- **Precise Hazard Management**
  - **Register renaming** with 32 physical registers.  
  - **Load-store hazard detection** with forwarding logic and conservative speculation.  
  - Accurate simulation of reservation station and ROB stalls.

- **Highly Configurable**
  - Adjust core architectural parameters via command-line flags or config:
    - `NF`: Fetch width  
    - `NI`: Issue queue size  
    - `NW`: Writeback bandwidth  
    - `NB`: Number of buses  
    - `NR`: ROB size  

- **Detailed Debugging**
  - Cycle-by-cycle pipeline logging for deep analysis of instruction flow.  

---

## Project Structure

```
├── src/
│   ├── main.cpp          # Entry point
│   ├── fetch.cpp         # Fetch stage
│   ├── decode.cpp        # Decode & rename
│   ├── issue.cpp         # Issue stage
│   ├── execute.cpp       # Execution stage
│   ├── commit.cpp        # Commit stage
│   ├── global.h          # Global namespace and configs
│   └── utils/            # Helpers and utilities
├── examples/
│   └── assembly.dat      # Example assembly input
├── docs/
│   └── pipeline_diagram.png
├── Makefile
└── README.md
```
---

## ⚙️ Installation and Usage

### **1. Build**

```bash
git clone https://github.com/DaseinDa/CPU_Simulator.git
cd CPU_Simulator
make
```

### **2. Run**

Default execution:

```bash
./myapp
```

Specify custom input program and parameters:

```bash
./myapp -p examples/assembly.dat -f 4 -i 16 -w 4 -r 16 -b 4 > log.txt 2>&1
```

| Flag | Parameter | Description                 |
| ---- | --------- | --------------------------- |
| `-p` | `PROGRAM` | Input assembly program file |
| `-f` | `NF`      | Fetch width per cycle       |
| `-i` | `NI`      | Issue queue size            |
| `-w` | `NW`      | Writeback bandwidth         |
| `-r` | `NR`      | ROB size                    |
| `-b` | `NB`      | Number of common data buses |

---

## Architecture Overview

The simulator implements a **parameterized Tomasulo-style pipeline**, as shown below:

![Pipeline Diagram](docs/pipeline_diagram.png)

Key modules:

- **BTB & Predictor:** Local branch predictor with independent counters for accurate speculation.  
- **ROB (Reorder Buffer):** Circular buffer for in-order commit and precise exceptions.  
- **Reservation Stations:** Distributed queues for each functional unit.  
- **History Snapshot:** Enables rapid recovery from branch mispredictions.

---

## Performance Analysis

The simulator provides cycle-level statistics to evaluate the impact of architectural changes:

| Configuration `(NF, NI, NW, NR, NB)` | Cycles | Fetch Stalls | Decode Stalls | ROB Stalls | RS Stalls |
| ------------------------------------ | ------ | ------------ | ------------- | ---------- | --------- |
| (4, 16, 4, 16, 4)                    | 19     | 0            | 0             | 0          | 1         |
| (4, 16, 2, 16, 2)                    | 21     | 0            | 0             | 0          | 0         |
| (4, 16, 4, 4, 4)                     | 32     | 3            | 5             | 10         | 1         |
| (4, 16, 4, 32, 4)                    | 19     | 0            | 0             | 0          | 1         |

**Insights**  

- ROB size (`NR`) is the most critical parameter; small ROBs cause severe stalls.  
- Writeback bandwidth (`NW`) directly affects performance throughput.  
- Fetch width (`NF`) and issue queue size (`NI`) have moderate impact under lighter workloads.  

---

## Future Enhancements

- Visualization of per-cycle pipeline state.  
- Cache and memory hierarchy modeling.  
- Multi-threaded simulation for SMT or multi-core research.  
- Expanded ISA with additional arithmetic and atomic instructions.

---
