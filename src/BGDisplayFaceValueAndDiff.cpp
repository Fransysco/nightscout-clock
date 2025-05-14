#include "BGDisplayFaceValueAndDiff.h"
#include "BGDisplayManager.h"
#include "globals.h"

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 32


void BGDisplayFaceValueAndDiff::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {

    DisplayManager.clearMatrix();
    DisplayManager.update();  // Apply clearing

    auto lastReading = readings.back();

    // treating special casse where the value representation is too wide, so we move it one pixel to the right

    if (SettingsManager.settings.bg_units == BG_UNIT::MMOLL && lastReading.sgv >= 180) {
        showReading(lastReading, 14, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    } else {
        showReading(lastReading, 13, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    }

    showTrendArrow(lastReading, 13, 1);

    String diff = getDiff(readings);

    if (dataIsOld) {
        DisplayManager.setTextColor(BG_COLOR_OLD);
    } else {
        DisplayManager.setTextColor(COLOR_WHITE);
    }

    DisplayManager.printText(33, 6, diff.c_str(), TEXT_ALIGNMENT::RIGHT, 2);

// Clear the area where the timer blocks are drawn
int startX = 0;
int endX = MATRIX_WIDTH - 1;
for (int x = startX; x <= endX; ++x) {
    DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, COLOR_RED);
}
    

    
// Calculate elapsed minutes and block count
int elapsedMinutes = (ServerManager.getTimezonedTime().tm_sec - lastReading.epoch) / 60;
int maxBlocks = 6; // Maximum blocks to display
int blockCount = elapsedMinutes > maxBlocks ? maxBlocks : elapsedMinutes;

// Calculate total width of blocks and the starting x position
int totalWidth = (blockCount * 5); // 4 pixels per block + 1 pixel for spacing
startX = (MATRIX_WIDTH - totalWidth + 1) / 2; // Adjusted for symmetry

// Draw each block
for (int i = 0; i < blockCount; ++i) {
    int blockStartX = startX + (i * 5); // 4 pixels for block + 1 pixel for spacing
    for (int x = blockStartX; x < blockStartX + 4; ++x) {
        DisplayManager.drawPixel(x, MATRIX_HEIGHT - 1, dataIsOld ? COLOR_RED : COLOR_GREEN);
    }
}
}
String BGDisplayFaceValueAndDiff::getDiff(const std::list<GlucoseReading> &readings) const {
    if (readings.size() < 2) {
#ifdef DEBUG_DISPLAY
        DEBUG_PRINTLN("Not enough readings to calculate diff");
#endif
        return "";
    }

    auto last = readings.back();
    std::list<GlucoseReading> foundReadings;
    // cycle through reading starting from the last one until the time of reading is less than 6.5 minutes prior to the last
    // reading. Store the found readings
    for (auto it = readings.rbegin(); it != readings.rend(); ++it) {
        if (last.epoch - it->epoch > (6 * 60 + 30)) {
            break;
        }
        foundReadings.push_front(*it);
    }
    // cucle through found readings, get min and max SGVs
    int minSGV = 9999;
    int maxSGV = 0;
    for (const auto &reading : foundReadings) {
        if (reading.sgv < minSGV) {
            minSGV = reading.sgv;
        }
        if (reading.sgv > maxSGV) {
            maxSGV = reading.sgv;
        }
    }
    int base = last.sgv;
    if (minSGV != base && maxSGV != base) {
#ifdef DEBUG_DISPLAY
        DEBUG_PRINTLN("Too many changes in last 6.5 minutes");
#endif
        return "?";
    }

    int diff = minSGV == base ? base - maxSGV : base - minSGV;

    if (std::abs(diff) > 99) {
#ifdef DEBUG_DISPLAY
        DEBUG_PRINTLN("Diff is too big: " + String(diff));
#endif
        return "?";
    }

    String diffString = "";
    if (diff >= 0) {
        diffString += "+";
    }

    diffString += getPrintableReading(diff);

#ifdef DEBUG_DISPLAY
    DEBUG_PRINTF("SGV Diff: %s (%d readings)", diffString.c_str(), foundReadings.size());
#endif

    return diffString;
}
