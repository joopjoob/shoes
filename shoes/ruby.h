//
// shoes/ruby.h
// Some defs for Ruby.
//
#include <ruby.h>
#include <rubysig.h>
#include "shoes/canvas.h"

#if defined(__cplusplus)
extern "C" {
#if 0
} /* satisfy cc-mode */
#endif
#endif

#include <st.h>

#if defined(__cplusplus)
#if 0
{ /* satisfy cc-mode */
#endif
}  /* extern "C" { */
#endif

#ifndef SHOES_RUBY_H
#define SHOES_RUBY_H

#if SHOES_WIN32
typedef VALUE (*HOOK)(...);
typedef int (*FOREACH)(...);
#define CASTFOREACH(x) reinterpret_cast<FOREACH>(x)
#define CASTHOOK(x) reinterpret_cast<HOOK>(x)
#undef fopen
#undef fclose
#undef fread
#else
typedef VALUE (*HOOK)();
#define CASTHOOK(x) x
#define CASTFOREACH(x) x
#endif

#ifdef WORDS_BIGENDIAN
#define BE_CPU_N(x, n)
#define LE_CPU_N(x, n) flip_endian((unsigned char *)(x), n) 
#else
#define BE_CPU_N(x, n) flip_endian((unsigned char *)(x), n) 
#define LE_CPU_N(x, n)
#endif

#define BE_CPU(x) BE_CPU_N(&(x), sizeof(x)) 
#define LE_CPU(x) LE_CPU_N(&(x), sizeof(x)) 

static inline void flip_endian(unsigned char* x, int length) {
  int i;
  unsigned char tmp;

  for(i = 0; i < (length / 2); i++) {
    tmp = x[i];
    x[i] = x[length - i - 1];
    x[length - i - 1] = tmp;
  }
}

#ifndef RARRAY_LEN
#define RARRAY_LEN(arr)  RARRAY(arr)->len
#define RSTRING_LEN(str) RSTRING(str)->len
#define RSTRING_PTR(str) RSTRING(str)->ptr
#endif

#undef s_host

extern VALUE cShoes, cApp, cDialog, cShoesWindow, cMouse, cCanvas, cFlow, cStack, cMask, cNative, cShape, cVideo, cImage, cEffect, cBlur, cShadow, cGlow, cEvery, cTimer, cAnim, cPattern, cBorder, cBackground, cPara, cBanner, cTitle, cSubtitle, cTagline, cCaption, cInscription, cLinkText, cTextBlock, cTextClass, cSpan, cStrong, cSub, cSup, cCode, cDel, cEm, cIns, cButton, cEditLine, cEditBox, cListBox, cProgress, cCheck, cRadio, cColor, cDownload, cResponse, cColors, cLink, cLinkHover, ssNestSlot;
extern VALUE aMsgList;
extern VALUE eInvMode, eNotImpl, eImageError;
extern VALUE reHEX_SOURCE, reHEX3_SOURCE, reRGB_SOURCE, reRGBA_SOURCE, reGRAY_SOURCE, reGRAYA_SOURCE, reLF;
extern VALUE symAltQuest, symAltSlash, symAltDot;
extern ID s_aref, s_mult, s_perc, s_bind, s_gsub, s_keys, s_update, s_new, s_run, s_to_pattern, s_to_i, s_to_s, s_URI, s_angle, s_arrow, s_autoplay, s_begin, s_call, s_center, s_change, s_choose, s_click, s_corner, s_curve, s_distance, s_displace_left, s_displace_top, s_downcase, s_draw, s_end, s_fill, s_finish, s_font, s_group, s_hand, s_hidden, s_host, s_hover, s_href, s_inner, s_insert, s_items, s_keypress, s_link, s_motion, s_path, s_port, s_progress, s_release, s_request_uri, s_save, s_wheel, s_scroll, s_start, s_attach, s_leading, s_leave, s_outer, s_points, s_match, s_text, s_title, s_top, s_right, s_bottom, s_left, s_up, s_down, s_height, s_resizable, s_remove, s_strokewidth, s_width, s_margin, s_margin_left, s_margin_right, s_margin_top, s_margin_bottom, s_radius, s_secret, s_now, s_debug, s_error, s_warn, s_info, s_rect, s_oval, s_line, s_star;
extern VALUE instance_eval_proc;

