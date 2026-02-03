#pragma once
#include <functional>
#include <string>

#include "Base.h"

/**
 * @class Event
 * @brief Base class for all events in the event system.
 * 
 * The Event class serves as the base for all specific event types in the engine's
 * event handling system. It provides common functionality such as type identification
 * and a handled flag to indicate whether the event has been processed.
 * 
 * Key features:
 * - Type identification via GetType() and IsType<T>()
 * - Handled flag to prevent further processing of already handled events
 * - ToString() method for easy logging and debugging
 * 
 * @note All specific event types should inherit from this class and implement
 *       their own type identification methods.
 * 
 * @example
 * @code
 * class KeyEvent : public Event {
 * public:
 *     EVENT_CLASS_TYPE(KeyEvent)
 *     // Additional KeyEvent-specific members...
 * };
 * 
 * Event* event = new KeyEvent();
 * if (event->IsType<KeyEvent>()) {
 *     // Handle key event
 * }
 * @endcode
 */
class Event {
 public:
 /**
  * @brief Destroy the Event object
  */
  virtual ~Event() = default;

  /**
   * @var Handled
   * @brief Indicates whether the event has been handled.
   * 
   * This flag is used to prevent further processing of the event once it has
   * been handled by an event listener. Event dispatchers check this flag before
   * invoking handlers.
   */
  bool Handled_ = false;

  /**
   * @brief Gets the event type as a string.
   * 
   * Returns a string representing the specific type of the event.
   * Must be overridden by derived event classes.
   * 
   * @return const char* The event type string
   */
  virtual const char* GetType() const { return typeid(*this).name(); }

  /**
   * @brief Checks if the event is of a specific type T.
   * 
   * Uses dynamic_cast to determine if the event instance can be cast to type T.
   * Returns true if the cast is successful, false otherwise.
   * 
   * @tparam T The event type to check against
   * @return bool True if the event is of type T, false otherwise
   */
  template <typename T>
  bool IsType() const {
    return dynamic_cast<const T*>(this) != nullptr;
  }

  /**
   * @brief Converts the event to a string representation.
   * 
   * Returns a string that describes the event. By default, it returns the event type.
   * Derived classes can override this method to provide more detailed information.
   * 
   * @return std::string A string representation of the event
   */
  virtual std::string ToString() const {
    return GetType();
  }
};

/**
 * @class EventDispatcher
 * @brief Dispatches events to appropriate handlers based on event type.
 * 
 * The EventDispatcher class is responsible for routing events to the correct
 * handler functions based on their type. It uses templates to allow for type-safe
 * dispatching of events.
 * 
 * Key features:
 * - Type-safe event dispatching using templates
 * - Prevents multiple handling of the same event via the Handled flag
 * 
 * @tparam T The specific event type to dispatch
 * 
 * @example
 * @code
 * Event* event = new KeyEvent();
 * EventDispatcher<KeyEvent> dispatcher(*event);
 * dispatcher.Dispatch<KeyEvent>([](KeyEvent& e) {
 *     // Handle key event
 *     return true; // Mark as handled
 * });
 * @endcode
 */
template <typename T>
class EventDispatcher {
 public:
  /**
  * @brief Constructs an EventDispatcher for a given event.
  */
  EventDispatcher(Event& event) : m_Event_(static_cast<T&>(event)) {}

  /**
   * @brief Dispatches the event to the provided handler if types match.
   * 
   * This function checks if the event type matches T. If it does, it invokes
   * the provided handler function with the event as an argument. If the handler
   * returns true, the event is marked as handled.
   * 
   * @tparam F The type of the handler function (should be callable with T&)
   * @param func The handler function to invoke if types match
   * @return bool True if the event was dispatched and handled, false otherwise
   */
  template <typename F>
  bool Dispatch(const F& func) {
    if (!m_Event_.Handled) {
      m_Event_.Handled = func(m_Event_);
      return true;
    }
    return false;
  }

 private:
 /**
  * @brief Reference to the event being dispatched.
  */
  T& m_Event_;
};

/**
 * @def EVENT_CLASS_TYPE(type)
 * @brief Macro to define type identification methods in event classes.
 */
#define EVENT_CLASS_TYPE(type)                        \
  static const char* GetStaticType() { return #type; } \
  virtual const char* GetType() const override { return GetStaticType(); }
