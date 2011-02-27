#include "remoteclient.h"

namespace CubeJRemote {

    SceneInfo::SceneInfo() : mapname("unnamed"), worldsize(0), mapversion(0) {}

    SceneInfo::SceneInfo(const char* name) : worldsize(0), mapversion(0) {
        if(name && *name) {
            mapname = name;
        }
        else {
            mapname = "unamed";
        }
    }

    SceneInfo::~SceneInfo() {}

    const char* SceneInfo::getSceneName() {
        return mapname.c_str();
    }

    void SceneInfo::setMapVersion(int version) {
        mapversion = version;
    }

    int SceneInfo::getMapVersion() {
        return mapversion;
    }

    void SceneInfo::setWorldSize(int size) {
        worldsize = size;
    }

    int SceneInfo::getWorldSize() {
        return worldsize;
    }

    SceneMgr::SceneMgr(MainComponent & _main) : maincomponent(_main), currentSceneIter(scenes.end()) {}

    SceneMgr::~SceneMgr() {
        clear();
    }

    void SceneMgr::newScene() {
        scenes.push_back(new SceneInfo);
    }

    void SceneMgr::clear() {
        for(std::vector<SceneInfo *>::iterator iter = scenes.begin(); iter != scenes.end(); iter++) {
            delete (*iter);
            scenes.erase(iter);
        }
    }

    bool SceneMgr::find(std::string str) {
//        std::vector<SceneInfo *>::iterator iter = scenes.begin();
//        std::string name(cstr);
//        for(scenes.begin(); iter != scenes.end(); iter++) {
//            if( name.compare( (*iter)->getSceneName()) == 0 )
//                return (*iter);
//        }
//        return 0;
        std::vector<SceneInfo *>::iterator iter = scenes.begin();
        std::string name(cstr);
        for(scenes.begin(); iter != scenes.end(); iter++) {
            if( name.compare( (*iter)->getSceneName()) == 0 )
                return (*iter);
        }
        return 0;
    }

    void SceneMgr::loadScene(const char* name) {
        if( find(name) ) {
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_CHANGESCENE> data(name);
            maincomponent.dispatcher.SendMessage(data);
        }
    }

    void SceneMgr::loadScene(unsigned int i) {
        if( i < scenes.size() ) {
            CubeJProtocol::MsgDataType<CubeJProtocol::MSG_REQ_CHANGESCENE> data(scenes[i]->getSceneName());
            maincomponent.dispatcher.SendMessage(data);
        }
    }

    void SceneMgr::saveCurrentScene() {
        if(getCurrentScene())
        {
            //SAVE
        }
    }

    void SceneMgr::saveCurrentScene(const char* name) {

    }

    const SceneInfo* SceneMgr::getCurrentScene() {
        if(currentSceneIter < scenes.end())
            return (*currentSceneIter);
        else
            return 0;
    }

    void SceneMgr::setCurrentScene(const char* cstr, int worldsize, int mapversion) {
//        SceneInfo* info = find(cstr);
//        if(!info) {
//            scenes.push_back(new SceneInfo(cstr));
//            info = find(cstr);
//        }
        currentSceneIter = find_if(scenes.begin(), scenes.end(), mem_fun(&SceneMgr::find));
//        for(currentSceneIter = scenes.begin(); currentSceneIter != scenes.end(); currentSceneIter++) {
//            if( *currentSceneIter == info ) {
//                (*currentSceneIter)->setMapVersion(mapversion);
//                (*currentSceneIter)->setWorldSize(worldsize);
//                updateSceneListing((*currentSceneIter)->getSceneName());
//                return;
//            }
//        }
    }

    const char* SceneMgr::getCurrentSceneName() {
        if(currentSceneIter < scenes.end() ) {
            (*currentSceneIter)->getSceneName();
        }
        return 0;
    }

    int SceneMgr::getNumScenes() {
        return scenes.size();
    }

    SceneInfo* SceneMgr::getSceneInfo(unsigned int i) {
        if(i < scenes.size())
            return scenes[i];
        else
            return 0;
    }
}
