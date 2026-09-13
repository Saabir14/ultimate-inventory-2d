#pragma once

#include "godot_cpp/variant/typed_array.hpp"
#include "resource/inventory.hpp"

#include "godot_cpp/classes/node.hpp"

namespace godot {
class InventoryUI : public Node {
    GDCLASS(InventoryUI, Node);

private:
	Ref<Inventory> inventory;

	// Keep track of all the nodes instantiated by the inventory UI
	TypedArray<Node> slot_ui_nodes;

	void _queue_free_slot_ui_nodes();

	void _update_ui();

	int32_t index = -1;

protected:
    static void _bind_methods();

    void _notification(int p_what);

public:
    InventoryUI();

    void set_inventory(const Ref<Inventory> &p_inventory);
    Ref<Inventory> get_inventory() const;


};
} //namespace godot
