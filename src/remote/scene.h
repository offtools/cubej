#ifndef REMOTE_SCENE_H
#define REMOTE_SCENE_H

#include "protocol.h"

namespace CubeJRemote {

    class SceneInfo
    {
        public:
            SceneInfo();
            SceneInfo(const char* _name);
            ~SceneInfo();

            const char* getSceneName();

        private:
            char* name;
    };

    class SceneMgr
    {
        public:
            SceneMgr();
            virtual ~SceneMgr();

            void newScene();
            void loadScene(int i);
            void loadScene(const char* name);
            void saveCurrentScene();
            void saveCurrentScene(char* name);

            const SceneInfo* getCurrentScene();
            const char* getCurrentSceneName();

            int getNumScenes();
            SceneInfo* getSceneInfo(int i);

            virtual void updateSceneListing(const char* name) = 0;

        protected:
            vector<SceneInfo *> scenes;

        private:
            SceneInfo* currentScene;
    };
}

#endif // REMOTE_SCENE_H
