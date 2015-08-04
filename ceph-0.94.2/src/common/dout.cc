#include <iostream>

#include "dout.h"

void dout_emergency(const char * const str)
{
  std::cerr << str;
  std::cerr.flush();
}

void dout_emergency(const std::string &str)
{
  std::cerr << str;
  std::cerr.flush();
}

void
doutprobe_init(void)
{
//   extern doutconf_t *__start_set_doutconf, *__stop_set_doutconf;
//   doutconf_t **start = &__start_set_doutconf;
//   doutconf_t **stop = &__stop_set_doutconf;
//   doutconf_t **pp;

//   for (pp = start; pp < stop; pp++) {
//     std::cerr	<< "mlvl " << (*pp)->doc_mlvl
//		<< "file " << (*pp)->doc_file
//		<< "func " << (*pp)->doc_func
//		<< "line " << (*pp)->doc_line
//		<< "state" << (*pp)->doc_state;
//     std::cerr.flush();
//   }
//
}

static bool
doutprobe_parse(const string& pattern, string *m, string *f, string *n, string *l)
{
  size_t i, j;
  string *p[] = { m, f, n, l };
  int k, last = 4;

  // pattern is '[module]:[file]:[func]:[line]'.
  // there are 4 components to be extracted.
  // each component is located by [i, j).
  i = j = 0;
  for (k = 0; k < last; k++) {
    j = pattern.find(":", i);
    if (j == string::npos && k != last - 1)
      break;
    *p[k] = pattern.substr(i, j - i);
    i = j + 1;
  }
  return k == last;
}

void
doutprobe_show(Formatter *fmt, const string& pattern)
{
  extern doutprobe_t *__start_set_doutprobe, *__stop_set_doutprobe;
  doutprobe_t **start = &__start_set_doutprobe, **stop = &__stop_set_doutprobe;
  doutprobe_t **pp;

  string m, f, n, l;
  if (!doutprobe_parse(pattern, &m, &f, &n, &l)) {
    fmt->dump_string("error", "dout show module:file:func:line");
    return;
  }

  for (pp = start; pp < stop; pp++) {
    if ((m == "" || m.compare((*pp)->dop_module) == 0) &&
	(f == "" || f.compare((*pp)->dop_file) == 0) &&
	(n == "" || n.compare((*pp)->dop_func) == 0) &&
	(l == "" || atol(l.c_str()) == (*pp)->dop_line)) {
      fmt->open_object_section("");
      fmt->dump_string("module", (*pp)->dop_module);
      fmt->dump_string("file", (*pp)->dop_file);
      fmt->dump_string("func", (*pp)->dop_func);
      fmt->dump_int("line", (*pp)->dop_line);
      fmt->dump_int("state", (*pp)->dop_state);
      fmt->close_section();
    }
  }
}

void
doutprobe_set(const string& pattern, const string& val)
{
  extern doutprobe_t *__start_set_doutprobe, *__stop_set_doutprobe;
  doutprobe_t **start = &__start_set_doutprobe, **stop = &__stop_set_doutprobe;
  doutprobe_t **pp;

  string m, f, n, l;
  if (!doutprobe_parse(pattern, &m, &f, &n, &l)) {
    return;
  }

  for (pp = start; pp < stop; pp++) {
    if ((m == "" || m.compare((*pp)->dop_module) == 0) &&
	(f == "" || f.compare((*pp)->dop_file) == 0) &&
	(n == "" || n.compare((*pp)->dop_func) == 0) &&
	(l == "" || atol(l.c_str()) == (*pp)->dop_line)) {
      (*pp)->dop_state = atol(val.c_str()) != 0 ? 1 : 0;
    }
  }
}
