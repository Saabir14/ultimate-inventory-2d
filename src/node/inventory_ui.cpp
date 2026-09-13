#include "inventory_ui.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "node/slot_ui.hpp"

using namespace godot;

void InventoryUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory", "inventory"), &InventoryUI::set_inventory);
	ClassDB::bind_method(D_METHOD("get_inventory"), &InventoryUI::get_inventory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory"), "set_inventory", "get_inventory");
}

InventoryUI::InventoryUI() {
	set_process_internal(true);
}

void InventoryUI::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_UNPARENTED:
			_queue_free_slot_ui_nodes();
			break;

		case NOTIFICATION_PARENTED:
			_update_ui();
			index = get_index();
			break;

		case NOTIFICATION_INTERNAL_PROCESS:
			const int32_t new_index = get_index();
			if (new_index != index) {
				index = new_index;
				_update_ui();
			}
			break;
	}
}

void InventoryUI::set_inventory(const Ref<Inventory> &p_inventory) {
	if (inventory.is_valid())
		inventory->disconnect("changed", callable_mp(this, &InventoryUI::_update_ui));

	inventory = p_inventory;

	if (inventory.is_valid())
		inventory->connect("changed", callable_mp(this, &InventoryUI::_update_ui));

	_update_ui();
}
Ref<Inventory> InventoryUI::get_inventory() const { return inventory; }

void InventoryUI::_queue_free_slot_ui_nodes() {
	for (int64_t i = 0; i < slot_ui_nodes.size(); i++) {
		Node *node = Object::cast_to<Node>(slot_ui_nodes[i]);
		if (node)
			node->queue_free();
	}
	slot_ui_nodes.clear();
}

void InventoryUI::_update_ui() {
	_queue_free_slot_ui_nodes();

	Node *parent = get_parent();
	if (inventory.is_null() || !parent)
		return;

	slot_ui_nodes.resize(inventory->size());

	for (int64_t i = slot_ui_nodes.size() - 1; i >= 0; i--) {
		Ref<InventorySlot> slot = inventory->get_slot(i);
		if (slot.is_null())
			continue;

		InventorySlotUI *node = slot->instantiate_slot_ui(PackedScene::GEN_EDIT_STATE_INSTANCE);
		slot_ui_nodes[i] = node;

		// Add slots right below (behind) this node
		if (node) {
			parent->add_child(node, false, INTERNAL_MODE_DISABLED);
			parent->move_child(node, get_index() + 1);
		}
	}
}
