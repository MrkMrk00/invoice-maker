#ifndef _H_UI
#define _H_UI

#include <iostream>
namespace invoice_maker::ui {
template <class T>
class StateContainer;

template <class T>
class State {
   private:
    T *value = nullptr;

   public:
    void reset() {
        if (this->value == nullptr) {
            return;
        }

        std::cout << __PRETTY_FUNCTION__ << " State is being deleted."
                  << std::endl;

        delete this->value;
        this->value = nullptr;
    }

    StateContainer<T> use() { return StateContainer<T>{this}; }
    friend T *StateContainer<T>::operator()();
};

template <class T>
class StateContainer {
   private:
    State<T> *state;
    bool was_used = false;

   public:
    explicit StateContainer<T>(State<T> *state_) : state(state_) {}

    T *operator()() {
        this->was_used = true;

        if (this->state->value == nullptr) {
            std::cout << __PRETTY_FUNCTION__ << " Creating new state"
                      << std::endl;

            this->state->value = new T;
            *this->state->value = {0};
        }

        return this->state->value;
    }

    ~StateContainer() {
        if (!was_used) {
            this->state->reset();
        }
    }
};
};  // namespace invoice_maker::ui

#endif  // !_H_UI
