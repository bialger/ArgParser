#include <iostream>
#include "BasicFunctions.hpp"

bool AlwaysTrue(char* str) {
  return true;
}

void SetRedColor() {
  /* Changes the color of the console output to red if not running
   * on Windows. */

  std::cout << (IsWindows() ? "" : "\x1B[31m");

  /* Conditional block to set the console text color to red if the program is
   * running on Windows, using the Windows API functions. */

  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
  }
}

void ResetColor() {
  if (IsWindows()) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
  }

  std::cout << (IsWindows() ? "" : "\x1B[0m");
}

void DisplayError(const std::string& message) {
  SetRedColor();
  std::cout << message;
  ResetColor();
}

bool IsWindows() {
  return
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    true
#else
      false
#endif
      ;
}

/* The code provides dummy function definitions for Windows console-related
 * functions when the code is being compiled in a non-Windows environment.
 * This ensures that the code can compile and run without errors in such
 * environments. The dummy functions have minimal functionality and simply
 * return their input parameters. */

#if not (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
int GetStdHandle(int a) { return a; }
int SetConsoleTextAttribute(int a, int b) { return a + b; }
#endif