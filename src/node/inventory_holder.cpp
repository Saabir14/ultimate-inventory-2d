#include "inventory_holder.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "resource/inventory.hpp"

using namespace godot;

void InventoryHolder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_attached_node"), &InventoryHolder::get_attached_node);
	ClassDB::bind_method(D_METHOD("set_attached_node", "node"), &InventoryHolder::set_attached_node);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "attached_node", PROPERTY_HINT_NODE_TYPE, "Node"), "set_attached_node", "get_attached_node");

	ClassDB::bind_method(D_METHOD("get_use_parent_as_attached_node"), &InventoryHolder::get_use_parent_as_attached_node);
	ClassDB::bind_method(D_METHOD("set_use_parent_as_attached_node", "use_parent"), &InventoryHolder::set_use_parent_as_attached_node);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_parent_as_attached_node"), "set_use_parent_as_attached_node", "get_use_parent_as_attached_node");

	ClassDB::bind_method(D_METHOD("get_inventory"), &InventoryHolder::get_inventory);
	ClassDB::bind_method(D_METHOD("set_inventory", "inventory"), &InventoryHolder::set_inventory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory"), "set_inventory", "get_inventory");

	ADD_SIGNAL(MethodInfo("inventory_set", PropertyInfo(Variant::OBJECT, "inventory", PROPERTY_HINT_RESOURCE_TYPE, "Inventory")));
}

void InventoryHolder::set_attached_node(Node *p_node) {
	if (attached_node == p_node)
		return;

	attached_node = p_node;
	use_parent_as_attached_node = false;
}
Node *InventoryHolder::get_attached_node() const { return attached_node; }

void InventoryHolder::set_use_parent_as_attached_node(bool p_use_parent) {
    use_parent_as_attached_node = p_use_parent;
    if (p_use_parent)
        attached_node = get_parent();
}
bool InventoryHolder::get_use_parent_as_attached_node() const { return use_parent_as_attached_node; }

void InventoryHolder::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_PARENTED:
            if (use_parent_as_attached_node)
                attached_node = get_parent();
            break;
    }
}

void InventoryHolder::set_inventory(const Ref<Inventory> &p_inventory) {
	inventory = p_inventory;
	emit_signal("inventory_set", inventory);
}
Ref<Inventory> InventoryHolder::get_inventory() const { return inventory; }
