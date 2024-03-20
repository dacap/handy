// handy text editor
// Copyright (c) 2021-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "os/os.h"
#include "text/text.h"

#include "term.h"

#include <algorithm>
#include <vector>

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
    fg.color(gfx::rgba(255, 255, 255));
    fg.style(os::Paint::Fill);
    bg.color(gfx::rgba(0, 0, 0));
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

  int get_char() override {
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
          if (ev.modifiers() == os::kKeyCtrlModifier &&
              ev.scancode() >= os::kKeyA &&
              ev.scancode() <= os::kKeyZ) {
            return ev.scancode() - os::kKeyA + 1;
          }

          switch (ev.scancode()) {
            case os::kKeyLShift:
            case os::kKeyRShift:
            case os::kKeyAlt:
            case os::kKeyAltGr:
            case os::kKeyLControl:
            case os::kKeyRControl:
            case os::kKeyLWin:
            case os::kKeyRWin:
            case os::kKeyMenu:
            case os::kKeyCommand:
              break;
            case os::kKeyEnter:
            case os::kKeyEnterPad: return 10;
            case os::kKeyDown: return 258;
            case os::kKeyUp: return 259;
            case os::kKeyLeft:
              if (ev.modifiers() & os::kKeyShiftModifier)
                return 393;     // KEY_SLEFT
              else
                return KEY_LEFT;
            case os::kKeyRight:
              if (ev.modifiers() & os::kKeyShiftModifier)
                return 402;     // KEY_SRIGHT
              else
                return KEY_RIGHT;
            case os::kKeyBackspace: return 127;
            case os::kKeyPageUp: return KEY_PPAGE;
            case os::kKeyPageDown: return KEY_NPAGE;
            default:
              return ev.unicodeChar();
          }
        }
      }
    }
    return 0;
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
