#include <Geode/modify/PauseLayer.hpp>

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/InfoLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/binding/LevelLeaderboard.hpp>
#include <Geode/binding/ProfilePage.hpp>

#include "shared/levelInspector.hpp"

using namespace geode::prelude;

class $modify(LevelInspectorPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = static_cast<CCMenu*>(this->getChildByID("right-button-menu"));
        if (!menu) return;

        // BOTÓN INFO
        if (!menu->getChildByID("info-button")) {
            auto icon = CCSprite::create("boton.png"_spr);
            auto sprite = CircleButtonSprite::create(icon);

            auto infoBtn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(LevelInspectorPauseLayer::onLevelInfo)
            );

            infoBtn->setID("info-button");
            menu->addChild(infoBtn);
        }

        // BOTÓN LEADERBOARD
        if (!menu->getChildByID("leaderboard-button")) {
            auto sprite = CCSprite::create("leaderboard.png"_spr);

            auto leaderboardBtn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(LevelInspectorPauseLayer::OpenLeaderboard)
            );

            leaderboardBtn->setID("leaderboard-button");
            menu->addChild(leaderboardBtn);
        }

        // BOTÓN CREADOR
        if (!menu->getChildByID("creator-button")) {
            auto sprite = CCSprite::create("moder.png"_spr);
            sprite->setScale(2.0f);

            auto creatorBtn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(LevelInspectorPauseLayer::openCreatorOfTheLevel)
            );

            creatorBtn->setID("creator-button");
            menu->addChild(creatorBtn);
        }

        menu->updateLayout();
    }

    void onLevelInfo(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto level = pl->m_level;
        std::string text;

        text += "<cg>Level</c>: " + levelinspector::toStdString(level->m_levelName) + "\n";
        text += "<cg>creator</c>: " + levelinspector::toStdString(level->m_creatorName) + "\n";
        text += "<cg>Level ID</c>: " + std::to_string(level->m_levelID.value()) + "\n";
        text += "<cg>Stars</c>: " + std::to_string(level->m_stars.value()) + "\n";
        text += "<cg>Song ID</c>: " + std::to_string(level->m_songID) + "\n";
        text += "<cp>Attempts</c>: " + std::to_string(level->m_attempts.value()) + "\n";
        text += "<cp>Downloads</c>: " + std::to_string(level->m_downloads) + "\n";
        text += "<cp>Likes</c>: " + std::to_string(level->m_likes) + "\n";
        text += "<cp>Level Length</c>: " + std::to_string(level->m_levelLength) + "\n";
        text += "<co>time played</c>: " + levelinspector::formatTime(levelinspector::elapsedSeconds());

        geode::createQuickPopup(
            "Level Inspector",
            text,
            "OK",
            "Comments",
            [level](auto, bool btn2) {
                if (!btn2) return;

                auto comments = InfoLayer::create(level, nullptr, nullptr);
                if (comments) comments->show();
            }
        );
    }

    void OpenLeaderboard(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto leaderboard = LevelLeaderboard::create(
            pl->m_level,
            LevelLeaderboardType::Global,
            LevelLeaderboardMode::Time
        );

        if (leaderboard) leaderboard->show();
    }

    void openCreatorOfTheLevel(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto settings = ProfilePage::create(pl->m_level->m_accountID, false);

        if (settings) settings->show();
    }
};