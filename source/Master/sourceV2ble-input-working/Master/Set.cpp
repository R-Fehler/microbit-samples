#include "Set.h"

Set::Set() : weight(0), reps(0), ID(0), name("none") {}
Set::Set(int w, int r, int ID) : weight(w), reps(r), ID(ID), name("none") {}
Set::Set(int w, int r, ManagedString name)
    : weight(w), reps(r), ID(0), name(name) {}
Set::Set(int w, int r, int ID, ManagedString name)
    : weight(w), reps(r), ID(ID), name(name) {}
Set::~Set(){};

int Set::get_weight() const { return weight; }
int Set::get_reps() const { return reps; }
int Set::get_ID() const { return ID; }
ManagedString Set::get_name() const { return name; }

void Set::set_weight(int w) { weight = w; }
void Set::set_reps(int r) { reps = r; }
void Set::set_ID(int id) { ID = id; }
void Set::set_name(ManagedString n) { name = n; }
void Set::set_date(int y, int m, int d) {
  date.year = y;
  date.month = m;
  date.day = d;
}
void Set::write_to_file(ManagedString fn) const {
  char buf[80];
  int ret =
      snprintf(buf, sizeof(buf), "%d/%d/%d , %s , %d , %d , %d ", date.year,
               date.month, date.day, name.toCharArray(), ID, weight, reps);
  appendLine(fn, ManagedString(buf));
}  // myfiles append line
void Set::read_from_file(ManagedString fn) const {}  // myFiles readtoserial
