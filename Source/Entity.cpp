#include "Entity.h"

Entity::Entity() {
    body = nullptr;
    listener = nullptr;
}

Entity::Entity(PhysBody* _body, Module* _listener)
    : body(_body), listener(_listener) {
    if (body != nullptr) {
        body->listener = listener;
    }
}