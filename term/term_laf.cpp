// handy text editor
// Copyright (c) 2021-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "os/os.h"
#include "text/text.h"

#include "event.h"
#include "term.h"

#include <algorithm>
#include <vector>

static Key::Scancode from_laf_scancode(const os::KeyScancode from) {
  static Key::Scancode map[] {
    Key::Scancode::NoScancode,     // kKeyNil
    Key::Scancode::KeyA,           // kKeyA
    Key::Scancode::KeyB,           // kKeyB
    Key::Scancode::KeyC,           // kKeyC
    Key::Scancode::KeyD,           // kKeyD
    Key::Scancode::KeyE,           // kKeyE
    Key::Scancode::KeyF,           // kKeyF
    Key::Scancode::KeyG,           // kKeyG
    Key::Scancode::KeyH,           // kKeyH
    Key::Scancode::KeyI,           // kKeyI
    Key::Scancode::KeyJ,           // kKeyJ
    Key::Scancode::KeyK,           // kKeyK
    Key::Scancode::KeyL,           // kKeyL
    Key::Scancode::KeyM,           // kKeyM
    Key::Scancode::KeyN,           // kKeyN
    Key::Scancode::KeyO,           // kKeyO
    Key::Scancode::KeyP,           // kKeyP
    Key::Scancode::KeyQ,           // kKeyQ
    Key::Scancode::KeyR,           // kKeyR
    Key::Scancode::KeyS,           // kKeyS
    Key::Scancode::KeyT,           // kKeyT
    Key::Scancode::KeyU,           // kKeyU
    Key::Scancode::KeyV,           // kKeyV
    Key::Scancode::KeyW,           // kKeyW
    Key::Scancode::KeyX,           // kKeyX
    Key::Scancode::KeyY,           // kKeyY
    Key::Scancode::KeyZ,           // kKeyZ
    Key::Scancode::Digit0,         // kKey0
    Key::Scancode::Digit1,         // kKey1
    Key::Scancode::Digit2,         // kKey2
    Key::Scancode::Digit3,         // kKey3
    Key::Scancode::Digit4,         // kKey4
    Key::Scancode::Digit5,         // kKey5
    Key::Scancode::Digit6,         // kKey6
    Key::Scancode::Digit7,         // kKey7
    Key::Scancode::Digit8,         // kKey8
    Key::Scancode::Digit9,         // kKey9
    Key::Scancode::Numpad0,        // kKey0Pad
    Key::Scancode::Numpad1,        // kKey1Pad
    Key::Scancode::Numpad2,        // kKey2Pad
    Key::Scancode::Numpad3,        // kKey3Pad
    Key::Scancode::Numpad4,        // kKey4Pad
    Key::Scancode::Numpad5,        // kKey5Pad
    Key::Scancode::Numpad6,        // kKey6Pad
    Key::Scancode::Numpad7,        // kKey7Pad
    Key::Scancode::Numpad8,        // kKey8Pad
    Key::Scancode::Numpad9,        // kKey9Pad
    Key::Scancode::F1,             // kKeyF1
    Key::Scancode::F2,             // kKeyF2
    Key::Scancode::F3,             // kKeyF3
    Key::Scancode::F4,             // kKeyF4
    Key::Scancode::F5,             // kKeyF5
    Key::Scancode::F6,             // kKeyF6
    Key::Scancode::F7,             // kKeyF7
    Key::Scancode::F8,             // kKeyF8
    Key::Scancode::F9,             // kKeyF9
    Key::Scancode::F10,            // kKeyF10
    Key::Scancode::F11,            // kKeyF11
    Key::Scancode::F12,            // kKeyF12
    Key::Scancode::Escape,         // kKeyEsc
    Key::Scancode::Backquote,      // kKeyTilde
    Key::Scancode::Minus,          // kKeyMinus
    Key::Scancode::Equal,          // kKeyEquals
    Key::Scancode::Backspace,      // kKeyBackspace
    Key::Scancode::Tab,            // kKeyTab
    Key::Scancode::BracketLeft,    // kKeyOpenbrace
    Key::Scancode::BracketRight,   // kKeyClosebrace
    Key::Scancode::Enter,          // kKeyEnter
    Key::Scancode::NoScancode,     // kKeyColon
    Key::Scancode::Quote,          // kKeyQuote
    Key::Scancode::Backslash,      // kKeyBackslash
    Key::Scancode::IntlBackslash,  // kKeyBackslash2
    Key::Scancode::Comma,          // kKeyComma
    Key::Scancode::Period,         // kKeyStop
    Key::Scancode::Slash,          // kKeySlash
    Key::Scancode::Space,          // kKeySpace
    Key::Scancode::Insert,         // kKeyInsert
    Key::Scancode::Delete,         // kKeyDel
    Key::Scancode::Home,           // kKeyHome
    Key::Scancode::End,            // kKeyEnd
    Key::Scancode::PageUp,         // kKeyPageUp
    Key::Scancode::PageDown,       // kKeyPageDown
    Key::Scancode::ArrowLeft,      // kKeyLeft
    Key::Scancode::ArrowRight,     // kKeyRight
    Key::Scancode::ArrowUp,        // kKeyUp
    Key::Scancode::ArrowDown,      // kKeyDown
    Key::Scancode::NumpadDivide,   // kKeySlashPad
    Key::Scancode::NumpadMultiply, // kKeyAsterisk
    Key::Scancode::NumpadSubtract, // kKeyMinusPad
    Key::Scancode::NumpadAdd,      // kKeyPlusPad
    Key::Scancode::NumpadDecimal,  // kKeyDelPad
    Key::Scancode::NumpadEnter,    // kKeyEnterPad
    Key::Scancode::PrintScreen,    // kKeyPrtscr
    Key::Scancode::Pause,          // kKeyPause
    Key::Scancode::NoScancode,     // kKeyAbntC1
    Key::Scancode::IntlYen,        // kKeyYen
    Key::Scancode::KanaMode,       // kKeyKana
    Key::Scancode::Convert,        // kKeyConvert
    Key::Scancode::NonConvert,     // kKeyNoconvert
    Key::Scancode::NoScancode,     // kKeyAt
    Key::Scancode::NoScancode,     // kKeyCircumflex
    Key::Scancode::NoScancode,     // kKeyColon2
    Key::Scancode::NoScancode,     // kKeyKanji
    Key::Scancode::NumpadEqual,    // kKeyEqualsPad
    Key::Scancode::Backquote,      // kKeyBackquote
    Key::Scancode::Semicolon,      // kKeySemicolon
    Key::Scancode::NoScancode,     // kKeyUnknown1
    Key::Scancode::NoScancode,     // kKeyUnknown2
    Key::Scancode::NoScancode,     // kKeyUnknown3
    Key::Scancode::NoScancode,     // kKeyUnknown4
    Key::Scancode::NoScancode,     // kKeyUnknown5
    Key::Scancode::NoScancode,     // kKeyUnknown6
    Key::Scancode::NoScancode,     // kKeyUnknown7
    Key::Scancode::NoScancode,     // kKeyUnknown8
    Key::Scancode::NoScancode,     // kKeyLShift
    Key::Scancode::NoScancode,     // kKeyRShift
    Key::Scancode::NoScancode,     // kKeyLControl
    Key::Scancode::NoScancode,     // kKeyRControl
    Key::Scancode::NoScancode,     // kKeyAlt
    Key::Scancode::NoScancode,     // kKeyAltGr
    Key::Scancode::NoScancode,     // kKeyLWin
    Key::Scancode::NoScancode,     // kKeyRWin
    Key::Scancode::NoScancode,     // kKeyMenu
    Key::Scancode::NoScancode,     // kKeyCommand
    Key::Scancode::NoScancode,     // kKeyScrLock
    Key::Scancode::NoScancode,     // kKeyNumLock
    Key::Scancode::NoScancode,     // kKeyCapsLock
  };
  if (from >= 0 && from < os::kKeyScancodes)
    return map[from];
  else
    return Key::Scancode::NoScancode;
}

