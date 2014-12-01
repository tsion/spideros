#include "display.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "multiboot.h"
#include "ports.h"
#include "types.h"

namespace {
void runInit(const char* stage, void (*initFn)()) {
  display::print("Initializing ", stage, "... ");
  initFn();
  display::println("done.");
}
}

extern "C" void kmain(const multiboot::Info* mbinfo, u32 magic) {
  display::init();

  if (magic != multiboot::BOOTLOADER_MAGIC) {
    // Something went not according to specs. Do not rely on the
    // multiboot data structure.
    display::println("error: The bootloader's magic number didn't match. Something must have gone wrong.");
    return;
  }

  // Print to screen to see everything is working.
  display::clearScreen();
  display::println("Welcome to spideros");
  display::println("===================");

  if (mbinfo->hasFlag(multiboot::BOOTLOADER_NAME)) {
    display::println("Bootloader:\t", (const char*) mbinfo->bootloaderName);
  }

  if (mbinfo->hasFlag(multiboot::COMMAND_LINE)) {
    display::println("Command line:\t", (const char*) mbinfo->commandLine);
  }

  display::println();
  runInit("GDT", gdt::init);
  runInit("IDT", idt::init);
  runInit("interrupt handlers", interrupts::init);
  runInit("keyboard", keyboard::init);
  display::println();
  display::println("Type away: ");

  // Handle timer interrupts.
  interrupts::setIrqHandler(0, [](interrupts::Registers*) {
    // static int ticks = 0;
    // ticks++;
    // if (ticks % 100 == 0) {
    //   display::println("Ticks: ", ticks);
    // }
  });

  interrupts::enable();
  while (true) {
    keyboard::KeyEvent event = keyboard::readEvent();
    if (event.character) {
      display::print(event.character);
    }
  }
}
