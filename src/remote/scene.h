#ifndef REMOTE_SCENE_H
#define REMOTE_SCENE_H


#include "config.h"
#include "protocol.h"

namespace CubeJRemote {

    class SceneInfo
    {
        public:
            SceneInfo();
            SceneInfo(const char *);
            ~SceneInfo();

            const char* getSceneName() const;
            void setMapVersion(int );
            const int getMapVersion() const;
            void setWorldSize(int );
            const int getWorldSize() const;

        private:
            std::string mapname;
            int worldsize;
            int mapversion;
    };
//
//    class SceneMgr
//    {
//        public:
//            SceneMgr(MainComponent &);
//            virtual ~SceneMgr();
//
//            void newScene();
//            void loadScene(unsigned int i);
//            void loadScene(const char* name);
//            void saveCurrentScene();
//            void saveCurrentScene(const char* name);
//
//            const SceneInfo* getCurrentScene();
//            void setCurrentScene(const char* name, int worldsize, int mapversion);
//            const char* getCurrentSceneName();
//
//            int getNumScenes();
//            SceneInfo* getSceneInfo(unsigned int i);
//
//            virtual void updateSceneListing(const char* name) = 0;
//
//        protected:
//            std::vector<SceneInfo *> scenes;
//            SceneInfo* find(const char* name);
//
//        private:
//            MainComponent& maincomponent;
//            std::vector<SceneInfo *>::iterator currentSceneIter;
//            void clear();
//    };
}

#endif // REMOTE_SCENE_H
