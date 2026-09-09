#include "node/slot_ui.hpp"

using namespace godot;

void InventorySlot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot_ui_scene", "slot_scene"), &InventorySlot::set_slot_ui_scene);
	ClassDB::bind_method(D_METHOD("get_slot_ui_scene"), &InventorySlot::get_slot_ui_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "slot_ui_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_slot_ui_scene", "get_slot_ui_scene");

	ClassDB::bind_method(D_METHOD("set_item", "item"), &InventorySlot::set_item);
	ClassDB::bind_method(D_METHOD("get_item"), &InventorySlot::get_item);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_item", "get_item");

	ClassDB::bind_method(D_METHOD("instantiate_slot", "edit_state"), &InventorySlot::instantiate_slot_ui, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("swap_item_from_slot", "slot"), &InventorySlot::swap_item_from_slot);
	ClassDB::bind_method(D_METHOD("place_item_from_slot", "slot"), &InventorySlot::place_item_from_slot);
	ClassDB::bind_method(D_METHOD("interact_item_from_slot", "slot"), &InventorySlot::interact_item_from_slot);

	GDVIRTUAL_BIND(_can_hold_item, "item");
}

void InventorySlot::set_slot_ui_scene(const Ref<PackedScene> &p_scene) {
	if (p_scene.is_null()) {
		slot_ui_scene = nullptr;
		return;
	}

	// Check type of root node
	Node *node = p_scene->instantiate();
	InventorySlotUI *slot_node = Object::cast_to<InventorySlotUI>(node);
	ERR_FAIL_COND_MSG(slot_node == nullptr, "set_slot_scene scene must have SlotNode as root node");
	node->queue_free();

	slot_ui_scene = p_scene;
}
Ref<PackedScene> InventorySlot::get_slot_ui_scene() const { return slot_ui_scene; }

// Instantiates a SlotNode from the slot_ui_scene
// The root node will (must) be SlotNode
// Instantiated root node will hold this slot resource as it's slot
InventorySlotUI *InventorySlot::instantiate_slot_ui(PackedScene::GenEditState p_edit_state) {
	if (slot_ui_scene.is_null())
		return nullptr;

	Node *node = slot_ui_scene->instantiate(p_edit_state);

	// node has to be SlotNode
	InventorySlotUI *slot_node = Object::cast_to<InventorySlotUI>(node);
	if (slot_node == nullptr) {
		node->queue_free();
		return nullptr;
	}

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
