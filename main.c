#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool windowShouldClose = false;

int year = 2024;
int current_month = 0;

void HandleKeys();
int GetDayOfWeek(int year, int month, int day);
char *PrintCalendar(int year);
void *safe_malloc(size_t n);

int main() {
  InitWindow(800, 450, "calendar");

  SetTargetFPS(60);

  while (!windowShouldClose) {
    char *calendar = PrintCalendar(2024);
    HandleKeys();
    if (WindowShouldClose())
      windowShouldClose = true;
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(TextFormat("     Calendar - %d\n\n", year), 300, 20, 20, RAYWHITE);
    DrawText(TextFormat("%s", calendar), 300, 200, 20, RAYWHITE);
    EndDrawing();
    free(calendar);
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
    current_month += 1;
  }
  if (IsKeyPressed(KEY_K)) {
    current_month -= 1;
  }
}

int dayOfWeek(int year, int month, int day) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  year -= month < 3;
  return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

typedef struct s_day {
  int dayOfWeek;
  int dayOfMonth;
} day;

typedef struct s_month {
  char *monthName;
  int numberOfDays;
  struct s_day day;
} month;

typedef struct s_date {
  struct s_month month;
} date;

char *getMonthName(int monthNumber) {
  switch (monthNumber) {
  case 0:
    return "January";
  case 1:
    return "February";
  case 2:
    return "March";
  case 3:
    return "April";
  case 4:
    return "May";
  case 5:
    return "June";
  case 6:
    return "July";
  case 7:
    return "August";
  case 8:
    return "September";
  case 9:
    return "October";
  case 10:
    return "November";
  case 11:
    return "December";
  default:
    return "Invalid month";
  }
}

int numberOfDays(int monthNumber, int year) {
  switch (monthNumber) {
  case 0:
    return 31;
  case 1:
    return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
  case 2:
    return 31;
  case 3:
    return 30;
  case 4:
    return 31;
  case 5:
    return 30;
  case 6:
    return 31;
  case 7:
    return 31;
  case 8:
    return 30;
  case 9:
    return 31;
  case 10:
    return 30;
  case 11:
    return 31;
  default:
    return 0;
  }
}

char *append(char *s1, const char *s2) {
  const size_t len1 = strlen(s1);
  const size_t len2 = strlen(s2);
  char *result = safe_malloc(len1 + len2 + 1);
  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1);
  free(s1);
  return result;
}

date createDate(int y, int m, int d) {
  day day;
  day.dayOfWeek = dayOfWeek(y, 1, 1);
  month month;
  month.monthName = getMonthName(m);
  month.numberOfDays = numberOfDays(m, y);
  date date;
  date.day = day;
  date.month = month;
  return date;
}

char *PrintCalendar(int year) {
  char *calendar = safe_malloc(1);
  memcpy(calendar, "", 1);
  date date = createDate(year, current_month, 1);
  calendar = append(calendar, TextFormat("\n ------------%s-------------\n",
                                         date.month.monthName));
  calendar = append(calendar, TextFormat(" Sun Mon Tue Wed Thu Fri Sat\n"));
  int k;
  for (k = 0; k < date.day.dayOfWeek; k++)
    printf("     ");
  for (int j = 1; j <= date.month.numberOfDays; j++) {
    calendar = append(calendar, TextFormat("%5d", j));
    if (++k > 6) {
      k = 0;
      calendar = append(calendar, TextFormat("\n"));
    }
  }
  date.day.dayOfWeek = k;
  return calendar;
}
