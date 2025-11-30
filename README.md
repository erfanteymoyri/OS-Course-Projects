<div align="center">
  <img src="https://cdn.freebiesupply.com/logos/large/2x/sharif-logo-png-transparent.png" width="150" height="150" alt="Sharif University Logo">
  <br><br>
  <h1 align="center">Operating Systems Course Assignments</h1>
  <h2 align="center">xv6, Docker, and Linux Kernel Development</h2>
</div>

---

### :dart: About This Project

This repository contains the assignments for the **Operating Systems** course at **Sharif University of Technology**. The course focuses on deep-diving into OS concepts through practical implementation.

Each assignment is structured into three distinct parts to cover different aspects of system programming:
1.  **xv6 Development:** Modifying and adding new features (system calls, user programs) to the educational xv6 operating system (MIT).
2.  **Containerization (Docker):** Practical exercises with Docker to understand isolation, images, and container management.
3.  **Linux Kernel Hacking:** Building the Linux kernel from source, configuring modules, and writing custom kernel modules.

### 👨‍💻 Authors

* **Erfan Teymouri**
* **Seyed Ahmad MossaviAwal**

---

### ⚙️ Assignments & Exercises

#### :one: Assignment 1: Introduction & Environment Setup

This assignment serves as an entry point to the three main pillars of the course.

**Part A: xv6 - Hello World**
* **Goal:** To familiarize with the xv6 build environment and adding user-space programs.
* **Tasks:**
    * Setting up the QEMU/xv6 environment (on Debian/Ubuntu).
    * Creating a new user program that prints `Hello World!` to the console.
    * Modifying the `Makefile` to include the new program in the build process.

**Part B: Docker - Container Basics**
* **Goal:** Understanding container lifecycle, images, and process isolation.
* **Key Operations:**
    * Working with **Alpine Linux** images (tag: `3.22.2`).
    * Managing containers: `run`, `stop`, `start`, and `rm`.
    * Executing commands inside a running container using `exec`.
    * Creating a custom container named `iloveos`.
    * **Analysis:** Comparing process lists (`ps aux`) inside the container vs. the host OS to understand PID namespaces.

**Part C: Linux Kernel - Compilation & Modules**
* **Goal:** Building a custom Linux kernel and writing a Loadable Kernel Module (LKM).
* **Tasks:**
    * **Kernel Compilation:**
        * Downloading Kernel source (v6.1.156).
        * Configuring the kernel (`make menuconfig`) and enabling debugging symbols (KASAN, etc.).
        * Compiling and installing the kernel and modules on a virtual machine.
    * **Kernel Module (`mymodule.c`):**
        * Writing a simple C module that prints messages to the kernel log (`kern.log` / `dmesg`) upon loading and unloading.
        * Creating a `Makefile` for the module.
        * Managing the module using `insmod` and `rmmod`.

---

#### :two: Assignment 2: Debugging, Benchmarking & Profiling

This assignment focuses on kernel introspection, performance comparison across environments, and code optimization techniques.

**Part A: xv6 - Debugging & System Calls**
* **Goal:** Mastering kernel debugging with GDB and extending OS functionality.
* **Tasks:**
    * **Advanced GDB:** Using `backtrace`, `layout src`, and inspecting the `trapframe` (specifically register `a7` for syscall numbers) and `sstatus` to understand user/kernel mode transitions.
    * **New System Call (`top`):** Implementing a custom system call to list active processes.
        * **Functionality:** Prints `PID`, `Command Name`, and `Memory Usage` of current processes.
        * *(Optional)* Implementation of `top -p <PID>` to filter by process ID.

**Part B: Infrastructure - Benchmarking & Isolation**
* **Goal:** Analyzing performance overhead of virtualization and containerization.
* **Sysbench Analysis:**
    * Running stress tests: `cpu` (prime numbers), `memory` (random access), and `fileio`.
    * **Comparison Matrix:** Bare Metal vs. Virtual Machine (Linux) vs. Docker Container.
    * **Metrics:** Analyzing P99 Latency, Average Latency, and Throughput to understand the overhead of each layer.
* **Fork Bomb:**
    * Creating a recursive process spawner to exhaust system resources (PIDs).
    * Observing system behavior (freeze/crash) and analyzing mitigation strategies.

**Part C: Performance Analysis Tools**
* **Goal:** identifying bottlenecks and optimizing code using Linux profiling tools.
* **Tasks:**
    * **Profiling with `perf`:** Analyzing a CPU-intensive C/C++ program.
    * **FlameGraphs:** Generating visual stack traces to identify "hot" functions consuming the most CPU time.
    * **Optimization:** Refactoring the inefficient code based on FlameGraph data and measuring the speedup.
    * **Tracy:** Utilizing the `tracy` profiler for real-time application analysis.

---

### :wrench: Tools Used

* **xv6 (RISC-V/x86):** Educational Operating System.
* **QEMU:** Emulator for running xv6 and testing kernels.
* **Docker:** Platform for developing, shipping, and running applications in containers.
* **GCC/Make:** Build tools.
* **Linux Kernel Source:** Mainline kernel source code.

---

### :rocket: How to Use

1.  Clone the repository:
    ```bash
    git clone [https://github.com/YourUsername/Your-Repo-Name.git](https://github.com/YourUsername/Your-Repo-Name.git)
    ```
2.  Navigate to the specific assignment folder (e.g., `Assignment-1`) to view the source codes and reports.
