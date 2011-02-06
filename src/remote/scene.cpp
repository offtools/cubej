#include "scene.h"
#include "remoteclient.h"

namespace CubeJRemote {

    SceneInfo::SceneInfo() {
        name = newstring("unnamed");
    }

    SceneInfo::SceneInfo(const char* _name) {
        if(_name && *name)
            name = newstring(_name);
        else
            name = newstring("unnamed");
    }

    SceneInfo::~SceneInfo() {
        delete name;
    }

    const char* SceneInfo::getSceneName() {
        return name;
    }

    SceneMgr::SceneMgr() : currentScene(0) {}

    SceneMgr::~SceneMgr() {
        scenes.deletecontents();
    }

    void SceneMgr::newScene() {
        scenes.add(new SceneInfo);
    }

    void SceneMgr::loadScene(const char* name) {
        loopv(scenes) if(strcmp(name, scenes[i]->getSceneName()) == 0)
        {
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_CHANGESCENE> data(name);
            SendMessage(data);
            currentScene = scenes[i];
        }
    }

    void SceneMgr::loadScene(int i) {
        if( scenes.inrange(i) )
            loadScene( scenes[i]->getSceneName() );
    }

    void SceneMgr::saveCurrentScene() {
        if(getCurrentScene())
        {
            //SAVE
        }
    }

    void SceneMgr::saveCurrentScene(char* name) {

    }

    const SceneInfo* SceneMgr::getCurrentScene() {
        if(currentScene)
            return currentScene;
        else
            return 0;
    }

    const char* SceneMgr::getCurrentSceneName() {
        if(currentScene) {
            currentScene->getSceneName();
        }
        return 0;
    }

    int SceneMgr::getNumScenes() {
        return scenes.length();
    }

    SceneInfo* SceneMgr::getSceneInfo(int i) {
        if(scenes.inrange(i))
            return scenes[i];
        else
            return 0;
    }
}