static Key::Modifiers from_laf_modifiers(const os::KeyModifiers from) {
  Key::Modifiers to = Key::Modifiers::NoModifiers;
  if (from & os::kKeyShiftModifier)
    to = Key::Modifiers(int(to) | int(Key::Modifiers::Shift));
  if (from & os::kKeyCtrlModifier)
    to = Key::Modifiers(int(to) | int(Key::Modifiers::Ctrl));
  if (from & os::kKeyAltModifier)
    to = Key::Modifiers(int(to) | int(Key::Modifiers::Alt));
  if (from & (os::kKeyCmdModifier | os::kKeyWinModifier))
    to = Key::Modifiers(int(to) | int(Key::Modifiers::Meta));
  return to;
}

class TermDelegate {
public:
  virtual ~TermDelegate() { }
  virtual os::Window* window() = 0;
  virtual gfx::Size charSize() = 0;
  virtual text::FontRef font() = 0;
  virtual text::FontMgrRef fontMgr() = 0;
};

static Panel* m_curPanel = nullptr;

class PanelLaf : public Panel {
  constexpr static const uint8_t NORMAL = 0;
  constexpr static const uint8_t REVERSE = 1;

  gfx::Rect m_rc;
  gfx::Point m_pt;
  std::vector<uint8_t> m_buffer;
  std::vector<uint8_t> m_attr;
  TermDelegate* m_delegate;
  uint8_t m_curAttr = NORMAL;

public:

