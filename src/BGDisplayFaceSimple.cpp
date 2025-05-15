#include "BGDisplayFaceSimple.h"
#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceSimple::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {

    DisplayManager.clearMatrix();

    auto lastReading = readings.back()

    showReading(readings.back(), 0, 6, TEXT_ALIGNMENT::CENTER, FONT_TYPE::MEDIUM, dataIsOld);

    // show arrow in the right part of the screen
    showTrendArrow(readings.back(), 32 - 5, 1);
  
     //I do declare
    int startX = 0;
    int endX = MATRIX_WIDTH - 1;
    
    // Calculate elapsed minutes and block count
int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;
int maxBlocks = 6; // Maximum blocks to display
int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;

// Calculate total width of blocks and the starting x position
int totalWidth = (blockCount * 5); // 4 pixels per block + 1 pixel for spacing
startX = 0; // Align to the left

// Draw each block
for (int i = 0; i < blockCount; ++i) {
    int blockStartX = startX + (i * 5); // 4 pixels for block + 1 pixel for spacing
    for (int x = blockStartX; x < blockStartX + 4; ++x) {
        DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
    }
}
    DisplayManager.update();
}
