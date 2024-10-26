# Image Editor

## How to Build

For Linux Fedora 40

Install the dependencies:

```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake ninja-build libxkbcommon-devel
sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel
sudo dnf install opencv-devel
```

Generate the Makefile and Build:

```bash
qmake && make
```

Run:

```bash
./main
```