  PanelLaf(const gfx::Rect& rc,
           TermDelegate* delegate)
    : m_rc(rc)
    , m_delegate(delegate) {
    m_buffer.resize(rc.w*rc.h, 0);
    m_attr.resize(rc.w*rc.h, 0);
  }

  ~PanelLaf() override {
    if (this == m_curPanel)
      m_curPanel = nullptr;
  }

  int x() const override {
    return m_rc.x;
  }

  int y() const override {
    return m_rc.y;
  }

  int width() const override {
    return m_rc.w;
  }

  int height() const override {
    return m_rc.h;
  }

  void clear() override {
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
    std::fill(m_attr.begin(), m_attr.end(), 0);
    m_pt = gfx::Point(0, 0);
  }

  void print(const char* s) override {
    for (; *s; ++s)
      print(*s);
  }

  void print(const char ch) override {
    const int i = m_pt.y*m_rc.w + m_pt.x;

    if (ch == 10) {
      if (i >= 0 && i < int(m_buffer.size()))
        m_attr[i] |= m_curAttr;

      m_pt.x = 0;
      ++m_pt.y;
      if (m_pt.y >= m_rc.h)
        m_pt.y = m_rc.h-1;
      return;
    }

    if (i >= 0 && i < int(m_buffer.size())) {
      m_buffer[i] = ch;
      m_attr[i] |= m_curAttr;
    }

    ++m_pt.x;
    if (m_pt.x >= m_rc.w) {
      m_pt.x = 0;
      ++m_pt.y;
      if (m_pt.y >= m_rc.h)
        m_pt.y = m_rc.h-1;
    }
  }

  void move(int x, int y) override {
    m_pt = gfx::Point(x, y);
    m_curPanel = this;
  }

