#include "menu/MenuSystem.h"

#include <algorithm> // std::max
#include <iostream>  // std::cerr
#include <stdexcept> // std::runtime_error
#include <utility>   // std::move

#include <nanovg.h>

// Переводит наш Color в цвет NanoVG.
static NVGcolor makeNanoVGColor(const Color& color) {
    return nvgRGBA(
        clampColorChannel(color.r),
        clampColorChannel(color.g),
        clampColorChannel(color.b),
        clampColorChannel(color.a)
    );
}

MenuSystem::MenuSystem() {
    // На первом уровне меню изначально выбран пункт с индексом 0.
    selectedIndices.push_back(0);
}

void MenuSystem::setRootMenu(std::unique_ptr<MenuItem> menu) {
    rootMenu = std::move(menu);

    currentPath.clear();
    selectedIndices.clear();
    selectedIndices.push_back(0);
}

void MenuSystem::addDialog(std::unique_ptr<DialogWindow> dialog) {
    dialogs.emplace(dialog->getId(), std::move(dialog));
}

void MenuSystem::setActionExecutor(std::function<void(const std::string&)> executor) {
    actionExecutor = std::move(executor);
}

void MenuSystem::open() {
    menuOpen = true;
}

void MenuSystem::close() {
    menuOpen = false;
    dialogOpen = false;
    activeDialogId.clear();

    currentPath.clear();
    selectedIndices.clear();
    selectedIndices.push_back(0);
}

bool MenuSystem::isOpen() const {
    return menuOpen;
}

void MenuSystem::handleKey(KeyCode keyCode) {
    // Если открыт диалог, клавиши сначала обрабатываются диалогом.
    if (dialogOpen) {
        handleDialogKey(keyCode);
        return;
    }

    if (keyCode == KeyCode::Up) {
        moveUp();
    }
    else if (keyCode == KeyCode::Down) {
        moveDown();
    }
    else if (keyCode == KeyCode::Enter) {
        enter();
    }
    else if (keyCode == KeyCode::Delete) {
        goBack();
    }
    else if (keyCode == KeyCode::Escape) {
        escape();
    }
}

void MenuSystem::moveUp() {
    MenuItem& currentMenu = getCurrentMenu();
    const std::size_t count = currentMenu.getChildrenCount();

    if (count == 0) {
        return;
    }

    std::size_t& selectedIndex = getCurrentSelectedIndex();

    if (selectedIndex == 0) {
        selectedIndex = count - 1;
    }
    else {
        --selectedIndex;
    }
}

void MenuSystem::moveDown() {
    MenuItem& currentMenu = getCurrentMenu();
    const std::size_t count = currentMenu.getChildrenCount();

    if (count == 0) {
        return;
    }

    std::size_t& selectedIndex = getCurrentSelectedIndex();
    selectedIndex = (selectedIndex + 1) % count;
}

void MenuSystem::enter() {
    MenuItem& currentMenu = getCurrentMenu();
    const std::size_t count = currentMenu.getChildrenCount();

    if (count == 0) {
        return;
    }

    std::size_t selectedIndex = getCurrentSelectedIndex();

    if (selectedIndex >= count) {
        selectedIndex = 0;
        getCurrentSelectedIndex() = 0;
    }

    MenuItem& selectedItem = currentMenu.getChild(selectedIndex);

    // Если у пункта есть дочерние пункты, открываем подменю.
    if (selectedItem.hasChildren()) {
        currentPath.push_back(selectedIndex);
        selectedIndices.push_back(0);
        return;
    }

    // Если у пункта есть действие, вызываем функцию из Application.
    if (selectedItem.hasAction() && actionExecutor) {
        actionExecutor(selectedItem.getActionName());
    }
}

void MenuSystem::goBack() {
    if (dialogOpen) {
        closeDialog();
        return;
    }

    // Если мы внутри подменю, возвращаемся на уровень выше.
    if (!currentPath.empty()) {
        currentPath.pop_back();

        if (selectedIndices.size() > 1) {
            selectedIndices.pop_back();
        }

        return;
    }

    // Если мы уже в главном меню, закрываем меню.
    close();
}

void MenuSystem::escape() {
    if (dialogOpen) {
        closeDialog();
        return;
    }

    close();
}

void MenuSystem::openDialog(const std::string& dialogId) {
    if (dialogs.find(dialogId) == dialogs.end()) {
        std::cerr << "Unknown dialog: " << dialogId << std::endl;
        return;
    }

    dialogOpen = true;
    activeDialogId = dialogId;
    selectedDialogButtonIndex = 0;
}

void MenuSystem::closeDialog() {
    dialogOpen = false;
    activeDialogId.clear();
    selectedDialogButtonIndex = 0;
}

