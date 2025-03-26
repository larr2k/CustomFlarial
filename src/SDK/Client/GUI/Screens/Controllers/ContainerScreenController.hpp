#pragma once

enum class ContainerEnum {
    INVENTORY,
    HOTBAR,
    CONTAINER_OUTPUT,
    OTHER
};

class Item;

class ContainerScreenController {
private:
    ContainerEnum getContainerType(std::string name);
    Item* getContainerItem(ContainerEnum type, int slot);
public:
    void _handlePlaceAll(std::string collectionName, int32_t slot);
    void _handleTakeAll(std::string collectionName, int32_t slot);

    void swap(std::string srcCollectionName, int32_t srcSlot, std::string dstCollectionName, int32_t dstSlot);
};