  void update() override {
    os::Window* window = m_delegate->window();
    os::Surface* surface = window->surface();
    text::FontRef font = m_delegate->font();
    gfx::Size charSize = m_delegate->charSize();
    os::Paint fg, bg;
    fg.color(gfx::rgba(230, 230, 230));
    fg.style(os::Paint::Fill);
    bg.color(gfx::rgba(46, 46, 50));
    bg.style(os::Paint::Fill);

    gfx::Region rgn;
    gfx::Point pt(m_rc.x*charSize.w,
                  m_rc.y*charSize.h);
    char buf[32];
    for (int v=0; v<m_rc.h; ++v) {
      for (int u=0; u<m_rc.w; ++u) {
        int at = m_attr[v*m_rc.w + u];
        int ch = m_buffer[v*m_rc.w + u];
        if (ch == 10) {
          pt.x = m_rc.x*charSize.w;
          pt.y += charSize.h;
          break;
        }

        buf[0] = ch;
        buf[1] = 0;

        gfx::Rect charRc(pt, charSize);
        rgn |= gfx::Region(charRc);
        surface->drawRect(charRc, (at & REVERSE ? fg: bg));

#if 1
        text::draw_text(
          surface, font, buf,
          gfx::PointF(pt.x+charSize.w/2,
                      pt.y+charSize.h*3/4),
          (at & REVERSE ? &bg: &fg),
          text::TextAlign::Center);
#else
        text::draw_text_with_shaper(
          surface, m_delegate->fontMgr(), font, buf,
          gfx::PointF(pt.x, pt.y),
          (at & REVERSE ? &bg: &fg),
          text::TextAlign::Left);
#endif

        pt.x += charSize.w;
      }
      pt.x = m_rc.x*charSize.w;
      pt.y += charSize.h;
    }

    // Draw text cursor
    if (this == m_curPanel) {
      fg.style(os::Paint::Stroke);
      gfx::Rect charRc((m_rc.x+m_pt.x)*charSize.w,
                       (m_rc.y+m_pt.y)*charSize.h,
                       charSize.w, charSize.h);
      surface->drawRect(charRc, fg);

      rgn |= gfx::Region(charRc);
    }

    window->invalidateRegion(rgn);
  }

  void update_lines(int y, int h) override {
    update();                   // TODO
  }

  // Gets next laf os::Event and converts it to our kind of Event.
  Event get_event() override {
    os::EventQueue* q = os::System::instance()->eventQueue();
    os::Event ev;
    while (true) {
      q->getEvent(ev, os::EventQueue::kWithoutTimeout);
      int ch = 0;
      switch (ev.type()) {

        case os::Event::ResizeWindow:
          update();
          break;

        case os::Event::KeyDown: {
          Event out(Event::Type::Key);
          Key key;

          key.codepoint = ev.unicodeChar();
          key.scancode = from_laf_scancode(ev.scancode());
          key.modifiers = from_laf_modifiers(ev.modifiers());
          key.repeat = (ev.repeat() > 0);

          out.key(key);
          return out;
        }
      }
    }

    return Event();
  }

  void attr_reverse() override {
    m_curAttr |= REVERSE;
  }

  void attr_normal() override {
    m_curAttr = 0;
  }

};

class TermLaf : public Term
              , public TermDelegate {
  os::SystemRef m_system;
  os::WindowRef m_window;
  text::FontMgrRef m_fontMgr;
  text::FontRef m_font;
  gfx::Size m_charSize;
  gfx::Size m_termSize;

public:
  TermLaf() {
    m_system = os::System::make();
    m_system->setAppMode(os::AppMode::GUI);
    // m_system->setTranslateDeadKeys(true);

    m_fontMgr = text::FontMgr::Make();

    // TODO load a mono-space font
    m_font = m_fontMgr->defaultFont(18);

    text::FontMetrics metrics;
    float width = metrics.avgCharWidth;
    float height = m_font->metrics(&metrics);
    if (width < 1.0f) {
      width = m_font->textLength("x")+2;
    }
    m_charSize = gfx::Size(width, height);

    m_window = m_system->makeWindow(m_charSize.w*80, m_charSize.h*25);
    m_window->setCursor(os::NativeCursor::Arrow);
    m_system->finishLaunching();
    m_system->activateApp();
    m_window->setVisible(true);

    m_termSize.w = m_window->surface()->width() / m_charSize.w;
    m_termSize.h = m_window->surface()->height() / m_charSize.h;
  }

  ~TermLaf() override {
  }

  int width() const override {
    return m_termSize.w;
  }

  int height() const override {
    return m_termSize.h;
  }

  PanelPtr makePanel(int x, int y, int w, int h) override {
    return std::make_shared<PanelLaf>(gfx::Rect(x, y, w, h),
                                      this);
  }

  // TermDelegate impl
  os::Window* window() override { return m_window.get(); }
  gfx::Size charSize() override { return m_charSize; }
  text::FontRef font() override { return m_font; }
  text::FontMgrRef fontMgr() override { return m_fontMgr; }

};

// static
TermPtr Term::MakeGUI() {
  return std::make_shared<TermLaf>();
}
