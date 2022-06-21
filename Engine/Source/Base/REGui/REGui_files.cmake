
set(FILES
  Private/REGui.cpp

  # Application
  Private/Application/GuiApplication.cpp
  Private/Application/GuiContext.cpp

  # Docking
  Private/Docking/DockArea.cpp
  Private/Docking/DockSpace.cpp

  # Gui
  Private/Gui/Gui.cpp
  Private/Gui/GuiImpl.cpp
  Private/Gui/GuiMessage.cpp
  Private/Gui/GuiRenderer.cpp
  Private/Gui/NativeWindow.cpp
  Private/Gui/NativeWindowImpl.cpp
  Private/Gui/Resource/GuiRendererResources.cpp
  Private/Gui/Resource/GuiTextureManager.cpp

  # Helper
  Private/Helper/ImGuiHelper.cpp

  # Layout
  Private/Layout/BasicSlot.cpp
  Private/Layout/Children.cpp
  Private/Layout/NoChildren.cpp
  Private/Layout/Slot.cpp
  Private/Layout/MultiSlot.cpp

  # Style
  Private/Style/Style.cpp
  Private/Style/ButtonStyle.cpp

  # Theme
  Private/Theme/Theme.cpp
  Private/Theme/ThemeDark.cpp
  Private/Theme/ThemeManager.cpp

  # Widget
  Private/Widget/Widget.cpp
  # Widget/Button
  Private/Widget/Button/AbstractButton.cpp
  Private/Widget/Button/Button.cpp
  Private/Widget/Button/ArrowButton.cpp
  Private/Widget/Button/ColoredButton.cpp
  Private/Widget/Button/SmallButton.cpp
  # Widget/Color
  Private/Widget/Color/ColorEdit.cpp
  Private/Widget/Color/ColorPicker.cpp
  # Widget/Container
  Private/Widget/Container/Collapsable.cpp
  Private/Widget/Container/Compound.cpp
  Private/Widget/Container/Container.cpp
  Private/Widget/Container/NullWidget.cpp
  # Widget/Control
  Private/Widget/Control/CheckBox.cpp
  Private/Widget/Control/ComboBox.cpp
  # Private/Input
  Private/Widget/Input/IntInput.cpp
  Private/Widget/Input/PasswordInput.cpp
  Private/Widget/Input/TextAreaInput.cpp
  Private/Widget/Input/TextInput.cpp
  # Widget/Layout
  Private/Widget/Layout/BoxLayout.cpp
  Private/Widget/Layout/Form.cpp
  Private/Widget/Layout/HorizontalBoxLayout.cpp
  Private/Widget/Layout/Layout.cpp
  Private/Widget/Layout/NewLine.cpp
  Private/Widget/Layout/SameLine.cpp
  Private/Widget/Layout/StackLayout.cpp
  Private/Widget/Layout/TreeNode.cpp
  Private/Widget/Layout/VerticalBoxLayout.cpp
  # Widget/Menu
  Private/Widget/Menu/ContextMenu.cpp
  Private/Widget/Menu/Menu.cpp
  Private/Widget/Menu/MenuItem.cpp
  Private/Widget/Menu/MainMenuBar.cpp
  Private/Widget/Menu/MenuBar.cpp
  Private/Widget/Menu/MenuSeparator.cpp
  # Widget/Modal
  Private/Widget/Modal/Modal.cpp
  # Notification
  Private/Widget/Notification/Notification.cpp
  Private/Widget/Notification/NotificationLayer.cpp
  # Widget/Panel
  Private/Widget/Panel/Panel.cpp
  # Widget/ProgressBar
  Private/Widget/ProgressBar/ProgressBar.cpp
  # Widget/Slider
  Private/Widget/Slider/DoubleSlider.cpp
  Private/Widget/Slider/FloatSlider.cpp
  Private/Widget/Slider/IntSlider.cpp
  # Widget/Tab
  Private/Widget/Tab/TabBar.cpp
  Private/Widget/Tab/TabItem.cpp
  # Widget/Table
  Private/Widget/Table/Table.cpp
  # Widget/Text
  Private/Widget/Text/Text.cpp
  Private/Widget/Text/ClickableText.cpp
  Private/Widget/Text/ColoredText.cpp
  Private/Widget/Text/DisabledText.cpp
  Private/Widget/Text/UnformattedText.cpp
  # Widget/Window
  Private/Widget/Window/MainWindow.cpp
  Private/Widget/Window/DockableMainWindow.cpp
)