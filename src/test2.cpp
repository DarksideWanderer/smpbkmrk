#include <SDL2/SDL.h>
#include <stdio.h>

// 计算指定像素尺寸在当前屏幕上的 DPI
void calculateDPI(int displayIndex, int targetWidthPx, int targetHeightPx) {
    float ddpi, hdpi, vdpi;
    SDL_Rect displayBounds;

    // 获取 DPI
    if (SDL_GetDisplayDPI(displayIndex, &ddpi, &hdpi, &vdpi) != 0) {
        printf("Failed to get DPI: %s\n", SDL_GetError());
        return;
    }

    // 获取显示器分辨率
    if (SDL_GetDisplayBounds(displayIndex, &displayBounds) != 0) {
        printf("Failed to get display bounds: %s\n", SDL_GetError());
        return;
    }

    // 计算物理尺寸（英寸）
    float widthInches = displayBounds.w / hdpi;
    float heightInches = displayBounds.h / vdpi;

    // 计算目标像素对应的物理尺寸
    float targetWidthInches = targetWidthPx / hdpi;
    float targetHeightInches = targetHeightPx / vdpi;

    printf("Display %d Information:\n", displayIndex);
    printf("  Resolution: %d x %d pixels\n", displayBounds.w, displayBounds.h);
    printf("  DPI: Diagonal = %.2f, Horizontal = %.2f, Vertical = %.2f\n", ddpi, hdpi, vdpi);
    printf("  Physical Size: %.2f x %.2f inches\n", widthInches, heightInches);
    printf("  Target %d x %d pixels corresponds to %.2f x %.2f inches\n",
           targetWidthPx, targetHeightPx, targetWidthInches, targetHeightInches);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // 选择显示器索引（默认 0）
    int displayIndex = 1;
    int targetWidthPx = 800;  // 你可以修改为需要计算的像素尺寸
    int targetHeightPx = 600;

    calculateDPI(displayIndex, targetWidthPx, targetHeightPx);

    SDL_Quit();
    return 0;
}
