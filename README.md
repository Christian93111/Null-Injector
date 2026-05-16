# Null Injector

Null Injector is a simple DLL injector for Windows. Designed with simplicity and stability in mind, it provides a clean experience for injecting dynamic-link libraries into running processes.

## Features

- **Manual Injection**: Simple and direct DLL injection into any running process.
- **Process Selection Dialog**: Easily search and find target processes with system native icons.
- **UWP/AppContainer Support**: Automatically applies `ALL APPLICATION PACKAGES` permissions to your DLL, ensuring compatibility with modern Windows apps.
- **System Tray Integration**: Hide the injector to the system tray for a clean workspace; toggle visibility with a double-click.
- **Admin Required**: Built-in manifest to automatically request administrative privileges for cross-session injection.

## Preview

![Null Injector Screenshot](https://raw.githubusercontent.com/Christian93111/Null-Injector/refs/heads/main/screenshot/1.png)

## Releases

Download the latest version from the [Releases](https://github.com/Christian93111/Null-Injector/releases/) page.

## How to Use

1. **Launch**: Run `NullInjector.exe` as Administrator.
2. **Select DLL**: Click **"Select"** to choose your `.dll` file.
3. **Select Process**: Click **"Search"** to find your target process from the list.
4. **Inject**: Click **"Inject"**. Check the status bar at the bottom for feedback.
5. **Minimize**: Click **"Hide on Tray"** to move the application to the system tray.

## Compatibility

- Windows 10 / 11 (x64)

## Troubleshooting

### Anti-Virus Flagging
Because this tool performs process memory manipulation, some anti-virus software may flag it as a Potentially Unwanted Program (PUP). This is a false positive due to the nature of the code. You can view the source code to verify it is safe.

### Missing VCRUNTIME140_1.dll
If you encounter an error stating `VCRUNTIME140_1.dll` was not found:

1. Download the [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist) or [Tech Power Up](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/)
2. Install the **x64** version (`vc_redist.x64.exe`).
3. Restart the application.

## Built With

- [wxWidgets](https://wxwidgets.org)

## Disclaimer

This project is intended for educational and developmental purposes only. Do not use on processes you do not own or have explicit permission to modify. Use at your own risk.