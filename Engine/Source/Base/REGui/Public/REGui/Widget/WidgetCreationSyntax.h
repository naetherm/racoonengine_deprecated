////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REGui/REGui.h"
#include <RECore/Core/Value.h>
#include <RECore/Utility/Move.h>
#include <RECore/Utility/Function.h>
#include <tuple>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
#define regui_begin_construction_args(WIDGET)                                             \
  public:                                                                                 \
    struct ConstructionArguments : public REGui::BaseNamedConstructionArguments<WIDGET> { \
      typedef ConstructionArguments _This;                                                \
      ConstructionArguments()


#define regui_attribute(TYPE, NAME)                                                       \
    RECore::Value<TYPE> mValue##NAME;                                                     \
    /** Default behaviour: Use normal value */                                            \
    WidgetConstructionType & set##NAME(const RECore::Value<TYPE>& value) {                \
      mValue##NAME = value;                                                               \
      return getThis();                                                                   \
    }                                                                                     \
    WidgetConstructionType & set##NAME(const RECore::Functor<TYPE>& value) {              \
      mValue##NAME = value;                                                               \
      return getThis();                                                                   \
    }                                                                                     \
    WidgetConstructionType & set##NAME(const TYPE& value) {                               \
      mValue##NAME = value;                                                               \
      return getThis();                                                                   \
    }                                                                                     \
    RECore::Value<TYPE> get##NAME() {                                                     \
      return mValue##NAME;                                                                \
    }                                                                                     \


#define regui_value(TYPE, NAME)                                                           \
    TYPE mValue##NAME;                                                                    \
    WidgetConstructionType & set##NAME(const TYPE& value) {                               \
      mValue##NAME = value;                                                               \
      return getThis();                                                                   \
    }                                                                                     \
    TYPE get##NAME() const {                                                              \
      return mValue##NAME;                                                                \
    }


#define regui_event(EVENT, NAME)                                                          \
    EVENT mEvent##NAME;                                                                   \
    WidgetConstructionType& set##NAME(const EVENT& value) {                               \
      mEvent##NAME = value;                                                               \
      return getThis();                                                                   \
    }                                                                                     \


#define regui_slots(TYPE)                                                                 \
    private:                                                                              \
      std::vector<TYPE*> mSlots;                                                          \
    public:                                                                               \
      _This& operator+(TYPE item) {                                                       \
        mSlots.push_back(&item);                                                          \
        return *this;                                                                     \
      }                                                                                   \
      const std::vector<TYPE*>& getSlots() const {                                        \
        return mSlots;                                                                    \
      }                                                                                   \
      TYPE* getSlotByIndex(RECore::uint32 index) {                                        \
        return mSlots[index];                                                             \
      }                                                                                   \


#define regui_widget(WIDGET, NAME) \
    private: \
      WIDGET* mWidget##NAME; \
    public: \
      _This& set##NAME(WIDGET* value) { \
        mWidget##NAME = value; \
        return getThis(); \
      } \
      WIDGET* get##NAME() const { \
        return mWidget##NAME; \
      } \


#define regui_end_construction_args()                                                     \
    };                                                                                    \
  private:




#define __builder(NAME)                                                                   \
  const typename NAME::ConstructionArguments

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @struct
 * BaseNamedConstructionArguments
 *
 * @brief
 * BaseNamedConstructionArguments is the base type of all construction arguments.
 * In the future it will provide all settings that are common to all construction arguments
 * and thereby their widgets.
 */
template<typename TWidget>
struct BaseNamedConstructionArguments {
  typedef typename TWidget::ConstructionArguments WidgetConstructionType;

  BaseNamedConstructionArguments() = default;

  WidgetConstructionType& getThis() {
    return *(static_cast<WidgetConstructionType*>(this));
  }
};

/**
 * @class
 * RequiredArgsBase
 *
 * @brief
 * RequiredArgsBase represents the base class for all required arguments during the creation of
 * a widget.
 */
class RequiredArgsBase {};

class RequiredArgs0 : public RequiredArgsBase {
public:

  RequiredArgs0() = default;

  template<typename TType>
  void callConstruct(TType* widget, __builder(TType)& args) {
    widget->construct(args);
  }
};

template<typename... TArgs>
class RequiredArgsVariadic : public RequiredArgsBase {
public:

  RequiredArgsVariadic(TArgs... args) {
    mElements = std::make_tuple(args...);
  }

  template<typename TType>
  void callConstruct(TType* widget, __builder(TType)& args) {
    widget->construct(args, RECore::Forward<TArgs&&>(args)...);
  }

private:

  std::tuple<TArgs...> mElements;
};

/**
 * @class
 * RequiredArgs
 *
 * @brief
 * RequiredArgs is a helper cass for generating an constructing the required arguments and their
 * widgets in a variadic way.
 */
template<typename... TArgs>
class RequiredArgs {
public:

  static RequiredArgsVariadic<TArgs...>* makeRequiredArgs(TArgs&&... args) {
    return new RequiredArgsVariadic<TArgs...>(args...);
  }
};

/**
 * @class
 * MakeDecl
 *
 * @brief
 * MakeDecl represents a helper class which will be used during the instantiation of widgets.
 */
template<typename TType>
class MakeDecl {
public:

  MakeDecl(TType* widget, RequiredArgsBase* args)
  : mWidget(widget)
  , mArgs(args) {}

  TType* operator <<=(__builder(TType) args) {
    ((TType*)mWidget)->construct(args);
    return mWidget;
  }
private:

  TType* mWidget;
  RequiredArgsBase* mArgs;
};


/**
 * @def
 * gui_new
 *
 * @brief
 * This macro will ease the creation of additional widgets of any kind.
 *
 * @verbatim
 * As an example we can look at an example on how to generate some random widgets in a vertical line.
 * This could look like the following:
 *  GUI_NEW(VerticalBoxLayout)
 *   +VerticalBoxLayout::Slot()
 *   [
 *     gui_new(Button)
 *      .setText("Button Text")
 *      .setDisabled(true)
 *   ]
 *   +VerticalBoxLayout::Slot()
 *   [
 *     gui_new(Text)
 *     .setText("Some Text")
 *   ]
 *   +VerticalBoxLayout::Slot()
 *   [
 *     gui_new(ColorPicker)
 *      .setColor(RECore::Color4::GREEN)
 *      .setEnableAlpha(true)
 *   ];
 *
 * Okay, but what is happening here? At first we create a vertical layout Box. To this layout box will be
 * added three slots (indicated by the '+VerticalBoxLayout::Slot()'). Each of this new slot is aligned on the vertical
 * side. To every of this slots a different widget will be assigned (a button, some text, and a color picker).
 */
#define gui_new(TYPE, ...) \
  REGui::MakeDecl<TYPE>(new TYPE(__VA_ARGS__), REGui::RequiredArgs<__VA_ARGS__>::makeRequiredArgs()) <<= TYPE::ConstructionArguments()


/**
 * @def
 * gui_assign
 *
 * @brief
 * The same as gui_new, but with the difference that we will also directly add the widget pointer as parameter
 * where the freshly created instance should be assigned to.
 */
#define gui_assign(VAR, TYPE, ...) \
  REGui::MakeDecl<TYPE>(VAR = new TYPE(__VA_ARGS__), REGui::RequiredArgs::makeRequiredArgs()) <<= TYPE::ConstructionArguments()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
