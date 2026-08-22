#include "inventory.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slots", "slots"), &Inventory::set_slots);
	ClassDB::bind_method(D_METHOD("get_slots"), &Inventory::get_slots);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "slots", PROPERTY_HINT_ARRAY_TYPE, "InventoryItem", PROPERTY_USAGE_DEFAULT, "Ref<InventoryItem>"), "set_items", "get_items");
}

void Inventory::set_slots(const TypedArray<Ref<InventorySlot>> &p_slots) { slots = p_slots; }
TypedArray<Ref<InventorySlot>> Inventory::get_slots() const { return slots; }