VALUE mfp_instance_eval(VALUE, VALUE);
VALUE ts_funcall2(VALUE, ID, int, VALUE *);
long rb_ary_index_of(VALUE, VALUE);
VALUE rb_ary_insert_at(VALUE, long, int, VALUE);
VALUE shoes_safe_block(VALUE, VALUE, VALUE);
void shoes_ruby_init(void);

#define BEZIER 0.55228475;

//
// Exception handling strings for eval
//
#define SHOES_META \
  "(class << Shoes; self; end).instance_eval do;"
#ifdef SHOES_WIN32
#define QUIT_ALERT_MSG() MessageBox(NULL, RSTRING_PTR(msg), "Shoes", MB_OK)
#else
#define QUIT_ALERT_MSG() printf("%s\n", RSTRING_PTR(msg))
#endif
#define QUIT_ALERT(v) \
   VALUE line_re = rb_eval_string("/:\\d+:\\s*/"); \
   VALUE msg = rb_funcall(v, rb_intern("message"), 0); \
   VALUE msg2 = rb_funcall(msg, rb_intern("split"), 2, line_re, INT2NUM(2)); \
   if (RARRAY_LEN(msg2) > 1) msg = rb_ary_entry(msg2, 1); \
   QUIT_ALERT_MSG(); \
   return SHOES_QUIT;

