echo This program runs the desktop app packager command to create a Windows 10 store package from an existing installer.
pause
DesktopAppConverter.exe -Installer "C:\Users\Xangis\code\SampliTron\installer\SampliTron1.2Setup.exe" -PackageName "SampliTron" -Version 1.2.0.0 -Makeappx -Destination "C:\Users\Xangis\code\SampliTron\installer" -InstallerArguments "/VERYSILENT" -Publisher "CN=Jason Champion" -Sign
Certutil -addStore TrustedPeople SampliTron\auto-generated.cer



