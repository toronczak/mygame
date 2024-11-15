#define MAX_BULLETS 10000

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    Color color;
    Vector2 size;
    float time;
    float startTime;
    bool fade;
    bool active;
} Bullet;

Bullet bullets[MAX_BULLETS];

void InitBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void UpdateBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].time == 0) {
            bullets[i].active = false;
        }
        if (bullets[i].fade) {
            bullets[i].color = ColorAlpha(bullets[i].color, bullets[i].time/bullets[i].startTime);
        }
        
        if (bullets[i].active) {
            // Update position
            bullets[i].position = Vector2Add(bullets[i].position, bullets[i].velocity);


            // Check out of screen
            if ((bullets[i].position.x < 0-bullets[i].size.x) || (bullets[i].position.x > GetScreenWidth()) ||
                (bullets[i].position.y < 0-bullets[i].size.x) || (bullets[i].position.y > GetScreenHeight())) {
                bullets[i].active = false;
            }

            // Check collision with targets
            // TODO: Implement collision detection
            bullets[i].time--;
        }
    }
}

void DrawBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            DrawRectangleV(bullets[i].position, bullets[i].size, bullets[i].color);
        }
    }
}

// New function to fire a bullet from a given position and direction
void FireBulletPrecise(float x, float y, float speed, Vector2 direction, Color color, float sizeX, float sizeY, bool fade, float time) {
    // Find the first inactive bullet in the array
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            // Set the bullet position to the given position
            bullets[i].position = (Vector2){ x,y };

            // Set the bullet velocity to the given direction scaled by a speed factor
            bullets[i].velocity = Vector2Scale(direction, speed);

            // Set the bullet color to the given color
            bullets[i].color = color;
            
            // Set the bullet size to the given size
            bullets[i].size = (Vector2){ sizeX, sizeY };
            
            // Set the bullet visibility time to the given time
            bullets[i].time = time;
            bullets[i].startTime = time;

            // Set whether the bullet fades over time or not
            bullets[i].fade = fade;

            // Set the bullet active flag to true
            bullets[i].active = true;

            // Break the loop as we only want to fire one bullet at a time
            break;
        }
    }
}

// New function to fire a bullet from a given position, speed and angle (in degrees)
void FireBullet(float x, float y, float speed, float angle, Color color, float sizeX, float sizeY, bool fade, float time) {
    FireBulletPrecise(x, y, speed, Vector2Normalize((Vector2){ x+cos(angle/57.2958)*1000000, y+sin(angle/57.2958)*1000000}), color, sizeX, sizeY, fade, time);
}