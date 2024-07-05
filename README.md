# DualMicro DoorLock

It is a good project to familiarise myself with embedded systems development.

## How to Use the Repository

### Prerequisites
- AVR-GCC
- Make
- avrdude (for flashing the microcontroller)
- Double ATmega32

### Directory Structure
- `src/`: application code/ main function
- `Common/`: types of target and common useful macros
- `HAL/`: Hardware Abstraction Layer implementation
- `MCAL/`: Microcontroller Abstraction Layer implementation
- `Simulator/`: schematic design of the project for validation
- `Debug/`: .elf file to be flashed

### Setting Up the Development Environment
1. Install AVR-GCC:
    - On Debian/Ubuntu: `sudo apt-get install gcc-avr avr-libc`
    - On macOS: `brew tap osx-cross/avr && brew install avr-gcc`
    - On Windows: Install [AVR-GCC](http://blog.zakkemble.net/avr-gcc-builds/)
        - Download and install the AVR-GCC package from the link.
        - Ensure the AVR-GCC binaries are added to your system PATH.
2. Install Make:
    - On Debian/Ubuntu: `sudo apt-get install make`
    - On macOS: `brew install make`
    - On Windows: Install [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm):
      - Download and install Make from the link.
      - Ensure the Make binaries are added to your system PATH.
3. Clone the repository:
    ```
    git clone https://github.com/hussein-shamy/AVR-mini-projects.git
    cd AVR-mini-projects
    ```
4. Build the project:
    ```
    make
    ```
5. Flash the firmware:
    ```
    make flash
    ```
