#ifndef SET
#define SET
#include "Files.h"
// enum exercise { Squat = 1, Bench, Deadlift };
struct date_str {
  date_str() : day(1), month(1), year(1900) {}
  int day;
  int month;
  int year;
};
class Set {
 public:
  Set();
  Set(int w, int r, int ID);
  Set(int w, int r, ManagedString name);
  Set(int w, int r, int ID, ManagedString name);
  ~Set();

  int get_weight() const;
  int get_reps() const;
  int get_ID() const;
  ManagedString get_name() const;

  void set_weight(int w);
  void set_reps(int r);
  void set_ID(int id);
  void set_name(ManagedString name);
  void set_date(int y, int m, int d);
  void write_to_file(ManagedString fn) const;   // myfiles append line
  void read_from_file(ManagedString fn) const;  // myFiles readtoserial

 private:
  int weight;
  int reps;
  int ID;
  ManagedString name;
  date_str date;
};
#endif  // !SET