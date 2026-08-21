#include "inventory_ui.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "inventory_holder.hpp"

using namespace godot;

void InventoryUI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_holder", "inventory_holder"), &InventoryUI::set_inventory_holder);
	ClassDB::bind_method(D_METHOD("get_inventory_holder"), &InventoryUI::get_inventory_holder);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory_holder", PROPERTY_HINT_NODE_TYPE, "InventoryHolder"), "set_inventory_holder", "get_inventory_holder");

	ClassDB::bind_method(D_METHOD("set_item_slot_scene", "item_slot_scene"), &InventoryUI::set_item_slot_scene);
	ClassDB::bind_method(D_METHOD("get_item_slot_scene"), &InventoryUI::get_item_slot_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_slot_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_item_slot_scene", "get_item_slot_scene");

	ClassDB::bind_method(D_METHOD("set_inventory", "inventory"), &InventoryUI::set_inventory);
	ClassDB::bind_method(D_METHOD("get_inventory"), &InventoryUI::get_inventory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory"), "set_inventory", "get_inventory");
}

void InventoryUI::set_inventory_holder(InventoryHolder *p_inventory_holder) { inventory_holder = p_inventory_holder; }
InventoryHolder *InventoryUI::get_inventory_holder() const { return inventory_holder; }

void InventoryUI::set_item_slot_scene(const Ref<PackedScene> &p_scene) { item_slot_scene = p_scene; }
Ref<PackedScene> InventoryUI::get_item_slot_scene() const { return item_slot_scene; }

void InventoryUI::set_inventory(const Ref<Inventory> &p_inventory) {
	ERR_FAIL_COND_MSG(inventory_holder == nullptr, vformat("InventoryUI %s couldn't set inventory as InventoryHolder is not set", this));
	inventory_holder->set_inventory(p_inventory);
}
Ref<Inventory> InventoryUI::get_inventory() const {
	if (inventory_holder == nullptr) {
		ERR_FAIL_V_MSG(nullptr, "Couldn't parse the input arguments.");
	}
	return inventory_holder->get_inventory();
}
