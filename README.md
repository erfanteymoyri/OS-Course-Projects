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

#### :three: Assignment 3: Process Management & System Call Internals

This assignment deepens the understanding of process tracking, namespace isolation, and the mechanics of system calls.

**Part A: xv6 - Process Tree Visualization**
* **Goal:** Extending the `top` command to display a hierarchical process tree.
* **Tasks:**
    * **New System Call (`next_process`):** Iterates through the process table to retrieve process details (PID, Parent PID, Heap Size, State, Name).
    * **User Program (`top -t`):** Using the new syscall to traverse and visualize the parent-child relationships using DFS/BFS.
    * **State Handling:** Correctly displaying `Running`, `Runnable`, `Sleeping`, and `Zombie` states.
    * **Testing:** Creating a complex process generation program (using `fork`/`exec`) to verify the tree structure.

**Part B: Docker - PID Namespaces (Zocker)**
* **Goal:** Implementing a minimal container runtime ("Zocker") to understand PID isolation.
* **Key Concepts:**
    * **Namespaces:** Using `unshare(2)` and `clone(2)` with `CLONE_NEWPID`.
    * **Process Isolation:** Ensuring the containerized process sees itself as PID 1, while having a different PID on the host.
    * **Capabilities:** Handling permissions (`CAP_SYS_ADMIN`) and `setcap` to run without `sudo` (optional).

**Part C: Linux Kernel - PCB & System Call Timing**
* **Goal:** Modifying the Linux Process Control Block (PCB) and analyzing system call overhead.
* **Tasks:**
    * **PCB Modification:** Researching `task_struct`, adding custom fields, and writing syscalls to read/write these fields.
    * **Timing Analysis:**
        * Implementing a high-precision timing mechanism inside the kernel.
        * Measuring the exact time spent in **User-to-Kernel** and **Kernel-to-User** transitions.
        * **Benchmarking:** Analyzing standard syscalls (`read`, `write`, `getpid`) vs. custom syscalls.

---

#### :four: Assignment 4: Threads, Namespaces & File Systems

This assignment covers concurrency implementation, advanced container isolation, and file system architecture.

**Part A: xv6 - User-Level Threads (Green Threads)**
* **Goal:** Implementing a cooperative threading library in user space without kernel awareness.
* **Tasks:**
    * **Thread Structure:** Defining `struct thread` to hold the stack, state (`RUNNING`, `RUNNABLE`, `FREE`), and context.
    * **Context Switching:** Implementing the mechanism to save/restore CPU registers (`ra`, `sp`, `s0-s11`) to switch between threads.
    * **Scheduling:** Writing a Round-Robin scheduler (`thread_schedule`) and a yield function (`thread_yield`) to switch execution between `thread_a`, `thread_b`, and `thread_c`.

**Part B: Docker - Advanced Isolation (Mounts & UTS)**
* **Goal:** Fixing namespace leaks in the custom "Zocker" runtime.
* **Key Challenges:**
    * **PID Namespace & `/proc`:** Correctly mounting a fresh `/proc` filesystem inside the container so `ps` only shows containerized processes.
    * **Mount Propagation:** Using `mount --make-rprivate` to prevent container mounts from affecting the host system.
    * **UTS Namespace:** Isolating the hostname using `sethostname` so the container has its own identity.

**Part C: File Systems - User-Space FS (FUSE Concepts)**
* **Goal:** Designing and implementing a simple file system that resides in a single file (`filesys.db`), mimicking FUSE architecture.
* **Architecture:**
    * **Virtual Disk:** Using a binary file (`filesys.db`) as the storage medium.
    * **Superblock:** Validating the file system using a Magic Number (`0xDEADBEEF`).
    * **Metadata:** Managing files using a Linked List structure (Name, Type, Permissions, Data pointers).
* **CLI Implementation:**
    * Developing a command-line tool to perform operations: `open`, `write`, `read`, `shrink`, `rm`, and `get_fs_stats` directly on the virtual disk file.

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
