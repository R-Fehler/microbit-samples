time_t seconds;
while (1) {
  seconds = 1542130731 + uBit.systemTime() / 1000;
  uBit.serial.printf("Time as seconds since January 1, 1970 = %d\n", seconds);

  uBit.serial.printf("Time as a basic string = %s", ctime(&seconds));

  char buffer[32];
  strftime(buffer, 32, "%I:%M %p\n", localtime(&seconds));
  uBit.serial.printf("Time as a custom formatted string = %s", buffer);
  uBit.sleep(1000);
}

/////////////////////////////////

time_t rawtime;
struct tm* timeinfo;
int year, month, day;
const char* weekday[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday"};

/* prompt user for date */
year = serialEingabeInt("Enter year:");
month = serialEingabeInt("Enter month:");
day = serialEingabeInt("Enter day:");

/* get current timeinfo and modify it to the user's choice */
time(&rawtime);
timeinfo = localtime(&rawtime);
timeinfo->tm_year = year - 1900;
timeinfo->tm_mon = month - 1;
timeinfo->tm_mday = day;

/* call mktime: timeinfo->tm_wday will be set */
rawtime = mktime(timeinfo);

uBit.serial.printf("That day is a %s.\n", weekday[timeinfo->tm_wday]);
uBit.serial.printf("Time as a basic string = %s", ctime(&rawtime));
