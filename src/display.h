#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

namespace display {

const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 25;

extern int cursorX, cursorY;
extern u8 color;

enum class Color : u8 {
  // Bright bit unset.
  BLACK = 0,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHT_GREY,

  // Bright bit set.
  DARK_GREY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_MAGENTA,
  YELLOW,
  WHITE
};

// A cell in text video memory. Do not reorder the fields.
struct Cell {
  u8 character;
  u8 color;

  Cell(u8 color, u8 character) : character(character), color(color) {}
};

void init();
void clearScreen();
void scroll();
void updateCursor();
Cell& cellAt(int x, int y);
void setColor(Color fg, Color bg);

// Printing functions.
void printInt(u32 n, int radix);
void printAt(char c, int x, int y);
void printChar(char c);
void print(char c);
void print(const char* str);
void print(i32 x);
void print(u32 x);


// Special number base formatting "tag" struct.
template<typename Int>
struct IntBase {
  Int val;
  int base;
  constexpr IntBase(Int val, int base) : val(val), base(base) {}
};

template<typename Int>
constexpr IntBase<Int> hex(Int x) {
  return IntBase<Int>(x, 16);
}

template<typename Int>
constexpr IntBase<Int> oct(Int x) {
  return IntBase<Int>(x, 8);
}

template<typename Int>
constexpr IntBase<Int> bin(Int x) {
  return IntBase<Int>(x, 2);
}

template<typename Int>
void print(IntBase<Int> x) {
  printInt(x.val, x.base);
}

// No-op base case for the variadic template print function.
inline void print() {}

// Variadic print function which calls print(arg) for each argument it gets.
template<typename First, typename Second, typename... Rest>
void print(First first, Second second, Rest... rest) {
  print(first);
  print(second);
  print(rest...);
}

// Variadic print function that adds a newline at the end.
template<typename... Args>
void println(Args... args) {
  print(args..., '\n');
}

inline void printWithFormat(int x, const char*) {
  print(x);
}

inline void printf(const char* format) {
  print(format);
}

template<typename First, typename... Rest>
void printf(const char* format, First firstArg, Rest... restArgs) {
  const u32 SPECIFIER_MAX_LENGTH = 64;
  char specifier[SPECIFIER_MAX_LENGTH];
  u32 specifierIndex = 0;
  bool parsingSpecifier = false;
  bool escaped = false;

  for (const char* p = format; *p != '\0'; ++p) {
    char c = *p;
    if (escaped) {
      if (c == '\\' || c == '{') {
        print(c);
      } else {
        print('\\', c);
      }
      escaped = false;
    } else if (c == '{' && !parsingSpecifier) {
      parsingSpecifier = true;
    } else if (parsingSpecifier) {
      if (c == '}') {
        specifier[specifierIndex] = '\0';
        printWithFormat(firstArg, specifier);
        printf(p + 1, restArgs...);
        return;
      } else {
        specifier[specifierIndex] = c;
        ++specifierIndex;
      }
    } else if (c == '\\') {
      escaped = true;
    } else {
      print(c);
    }
  }

  if (escaped) {
    print('\\');
  } else if (parsingSpecifier) {
    // TODO: Unmatched { hit end of string. Should panic.
  }
}

} // namepspace display

#endif // DISPLAY_H
