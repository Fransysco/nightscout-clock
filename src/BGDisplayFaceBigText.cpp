#include "BGDisplayFaceBigText.h"
#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceBigText::showReadings(const std::list<GlucoseReading> &readings, bool dataIsOld) const {

    DisplayManager.clearMatrix();

    showReading(readings.back(), 0, 7, TEXT_ALIGNMENT::LEFT, FONT_TYPE::LARGE, dataIsOld);

    // show arrow in the right part of the screen
    showTrendArrow(readings.back(), 32 - 5, 1);

    auto lastReading = readings.back();
    
    // Calculate elapsed minutes
    int elapsedMinutes = (ServerManager.getUtcEpoch() - lastReading.epoch) / 60;

    // Call the shared timer block function
    BGDisplayManager::drawTimerBlocks(elapsedMinutes, 5, dataIsOld);

    DisplayManager.update();
}