#define NUM2RGBINT(x) (rb_obj_is_kind_of(x, rb_cFloat) ? NUM2DBL(x) * 255 : NUM2INT(x))
#define DEF_COLOR(name, r, g, b) rb_hash_aset(cColors, ID2SYM(rb_intern("" # name)), shoes_color_new(r, g, b, 255))
#define GET_STRUCT(ele, var) \
  shoes_##ele *var; \
  Data_Get_Struct(self, shoes_##ele, var)


//
// Common funcs for dealing with attribute hashes
//
#define ATTR(attr, n)                  shoes_hash_get(attr, s_##n)
#define PX(attr, n, dn, pn)            shoes_px(shoes_hash_get(attr, s_##n), dn, pn, 1)
#define PXN(attr, n, dn, pn)            shoes_px(shoes_hash_get(attr, s_##n), dn, pn, 0)
#define PX2(attr, n1, n2, dn, dr, pn)  shoes_px2(attr, s_##n1, s_##n2, dn, dr, pn)
#define ATTR2(typ, attr, n, dn)        shoes_hash_##typ(attr, s_##n, dn)
#define ATTRSET(attr, k, v)            attr = shoes_hash_set(attr, s_##k, v)
#define ATTR_MARGINS(attr, dm, canvas) \
  int lmargin, rmargin, tmargin, bmargin; \
  VALUE margino = ATTR(attr, margin); \
  if (rb_obj_is_kind_of(margino, rb_cArray)) \
  { \
    lmargin = shoes_px(rb_ary_entry(margino, 0), dm, CPW(canvas), 1); \
    tmargin = shoes_px(rb_ary_entry(margino, 1), dm, CPH(canvas), 1); \
    rmargin = shoes_px(rb_ary_entry(margino, 2), dm, CPW(canvas), 1); \
    bmargin = shoes_px(rb_ary_entry(margino, 3), dm, CPH(canvas), 1); \
  } \
  else \
  { \
    lmargin = rmargin = PX(attr, margin, dm, CPW(canvas)); \
    tmargin = bmargin = PX(attr, margin, dm, CPH(canvas)); \
  } \
  lmargin = PX(attr, margin_left, lmargin, CPW(canvas)); \
  rmargin = PX(attr, margin_right, rmargin, CPW(canvas)); \
  tmargin = PX(attr, margin_top, tmargin, CPH(canvas)); \
  bmargin = PX(attr, margin_bottom, bmargin, CPH(canvas))

#define CHECK_HOVER(self_t, h, touch) \
  if ((self_t->hover & HOVER_MOTION) != h && !NIL_P(self_t->attr)) \
  { \
    VALUE action = ID2SYM(h ? s_hover : s_leave); \
    VALUE proc = rb_hash_aref(self_t->attr, action); \
    if (!NIL_P(proc)) \
      shoes_safe_block(self, proc, rb_ary_new3(1, self)); \
    if (touch != NULL) *touch += 1; \
    self_t->hover = (self_t->hover & HOVER_CLICK) | h; \
  }

#define IS_INSIDE(self_t, x, y) \
  (self_t->place.iw > 0 && self_t->place.ih > 0 && \
   x >= self_t->place.ix + self_t->place.dx && \
   x <= self_t->place.ix + self_t->place.dx + self_t->place.iw && \
   y >= self_t->place.iy + self_t->place.dy && \
   y <= self_t->place.iy + self_t->place.dy + self_t->place.ih)

int shoes_px(VALUE, int, int, int);
int shoes_px2(VALUE, ID, ID, int, int, int);
VALUE shoes_hash_set(VALUE, ID, VALUE);
VALUE shoes_hash_get(VALUE, ID);
int shoes_hash_int(VALUE, ID, int);
double shoes_hash_dbl(VALUE, ID, double);
char *shoes_hash_cstr(VALUE, ID, char *);
VALUE rb_str_to_pas(VALUE);
void shoes_place_exact(shoes_place *, VALUE, int, int);
void shoes_place_decide(shoes_place *, VALUE, VALUE, int, int, unsigned char, int);
void shoes_ele_remove_all(VALUE);
void shoes_cairo_rect(cairo_t *, double, double, double, double, double);

#define CANVAS_DEFS(f) \
  f(".close", close, 0); \
  f(".gutter", get_gutter_width, 0); \
  f(".nostroke", nostroke, 0); \
  f(".stroke", stroke, -1); \
  f(".strokewidth", strokewidth, 1); \
  f(".nofill", nofill, 0); \
  f(".fill", fill, -1); \
  f("+rect", rect, -1); \
  f("+oval", oval, -1); \
  f("+line", line, -1); \
  f("+arrow", arrow, -1); \
  f("+star", star, -1); \
  f("+para", para, -1); \
  f("+banner", banner, -1); \
  f("+title", title, -1); \
  f("+subtitle", subtitle, -1); \
  f("+tagline", tagline, -1); \
  f("+caption", caption, -1); \
  f("+inscription", inscription, -1); \
  f(".code", code, -1); \
  f(".del", del, -1); \
  f(".em", em, -1); \
  f(".ins", ins, -1); \
  f(".link", link, -1); \
  f(".span", span, -1); \
  f(".strong", strong, -1); \
  f(".sub", sub, -1); \
  f(".sup", sup, -1); \
  f("+background", background, -1); \
  f("+border", border, -1); \
  f("+video", video, -1); \
  f(".blur", blur, -1); \
  f(".glow", glow, -1); \
  f(".shadow", shadow, -1); \
  f("+image", image, -1); \
  f(".imagesize", imagesize, 1); \
  f("+animate", animate, -1); \
  f("+every", every, -1); \
  f("+timer", timer, -1); \
  f("+shape", shape, -1); \
  f("+move_to", move_to, 2); \
  f("+line_to", line_to, 2); \
  f("+curve_to", curve_to, 6); \
  f(".transform", transform, 1); \
  f(".translate", translate, 2); \
  f(".rotate", rotate, 1); \
  f(".scale", scale, -1); \
  f(".skew", skew, -1); \
  f(".push", push, 0); \
  f(".pop", pop, 0); \
  f(".reset", reset, 0); \
  f("+button", button, -1); \
  f("+list_box", list_box, -1); \
  f("+edit_line", edit_line, -1); \
  f("+edit_box", edit_box, -1); \
  f("+progress", progress, -1); \
  f("+check", check, -1); \
  f("+radio", radio, -1); \
  f(".app", get_app, 0); \
  f(".after", after, -1); \
  f(".before", before, -1); \
  f(".append", append, -1); \
  f(".prepend", prepend, -1); \
  f("+flow", flow, -1); \
  f("+stack", stack, -1); \
  f("+mask", mask, -1); \
  f("+widget", widget, -1); \
  f(".start", start, -1); \
  f(".finish", finish, -1); \
  f(".hover", hover, -1); \
  f(".leave", leave, -1); \
  f(".click", click, -1); \
  f(".release", release, -1); \
  f(".motion", motion, -1); \
  f(".keypress", keypress, -1); \
  f(".clear", clear_contents, -1); \
  f(".visit", goto, 1); \
  f(".mouse", mouse, 0); \
  f(".cursor", get_cursor, 0); \
  f(".cursor=", set_cursor, 1); \
  f(".clipboard", get_clipboard, 0); \
  f(".clipboard=", set_clipboard, 1); \
  f(".download", download, -1); \
  f(".owner", owner, 0); \
  f(".window", window, -1); \
  f(".dialog", dialog, -1); \
  f(".window_plain", window_plain, 0); \
  f(".dialog_plain", dialog_plain, 0); \
  f("._snapshot", snapshot, -1)

#endif
