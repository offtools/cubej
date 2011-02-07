#ifndef REMOTE_SCENE_H
#define REMOTE_SCENE_H

#include <string>
#include <iostream>
#include <vector>

#include "protocol.h"

namespace CubeJRemote {

    class SceneInfo
    {
        public:
            SceneInfo();
            SceneInfo(const char *);
            ~SceneInfo();

            const char* getSceneName();
            void setMapVersion(int );
            int getMapVersion();
            void setWorldSize(int );
            int getWorldSize();

        private:
            std::string mapname;
            int worldsize;
            int mapversion;
    };

    class SceneMgr
    {
        public:
            SceneMgr();
            virtual ~SceneMgr();

            void newScene();
            void loadScene(unsigned int i);
            void loadScene(const char* name);
            void saveCurrentScene();
            void saveCurrentScene(const char* name);

            const SceneInfo* getCurrentScene();
            void setCurrentScene(const char* name, int worldsize, int mapversion);
            const char* getCurrentSceneName();

            int getNumScenes();
            SceneInfo* getSceneInfo(unsigned int i);

            virtual void updateSceneListing(const char* name) = 0;

        protected:
            std::vector<SceneInfo *> scenes;
            SceneInfo* find(const char* name);

        private:
            std::vector<SceneInfo *>::iterator currentSceneIter;
            void clear();
    };
}

#endif // REMOTE_SCENE_H
