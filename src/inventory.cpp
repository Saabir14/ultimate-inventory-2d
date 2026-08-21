#include "inventory.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_items", "items"), &Inventory::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Inventory::get_items);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, "InventoryItem", PROPERTY_USAGE_DEFAULT, "Ref<InventoryItem>"), "set_items", "get_items");
}

Inventory::Inventory() { set_local_to_scene(true); }

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> &p_items) { items = p_items; }
TypedArray<Ref<InventoryItem>> Inventory::get_items() const { return items; }
