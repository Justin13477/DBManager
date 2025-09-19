# DBManager
A SQLite Database Viewer/Manager with some basic/limited functionality

##  Purpose
This project was built as a learning exercise to:
- Practice working with sqlite in C++
- Refine my data management and application architecture skills
- Gain experience with imgui for building GUI applications
- Learn how to design more scalable software

---

## Core Functionality (so far)
- Query console embedded in the app  
- Database view with basic operations for tables and columns  

---

## 🛠️ Build Instructions
### Prerequisites
- **C++20** or newer  
- **CMake 3.20+**  
- Dependencies (included in the project or submodules):  
  - [fmt](https://github.com/fmtlib/fmt) — fast formatting library  
  - [SQLite3](https://www.sqlite.org/) — database engine  
  - [GLAD](https://glad.dav1d.de/) — OpenGL loader  
  - [GLFW](https://www.glfw.org/) — window/context/input  
  - [ImGui](https://github.com/ocornut/imgui) — immediate mode GUI  

### Build
```bash
git clone https://github.com/yourusername/DBManager.git
cd DBManager
mkdir build && cd build
cmake ..
cmake --build .
