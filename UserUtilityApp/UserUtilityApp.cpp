#include <windows.h>
#include <iostream>
#include "public.h"  // Ensure this includes the IOCTL definitions and GUID

HANDLE GetDeviceHandle() {
    HANDLE hDevice = CreateFile(
        L"\\??\\WebSecUsbDriver", // Use the symbolic link
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open device, error: " << GetLastError() << std::endl;
    }

    return hDevice;
}

int main() {
    HANDLE hDevice = GetDeviceHandle();
    if (hDevice == INVALID_HANDLE_VALUE) {
        return 1;
    }

    ULONG otp = 123456;  // Example OTP
    BOOLEAN result = FALSE;
    DWORD bytesReturned;

    BOOL success = DeviceIoControl(
        hDevice,
        IOCTL_USBOTP_VALIDATE_OTP,
        &otp,
        sizeof(otp),
        &result,
        sizeof(result),
        &bytesReturned,
        NULL
    );

    if (!success) {
        std::cerr << "DeviceIoControl failed, error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "OTP validation result: " << (result ? "Valid" : "Invalid") << std::endl;
    }

    CloseHandle(hDevice);
    return 0;
}
