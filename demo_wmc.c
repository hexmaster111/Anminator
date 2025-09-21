// cc demo_wmc.c -odemo_wmc -lraylib -lm

#include <raylib.h>
#include <raymath.h>

#include <rlgl.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "betterlist.h"

#define TURTLE_IMPL
#include "turtle.h"

#define ARRAY_SIZE(ARRAY) sizeof((ARRAY)) / sizeof((ARRAY)[0])

#define BG ((Color){0x21, 0x21, 0x21, 255})
// #define BG (WHITE)

#define CIRCLE_COLOR ((Color){0x06, 0x06, 0x08, 255})

struct ArchPart
{
    Vector2 points[5];
    Color color;
} arch_part[] = {
    // trans
    // {.points={{427.00000000,128.00000000},{446.00000000,233.00000000},{390.00000000,32.00000000},{329.00000000,33.00000000},{346.00000000,1051.00000000}}, .color={255,255,255,255}},
    // {.points={{473.00000000,328.00000000},{462.00000000,277.00000000},{452.00000000,242.00000000},{437.00000000,190.00000000},{419.00000000,149.00000000}}, .color={250,169,184,255}},
    // {.points={{345.00000000,150.00000000},{340.00000000,220.00000000},{348.00000000,270.00000000},{357.00000000,309.00000000},{418.00000000,427.00000000}}, .color={250,169,184,255}},
    // {.points={{290.00000000,109.00000000},{376.00000000,365.00000000},{412.00000000,421.00000000},{438.00000000,467.00000000},{550.00000000,566.00000000}}, .color={91,206,250,255}},
    // {.points={{425.00000000,88.00000000},{469.00000000,320.00000000},{479.00000000,360.00000000},{490.00000000,412.00000000},{533.00000000,646.00000000}}, .color={91,206,250,255}},

    // normal
    {.points = {{427.00000000, 128.00000000}, {446.00000000, 233.00000000}, {390.00000000, 32.00000000}, {329.00000000, 33.00000000}, {346.00000000, 1051.00000000}}, .color = {0x97, 0xaf, 0xe5, 255}},
    {.points = {{473.00000000, 328.00000000}, {462.00000000, 277.00000000}, {452.00000000, 242.00000000}, {437.00000000, 190.00000000}, {419.00000000, 149.00000000}}, .color = {0x77, 0x93, 0xde, 255}},
    {.points = {{345.00000000, 150.00000000}, {340.00000000, 220.00000000}, {348.00000000, 270.00000000}, {357.00000000, 309.00000000}, {418.00000000, 427.00000000}}, .color = {0x77, 0x93, 0xde, 255}},
    {.points = {{290.00000000, 109.00000000}, {376.00000000, 365.00000000}, {412.00000000, 421.00000000}, {438.00000000, 467.00000000}, {550.00000000, 566.00000000}}, .color = {0x54, 0x5d, 0xa4, 255}},
    {.points = {{425.00000000, 88.00000000}, {469.00000000, 320.00000000}, {479.00000000, 360.00000000}, {490.00000000, 412.00000000}, {533.00000000, 646.00000000}}, .color = {0x54, 0x5d, 0xa4, 255}},
};

