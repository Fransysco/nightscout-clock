#include "BGDisplayFaceGraphBase.h"
#include "BGDisplayManager.h"
#include "globals.h"
#include <Arduino.h>

void BGDisplayFaceGraph::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {
    showGraph(0, 32, 180, readings);

    auto lastReading = readings.back();

    int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;
    int maxBlocks = 5; // Maximum blocks
    int blockWidth = 4; // Width of each block in pixels
    int blockSpacing = 1; // Spacing between blocks in pixels
    int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;

    // Calculate total width of all blocks and center them
    int totalWidth = (blockWidth + blockSpacing) * blockCount - blockSpacing;
    int startX = (MATRIX_WIDTH - totalWidth) / 2; // Center the blocks horizontally

    // Draw each block
    for (int i = 0; i < blockCount; ++i) {
        int blockStartX = startX + i * (blockWidth + blockSpacing); // Calculate starting position for each block
        for (int x = blockStartX; x < blockStartX + blockWidth; ++x) {
            DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
        }
    }

    DisplayManager.update();
}

