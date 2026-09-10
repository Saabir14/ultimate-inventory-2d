#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/resource_uid.hpp"
#include "node/slot_ui.hpp"

using namespace godot;

void InventorySlot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot_ui_scene_path", "slot_scene_path"), &InventorySlot::set_slot_ui_scene_path);
	ClassDB::bind_method(D_METHOD("get_slot_ui_scene_path"), &InventorySlot::get_slot_ui_scene_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "slot_ui_scene_path", PROPERTY_HINT_FILE, "*.tscn, *.scn, *.res"), "set_slot_ui_scene_path", "get_slot_ui_scene_path");

	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventorySlot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventorySlot::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");

	ClassDB::bind_method(D_METHOD("instantiate_slot", "edit_state"), &InventorySlot::instantiate_slot_ui, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("swap_item_from_slot", "slot"), &InventorySlot::swap_item_from_slot);
	ClassDB::bind_method(D_METHOD("place_item_from_slot", "slot"), &InventorySlot::place_item_from_slot);
	ClassDB::bind_method(D_METHOD("interact_item_from_slot", "slot"), &InventorySlot::interact_item_from_slot);

	GDVIRTUAL_BIND(_can_hold_item, "item");
}

void InventorySlot::set_slot_ui_scene_path(const StringName scene_path) { slot_ui_scene_path = scene_path; }
StringName InventorySlot::get_slot_ui_scene_path() const { return slot_ui_scene_path; }

InventorySlotUI *InventorySlot::instantiate_slot_ui(PackedScene::GenEditState p_edit_state) {
	const StringName path = ResourceUID::ensure_path(slot_ui_scene_path);
	ERR_FAIL_COND_V_MSG(!ResourceLoader::get_singleton()->exists(path), nullptr, "No file found at " + path);

	const Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(path);
	ERR_FAIL_COND_V_MSG(!scene.is_valid(), nullptr, "File is not PackedScene at " + path);

	Node *node = scene->instantiate(p_edit_state);

	// node has to be SlotNode
	InventorySlotUI *slot_node = Object::cast_to<InventorySlotUI>(node);
	ERR_FAIL_COND_V_MSG(!slot_node, nullptr, "No SlotUI as root node for PackedScene at " + path);

	slot_node->set_slot(this);
	return slot_node;
}

void InventorySlot::set_item(const Ref<InventoryItem> &p_item) {
	item = p_item;
	emit_changed();
}
Ref<InventoryItem> InventorySlot::get_item() const { return item; }

bool InventorySlot::_can_hold_item(const Ref<InventoryItem> &p_item) { return true; }

bool InventorySlot::place_item(const Ref<InventoryItem> &p_item) {
	if (!_can_hold_item(p_item))
		return false;
	if (item.is_null()) {
		set_item(p_item);
		return true;
	}
	return item->place(p_item);
}

bool InventorySlot::swap_item_from_slot(const Ref<InventorySlot> &p_slot) {
	// Check if this slot can hold input slot's item
	if (!_can_hold_item(p_slot->item))
		return false;
	// Check if input slot can hold this slot's item
	if (!p_slot->_can_hold_item(item))
		return false;

	// Swap items
	std::swap(item, p_slot->item);

	// Let slots know their items have changed
	// Since std::swap was used, this needs
	// to be done manually here
	emit_changed();
	p_slot->emit_changed();

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