void MenuSystem::draw(NVGcontext* context, float screenWidth, float screenHeight) {
    if (!menuOpen || rootMenu == nullptr) {
        return;
    }

    drawCurrentMenu(context, screenWidth, screenHeight);

    if (dialogOpen) {
        drawDialog(context, screenWidth, screenHeight);
    }
}

MenuItem& MenuSystem::getCurrentMenu() {
    if (rootMenu == nullptr) {
        throw std::runtime_error("Root menu is not set");
    }

    MenuItem* currentMenu = rootMenu.get();

    // Проходим по currentPath и спускаемся во вложенные подменю.
    for (std::size_t index : currentPath) {
        currentMenu = &currentMenu->getChild(index);
    }

    return *currentMenu;
}

std::size_t& MenuSystem::getCurrentSelectedIndex() {
    if (selectedIndices.empty()) {
        selectedIndices.push_back(0);
    }

    return selectedIndices.back();
}

void MenuSystem::handleDialogKey(KeyCode keyCode) {
    auto dialogIterator = dialogs.find(activeDialogId);

    if (dialogIterator == dialogs.end()) {
        closeDialog();
        return;
    }

    DialogWindow& dialog = *dialogIterator->second;
    std::vector<DialogButton>& buttons = dialog.getButtons();

    if (keyCode == KeyCode::Escape || keyCode == KeyCode::Delete) {
        closeDialog();
        return;
    }

    if (buttons.empty()) {
        return;
    }

    if (keyCode == KeyCode::Up) {
        if (selectedDialogButtonIndex == 0) {
            selectedDialogButtonIndex = buttons.size() - 1;
        }
        else {
            --selectedDialogButtonIndex;
        }
    }
    else if (keyCode == KeyCode::Down) {
        selectedDialogButtonIndex = (selectedDialogButtonIndex + 1) % buttons.size();
    }
    else if (keyCode == KeyCode::Enter) {
        const DialogButton& button = buttons.at(selectedDialogButtonIndex);

        if (!button.getActionName().empty() && actionExecutor) {
            actionExecutor(button.getActionName());
        }
    }
}

void MenuSystem::drawCurrentMenu(NVGcontext* context, float screenWidth, float screenHeight) {
    MenuItem& currentMenu = getCurrentMenu();
    const std::size_t count = currentMenu.getChildrenCount();

    // Затемняем фон под меню. Это особенно важно для паузы, победы и поражения,
    // потому что они рисуются поверх игрового поля.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 0.0f, screenWidth, screenHeight);
    nvgFillColor(context, nvgRGBA(0, 0, 0, 135));
    nvgFill(context);

    const float gap = 12.0f;
    float totalButtonsHeight = 0.0f;
    float maxButtonWidth = 0.0f;

    for (std::size_t i = 0; i < count; ++i) {
        totalButtonsHeight += currentMenu.getChild(i).getHeight();
        maxButtonWidth = std::max(maxButtonWidth, currentMenu.getChild(i).getWidth());
    }

    if (count > 0) {
        totalButtonsHeight += gap * static_cast<float>(count - 1);
    }

    const float titleHeight = 46.0f;
    const float titleBottomGap = 30.0f;
    const float contentHeight = titleHeight + titleBottomGap + totalButtonsHeight;

    const float panelWidth = std::max(540.0f, maxButtonWidth + 100.0f);
    const float panelHeight = contentHeight + 70.0f;
    const float panelX = (screenWidth - panelWidth) / 2.0f;
    const float panelY = (screenHeight - panelHeight) / 2.0f;

    // Непрозрачная панель делает текст меню читаемым на любом игровом фоне.
    nvgBeginPath(context);
    nvgRoundedRect(context, panelX, panelY, panelWidth, panelHeight, 18.0f);
    nvgFillColor(context, nvgRGBA(25, 22, 18, 245));
    nvgFill(context);

    nvgBeginPath(context);
    nvgRoundedRect(context, panelX, panelY, panelWidth, panelHeight, 18.0f);
    nvgStrokeColor(context, nvgRGBA(255, 205, 0, 255));
    nvgStrokeWidth(context, 3.0f);
    nvgStroke(context);

    const float titleY = panelY + 42.0f;

    nvgFontFace(context, "sans");
    nvgFontSize(context, 34.0f);
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, nvgRGBA(255, 220, 0, 255));
    nvgText(context, screenWidth / 2.0f, titleY, currentMenu.getText().c_str(), nullptr);

    if (count == 0) {
        return;
    }

    float y = panelY + 35.0f + titleHeight + titleBottomGap;

    for (std::size_t i = 0; i < count; ++i) {
        MenuItem& item = currentMenu.getChild(i);

        const float width = item.getWidth();
        const float height = item.getHeight();
        const float x = (screenWidth - width) / 2.0f;

        drawMenuButton(
            context,
            item,
            i == getCurrentSelectedIndex(),
            x,
            y,
            width,
            height
        );

        y += height + gap;
    }
}

