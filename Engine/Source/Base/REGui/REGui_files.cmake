

set(FILES
  Private/REGui.cpp

  # Application
  Private/Application/MinGuiApplication.cpp
  Private/Application/GuiApplication.cpp
  Private/Application/GuiContext.cpp

  # Gui
  # The current fast single-window implementation
  Private/Gui/GuiManager.cpp
  Private/Gui/GuiManagerImpl.cpp
  # The later (new) interface
  Private/Gui/AppWindow.cpp
  Private/Gui/Gui.cpp
  Private/Gui/GuiImpl.cpp
  Private/Gui/GuiMessage.cpp
  Private/Gui/NativeWindow.cpp
  Private/Gui/NativeWindowImpl.cpp
  Private/Gui/Screen.cpp
  Private/Gui/WindowManager.cpp

  # Theme
  Private/Theme/Theme.cpp

  # Widget
  Private/Widget/Widget.cpp
  Private/Widget/DataWidget.cpp

  # Container
  Private/Widget/Container/Container.cpp
  Private/Widget/Container/Frame.cpp
  Private/Widget/Container/Panel.cpp

  # Input
  Private/Widget/Input/InputFloat.cpp

  # Layout
  Private/Widget/Layout/Stack.cpp

  # Menu
  Private/Widget/Menu/MenuBar.cpp

  # Window
  Private/Widget/Window/MainWindow.cpp
)