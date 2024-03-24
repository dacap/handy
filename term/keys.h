// handy term library
// Copyright (c) 2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base/codepoint.h"
#include "base/ints.h"

using Codepoint = base::codepoint_t;

struct Key {

  enum class Scancode : int32_t {
    NoScancode = 0,

    // Writing system keys
    // Based on https://www.w3.org/TR/uievents-code/#key-alphanumeric-writing-system

    Backquote,
    Backslash,
    BracketLeft,
    BracketRight,
    Comma,
    Digit0,
    Digit1,
    Digit2,
    Digit3,
    Digit4,
    Digit5,
    Digit6,
    Digit7,
    Digit8,
    Digit9,
    Equal,
    IntlBackslash,
    IntlRo,
    IntlYen,
    KeyA,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,
    Minus,
    Period,
    Quote,
    Semicolon,
    Slash,

    // Functional keys
    // Based on https://www.w3.org/TR/uievents-code/#key-alphanumeric-functional

    AltLeft,
    AltRight,
    Backspace,
    CapsLock,
    ContextMenu,
    ControlLeft,
    ControlRight,
    Enter,
    MetaLeft,
    MetaRight,
    ShiftLeft,
    ShiftRight,
    Space,
    Tab,

    // Functional keys found on Japanese and Korean keyboards

    Convert,
    KanaMode,
    Lang1,
    Lang2,
    Lang3,
    Lang4,
    Lang5,
    NonConvert,

    // Control Pad section
    // Based on https://www.w3.org/TR/uievents-code/#key-controlpad-section

    Delete,
    End,
    Help,
    Home,
    Insert,
    PageDown,
    PageUp,

    // Arrow Pad section
    // Based on https://www.w3.org/TR/uievents-code/#key-arrowpad-section

    ArrowDown,
    ArrowLeft,
    ArrowRight,
    ArrowUp,

    // Num Pad section
    // https://www.w3.org/TR/uievents-code/#key-numpad-section

    NumLock,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    NumpadAdd,
    NumpadBackspace,
    NumpadClear,
    NumpadClearEntry,
    NumpadComma,
    NumpadDecimal,
    NumpadDivide,
    NumpadEnter,
    NumpadEqual,
    NumpadHash,
    NumpadMemoryAdd,
    NumpadMemoryClear,
    NumpadMemoryRecall,
    NumpadMemoryStore,
    NumpadMemorySubtract,
    NumpadMultiply,
    NumpadParenLeft,
    NumpadParenRight,
    NumpadStar,
    NumpadSubtract,

    // Function section
    // Based on https://www.w3.org/TR/uievents-code/#key-function-section

    Escape,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    Fn,
    FnLock,
    PrintScreen,
    ScrollLock,
    Pause,

  };

  enum class Modifiers {
    NoModifiers = 0,
    Ctrl = 1,
    Shift = 2,
    Alt = 4,
    Meta = 8,
  };

  Codepoint codepoint = 0;      // Unicode codepoint
  Scancode scancode = Scancode::NoScancode;
  Modifiers modifiers = Modifiers::NoModifiers;
  bool repeat = false;

  bool hasModifiers(Modifiers m) const {
    return ((int(modifiers) & int(m)) == int(m) ? true: false);
  }

  bool ctrlKey() const { return hasModifiers(Modifiers::Ctrl); }
  bool shiftKey() const { return hasModifiers(Modifiers::Shift); }
  bool altKey() const { return hasModifiers(Modifiers::Alt); }
  bool metaKey() const { return hasModifiers(Modifiers::Meta); }
};
