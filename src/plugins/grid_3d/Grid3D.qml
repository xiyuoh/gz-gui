/*
 * Copyright (C) 2018 Open Source Robotics Foundation
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
import QtQuick 2.9
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.3
import "qrc:/qml"

GridLayout {
  columns: 4
  columnSpacing: 10
  Layout.minimumWidth: 300
  Layout.minimumHeight: 625
  anchors.fill: parent
  anchors.leftMargin: 10
  anchors.rightMargin: 10

  // Dummy item to use getDecimals()
  IgnHelpers {
    id: ignHelpers
  }

  Connections {
    target: Grid3D
    onNewParams: {
      horizontalCellCount.value = _hCellCount;
      verticalCellCount.value = _vCellCount;
      cellLength.value = _cellLength;
      gzPoseInstance.xValue = _pos.x;
      gzPoseInstance.yValue = _pos.y;
      gzPoseInstance.zValue = _pos.z;
      gzPoseInstance.rollValue = _rot.x;
      gzPoseInstance.pitchValue = _rot.y;
      gzPoseInstance.yawValue = _rot.z;
      gzColorGrid.r = _color.r;
      gzColorGrid.g = _color.g;
      gzColorGrid.b = _color.b;
      gzColorGrid.a = _color.a;
    }
  }

  ComboBox {
    id: combo
    Layout.columnSpan: 2
    Layout.fillWidth: true
    model: Grid3D.nameList
    onCurrentIndexChanged: {
      if (currentIndex < 0)
        return;

      Grid3D.OnName(textAt(currentIndex));
    }
    popup.width: parent.width
    ToolTip.visible: hovered
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: qsTr("Grids in the scene")
  }

  RoundButton {
    text: "\u21bb"
    Layout.columnSpan: 1
    Material.background: Material.primary
    onClicked: {
      Grid3D.OnRefresh();
    }
    ToolTip.visible: hovered
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: qsTr("Refresh list of grids")
  }

  CheckBox {
    Layout.alignment: Qt.AlignHCenter
    id: showgrid
    Layout.columnSpan: 1
    text: qsTr("Show")
    checked: true
    onClicked: {
      Grid3D.OnShow(checked)
    }
  }

  Text {
    Layout.columnSpan: 4
    text: "Cell Count"
    color: "dimgrey"
    font.bold: true
  }

  Text {
    Layout.columnSpan: 2
    id: vercelltext
    color: "dimgrey"
    text: "Vertical"
  }

  IgnSpinBox {
    Layout.columnSpan: 2
    Layout.fillWidth: true
    id: verticalCellCount
    maximumValue: Number.MAX_VALUE
    minimumValue: 0
    value: 0
    onEditingFinished: Grid3D.UpdateVCellCount(verticalCellCount.value)
  }

  Text {
    Layout.columnSpan: 2
    id: honcelltext
    color: "dimgrey"
    text: "Horizontal"
  }

  IgnSpinBox {
    Layout.columnSpan: 2
    Layout.fillWidth: true
    id: horizontalCellCount
    maximumValue: Number.MAX_VALUE
    minimumValue: 1
    value: 20
    onEditingFinished: Grid3D.UpdateHCellCount(horizontalCellCount.value)
  }

  Text {
    Layout.columnSpan: 4
    id: celllengthtext
    text: "Cell Length"
    color: "dimgrey"
    font.bold: true
  }

  Text {
    Layout.columnSpan: 2
    id: length
    color: "dimgrey"
    text: "Length (m)"
  }
  IgnSpinBox {
    Layout.columnSpan: 2
    Layout.fillWidth: true
    id: cellLength
    maximumValue: Number.MAX_VALUE
    minimumValue: 0.0000001
    value: 1.00
    decimals: ignHelpers.getDecimals(cellLength.width)
    stepSize: 0.01
    onEditingFinished: Grid3D.UpdateCellLength(cellLength.value)
  }

  Text {
    Layout.columnSpan: 2
    id: cartesian
    color: "dimgrey"
    font.bold: true
    text: "Position (m)"
  }

  Text {
    Layout.columnSpan: 2
    id: principal
    text: "Rotation (rad)"
    color: "dimgrey"
    font.bold: true
  }

  GzPose {
    id: gzPoseInstance
    Layout.columnSpan: 4
    Layout.fillWidth: true
    readOnly: false
    xValue: 0.00
    yValue: 0.00
    zValue: 0.00
    rollValue: 0.00
    pitchValue: 0.00
    yawValue: 0.00

    onGzPoseSet: {
      // _x, _y, _z, _roll, _pitch, _yaw are parameters of signal gzPoseSet
      // from gz-gui GzPose.qml
      Grid3D.SetPose(_x, _y, _z, _roll, _pitch, _yaw)
    }
    expand: true
  }

  Text {
    Layout.columnSpan: 4
    text: "Color"
    color: "dimgrey"
    font.bold: true
  }

  Text {
    Layout.columnSpan: 2
    color: "dimgrey"
    text: "Grid Color"
  }

  GzColor { 
    id: gzColorGrid
    Layout.columnSpan: 2
    Layout.alignment: Qt.AlignRight
    Layout.bottomMargin: 5
    Layout.rightMargin: 20
    onGzColorSet: Grid3D.SetColor(gzColorGrid.r, gzColorGrid.g, gzColorGrid.b, gzColorGrid.a)
  }
}

