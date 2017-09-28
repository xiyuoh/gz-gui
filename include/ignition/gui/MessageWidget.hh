/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGNITION_GUI_MESSAGEWIDGET_HH_
#define IGNITION_GUI_MESSAGEWIDGET_HH_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <ignition/math/Vector3.hh>

#include "ignition/gui/qt.h"
#include "ignition/gui/PropertyWidget.hh"
#include "ignition/gui/System.hh"

namespace google
{
  namespace protobuf
  {
    class Message;
    class Reflection;
    class FieldDescriptor;
  }
}

namespace ignition
{
  namespace gui
  {
    class MessageWidgetPrivate;

    /// \brief A widget generated from a google protobuf message.
    class IGNITION_GUI_VISIBLE MessageWidget : public QWidget
    {
      Q_OBJECT

      /// \brief Constructor
      public: MessageWidget();

      /// \brief Destructor
      public: ~MessageWidget();

      /// \brief Load from a google protobuf message.
      /// \param[in] _msg Message to load from.
      public: void Load(const google::protobuf::Message *_msg);

      /// \brief Get the updated message.
      /// \return Updated message.
      public: google::protobuf::Message *Msg();

      /// \brief Signal that a property widget's value has changed.
      /// \param[in] _name Scoped name of widget.
      /// \param[in] _value New value.
      signals: void ValueChanged(const std::string &_name, const QVariant _value);

      /// \brief Set whether a child widget should be visible.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _visible True to set the widget to be visible.
      public: void SetWidgetVisible(const std::string &_name, bool _visible);

      /// \brief Get whether a child widget is visible.
      /// \param[in] _name Name of the child widget.
      /// \return True if the widget is visible.
      public: bool WidgetVisible(const std::string &_name) const;

      /// \brief Set whether a child widget should be read-only.
      /// \param[in] _name Name of the child widget.
      /// \param[in] _visible True to set the widget to be read-only.
      public: void SetWidgetReadOnly(const std::string &_name, bool _readOnly);

      /// \brief Get whether a child widget is read-only.
      /// \param[in] _name Name of the child widget.
      /// \return True if the widget is read-only.
      public: bool WidgetReadOnly(const std::string &_name) const;

      /// \brief Update the widgets from a message.
      /// \param[in] _msg Message used for updating the widgets.
      public: void UpdateFromMsg(const google::protobuf::Message *_msg);

      /// \brief Set a value of a property widget.
      /// \param[in] _name Name of the property widget.
      /// \param[in] _value Value to set to.
      /// \return True if the value is set successfully.
      public: bool SetPropertyValue(const std::string &_name,
                                    const QVariant _value);

      /// \brief Get value from a property widget.
      /// \param[in] _name Name of the property widget.
      /// \return Value as QVariant.
      public: QVariant PropertyValue(const std::string &_name) const;

      /// \brief Register a child widget as a child of this widget, so it can
      /// be updated. Note that the widget is not automatically added to a
      /// layout.
      /// \param[in] _name Unique name to indentify the child within this widget
      /// \param[in] _child Child widget to be added. It doesn't need to be a
      /// PropertyWidget.
      /// \return True if child successfully added.
      public: bool AddPropertyWidget(const std::string &_name,
          PropertyWidget *_child);

      /// \brief Get a config child widget by its name.
      /// \param[in] _name Scoped name of the child widget.
      /// \return The child widget with the given name or nullptr if it wasn't
      /// found.
      public: PropertyWidget *PropertyWidgetByName(
          const std::string &_name) const;

      /// \brief Get the number of child widgets.
      /// \return The number of child widgets.
      public: unsigned int PropertyWidgetCount() const;

      /// \brief Parse the input message and either create widgets for
      /// configuring fields of the message, or update the widgets with values
      /// from the message.
      /// \param[in] _msg Message.
      /// \param[in] _update True to parse only fields that are specified in
      /// the message rather than all the available fields in the message
      /// \param[in] _name Name used when creating new widgets.
      /// \param[in] _level Level of the widget in the tree.
      /// return Updated widget.
      private: QWidget *Parse(google::protobuf::Message *_msg,
          bool _update = false, const std::string &_name = "",
          const int _level = 0);

      /// \brief Update the message field using values from the widgets.
      /// \param[in] _msg Message to be updated.
      /// \param[in] _name Optional name of a property field. By default, takes
      /// the whole message.
      /// \return True if successful
      private: bool FillMsg(google::protobuf::Message *_msg,
          const std::string &_name = "");

      /// \brief Signal emitted when mass value changes.
      /// \param[in] _value Mass value.
// TODO: handle this
//      Q_SIGNALS: void MassValueChanged(const double &_value);

      /// \brief Callback when mass value changes in child widget.
      /// \param[in] _value Mass value.
//      private slots: void OnMassValueChanged(const double _value);

      /// \brief Qt event filter currently used to filter mouse wheel events.
      /// \param[in] _obj Object that is watched by the event filter.
      /// \param[in] _event Qt event.
      /// \return True if the event is handled.
      private: bool eventFilter(QObject *_obj, QEvent *_event);

      /// \internal
      /// \brief Pointer to private data.
      private: std::unique_ptr<MessageWidgetPrivate> dataPtr;
    };
  }
}
#endif
