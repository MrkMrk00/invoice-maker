#ifndef _H_UI
#define _H_UI

#include <iostream>

namespace invoice_maker::ui {

template <class T>
class StateContainer;

template <class T>
class State final {
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

    ~State() { delete this->value; }

    StateContainer<T> use() { return StateContainer<T>{this}; }
    friend T *StateContainer<T>::operator()();
};

template <class T>
class StateContainer final {
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
