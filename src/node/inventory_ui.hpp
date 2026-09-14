#pragma once

#include "godot_cpp/variant/typed_array.hpp"
#include "node/inventory_holder.hpp"
#include "resource/inventory.hpp"

#include "godot_cpp/classes/node.hpp"

namespace godot {
class InventoryUI : public Node {
	GDCLASS(InventoryUI, Node);

private:
    // InventoryHolder reference to get the inventory
	InventoryHolder *inventory_holder = nullptr;
	// Inventory reference
   	Ref<Inventory> inventory;

	// Keep track of all the nodes instantiated by the inventory UI
	TypedArray<Node> slot_ui_nodes;

	// Queue free all slot UI nodes and remove from the slot_ui_nodes array
	void _queue_free_slot_ui_nodes();

	// Update the UI when Inventory changes
	void _update_ui();
	// Stores the current index of this node
	// Used to pool change of index to update UI
	int32_t index = -1;

protected:
	static void _bind_methods();

	void _notification(int p_what);

public:
	InventoryUI();

	void set_inventory(const Ref<Inventory> &p_inventory);
	Ref<Inventory> get_inventory() const;

	void set_inventory_holder(InventoryHolder *p_inventory_holder);
	InventoryHolder *get_inventory_holder() const;
};
} //namespace godot
