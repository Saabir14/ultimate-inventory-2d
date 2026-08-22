#include "inventory_slot.hpp"
#include "resource/inventory_item.hpp"

using namespace godot;

void InventorySlot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventorySlot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventorySlot::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");
}

void InventorySlot::set_item(const Ref<InventoryItem> &p_item) { item = p_item; }
Ref<InventoryItem> InventorySlot::get_item() const { return item; }
