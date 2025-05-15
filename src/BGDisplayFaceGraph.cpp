#include "BGDisplayFaceGraphBase.h"
#include "BGDisplayManager.h"
#include "globals.h"
#include <Arduino.h>

void BGDisplayFaceGraph::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {
    showGraph(0, 32, 180, readings);

    auto lastReading = readings.back();

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

