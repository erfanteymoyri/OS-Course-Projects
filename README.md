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

* **[Your Name]**
* **[Partner's Name if applicable]**

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
