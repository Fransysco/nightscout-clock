#include "BGDisplayFaceGraphBase.h"
#include "BGDisplayManager.h"
#include "globals.h"
#include <Arduino.h>

void BGDisplayFaceGraph::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {
    showGraph(0, 32, 180, readings);

    auto lastReading = readings.back();

    // Declare and calculate
    int maxBlocks = 5; // Limit to maximum of 5 blocks
    int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;
    int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;

    // Calculate block width and spacing to evenly distribute
    int totalAvailableWidth = MATRIX_WIDTH; // Total width of the display
    int blockWidth = totalAvailableWidth / blockCount; // Width of each block (including spacing)
    int blockPixelWidth = blockWidth - 1; // Actual block width (spacing is 1 pixel)

    // Draw each block
    for (int i = 0; i < blockCount; ++i) {
        int blockStartX = i * blockWidth; // Calculate starting position for each block
        for (int x = blockStartX; x < blockStartX + blockPixelWidth; ++x) {
            DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
        }
    }

    DisplayManager.update();

    
}
