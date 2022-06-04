
set(FILES
  Private/REGui.cpp

  # Application
  Private/Application/GuiApplication.cpp
  Private/Application/GuiContext.cpp

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
  Private/Widget/Container/Container.cpp
  # Widget/Control
  Private/Widget/Control/CheckBox.cpp
  Private/Widget/Control/ComboBox.cpp
  # Widget/Layout
  Private/Widget/Layout/NewLine.cpp
  Private/Widget/Layout/SameLine.cpp
  # Widget/Menu
  Private/Widget/Menu/Menu.cpp
  Private/Widget/Menu/MenuItem.cpp
  Private/Widget/Menu/MainMenuBar.cpp
  Private/Widget/Menu/MenuBar.cpp
  # Widget/Panel
  Private/Widget/Panel/Panel.cpp
  # Widget/Text
  Private/Widget/Text/Text.cpp
  Private/Widget/Text/ClickableText.cpp
  Private/Widget/Text/ColoredText.cpp
  Private/Widget/Text/DisabledText.cpp
  # Widget/Window
  Private/Widget/Window/MainWindow.cpp
  Private/Widget/Window/DockableMainWindow.cpp
)