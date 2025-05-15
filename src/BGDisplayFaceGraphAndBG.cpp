#include "BGDisplayFaceGraphAndBG.h"
#include "BGDisplayManager.h"
#include "enums.h"
#include "globals.h"
#include <Arduino.h>

void BGDisplayFaceGraphAndBG::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {

    GlucoseReading reading = readings.back();
    String printableReading = getPrintableReading(reading.sgv);
    uint8_t textWidth = DisplayManager.getTextWidth(printableReading.c_str(), 2);
    // 32 is the width of the display, 2 for arrow and space
    uint8_t grqphWidth = 32 - textWidth - 2;
    uint8_t minutesToShow = grqphWidth * 5;

#ifdef DEBUG_DISPLAY
    DEBUG_PRINTF("For the value %d, printable is: %s, text width: %u, graph width: %u\n", reading.sgv, printableReading.c_str(),
                 textWidth, grqphWidth);
#endif

    showGraph(0, grqphWidth, minutesToShow, readings);
    showReading(readings.back(), 31, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    showTrendVerticalLine(31, readings.back().trend);

    auto lastReading = readings.back(); // Declare and initialize lastReading

    // Timer block logic
    int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;
    const int maxBlocks = 5; // Maximum number of blocks
    const int blockSpacing = 1; // Space between blocks

    // Dynamically calculate the block size based on available space
    int totalSpacing = blockSpacing * (maxBlocks - 1); // Total space needed for gaps
    int blockWidth = (MATRIX_WIDTH - totalSpacing) / maxBlocks; // Maximum block width

    int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;
    int startX = 0; // Always start from the left edge

    // Draw each block
    for (int i = 0; i < blockCount; ++i) {
        int blockStartX = startX + i * (blockWidth + blockSpacing); // Calculate the start position of the block
        for (int x = blockStartX; x < blockStartX + blockWidth; ++x) {
            DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
        }
    }

    DisplayManager.update();
}

void BGDisplayFaceGraphAndBG::showTrendVerticalLine(int x, BG_TREND trend) const {
    switch (trend) {
        case BG_TREND::DOUBLE_UP:
            DisplayManager.drawPixel(x, 0, COLOR_RED);
            DisplayManager.drawPixel(x, 1, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::DOUBLE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            DisplayManager.drawPixel(x, 6, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 7, COLOR_RED);
            break;
        case BG_TREND::SINGLE_UP:
            DisplayManager.drawPixel(x, 1, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::SINGLE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            DisplayManager.drawPixel(x, 6, COLOR_YELLOW);
            break;
        case BG_TREND::FORTY_FIVE_UP:
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::FORTY_FIVE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            break;
        case BG_TREND::FLAT:
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            break;
        default:
            DisplayManager.setTextColor(COLOR_BLACK);
            break;
    }
    DisplayManager.update();
}
