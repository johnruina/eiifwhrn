#ifndef CHARACTER_CLASS
#define CHARACTER_CLASS

#include <vector>
#include <string>
#include <functional>
#include "t.h"
#include "Rig.h"
#include "Object.h"

// Forward declarations
class CharacterBody;
class BodyComponent;

class Action {
public:
    std::string Name;
    std::function<void(CharacterBody*, BodyComponent*)> FunctionPointer;
};

class InputAction {
public:
    std::string BodyComponentName;
    std::string ActionName;
};

class BodyComponent {
public:
    std::string name;
    float integrity = 1.0f;
    BodyComponent* ParentComponent;
    std::vector<Action*> Actions;
};

class CharacterBody : public t_package {
public:
    Rig* rig;
    std::vector<BodyComponent*> components;
    BodyComponent* RootComponent;

    BodyComponent* GetComponentOfName(std::string name);
    void ExecuteAction(InputAction* action);
};

class CharacterMind {
public:
    InputAction* ProduceAction();
};

class Character : public Object, public Renderable {
public:
    CharacterMind* mind;
    CharacterBody* body;

    Character();
    void Render(Shader& shader) override;
};

#endif