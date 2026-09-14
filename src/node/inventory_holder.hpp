#pragma once

#include "godot_cpp/classes/node.hpp"
#include "resource/inventory.hpp"

namespace godot {
class InventoryHolder : public Node {
	GDCLASS(InventoryHolder, Node)

private:
	Node *attached_node = nullptr;
	bool use_parent_as_attached_node = true;

	Ref<Inventory> inventory;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_attached_node(Node *p_node);
	Node *get_attached_node() const;

	void set_use_parent_as_attached_node(bool p_use_parent);
	bool get_use_parent_as_attached_node() const;

	void set_inventory(const Ref<Inventory> &p_inventory);
	Ref<Inventory> get_inventory() const;
};
} //namespace godot
