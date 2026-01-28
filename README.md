# 🕵️ C++ Stealth Keylogger (Windows Internals)

A lightweight, stealthy keystroke logger built in **C++** using the Windows API (`SetWindowsHookEx`). This tool demonstrates how malware interacts with system processes and how endpoint security solutions monitor user input.

## ⚡ Features
* **Global Keyboard Hook:** Uses low-level system hooks to capture keystrokes system-wide, not just in the active window.
* **Context-Aware Logging:** Detects and logs the **Active Window Title** (e.g., "[WINDOW: Gmail - Google Chrome]") to provide context for the captured text.
* **Stealth Mode:** Automatically hides the console window upon execution, running invisibly in the background.
* **Special Key Handling:** Correctly maps virtual key codes for Backspace, Tab, Enter, etc.

## 🛠️ Technical Implementation
* **Language:** C++
* **API:** Windows API (`User32.dll`, `Kernel32.dll`)
* **Mechanism:**
    * `SetWindowsHookEx(WH_KEYBOARD_LL, ...)` to inject the hook.
    * `GetForegroundWindow()` and `GetWindowTextA()` for context extraction.
    * `AllocConsole()` and `ShowWindow()` for process stealthing.

## ⚠️ Disclaimer
**Educational Use Only.** This software is designed for authorized security research and "Red Team" simulations. Installing keyloggers on computers without permission is illegal.

---
*Built by [Ilyas Tlegenov](https://github.com/YOUR_USERNAME)*