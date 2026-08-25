#include "inventory_slot.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "resource/inventory_item.hpp"
#include <utility>

using namespace godot;

void InventorySlot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot_ui_scene", "slot_ui_scene"), &InventorySlot::set_slot_ui_scene);
	ClassDB::bind_method(D_METHOD("get_slot_ui_scene"), &InventorySlot::get_slot_ui_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot_ui_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_slot_ui_scene", "get_slot_ui_scene");

	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventorySlot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventorySlot::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");

	ClassDB::bind_method(D_METHOD("swap_item_from_slot", "slot"), &InventorySlot::swap_item_from_slot);
	ClassDB::bind_method(D_METHOD("place_item_from_slot", "slot"), &InventorySlot::place_item_from_slot);
	ClassDB::bind_method(D_METHOD("interact_item_from_slot", "slot"), &InventorySlot::interact_item_from_slot);

	GDVIRTUAL_BIND(_can_hold_item, "item");
}

void InventorySlot::set_slot_ui_scene(const Ref<PackedScene> &p_scene) {
	slot_ui_scene = p_scene;
	emit_changed();
}
Ref<PackedScene> InventorySlot::get_slot_ui_scene() const { return slot_ui_scene; }

void InventorySlot::set_item(const Ref<InventoryItem> &p_item) {
	item = p_item;
	emit_changed();
}
Ref<InventoryItem> InventorySlot::get_item() const { return item; }

bool InventorySlot::_can_hold_item(const Ref<InventoryItem> &p_item) { return true; }

bool InventorySlot::place_item(const Ref<InventoryItem> &p_item) { return _can_hold_item(p_item) ? item->place(p_item) : false; }

bool InventorySlot::swap_item_from_slot(const Ref<InventorySlot> &p_slot) {
	// Check if this slot can hold input slot's item
	if (!_can_hold_item(p_slot->item))
		return false;
	// Check if input slot can hold this slot's item
	if (!p_slot->_can_hold_item(item))
		return false;

	std::swap(p_slot->item, item);
	return true;
}

bool InventorySlot::place_item_from_slot(const Ref<InventorySlot> &p_slot) {
	// If item is empty, just try existing swap
	if (item.is_null())
		return swap_item_from_slot(p_slot);

	// Try adding item from slot inside the item
	return place_item(p_slot->item);
}

bool InventorySlot::interact_item_from_slot(const Ref<InventorySlot> &p_slot) {
	if (place_item_from_slot(p_slot))
		return true;
	if (swap_item_from_slot(p_slot))
		return true;
	return false;
}
