#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool windowShouldClose = false;

struct tm current_time;

const int w = 450, h = 600;
const int mw = w / 3, mh = h / 4;
const int p = 10;

void HandleKeys();
int GetDayOfWeek(int year, int month, int day);
void PrintCalendar();
void *safe_malloc(size_t n);

int main() {
  time_t t = time(NULL);
  current_time = *localtime(&t);
  InitWindow(w, h, "");

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
    current_time.tm_mon += 1;
  }
  if (IsKeyPressed(KEY_K)) {
    current_time.tm_mon -= 1;
  }
  if (IsKeyPressed(KEY_H)) {
    current_time.tm_mday -= 1;
  }
  if (IsKeyPressed(KEY_L)) {
    current_time.tm_mday += 1;
  }
}

int dayOfWeek(int year, int month, int day) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  year -= month < 3;
  return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

void PrintCalendar() {
  time_t t = time(NULL);
  current_time = *localtime(&t);
  current_time.tm_mon = 0;
  for (int j = 0; j < h; j += mh) {
    for (int i = 0; i < w; i += mw) {
      char *calendar = safe_malloc(0);
      strftime(calendar, 16, "%B  %Y", &current_time);
      DrawText(TextFormat(calendar), i + p, j + p, 12, WHITE);
      current_time.tm_mon += 1;
    }
  }
}
