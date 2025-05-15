#include "BGDisplayFaceBigText.h"
#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceBigText::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {

    DisplayManager.clearMatrix();

    showReading(readings.back(), 0, 7, TEXT_ALIGNMENT::LEFT, FONT_TYPE::LARGE, dataIsOld);

    // show arrow in the right part of the screen
    showTrendArrow(readings.back(), 32 - 5, 1);

    auto lastReading = readings.back()
    
   // Timer block logic
    int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;
    const int maxBlocks = 5; // Maximum number of blocks
    const int blockSpacing = 1; // Space between blocks

    // Dynamically calculate the block size based on available space
    int totalSpacing = blockSpacing * (maxBlocks - 1); // Total space needed for gaps
    int blockWidth = (MATRIX_WIDTH - totalSpacing) / maxBlocks; // Maximum block width

    int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;
    int startX = 1;

    // Draw each block
    for (int i = 0; i < blockCount; ++i) {
        int blockStartX = startX + i * (blockWidth + blockSpacing); // Calculate the start position of the block
        for (int x = blockStartX; x < blockStartX + blockWidth; ++x) {
            DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
        }
    }

    DisplayManager.update();
}
