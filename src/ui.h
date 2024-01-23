#ifndef _H_UI
#define _H_UI

#include <iostream>
#include <memory>

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
#if INVOICE_MAKER_DEBUG
        std::cout << __PRETTY_FUNCTION__ << " State is being deleted."
                  << std::endl;
#endif

        delete this->value;
        this->value = nullptr;
    }

    StateContainer<T> use() { return StateContainer<T>{this}; }
    ~State() { delete this->value; }
    friend T *StateContainer<T>::operator->();
    friend std::unique_ptr<T> StateContainer<T>::copy();
};

template <class T>
class StateContainer final {
   private:
    State<T> *state;
    bool was_used = false;

   public:
    explicit StateContainer<T>(State<T> *state_) : state(state_) {}

    StateContainer<T>(StateContainer &) = delete;
    StateContainer<T>(const StateContainer<T> &) = delete;
    StateContainer<T>(StateContainer<T> &&) = delete;
    StateContainer<T>(const StateContainer<T> &&) = delete;

    std::unique_ptr<T> copy() {
        if (this->state->value == nullptr) {
#if INVOICE_MAKER_DEBUG
            std::cout << __PRETTY_FUNCTION__ << " Creating new state"
                      << std::endl;
#endif

            this->state->value = new T;
        }

        return std::make_unique<T>(*this->state);
    }

    T *operator->() {
        this->was_used = true;
        if (this->state->value == nullptr) {
#if INVOICE_MAKER_DEBUG
            std::cout << __PRETTY_FUNCTION__ << " Creating new state"
                      << std::endl;
#endif

            this->state->value = new T;
        }

        return this->state->value;
    }

    ~StateContainer() {
        if (!this->was_used) {
            this->state->reset();
        }
    }
};
};  // namespace invoice_maker::ui

#endif  // !_H_UI