void MenuSystem::drawDialog(NVGcontext* context, float screenWidth, float screenHeight) {
    auto dialogIterator = dialogs.find(activeDialogId);

    if (dialogIterator == dialogs.end()) {
        return;
    }

    DialogWindow& dialog = *dialogIterator->second;

    const float dialogWidth = dialog.getWidth();
    const float dialogHeight = dialog.getHeight();

    const float x = (screenWidth - dialogWidth) / 2.0f;
    const float y = (screenHeight - dialogHeight) / 2.0f;

    // Затемнение фона под диалогом.
    nvgBeginPath(context);
    nvgRect(context, 0.0f, 0.0f, screenWidth, screenHeight);
    nvgFillColor(context, nvgRGBA(0, 0, 0, 150));
    nvgFill(context);

    // Основной прямоугольник диалогового окна.
    nvgBeginPath(context);
    nvgRoundedRect(context, x, y, dialogWidth, dialogHeight, 18.0f);
    nvgFillColor(context, nvgRGBA(35, 35, 45, 255));
    nvgFill(context);

    // Заголовок диалога.
    nvgFontFace(context, "sans");
    nvgFontSize(context, 30.0f);
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, nvgRGBA(255, 255, 255, 255));
    nvgText(context, x + dialogWidth / 2.0f, y + 45.0f, dialog.getTitle().c_str(), nullptr);

    // Основной текст диалога.
    nvgFontSize(context, 20.0f);
    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFillColor(context, nvgRGBA(230, 230, 230, 255));
    nvgTextBox(
        context,
        x + 35.0f,
        y + 85.0f,
        dialogWidth - 70.0f,
        dialog.getText().c_str(),
        nullptr
    );

    std::vector<DialogButton>& buttons = dialog.getButtons();

    if (buttons.empty()) {
        return;
    }

    const float gap = 14.0f;

    float totalButtonsWidth = 0.0f;
    float maxButtonHeight = 0.0f;

    for (DialogButton& button : buttons) {
        totalButtonsWidth += button.getWidth();
        maxButtonHeight = std::max(maxButtonHeight, button.getHeight());
    }

    totalButtonsWidth += gap * static_cast<float>(buttons.size() - 1);

    float buttonX = x + (dialogWidth - totalButtonsWidth) / 2.0f;
    const float buttonY = y + dialogHeight - maxButtonHeight - 28.0f;

    for (std::size_t i = 0; i < buttons.size(); ++i) {
        DialogButton& button = buttons.at(i);
        const bool selected = i == selectedDialogButtonIndex;

        nvgBeginPath(context);
        nvgRoundedRect(
            context,
            buttonX,
            buttonY,
            button.getWidth(),
            button.getHeight(),
            12.0f
        );

        if (selected) {
            nvgFillColor(context, nvgRGBA(90, 140, 230, 255));
        }
        else {
            nvgFillColor(context, nvgRGBA(70, 70, 85, 255));
        }

        nvgFill(context);

        drawTextCentered(
            context,
            button.getText(),
            buttonX,
            buttonY,
            button.getWidth(),
            button.getHeight(),
            20.0f
        );

        buttonX += button.getWidth() + gap;
    }
}

void MenuSystem::drawMenuButton(
    NVGcontext* context,
    MenuItem& item,
    bool selected,
    float x,
    float y,
    float width,
    float height
) {
    nvgBeginPath(context);
    nvgRoundedRect(context, x, y, width, height, 14.0f);

    if (selected) {
        nvgFillColor(context, makeNanoVGColor(item.getSelectedColor()));
    }
    else {
        nvgFillColor(context, makeNanoVGColor(item.getNormalColor()));
    }

    nvgFill(context);

    std::string text = item.getText();

    if (item.hasChildren()) {
        text += " >";
    }

    nvgFontFace(context, "sans");
    nvgFontSize(context, 22.0f);
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, makeNanoVGColor(item.getTextColor()));
    nvgText(context, x + width / 2.0f, y + height / 2.0f, text.c_str(), nullptr);
}

void MenuSystem::drawTextCentered(
    NVGcontext* context,
    const std::string& text,
    float x,
    float y,
    float width,
    float height,
    float fontSize
) {
    nvgFontFace(context, "sans");
    nvgFontSize(context, fontSize);
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, nvgRGBA(255, 255, 255, 255));
    nvgText(context, x + width / 2.0f, y + height / 2.0f, text.c_str(), nullptr);
}
