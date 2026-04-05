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
using namespace std;

class $modify(LevelInspectorPauseLayer, PauseLayer) {

    CCMenuItemSpriteExtra* button(
            CCMenu* menu,
            const char* spriteN,
            SEL_MenuHandler callback,
            const char* id,
            float iconScale = 1.0f
        ) {
            auto icon = CCSprite::create(spriteN);
            if (!icon) return nullptr;
            icon->setScale(iconScale);
            auto sprite = CircleButtonSprite::create(icon);

            auto btn = CCMenuItemSpriteExtra::create(sprite, this, callback);
            btn->setID(id);
            menu->addChild(btn);

            return btn;
        }

    CCMenuItemSpriteExtra* iconButton( // If you see this you a duck
        CCMenu* menu,
        const char* spriteN,
        SEL_MenuHandler callback,
        const char* id,
        float scale = 1.0f
    ) {
        auto sprite = CCSprite::create(spriteN);
        if (!sprite) return nullptr;
        sprite->setScale(scale);

        auto btn = CCMenuItemSpriteExtra::create(sprite,this,callback);
        btn->setID(id);
        menu->addChild(btn);

        return btn;
    }

    void customSetup() {
        PauseLayer::customSetup();

        auto menu = static_cast<CCMenu*>(this->getChildByID("right-button-menu"));
        if (!menu) return;

        //container
        auto layout = ColumnLayout::create()
            ->setGap(8.f)
            ->setAxisReverse(true)
            ->setAutoScale(false);

        menu->setLayout(layout);

        //buttons
        CCMenuItemSpriteExtra* creatorBtn = nullptr;

        if (!menu->getChildByID("infoButton") || !menu->getChildByID("leaderboardButton") || !menu->getChildByID("creatorButton")) {
            button(menu, "boton.png"_spr,menu_selector(LevelInspectorPauseLayer::onLevelInfo),"infoButton",1.4f); //infomatio button 
            iconButton(menu, "profile.png"_spr,menu_selector(LevelInspectorPauseLayer::openCreatorOfTheLevel),"creatorButton",2.4f); //creator button
            iconButton(menu, "leaderboard.png"_spr,menu_selector(LevelInspectorPauseLayer::openLeaderboard),"leaderboardButton",1.0f); //lader
        }

        //i dont like math
        menu->updateLayout();
    }

    //============================================================//
    void onLevelInfo(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto level = pl->m_level;
        std::string text;
        std::string length;
        std::string diff;

        //level is hard=?
        switch (static_cast<int>(level->m_difficulty)) {
            case -1: diff = "Auto"; break;
            case 0: diff = "Easy"; break;
            case 1: diff = "Normal"; break;
            case 2: diff = "Hard"; break;
            case 3: diff = "Harder"; break;
            case 4: diff = "Insane"; break;
            case 5: diff = "Demon"; break;
            default: diff = "Unknown"; break;
        }

        // case 6: length = "Eon"; break;
        switch (level->m_levelLength) {
            case 0: length = "Tiny"; break;
            case 1: length = "Short"; break;
            case 2: length = "Medium"; break;
            case 3: length = "Long"; break;
            case 4: length = "XL"; break;
            case 5: length = "Platformer"; break;
            default: length = "Unknown"; break;
        }

        text += "<cg>Level</c>: " + levelinspector::toStdString(level->m_levelName) + "\n";
        text += "<cg>creator</c>: " + levelinspector::toStdString(level->m_creatorName) + "\n";
        text += "<cg>Level ID</c>: " + to_string(level->m_levelID.value()) + "\n";
        text += "<cg>Stars</c>: " + to_string(level->m_stars.value()) + "\n";
        text += "<cg>Difficulty</c>: " + diff + "\n";
        text += "<cg>Song ID</c>: " + to_string(level->m_songID) + "\n";
        text += "<cp>Attempts</c>: " + to_string(level->m_attempts.value()) + "\n";
        text += "<cp>Downloads</c>: " + to_string(level->m_downloads) + "\n";
        text += "<cp>Likes</c>: " + to_string(level->m_likes) + "\n";
        text += "<cp>Level Length</c>: " + length + "\n";
        text += "<co>time played</c>: " + levelinspector::formatTime(levelinspector::elapsedSeconds());

        geode::createQuickPopup( //dont use FLAlert why im fed up with all the fucking errors it gives
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
        this->runAction(CCEaseBackOut::create(
            CCScaleTo::create(0.2f, 1.0f)
        ));
    }

//============================================================//
    void openLeaderboard(CCObject*) { 
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto leaderboard = LevelLeaderboard::create(
            pl->m_level,
            LevelLeaderboardType::Global,
            LevelLeaderboardMode::Time
        );

        if (leaderboard) leaderboard->show(); //for safety lol
    }

//============================================================//
    void openCreatorOfTheLevel(CCObject*) {
        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        auto creator = ProfilePage::create(pl->m_level->m_accountID, false);

        if (creator) creator->show(); 
    }
};