void SavePoints()
{

    FILE *f = fopen("archpoints.h", "w+");

    if (f == NULL)
    {
        perror("fopen");
        f = stdout;
    }

    for (size_t i = 0; i < ARRAY_SIZE(arch_part); i++)
    {

        fprintf(f,
                "{.points={{%.8f,%.8f},{%.8f,%.8f},{%.8f,%.8f},{%.8f,%.8f},{%.8f,%.8f}}, .color={%d,%d,%d,%d}},\n",
                arch_part[i].points[0].x, arch_part[i].points[0].y,
                arch_part[i].points[1].x, arch_part[i].points[1].y,
                arch_part[i].points[2].x, arch_part[i].points[2].y,
                arch_part[i].points[3].x, arch_part[i].points[3].y,
                arch_part[i].points[4].x, arch_part[i].points[4].y,
                arch_part[i].color.r,
                arch_part[i].color.g,
                arch_part[i].color.b,
                arch_part[i].color.a);
    }

    if (f != stdout)
    {
        fclose(f);
    }
}

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1063, 1063, "Spirle Demo");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Texture2D ref = LoadTextureFromImage(LoadImage("logo.png"));

    const Vector2 center = {GetScreenWidth() / 2.0, GetScreenHeight() / 2.0};

    float curr_angle = 0;
    int edit_number = -1;
    bool show_front = false, show_back = false, do_spin = false;

    while (!WindowShouldClose())
    {

        if (do_spin)
            curr_angle = Wrap(curr_angle + -.1, 0, 360);

        BeginDrawing();

#ifdef DEBUGGING
        DrawFPS(0, 0);
        DrawText(TextFormat("Editing %d", edit_number + 1), 300, 10, 24, GREEN);
#endif // DEBUGGING
        ClearBackground(BG);

        if (IsKeyPressed(KEY_X))
        {
            show_back = !show_back;
        }

        if (IsKeyPressed(KEY_Z))
        {
            show_front = !show_front;
        }

        if (IsKeyPressed(KEY_PERIOD))
        {
            do_spin = !do_spin;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            show_front = show_back = false;
            edit_number = -1;
            curr_angle = 0;
            do_spin = false;
        }
        if (IsKeyPressed(KEY_ONE))
            edit_number = 0;
        if (IsKeyPressed(KEY_TWO))
            edit_number = 1;
        if (IsKeyPressed(KEY_THREE))
            edit_number = 2;
        if (IsKeyPressed(KEY_FOUR))
            edit_number = 3;
        if (IsKeyPressed(KEY_FIVE))
            edit_number = 4;
        if (IsKeyPressed(KEY_ZERO))
            edit_number = -1;

        if (show_back)
            DrawTexture(ref, 0, 0, WHITE);

        if (IsKeyPressed(KEY_S))
        {
            SavePoints();
        }

        for (size_t i = 0; i < 7; i++)
        {
            for (size_t j = 0; j < ARRAY_SIZE(arch_part); j++)
            {
                float angle = ((360.0 / 7.0) * i) + curr_angle;

                struct ArchPart ap = arch_part[j];

                for (size_t k = 0; k < ARRAY_SIZE(ap.points); k++)
                {
                    ap.points[k] = Vector2Add(center, Vector2Rotate(Vector2Subtract(ap.points[k], center), angle * DEG2RAD));
                }

                DrawSplineBasis(ap.points, ARRAY_SIZE(ap.points), 18, ap.color);
            }
        }

        // editor
        bool got_one = false;
        for (size_t i = 0; i < ARRAY_SIZE(arch_part); i++)
        {
            struct ArchPart *ap = &arch_part[i];

            for (size_t j = 0; j < ARRAY_SIZE(ap->points); j++)
            {
                if (ap->points[j].x == 0 && ap->points[j].y == 0)
                {
                    ap->points[j].x = i * 12;
                    ap->points[j].y = j * 12;
                }
                if (i == edit_number)
                    DrawCircle(ap->points[j].x, ap->points[j].y, 12, ColorBrightness(ap->color, .5));
                if (
                    (!got_one &&
                     i == edit_number &&
                     IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                     CheckCollisionPointCircle(GetMousePosition(), ap->points[j], 12)) ||
                    (i == edit_number && IsKeyDown(KEY_KP_1 + j)))
                {
                    ap->points[j] = GetMousePosition();
                    got_one = true;
                }
            }
        }
        // end editor

        // rendering
        // DrawCircleV(center, 101, (Color){0x54, 0x5d, 0xa4, 255});
        DrawCircleV(center, 101, arch_part[3].color);

        DrawRing(center, 380, 380 + 20, 0, 360, 100, BLACK);

        DrawRing(center, 300, 360, -90 + -curr_angle, -270 + -curr_angle, 100, GRAY);

        static Vector2 w_point_point_dist = {86, 282};
        Vector2 w_start = center;

#ifdef DEBUGGING
        int iota = 20;
        GuiSlider((Rectangle){10, iota, 200, 16}, "", "", &w_point_point_dist.x, -200, 200);
        iota += 16;
        GuiSlider((Rectangle){10, iota, 200, 16}, "", "", &w_point_point_dist.y, -200, 400);
#endif // DEBUGGING

        w_start.x += -195; // -195
        w_start.y += -206; // -206

        Vector2 w_end = w_start;

        w_end = Vector2Add(w_end, w_point_point_dist);
        DrawLineEx(w_start, w_end, 36, GRAY);

        w_start.x += w_point_point_dist.x * 2;
        DrawLineEx(w_start, w_end, 36, GRAY);

        w_end.x += w_point_point_dist.x * 2;
        DrawLineEx(w_start, w_end, 36, GRAY);

        w_start.x += w_point_point_dist.x * 2;
        DrawLineEx(w_start, w_end, 36, GRAY);

        Vector2 m_start = center;

        static float x_off = -21, y_off = -59;

#ifdef DEBUGGING
        iota += 16;
        GuiSlider((Rectangle){10, iota, 200, 16}, "", "", &x_off, -100, 100);
        iota += 16;
        GuiSlider((Rectangle){10, iota, 200, 16}, "", "", &y_off, -100, 100);
#endif

        m_start.x += -22.82; // -21
        m_start.y += -66.30; // -59

        Vector2 m_end = m_start;
        m_end = Vector2Add(m_end, w_point_point_dist);
        m_end.x -= w_point_point_dist.x * 2;
        DrawLineEx(m_start, m_end, 36, GRAY);

        m_end.x += w_point_point_dist.x * 2;
        DrawLineEx(m_start, m_end, 36, GRAY);

        m_start.x += w_point_point_dist.x * 2;
        DrawLineEx(m_start, m_end, 36, GRAY);

        m_end.x += w_point_point_dist.x * 2;
        DrawLineEx(m_start, m_end, 36, GRAY);

        Vector2 full_caps[] =
            {
                {403.000000, 576.000000},
                {490.000000, 321.000000},
                {576.000000, 576.000000},
                {489.000000, 461.000000},
                {576.000000, 716.000000},
                {663.000000, 460.000000},
            };

        for (size_t i = 0; i < ARRAY_SIZE(full_caps); i++)
        {
            DrawRectangleV(full_caps[i], (Vector2){36, 36}, GRAY);
        }

        // DrawRectangleV(GetMousePosition(), (Vector2){36, 36}, GRAY);
        // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        // {
        //     printf("%f,%f\n", GetMousePosition().x, GetMousePosition().y);
        // }

        if (show_front)
            DrawTexture(ref, 0, 0, WHITE);

        EndDrawing();
    }

    return 0;
}
