#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool windowShouldClose = false;

Font font;

struct tm current_time;

const int w = 700, h = 600;

void HandleKeys();
int GetDayOfWeek(int year, int month, int day);
void PrintCalendar();
void *safe_malloc(size_t n);

int main() {
  time_t t = time(NULL);
  current_time = *localtime(&t);
  InitWindow(w, h, "");

  char *fontPath = getenv("FONT_PATH");

  printf("Loading font %s\n", fontPath);
  font = LoadFont(fontPath);

  SetTargetFPS(60);

  while (!windowShouldClose) {
    HandleKeys();
    BeginDrawing();
    ClearBackground(BLACK);
    PrintCalendar();
    EndDrawing();
    if (WindowShouldClose())
      windowShouldClose = true;
  }

  CloseWindow();

  return 0;
}

void *safe_malloc(size_t n) {
  void *p = malloc(n);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
    abort();
  }
  return p;
}

void HandleKeys() {
  if (IsKeyDown(KEY_Q)) {
    windowShouldClose = true;
  }
  if (IsKeyPressed(KEY_J)) {
    current_time.tm_mday += 7;
    mktime(&current_time);
  }
  if (IsKeyPressed(KEY_K)) {
    current_time.tm_mday -= 7;
    mktime(&current_time);
  }
  if (IsKeyPressed(KEY_H)) {
    current_time.tm_mday -= 1;
    mktime(&current_time);
  }
  if (IsKeyPressed(KEY_L)) {
    current_time.tm_mday += 1;
    mktime(&current_time);
  }
  if (IsKeyPressed(KEY_R)) {
    time_t t = time(NULL);
    current_time = *localtime(&t);
  }
}

int isLeapYear(int year) {
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0)
        return 1; // Leap year
      else
        return 0; // Not a leap year
    } else
      return 1; // Leap year
  } else
    return 0; // Not a leap year
}

int getTotalDaysInMonth(struct tm *timeptr) {
  int month = timeptr->tm_mon;        // tm_mon is months since January (0-11)
  int year = timeptr->tm_year + 1900; // tm_year is years since 1900
  switch (month) {
  case 0:  // January
  case 2:  // March
  case 4:  // May
  case 6:  // July
  case 7:  // August
  case 9:  // October
  case 11: // December
    return 31;
  case 3:  // April
  case 5:  // June
  case 8:  // September
  case 10: // November
    return 30;
  case 1: // February
    if (isLeapYear(year))
      return 29;
    else
      return 28;
  default:
    return -1; // Invalid month
  }
}

int getTotalWeeksInMonth(struct tm *timeptr) {
  struct tm firstDay = *timeptr;
  firstDay.tm_mday = 1;
  mktime(&firstDay);
  int totalDays = getTotalDaysInMonth(timeptr);
  int firstWeekDay = firstDay.tm_wday;
  struct tm lastDay = *timeptr;
  lastDay.tm_mday = totalDays;
  mktime(&lastDay);
  int lastWeekDay = lastDay.tm_wday;
  int totalWeeks = (totalDays + firstWeekDay + (6 - lastWeekDay)) / 7;
  return totalWeeks;
}

void PrintCalendar() {
  const int dx = 30, dy = 15;
  const int totalDaysWeek = 7;
  const int monthWidth = w / 3, monthHeight = h / 4;
  const int p = 10;

  struct tm time = current_time;
  time.tm_mon = 0;
  time.tm_mday = 1;
  mktime(&time);
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 3; i++) {
      int x0 = (i * monthWidth) + p, y0 = (j * monthHeight) + p;
      int totalDays = getTotalDaysInMonth(&time);
      int totalWeeks = getTotalWeeksInMonth(&time);
      int weekday = time.tm_wday;
      char *month_name = safe_malloc(0);
      strftime(month_name, 16, "%B  %Y", &time);
      DrawTextEx(font, TextFormat(month_name), (Vector2){x0, y0}, 17, 2, PINK);
      for (time.tm_wday = 0; time.tm_wday < totalDaysWeek; time.tm_wday++) {
        int x = (x0 + (time.tm_wday * dx)), y = (y0 + dy);
        char *weekday = safe_malloc(0);
        strftime(weekday, 4, "%a", &time);
        DrawTextEx(font, TextFormat(weekday), (Vector2){x, y}, 15, 1, YELLOW);
      }
      int l = weekday;
      int k = 0;
      for (time.tm_mday = 1; time.tm_mday <= totalDays; time.tm_mday++) {
        int x = x0 + l++ * dx, y = y0 + p + k * dy + 20;
        l = l > totalDaysWeek - 1 ? 0 : l;
        if (l == 0) {
          k = k > totalWeeks - 1 ? 0 : k + 1;
        }
        char *day = safe_malloc(0);
        strftime(day, 3, "%d", &time);
        Color c = (time.tm_mday == current_time.tm_mday &&
                   time.tm_mon == current_time.tm_mon)
                      ? GREEN
                      : MAROON;
        DrawTextEx(font, TextFormat(day), (Vector2){x, y}, 15, 2, c);
      }
      time.tm_mon += 1;
      time.tm_mday = 1;
      mktime(&time);
    }
  }
}
