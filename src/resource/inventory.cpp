#include "inventory.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

using namespace godot;

void Inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_slot", "slot"), &Inventory::set_slot);
	ClassDB::bind_method(D_METHOD("get_slot"), &Inventory::get_slot);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tempelate_slot", PROPERTY_HINT_RESOURCE_TYPE, "InventorySlot"), "set_slot", "get_slot");

	ClassDB::bind_method(D_METHOD("set_items", "items"), &Inventory::set_items);
	ClassDB::bind_method(D_METHOD("get_items"), &Inventory::get_items);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_RESOURCE_TYPE, "InventoryItem"), "set_items", "get_items");
}

void Inventory::set_slot(const Ref<InventorySlot> &p_slot) { slot = p_slot; }
Ref<InventorySlot> Inventory::get_slot() const { return slot; }

void Inventory::set_items(const TypedArray<Ref<InventoryItem>> &p_items) { items = p_items; }
TypedArray<Ref<InventoryItem>> Inventory::get_items() const { return items; }
