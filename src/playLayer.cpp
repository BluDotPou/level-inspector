#include <Geode/modify/PlayLayer.hpp>
#include "shared/levelInspector.hpp"

using namespace geode::prelude;

class $modify(LevelInspectorPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2))
            return false;

        levelinspector::markLevelStart();
        return true;
    }
};