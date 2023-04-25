#include "raylib.h"

int main() {
  InitWindow(853, 480, "Playground");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats!", 381, 230, 20, LIGHTGRAY);
    DrawFPS(10, 10);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
