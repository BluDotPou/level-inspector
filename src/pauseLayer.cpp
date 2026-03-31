#include <Geode/modify/PauseLayer.hpp>

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/InfoLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/binding/LevelLeaderboard.hpp>
#include "shared/levelInspector.hpp"

using namespace geode::prelude;

class $modify(LevelInspectorPauseLayer, PauseLayer) {
    struct Fields {
        FLAlertLayer* infoAlert = nullptr;
    };

    void customSetup() {
        PauseLayer::customSetup();

        auto menu = static_cast<CCMenu*>(this->getChildByID("right-button-menu"));
        if (!menu) return;

        if (!menu->getChildByID("info-button")) {
            auto sprite = CircleButtonSprite::create(
                CCSprite::create("boton.png"_spr)
            );
            auto infoBtn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(LevelInspectorPauseLayer::onLevelInfo)
            );
            infoBtn->setID("info-button");
            menu->addChild(infoBtn);
        }

        if (!menu->getChildByID("leaderboard-button")) {
            auto sprite = CCSprite::create("leaderboard.png"_spr);
            auto leaderboardbtn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(LevelInspectorPauseLayer::OpenLeaderboard)
            );
            leaderboardbtn->setID("leaderboard-button");
            menu->addChild(leaderboardbtn);
        }

        menu->updateLayout();
    }

    void onLevelInfo(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto level = pl->m_level;
        std::string text;

        text += "<cg>Nivel</c>: " + levelinspector::toStdString(level->m_levelName) + "\n";
        text += "<cg>Creador</c>: " + levelinspector::toStdString(level->m_creatorName) + "\n";
        text += "<cg>Level ID</c>: " + std::to_string(level->m_levelID.value()) + "\n";
        text += "<cg>Stars</c>: " + std::to_string(level->m_stars.value()) + "\n";
        text += "<cg>Song ID</c>: " + std::to_string(level->m_songID) + "\n";
        text += "<cp>Attempts</c>: " + std::to_string(level->m_attempts.value()) + "\n";
        text += "<cp>Downloads</c>: " + std::to_string(level->m_downloads) + "\n";
        text += "<cp>Likes</c>: " + std::to_string(level->m_likes) + "\n";
        text += "<cp>Level Length</c>: " + std::to_string(level->m_levelLength) + "\n";
        text += "<co>Tiempo jugado</c>: ";
        text += levelinspector::formatTime(levelinspector::elapsedSeconds());

        auto alert = FLAlertLayer::create(
            this,
            "Level Inspector",
            text,
            "OK",
            "Comments"
        );

        m_fields->infoAlert = alert;
        alert->show();
    }

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
        if (layer != m_fields->infoAlert) return;
        m_fields->infoAlert = nullptr;

        if (!btn2) return;

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto comments = InfoLayer::create(pl->m_level, nullptr, nullptr);
        if (comments) comments->show();
    }

    void OpenLeaderboard(CCObject* layer) {

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto Leaderboard = LevelLeaderboard::create(pl->m_level, LevelLeaderboardType::Global, LevelLeaderboardMode::Time);
        if (Leaderboard) Leaderboard->show();
    }
};