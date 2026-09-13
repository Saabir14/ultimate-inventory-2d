#include "inventory_holder.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "resource/inventory.hpp"

using namespace godot;

void InventoryHolder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_inventory"), &InventoryHolder::get_inventory);
	ClassDB::bind_method(D_METHOD("set_inventory", "inventory"), &InventoryHolder::set_inventory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory"), "set_inventory", "get_inventory");

	ADD_SIGNAL(MethodInfo("inventory_set", PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory")));
}

void InventoryHolder::set_inventory(const Ref<Inventory> &p_inventory) {
	inventory = p_inventory;
	emit_signal("inventory_set", inventory);
}
Ref<Inventory> InventoryHolder::get_inventory() const { return inventory; }
