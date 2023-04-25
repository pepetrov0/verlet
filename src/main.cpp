#include "raylib.h"
#include <math.h>
#include <vector>

#define GRAVITY 250

typedef struct {
  int id;
  float x;
  float y;
  float old_x;
  float old_y;
  float acc_x;
  float acc_y;
  float r;
} Object;

typedef std::vector<Object> Objects;

void run_physics(Objects *objects, float dt) {
  for (auto it = objects->begin(); it != objects->end(); it++) {
    const auto cx = it->x;
    const auto cy = it->y;

    it->x = 2 * it->x - it->old_x + it->acc_x * dt * dt;
    it->y = 2 * it->y - it->old_y + it->acc_y * dt * dt;

    it->old_x = cx;
    it->old_y = cy;

    it->acc_x = 0;
    it->acc_y = 0;
  }
}

void apply_gravity(Objects *objects) {
  for (auto it = objects->begin(); it != objects->end(); it++) {
    it->acc_y += GRAVITY;
  }
}

void constrain_to_screen(Objects *objects, float x, float y, float r) {
  for (auto it = objects->begin(); it != objects->end(); it++) {
    const float dx = it->x - x;
    const float dy = it->y - y;
    const float d = sqrt(dx * dx + dy * dy);

    if (d <= r - it->r)
      continue;

    const float nx = dx / d;
    const float ny = dy / d;

    it->x = x + nx * (r - it->r);
    it->y = y + ny * (r - it->r);
  }
}

void solve_collisions(Objects *objects) {
  for (auto it0 = objects->begin(); it0 != objects->end(); it0++) {
    for (auto it1 = objects->begin(); it1 != objects->end(); it1++) {
      if (it0->id == it1->id)
        continue;

      const float tr = it0->r + it1->r;
      const float dx = it0->x - it1->x;
      const float dy = it0->y - it1->y;
      const float d = sqrt(dx * dx + dy * dy);

      if (d >= tr)
        continue;

      const float nx = dx / d;
      const float ny = dy / d;
      const float dd = tr - d;

      it0->x += 0.5f * dd * nx;
      it0->y += 0.5f * dd * ny;

      it1->x -= 0.5f * dd * nx;
      it1->y -= 0.5f * dd * ny;
    }
  }
}

int main() {
  SetConfigFlags(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
  InitWindow(853, 480, "Verlet");
  SetTargetFPS(60);

  float accumulator = 0.0f;

  int oida = 0;
  Objects objects;

  while (!WindowShouldClose()) {
    const auto mouse = GetMousePosition();
    const auto width = GetScreenWidth();
    const auto height = GetScreenHeight();
    const auto radius = fmin(width * 0.4f, height * 0.4f);
    const auto frametime = GetFrameTime();
    const auto dt = 1.0f / 60.0f;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      const auto o = Object{.id = oida++,
                            .x = mouse.x,
                            .y = mouse.y,
                            .old_x = mouse.x,
                            .old_y = mouse.y - 10,
                            .acc_x = 0,
                            .acc_y = 0,
                            .r = 10};
      objects.push_back(o);
    }

    accumulator += frametime;
    while (accumulator > dt) {
      accumulator -= dt;

      apply_gravity(&objects);
      constrain_to_screen(&objects, width / 2.0f, height / 2.0f, radius);
      solve_collisions(&objects);
      run_physics(&objects, dt);
    }

    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    DrawCircle(width / 2, height / 2, radius, RAYWHITE);

    for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
      DrawCircle(it->x, it->y, it->r, it->id % 2 ? DARKGRAY : BLACK);
    }

    DrawFPS(10, 10);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
